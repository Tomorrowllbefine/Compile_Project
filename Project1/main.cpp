
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include <iomanip>
#include<unordered_set>
using namespace std;

string letterRegex = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z)";
string digitRegex = "(0|1|2|3|4|5|6|7|8|9)";

// 存储正则运算符中与实际Symbol字符冲突的字符其它映射
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

// 运算符op转换为对应优先级 (从低->高）
// 输入：运算符  输出：优先级数
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
    default: return 0; // 不是op
    }
}

// 判断是否加入连接运算符
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
 * @Brief: 初始化正则运算符映射，避免冲突
 */
void init_map()
{
    regexOpTo.insert(std::make_pair('#', '#')); // 开闭符
    regexOpTo.insert(std::make_pair('(', '[')); // 左括号
    regexOpTo.insert(std::make_pair('|', '|')); // 或
    regexOpTo.insert(std::make_pair('+', '@')); // 连接符
    regexOpTo.insert(std::make_pair(')', ']')); // 右括号
    regexOpTo.insert(std::make_pair('*', '&')); // 闭包
    regexOpTo.insert(std::make_pair('?', '?')); // 可选

    ToRegexOp.insert(std::make_pair('#', '#'));
    ToRegexOp.insert(std::make_pair('[', '('));
    ToRegexOp.insert(std::make_pair('|', '|'));
    ToRegexOp.insert(std::make_pair('@', '+'));
    ToRegexOp.insert(std::make_pair(']', ')'));
    ToRegexOp.insert(std::make_pair('&', '*'));
    ToRegexOp.insert(std::make_pair('?', '?'));
}

/**
 * @Brief: 获取regex文本文件内容,逐行分割
 * @Output: inputLines
 */
vector<string> getInputLines() {
    // 定义存储输入内容的vector
    vector<string> inputLines;
    cout << "请输入内容（输入 exit 结束）：" << endl;
    string line;
    while (getline(cin, line)) {
        if (line == "exit") {
            break;
        }
        inputLines.push_back(line);
    }

    // 输出存储的内容
    cout << "输入内容存储如下：" << endl;
    for (const string& word : inputLines) {
        cout << word << endl;
    }
    return inputLines;
}


/**
 * @Brief: 1.分隔=号后面的内容并用'|'拼接; 
 *         2.装配、替换掉正则表达式运算符; 
 * @Input: vector<string> inputLines
 * @Output: string regex
 */
string getRegex(vector<string> inputLines) {
    vector<string> regexList;
/*** 分隔出 = 后面的正则表达式 */ 
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
            cout << "\n原语句: " << line << endl;
            regex += "(";
            regex += line;
            regex += ")|";
            cout << "转换类型后内容是: " << regex << endl;
        }
        regex.pop_back();
    }
    else {
        regex += regexList[0];
    }
    cout << "\n\nregex: " << regex << endl;

/*** 转换digit和letter */ 
    size_t pos = 0;
    while ((pos = regex.find("digit", pos)) != std::string::npos) {
        // 替换 "digit" 为 "(0|1|2|3|4|5|6|7|8|9)"
        regex.replace(pos, 5, digitRegex);
        // 移动到下一个位置，避免无限循环
        pos += digitRegex.length(); // 15 是 "(0|1|2|3|4|5|6|7|8|9)" 的长度
    }
    pos = 0;
    while ((pos = regex.find("letter", pos)) != std::string::npos) {
        regex.replace(pos, 6, letterRegex);
        // 移动到下一个位置，避免无限循环
        pos += letterRegex.length(); 
    }

/*** 加上分界符以及替换冲突正则运算符 */
    regex.insert(0, "#"); // 头插入开始符
    regex.append("#"); // 尾插入结束符
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
            regex[i] = regexOpTo[regex[i]]; // 替换
        }
    }
    cout << "\n\n替换后的regex: " << regex << endl;
    // 将regex加入cat连接符
    int left = 0, right = 1;
    while (regex[right] != '#')
    {
        if (if_cat(regex[left], regex[right]))
            regex.insert(left + 1, "@");
        left++;  right++;
    }
    cout << "\n\n最终regex: " << regex << endl;

    return regex;
}



int main() 
{
    // 初始化映射表
    init_map();
    // 获取输入行
    vector<string> inputLines = getInputLines();
    // 获取装配完的正则表达式
    string regex = getRegex(inputLines);

}




