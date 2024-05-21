
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include <iomanip>
#include<unordered_set>
using namespace std;

string letterRegex = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)";
string digitRegex = "(0|1|2|3|4|5|6|7|8|9)";

// �洢�������������ʵ��Symbol�ַ���ͻ���ַ�����ӳ��
map<char, char> regexOpTo;
map<char, char> ToRegexOp;

enum Type {KEYWORD, SYMBOL, IDENTIFIER, NUMBER, COMMENT};
unordered_set<std::string> keywords = {
    "if", "then", "else", "end", "repeat", "until", "read", "write"
};
unordered_set<std::string> symbols = {
    "+", "-", "*", "/", "%", "<", "<>", "<=", ">=", ">", "=", ";", ":="
};
unordered_set<std::string> comments = {
    "{", "}"
};

// �����opת��Ϊ��Ӧ���ȼ� (�ӵ�->�ߣ�
// ���룺�����  ��������ȼ���
int o_astype(char o)
{
    switch (o)
    {
    case '#': return 1; // #
    case '[': return 2; // (
    case '|': return 3; // |
    case '@': return 4; // +
    case ']': return 5; // )
    case '&': return 6; // &
    case '?': return 6; // ?
    default: return 0; // ����op
    }
}

// �ж��Ƿ�������������
bool if_cat(char l, char r)
{
    bool res = 0;
    if (!o_astype(l) && r == '[') // char+( 
        res = 1;
    else if (!o_astype(l) && !o_astype(r)) // char+char
        res = 1;
    else if (l == ']' && !o_astype(r)) // )+char
        res = 1;
    else if (l == '&' && !o_astype(r)) // *+char
        res = 1;
    else if (l == ']' && r == '[')  // )+(
        res = 1;
    else if (l == '&' && r == '[')  // *+(
        res = 1;
    else if (l == '?' && !o_astype(r)) // ?+char
        res = 1;
    else if (l == '?' && r == '[')  // ?+(
        res = 1;
    else res = 0;

    return res;
}




/**
 * @Brief: ��ʼ�����������ӳ�䣬�����ͻ
 */
void init_map()
{
    regexOpTo.insert(std::make_pair('#', '#')); // ���շ�
    regexOpTo.insert(std::make_pair('(', '[')); // ������
    regexOpTo.insert(std::make_pair('|', '|')); // ��
    regexOpTo.insert(std::make_pair('+', '@')); // ���ӷ�
    regexOpTo.insert(std::make_pair(')', ']')); // ������
    regexOpTo.insert(std::make_pair('*', '&')); // �հ�
    regexOpTo.insert(std::make_pair('?', '?')); // ��ѡ

    ToRegexOp.insert(std::make_pair('#', '#'));
    ToRegexOp.insert(std::make_pair('[', '('));
    ToRegexOp.insert(std::make_pair('|', '|'));
    ToRegexOp.insert(std::make_pair('@', '+'));
    ToRegexOp.insert(std::make_pair(']', ')'));
    ToRegexOp.insert(std::make_pair('&', '*'));
    ToRegexOp.insert(std::make_pair('?', '?'));
}

/**
 * @Brief: ��ȡregex�ı��ļ�����,���зָ�
 * @Output: inputLines
 */
vector<string> getInputLines() {
    // ����洢�������ݵ�vector
    vector<string> inputLines;
    cout << "���������ݣ����� exit ��������" << endl;
    string line;
    while (getline(cin, line)) {
        if (line == "exit") {
            break;
        }
        inputLines.push_back(line);
    }

    // ����洢������
    cout << "�������ݴ洢���£�" << endl;
    for (const string& word : inputLines) {
        cout << word << endl;
    }
    return inputLines;
}


/**
 * @Brief: 1.�ָ�=�ź�������ݲ���'|'ƴ��; 
 *         2.װ�䡢�滻��������ʽ�����; 
 * @Input: vector<string> inputLines
 * @Output: string regex
 */
string getRegex(vector<string> inputLines) {
    vector<string> regexList;
/*** �ָ��� = �����������ʽ */ 
    for (int i = 0; i < inputLines.size(); i++) {
        size_t pos = inputLines[i].find('=');
        if (pos != string::npos) {
            string value = inputLines[i].substr(pos + 1);
            regexList.push_back(value);
        }
    }
    string regex = "";
    if (inputLines.size() > 1) {
        for (auto line : regexList) {
            cout << "\nԭ���: " << line << endl;
            regex += "(";
            regex += line;
            regex += ")|";
            cout << "ת�����ͺ�������: " << regex << endl;
        }
        regex.pop_back();
    }
    else {
        regex += regexList[0];
    }
    cout << "\n\nregex: " << regex << endl;

/*** ת��digit��letter */ 
    size_t pos = 0;
    while ((pos = regex.find("digit", pos)) != std::string::npos) {
        // �滻 "digit" Ϊ "(0|1|2|3|4|5|6|7|8|9)"
        regex.replace(pos, 5, digitRegex);
        // �ƶ�����һ��λ�ã���������ѭ��
        pos += digitRegex.length(); // 15 �� "(0|1|2|3|4|5|6|7|8|9)" �ĳ���
    }
    pos = 0;
    while ((pos = regex.find("letter", pos)) != std::string::npos) {
        regex.replace(pos, 6, letterRegex);
        // �ƶ�����һ��λ�ã���������ѭ��
        pos += letterRegex.length(); 
    }

/*** ���Ϸֽ���Լ��滻��ͻ��������� */
    regex.insert(0, "#"); // ͷ���뿪ʼ��
    regex.append("#"); // β���������
    for (int i = 0; i < regex.size(); i++) {
        if(regexOpTo.find(regex[i]) != regexOpTo.end())
        {
            if (regex[i] == '+') continue;
            if (regex[i] == '*') {
                if (i > 0 && i < regex.size() - 1) {
                    if (regex[i-1] == '|' && regex[i + 1] == '|')
                        continue;
                }
            }
            regex[i] = regexOpTo[regex[i]]; // �滻
        }
    }
    cout << "\n\n�滻���regex: " << regex << endl;
    // ��regex����cat���ӷ�
    int left = 0, right = 1;
    while (regex[right] != '#')
    {
        if (if_cat(regex[left], regex[right]))
            regex.insert(left + 1, "@");
        left++;  right++;
    }
    cout << "\n\n����regex: " << regex << endl;

    return regex;
}



int main() 
{
    // ��ʼ��ӳ���
    init_map();
    // ��ȡ������
    vector<string> inputLines = getInputLines();
    // ��ȡװ�����������ʽ
    string regex = getRegex(inputLines);

}




