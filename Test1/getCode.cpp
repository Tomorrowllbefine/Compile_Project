#include<iostream>
#include<vector>
#include"RegExp.h"
#include <fstream>
using namespace std;


// 调整缩进
string getTabs(int n)
{
    string texts = "";
    for (int i = 0; i < n; i++)
    {
        texts += "    ";
    }
    return texts;
}

// 获取接收状态字符串形式
string getEndStates_string(IntSet endStates)
{
    string str = "";
    for(int state : endStates)
    {
        str += to_string(state) + ",";
    }
    str.pop_back();
    return str;
}

// 判断当前字符是否需要最长匹配逻辑
bool IsLongestMatchChar(char ch)
{
    if(ch == '<' || ch == '>') return true;
    if( ('a'<= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') ) return true;
    if('0'<= ch && ch <= '9') return true;
    return false;
}

// 内层switch
void getSwitchCharCodes(DFA minDfa, vector<string>& lines, int& tab, int curState)
{
    bool flag_comment = false;
    int next_state = -1;
    // 检查'~'
    if(minDfa.terminator.count(wordTOletter[string(1,'~')][0]))
    {
        int index = wordTOletter[string(1,'~')][0] - 'a';
        if(minDfa.trans[curState][index] != -1){
            flag_comment = true;
            next_state = minDfa.trans[curState][index];
        }
    }

    if(flag_comment)
    {// 特殊处理
        lines.push_back(getTabs(tab) + "if (ch == '}') {");
        tab++;
        int c = wordTOletter[string(1,'}')][0] - 'a';
        int next = minDfa.trans[curState][c];
        lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
        tab--;
        lines.push_back(getTabs(tab) + "}");
        lines.push_back(getTabs(tab) + "else state = " + to_string(next_state) + ";");
    }
    else
    { // 正常处理
        lines.push_back(getTabs(tab) + "switch (ch)");
        lines.push_back(getTabs(tab) + "{");
        // 遍历ab_char吸收字符数组
        for(char ab_char : minDfa.terminator)
        {
            int index = ab_char - 'a';
            int next = minDfa.trans[curState][index]; // 下一跳转状态
            if(next != -1)
            {// 下一跳存在
                char real_char = letterTOword[string(1,ab_char)][0]; // 获取当前吸收字符真实字母
                cout<<"Test real_char: "<<real_char<<endl;
                lines.push_back(getTabs(tab) + "case '" + real_char + "':");
                tab++;
                if(IsLongestMatchChar(real_char))
                {
                    lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
                    lines.push_back(getTabs(tab) + "next = fgetc(file);");
                    lines.push_back(getTabs(tab) + "longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配");
                    lines.push_back(getTabs(tab) + "fseek(file, -1L, SEEK_CUR); // 回退");
                    lines.push_back(getTabs(tab) + "break;");
                }else{
                    lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
                    lines.push_back(getTabs(tab) + "break;");
                }
                tab--;
            }
        }
        // default统一处理非吸收符
        lines.push_back(getTabs(tab) + "default:");
        lines.push_back(getTabs(tab+1) + "error = true;");
        lines.push_back(getTabs(tab) + "}// end ch_switch");
    }

}

// 外层switch
void getSwitchStateCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // 初始状态编号
    IntSet end_state = minDfa.endStates; // 接收状态编号
    lines.push_back(getTabs(tab) + "switch (state)");
    lines.push_back(getTabs(tab) + "{");
    int stateCount = 0;
    while (stateCount < minDfaStateNum)
    {
        lines.push_back(getTabs(tab) + "case " + to_string(stateCount) + ":");
        tab++;
        lines.push_back(getTabs(tab) + "ch = fgetc(file);");
        // 组件内部switch
        getSwitchCharCodes(minDfa, lines, tab, stateCount);
        lines.push_back(getTabs(tab) + "break;\n");
        tab--;
        stateCount++;
    }
    lines.push_back(getTabs(tab) + "}// end state_switch");
}

// 内层while循环
void getWhileInCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // 初始状态编号
    IntSet end_state = minDfa.endStates; // 接收状态编号
    lines.push_back(getTabs(tab) + "while (!acceptStates.count(state) || longestMatch) // 循环条件：非终态 或 可以最长匹配； 退出条件：终态 且 不可以最长匹配");
    lines.push_back(getTabs(tab) + "{");
    tab++;
    lines.push_back(getTabs(tab) + "longestMatch = false; // 重置");
    getSwitchStateCodes(minDfa, lines, tab);
    lines.push_back(getTabs(tab) + "token[token_size].push_back(ch); // 吸收当前字符");
    lines.push_back(getTabs(tab) + "if (error == true) break;");
    tab--;
    lines.push_back(getTabs(tab) + "} // end while_in\n");
}

// 外层while循环
void getWhileOutCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // 初始状态编号
    IntSet end_state = minDfa.endStates; // 接收状态编号
    lines.push_back("\n");
    lines.push_back(getTabs(tab) + "while ((ch= fgetc(file)) != EOF)"); // 外层循环
    lines.push_back(getTabs(tab) + "{");
    tab++;
    lines.push_back(getTabs(tab) + "if(!IsFilter(ch)) fseek(file, -1L, SEEK_CUR); // 非过滤回退");
    lines.push_back(getTabs(tab) + "token.push_back("");");
    lines.push_back(getTabs(tab) + "char next = ' ';");
    lines.push_back(getTabs(tab) + "state = " + to_string(start_state) + "; // 重置状态");
    // 获取内存循环代码
    getWhileInCodes(minDfa, lines, tab);
    // 判断token类型
    string judgeType_code = R"(
        // 判断当前token类型
        if (IsKeyword(token[token_size])) { // keyword
            type.push_back((int)KEYWORD);
        }
        else if (IsDigit(token[token_size])) { // number
            type.push_back((int)NUMBER);
        }
        else if (IsSeparater(token[token_size][0]) || IsOperator_2(token[token_size]) || IsOperator_1(token[token_size][0])) { // symbol
            type.push_back((int)SYMBOL);
        }
        else if (IsComment(token[token_size])) { // comment
            type.push_back((int)COMMENT);
        }
        else {
            // identifier
            type.push_back((int)IDENTIFIER);
        }
        type_size++;
        token_size ++;
)";
    lines.push_back(judgeType_code);
    tab--;
    lines.push_back(getTabs(tab) + "} //end while_out");
}




// 主函数
string AnalyseClass::getCode(DFA minDfa)
{
    string codes = "";
    vector<string> lines;
    int start_state = minDfa.startState; // 初始状态编号
    IntSet end_state = minDfa.endStates; // 接收状态编号集
    int tab = 0;
	
    // 处理前缀
    std::string head = R"(
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<set>
#include<map>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
)";
    lines.push_back(head);
    std::string front_define = R"(
enum TOKEN { KEYWORD, IDENTIFIER, SYMBOL, NUMBER, COMMENT };
map<int, std::string> tokenNames = {
    {0, "KEYWORD"},
    {1, "IDENTIFIER"},
    {2, "SYMBOL"},
    {3, "NUMBER"},
    {4, "COMMENT"}
};
vector<string> keyword = { "if","then","else","end","repeat","until","read","write" }; //关键字
vector<char> operator_1 = { '+','-','*','/','<','=' };     // 运算符1
vector<string> operator_2 = { ":=", "<>", "<=", ">="};                      // 运算符2
vector<char> separater = { ';' };                           // 分隔符
vector<char> filter = { ' ','\t','\n' };                    // 过滤符

// 接收状态集合
set<int> acceptStates;

// 创建关键字集合
// 类型集合

vector<string> token ;
vector<int> type;
int token_size = 0;
int type_size = 0;

// 判断是否是过滤符
bool IsFilter(char ch)
{
    for (auto item : filter)
    {
        if (item == ch) return true;
    }
    return false;
}

// 判断是否为数字
bool IsDigit(string word) {
    for (int i = 0; i < word.size(); i++)
    {
        char ch = word[i];
        if (!(ch >= '0' && ch <= '9'))
            return false;
    }
    return true;
}

// 判断是否为字母
bool IsLetter(char ch)
{
     if ( !((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) )
            return false;
    return true;
}

/**判断是否为关键字**/
bool IsKeyword(string word) {
    for (string item : keyword) {
        if (word == item) {
            return true;
        }
    }
    return false;
}
// 判断是否是运算符1
bool IsOperator_1(char ch)
{
    for (char item : operator_1)
    {
        if (ch == item) return true;
    }
    return false;
}

// 判断是否是运算符2
bool IsOperator_2(string word)
{
    for (string item : operator_2)
    {
        if (item == word) return true;
    }
    return false;
}

// 判断是否为分隔符
bool IsSeparater(char ch) {
    for (char item : separater) {
        if (ch == item)
            return true;
    }
    return false;
}
// 判断是否是注释
bool IsComment(string word)
{
    return (word[0] == '{' && word.back() == '}');
}

// 是否最长匹配
bool IsLongestMatch(char cur, char next)
{
    // digit, digit   digit, letter
    if (isdigit(cur) && isdigit(next)) {
        // 判断当前token类别
        if (isdigit(token[token_size][0])) {
            return true; // 数字token，可以继续匹配数字
        }
    }
    else if (isdigit(cur) && isalpha(next)) {
        if (isdigit(token[token_size][0])) {
            return false; // 数字token，不可以继续匹配字符
        }
        else if (isalpha(token[token_size][0])) {
            return true; // 标识符token，可以继续匹配数字
        }
    }// letter, digit  letter, digit
    else if (isalpha(cur) && isalpha(next) || isalpha(cur) && isdigit(next)) {
        return true;
    }// >, =
    else if (cur == '>' && next == '=') {
        return true;
    }// <,=  <,>
    else if (cur == '<' && next == '=') {
        return true;
    }
    else if (cur == '<' && next == '>') {
        return true;
    }
    else return false;
}
)";
    lines.push_back(front_define);
    string output = R"(
// 输出内容
void output()
{
    std::ofstream outfile("res.txt");

    // 检查文件是否成功打开
    if (!outfile.is_open()) {
        std::cerr << "无法打开文件以进行写入" << std::endl;
        return;
    }

    // 使用迭代器遍历vector并写入每一行
    for (int i = 0; i < token.size(); i++)
    {
        outfile << token[i] << "\t" << tokenNames[type[i]] << endl;
        cout << token[i] << "\t" << tokenNames[type[i]] << endl;
    }
    // 关闭文件
    outfile.close();
}
)";
    lines.push_back(output);


    lines.push_back("int main()");
    lines.push_back("{");

    // main方法前部分
    tab++;
    lines.push_back(getTabs(tab) + "// total state count: " + to_string(minDfaStateNum));
    lines.push_back(getTabs(tab) + "// start_state: "
        + to_string(start_state) + "   accept_state: " + getEndStates_string(end_state)+"\n");
    lines.push_back(getTabs(tab) + "acceptStates = {" + getEndStates_string(end_state) + "}; // 初始化接收集");

    lines.push_back(getTabs(tab) + "int state = " + to_string(start_state) + ";");
    lines.push_back(getTabs(tab) + "char ch = ' ';");
    lines.push_back(getTabs(tab) + "bool error = false;");
    lines.push_back(getTabs(tab) + "bool longestMatch = false;");
    lines.push_back(getTabs(tab) + "FILE* file = NULL;");
    lines.push_back(getTabs(tab) + "fopen_s(&file,\"tiny_test.txt\", \"r\");");
    lines.push_back(getTabs(tab) + "if (file == NULL) {");
    lines.push_back(getTabs(tab+1) + "perror(\"Error opening file\");");
    lines.push_back(getTabs(tab+1) + "return 1;");
    lines.push_back(getTabs(tab) + "}");

    // while循环处理
    getWhileOutCodes(minDfa, lines, tab);

    // 处理main方法后部分
    lines.push_back("\n");
    lines.push_back(getTabs(tab) + "fclose(file);");
    lines.push_back(getTabs(tab) + "output();");
    tab--;
    lines.push_back(getTabs(tab) + "}");

    std::ofstream file("output.txt", std::ios::binary);
    for (string line : lines)
    {
        codes += line;
        codes += '\n';
        cout << line << endl;
    }
    file << codes;
    file.close();
    return codes;
}


