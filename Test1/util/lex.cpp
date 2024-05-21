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
vector<string> keyword = { "if","then","else","end","repeat","until","read","write" }; //�ؼ���
vector<char> operator_1 = { '+','-','*','/','<','=' };     // �����1
vector<string> operator_2 = { ":=", "<>", "<=", ">=" };                      // �����2
vector<char> separater = { ';' };                           // �ָ���
vector<char> filter = { ' ','\t','\n' };                    // ���˷�

// ����״̬����
set<int> acceptStates;

// �����ؼ��ּ���
// ���ͼ���

vector<string> token;
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
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')))
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


// �������
void output()
{
    std::ofstream outfile("analyse_res.txt");

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

int main()
{
    // total state count: 8
    // start_state: 0   accept_state: 1,3,4,5,7

    acceptStates = { 1,3,4,5,7 }; // ��ʼ�����ռ�
    int state = 0;
    char ch = ' ';
    bool error = false;
    bool longestMatch = false;
    //FILE* file = NULL;
    fpos_t pos; // ע�⣺fpos_t �����ڱ����ļ�λ�õ��ʵ����� 
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
        fsetpos(file, &pos); // �ǹ��˻���

        token.push_back("");
        char next = ' ';
        state = 0; // ����״̬
        while (!acceptStates.count(state) || longestMatch) // ѭ������������̬ �� �����ƥ�䣻 �˳���������̬ �� �������ƥ��
        {
            longestMatch = false; // ����
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
                    longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                    fsetpos(file, &pos); // ����
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
                    longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                    fsetpos(file, &pos); // ����
                    break;
                case '%':
                    state = 4;
                    break;
                default:
                    if (isdigit(ch)) {
                        state = 3;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                        fsetpos(file, &pos); // ����
                    }
                    if (isalpha(ch)) {
                        state = 7;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                        fsetpos(file, &pos); // ����
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
                    longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                    fsetpos(file, &pos); // ����
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
                        longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                        fsetpos(file, &pos); // ����
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
                        longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                        fsetpos(file, &pos); // ����
                    }
                    if (isalpha(ch)) {
                        state = 7;
                        fgetpos(file, &pos);
                        next = fgetc(file);
                        longestMatch = IsLongestMatch(ch, next); // �ж��Ƿ���Ҫ�ƥ��
                        fsetpos(file, &pos); // ����
                    }
                }// end ch_switch
                break;

            }// end state_switch
            token[token_size].push_back(ch); // ���յ�ǰ�ַ�
            if (error == true) break;
        } // end while_in


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
        token_size++;
        fgetpos(file, &pos);
        ch = fgetc(file);

    } //end while_out


    fclose(file);
    output();
}

