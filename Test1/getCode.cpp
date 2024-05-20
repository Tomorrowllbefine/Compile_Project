#include<iostream>
#include<vector>
#include"RegExp.h"
#include <fstream>
using namespace std;


// ��������
string getTabs(int n)
{
    string texts = "";
    for (int i = 0; i < n; i++)
    {
        texts += "    ";
    }
    return texts;
}

// ��ȡ����״̬�ַ�����ʽ
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

// �жϵ�ǰ�ַ��Ƿ���Ҫ�ƥ���߼�
bool IsLongestMatchChar(char ch)
{
    if(ch == '<' || ch == '>') return true;
    if( ('a'<= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') ) return true;
    if('0'<= ch && ch <= '9') return true;
    return false;
}

// �ڲ�switch
void getSwitchCharCodes(DFA minDfa, vector<string>& lines, int& tab, int curState)
{
    bool flag_comment = false;
    int next_state = -1;
    // ���'~'
    if(minDfa.terminator.count(wordTOletter[string(1,'~')][0]))
    {
        int index = wordTOletter[string(1,'~')][0] - 'a';
        if(minDfa.trans[curState][index] != -1){
            flag_comment = true;
            next_state = minDfa.trans[curState][index];
        }
    }

    if(flag_comment)
    {// ���⴦��
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
    { // ��������
        lines.push_back(getTabs(tab) + "switch (ch)");
        lines.push_back(getTabs(tab) + "{");
        // ����ab_char�����ַ�����
        for(char ab_char : minDfa.terminator)
        {
            int index = ab_char - 'a';
            int next = minDfa.trans[curState][index]; // ��һ��ת״̬
            if(next != -1)
            {// ��һ������
                char real_char = letterTOword[string(1,ab_char)][0]; // ��ȡ��ǰ�����ַ���ʵ��ĸ
                cout<<"Test real_char: "<<real_char<<endl;
                lines.push_back(getTabs(tab) + "case '" + real_char + "':");
                tab++;
                if(IsLongestMatchChar(real_char))
                {
                    lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
                    lines.push_back(getTabs(tab) + "next = fgetc(file);");
                    lines.push_back(getTabs(tab) + "longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��");
                    lines.push_back(getTabs(tab) + "fseek(file, -1L, SEEK_CUR); // ����");
                    lines.push_back(getTabs(tab) + "break;");
                }else{
                    lines.push_back(getTabs(tab) + "state = " + to_string(next) + ";");
                    lines.push_back(getTabs(tab) + "break;");
                }
                tab--;
            }
        }
        // defaultͳһ��������շ�
        lines.push_back(getTabs(tab) + "default:");
        lines.push_back(getTabs(tab+1) + "error = true;");
        lines.push_back(getTabs(tab) + "}// end ch_switch");
    }

}

// ���switch
void getSwitchStateCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // ��ʼ״̬���
    IntSet end_state = minDfa.endStates; // ����״̬���
    lines.push_back(getTabs(tab) + "switch (state)");
    lines.push_back(getTabs(tab) + "{");
    int stateCount = 0;
    while (stateCount < minDfaStateNum)
    {
        lines.push_back(getTabs(tab) + "case " + to_string(stateCount) + ":");
        tab++;
        lines.push_back(getTabs(tab) + "ch = fgetc(file);");
        // ����ڲ�switch
        getSwitchCharCodes(minDfa, lines, tab, stateCount);
        lines.push_back(getTabs(tab) + "break;\n");
        tab--;
        stateCount++;
    }
    lines.push_back(getTabs(tab) + "}// end state_switch");
}

// �ڲ�whileѭ��
void getWhileInCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // ��ʼ״̬���
    IntSet end_state = minDfa.endStates; // ����״̬���
    lines.push_back(getTabs(tab) + "while (!acceptStates.count(state) || longestMatch) // ѭ������������̬ �� �����ƥ�䣻 �˳���������̬ �� �������ƥ��");
    lines.push_back(getTabs(tab) + "{");
    tab++;
    lines.push_back(getTabs(tab) + "longestMatch = false; // ����");
    getSwitchStateCodes(minDfa, lines, tab);
    lines.push_back(getTabs(tab) + "token[token_size].push_back(ch); // ���յ�ǰ�ַ�");
    lines.push_back(getTabs(tab) + "if (error == true) break;");
    tab--;
    lines.push_back(getTabs(tab) + "} // end while_in\n");
}

// ���whileѭ��
void getWhileOutCodes(DFA minDfa, vector<string>& lines, int& tab)
{
    int start_state = minDfa.startState; // ��ʼ״̬���
    IntSet end_state = minDfa.endStates; // ����״̬���
    lines.push_back("\n");
    lines.push_back(getTabs(tab) + "while ((ch= fgetc(file)) != EOF)"); // ���ѭ��
    lines.push_back(getTabs(tab) + "{");
    tab++;
    lines.push_back(getTabs(tab) + "if(!IsFilter(ch)) fseek(file, -1L, SEEK_CUR); // �ǹ��˻���");
    lines.push_back(getTabs(tab) + "token.push_back("");");
    lines.push_back(getTabs(tab) + "char next = ' ';");
    lines.push_back(getTabs(tab) + "state = " + to_string(start_state) + "; // ����״̬");
    // ��ȡ�ڴ�ѭ������
    getWhileInCodes(minDfa, lines, tab);
    // �ж�token����
    string judgeType_code = R"(
        // �жϵ�ǰtoken����
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




// ������
string AnalyseClass::getCode(DFA minDfa)
{
    string codes = "";
    vector<string> lines;
    int start_state = minDfa.startState; // ��ʼ״̬���
    IntSet end_state = minDfa.endStates; // ����״̬��ż�
    int tab = 0;
	
    // ����ǰ׺
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
vector<string> keyword = { "if","then","else","end","repeat","until","read","write" }; //�ؼ���
vector<char> operator_1 = { '+','-','*','/','<','=' };     // �����1
vector<string> operator_2 = { ":=", "<>", "<=", ">="};                      // �����2
vector<char> separater = { ';' };                           // �ָ���
vector<char> filter = { ' ','\t','\n' };                    // ���˷�

// ����״̬����
set<int> acceptStates;

// �����ؼ��ּ���
// ���ͼ���

vector<string> token ;
vector<int> type;
int token_size = 0;
int type_size = 0;

// �ж��Ƿ��ǹ��˷�
bool IsFilter(char ch)
{
    for (auto item : filter)
    {
        if (item == ch) return true;
    }
    return false;
}

// �ж��Ƿ�Ϊ����
bool IsDigit(string word) {
    for (int i = 0; i < word.size(); i++)
    {
        char ch = word[i];
        if (!(ch >= '0' && ch <= '9'))
            return false;
    }
    return true;
}

// �ж��Ƿ�Ϊ��ĸ
bool IsLetter(char ch)
{
     if ( !((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) )
            return false;
    return true;
}

/**�ж��Ƿ�Ϊ�ؼ���**/
bool IsKeyword(string word) {
    for (string item : keyword) {
        if (word == item) {
            return true;
        }
    }
    return false;
}
// �ж��Ƿ��������1
bool IsOperator_1(char ch)
{
    for (char item : operator_1)
    {
        if (ch == item) return true;
    }
    return false;
}

// �ж��Ƿ��������2
bool IsOperator_2(string word)
{
    for (string item : operator_2)
    {
        if (item == word) return true;
    }
    return false;
}

// �ж��Ƿ�Ϊ�ָ���
bool IsSeparater(char ch) {
    for (char item : separater) {
        if (ch == item)
            return true;
    }
    return false;
}
// �ж��Ƿ���ע��
bool IsComment(string word)
{
    return (word[0] == '{' && word.back() == '}');
}

// �Ƿ��ƥ��
bool IsLongestMatch(char cur, char next)
{
    // digit, digit   digit, letter
    if (isdigit(cur) && isdigit(next)) {
        // �жϵ�ǰtoken���
        if (isdigit(token[token_size][0])) {
            return true; // ����token�����Լ���ƥ������
        }
    }
    else if (isdigit(cur) && isalpha(next)) {
        if (isdigit(token[token_size][0])) {
            return false; // ����token�������Լ���ƥ���ַ�
        }
        else if (isalpha(token[token_size][0])) {
            return true; // ��ʶ��token�����Լ���ƥ������
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
// �������
void output()
{
    std::ofstream outfile("res.txt");

    // ����ļ��Ƿ�ɹ���
    if (!outfile.is_open()) {
        std::cerr << "�޷����ļ��Խ���д��" << std::endl;
        return;
    }

    // ʹ�õ���������vector��д��ÿһ��
    for (int i = 0; i < token.size(); i++)
    {
        outfile << token[i] << "\t" << tokenNames[type[i]] << endl;
        cout << token[i] << "\t" << tokenNames[type[i]] << endl;
    }
    // �ر��ļ�
    outfile.close();
}
)";
    lines.push_back(output);


    lines.push_back("int main()");
    lines.push_back("{");

    // main����ǰ����
    tab++;
    lines.push_back(getTabs(tab) + "// total state count: " + to_string(minDfaStateNum));
    lines.push_back(getTabs(tab) + "// start_state: "
        + to_string(start_state) + "   accept_state: " + getEndStates_string(end_state)+"\n");
    lines.push_back(getTabs(tab) + "acceptStates = {" + getEndStates_string(end_state) + "}; // ��ʼ�����ռ�");

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

    // whileѭ������
    getWhileOutCodes(minDfa, lines, tab);

    // ����main�����󲿷�
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


