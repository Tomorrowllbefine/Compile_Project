#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<set>
#include<map>
#include<cstdio> 
using namespace std;
#define _CRT_SECURE_NO_WARNINGS


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
vector<string> operator_2 = { ":=", "<>", "<=", ">=" };                      // 运算符2
vector<char> separater = { ';' };                           // 分隔符
vector<char> filter = { ' ','\t','\n' };                    // 过滤符

// 接收状态集合
set<int> acceptStates;

// 创建关键字集合
// 类型集合

vector<string> token;
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
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
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


// 输出内容
void output()
{
    std::ofstream outfile("analyse_res.txt");

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

int main()
{
    // total state count: 8
    // start_state: 0   accept_state: 1,3,4,5,7

    acceptStates = { 1,3,4,5,7 }; // 初始化接收集
    int state = 0;
    char ch = ' ';
    bool error = false;
    bool longestMatch = false;
    //FILE* file = NULL;
    fpos_t pos; // 注意：fpos_t 是用于保存文件位置的适当类型 
    //fopen_s(&file, "tiny_test.txt", "r");
    FILE* file = fopen("tiny_test.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    fgetpos(file, &pos);
    ch = fgetc(file);


    while (ch != EOF)
    {
        while (IsFilter(ch)) {
            fgetpos(file, &pos);
            ch = fgetc(file);
        }
        fsetpos(file, &pos); // 非过滤回退

        token.push_back("");
        char next = ' ';
        state = 0; // 重置状态
        while (!acceptStates.count(state) || longestMatch) // 循环条件：非终态 或 可以最长匹配； 退出条件：终态 且 不可以最长匹配
        {
            longestMatch = false; // 重置
            switch (state)
            {
            case 0:
                ch = fgetc(file);
                switch (ch)
                {
                case '<':
                    state = 1;
                    fgetpos(file, &pos);
                    next = fgetc(file);
                    longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                    fsetpos(file, &pos); // 回退
                    break;
                case '/':
                    state = 4;
                    break;
                case '-':
                    state = 4;
                    break;
                case '*':
                    state = 4;
                    break;
                case '+':
                    state = 4;
                    break;
                case ';':
                    state = 4;
                    break;
                case '{':
                    state = 2;
                    break;
                case ':':
                    state = 6;
                    break;
                case '=':
                    state = 4;
                    break;
                case '>':
                    state = 5;
                    fgetpos(file, &pos);
                    next = fgetc(file);
                    longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                    fsetpos(file, &pos); // 回退
                    break;
                case '%':
                    state = 4;
                    break;
                default:
                    if (isdigit(ch)) {
                        state = 3;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                        fsetpos(file, &pos); // 回退
                    }
                    if (isalpha(ch)) {
                        state = 7;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                        fsetpos(file, &pos); // 回退
                    }
                }// end ch_switch
                break;

            case 1:
                ch = fgetc(file);
                switch (ch)
                {
                case '=':
                    state = 4;
                    break;
                case '>':
                    state = 4;
                    fgetpos(file, &pos);
                    next = fgetc(file);
                    longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                    fsetpos(file, &pos); // 回退
                    break;
                default:
                    error = true;
                }// end ch_switch
                break;

            case 2:
                ch = fgetc(file);
                if (ch == '}') {
                    state = 4;
                }
                else state = 2;
                break;

            case 3:
                ch = fgetc(file);
                switch (ch)
                {
                default:
                    if (isdigit(ch)) {
                        state = 3;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                        fsetpos(file, &pos); // 回退
                    }
                }// end ch_switch
                break;

            case 4:
                ch = fgetc(file);
                switch (ch)
                {
                default:
                    error = true;
                }// end ch_switch
                break;

            case 5:
                ch = fgetc(file);
                switch (ch)
                {
                case '=':
                    state = 4;
                    break;
                default:
                    error = true;
                }// end ch_switch
                break;

            case 6:
                ch = fgetc(file);
                switch (ch)
                {
                case '=':
                    state = 4;
                    break;
                default:
                    error = true;
                }// end ch_switch
                break;

            case 7:
                ch = fgetc(file);
                switch (ch)
                {
                default:
                    if (isdigit(ch)) {
                        state = 7;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                        fsetpos(file, &pos); // 回退
                    }
                    if (isalpha(ch)) {
                        state = 7;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // 判断是否需要最长匹配
                        fsetpos(file, &pos); // 回退
                    }
                }// end ch_switch
                break;

            }// end state_switch
            token[token_size].push_back(ch); // 吸收当前字符
            if (error == true) break;
        } // end while_in


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
        token_size++;
        fgetpos(file, &pos);
        ch = fgetc(file);

    } //end while_out


    fclose(file);
    output();
}

