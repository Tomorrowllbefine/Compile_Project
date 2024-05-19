#include<iostream>
#include<cstring>
#include<string>
#include<stack>
#include<vector>
#include<set>
#include<queue>
#include<unordered_map>
#include <iomanip>
#include<cstdio>
#include<fstream>
#include <sstream>
#include"RegExp.h"
#include<map>
#include<QString>

#define MAX 128

using namespace std;

string regexp_str;	//输入的正则表达式
map<string,string>letterTOword;
map<string,string>wordTOletter;

/**
 * 建立映射表
 * @brief init_map
 */
void init_map()
{
    // e表示空
    letterTOword.insert(std::make_pair("a", "("));
    letterTOword.insert(std::make_pair("b", "<"));
    letterTOword.insert(std::make_pair("c", "/"));
    letterTOword.insert(std::make_pair("d", "-"));
    letterTOword.insert(std::make_pair("p", "="));
    letterTOword.insert(std::make_pair("f", "*"));
    letterTOword.insert(std::make_pair("g", "+"));
    letterTOword.insert(std::make_pair("h", ")"));
    letterTOword.insert(std::make_pair("i", ";"));
    letterTOword.insert(std::make_pair("j", "{"));
    letterTOword.insert(std::make_pair("k", "~"));
    letterTOword.insert(std::make_pair("l", "}"));
    letterTOword.insert(std::make_pair("m", "digit"));
    letterTOword.insert(std::make_pair("n", "letter"));
    letterTOword.insert(std::make_pair("o", ":"));
    letterTOword.insert(std::make_pair("e", "e"));   // e还是表示空串
    letterTOword.insert(std::make_pair("q", ">"));
    letterTOword.insert(std::make_pair("r", "%"));


    wordTOletter.insert(std::make_pair("(", "a"));
    wordTOletter.insert(std::make_pair("<", "b"));
    wordTOletter.insert(std::make_pair("/", "c"));
    wordTOletter.insert(std::make_pair("-", "d"));
    wordTOletter.insert(std::make_pair("=", "p"));
    wordTOletter.insert(std::make_pair("*", "f"));
    wordTOletter.insert(std::make_pair("+", "g"));
    wordTOletter.insert(std::make_pair(")", "h"));
    wordTOletter.insert(std::make_pair(";", "i"));
    wordTOletter.insert(std::make_pair("{", "j"));
    wordTOletter.insert(std::make_pair("~", "k"));
    wordTOletter.insert(std::make_pair("}", "l"));
    wordTOletter.insert(std::make_pair("digit","m"));
    wordTOletter.insert(std::make_pair("letter", "n"));
    wordTOletter.insert(std::make_pair(":", "o"));
    wordTOletter.insert(std::make_pair(">", "q"));
    wordTOletter.insert(std::make_pair("%", "r"));
}


//分割字符串函数(根据“,”进行分割)
vector<string> cutStringByComma(const string& str)
{
    vector<string> result;
    stringstream ss(str);
    string token;
    while (getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

/**
 * 分割字符串函数(根据“|”进行分割)
 * @brief cutStringByOr
 * @param str
 * @return
 */
vector<string> cutStringByOr(const string& str)
{
    vector<string> result;
    stringstream ss(str);
    string token;
    while (getline(ss, token, '|')) {
        result.push_back(token);
    }
    return result;
}


/********************中缀转后缀********************/

/*在字符串s第n位后面插入字符ch*/
void insert(string& s, int n, char ch)
{
    s += '#';
    for (int i = s.size() - 1; i > n; i--)
    {
        s[i] = s[i - 1];
    }
    s[n] = ch;
}

//对字符串s进行预处理
void NFAClass::preprocess(string& s)
{
    int i = 0, length = s.size();

    while (i < length)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] == '*') || (s[i] == ')'))
        {
            if ((s[i + 1] >= 'a' && s[i + 1] <= 'z') || s[i + 1] == '(')
            {
                insert(s, i + 1, '&');
                length++;
            }
        }
        i++;
    }
}

/*中缀转后缀时用到的优先级比较，即为每个操作符赋一个权重，通过权重大小比较优先级*/
int NFAClass::priority(char ch) {
    switch (ch) {
    case '*':
        return 3;
    case '?':
        return 3;
    case '+':
        return 3;
    case '&':
        return 2;
    case '|':
        return 1;
    case '(':
        return 0;
    }
    return -1;
}

//中缀表达式转后缀表达式
string NFAClass::Postfix(string s)
{
    preprocess(s);			//对字符串进行预处理
    string str;				/*要输出的后缀字符串*/
    stack<char> operate;		/*运算符栈*/

    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z') {		/*如果是操作数直接输出*/
            str += s[i];
        }
        else {							/*遇到运算符时*/
            if (s[i] == '(') {			/*遇到左括号压入栈中*/
                operate.push(s[i]);
            }
            else if (s[i] == ')') {		/*遇到右括号时*/
                char ch = operate.top();
                while (ch != '(')		/*将栈中元素出栈，直到栈顶为左括号*/
                {
                    str += ch;
                    operate.pop();
                    ch = operate.top();
                }
                operate.pop();				/*最后将左括号出栈*/
            }
            else					/*遇到其他操作符时*/
            {
                if (!operate.empty())			/*如果栈不为空*/
                {
                    char ch = operate.top();
                    /*弹出栈中优先级大于等于当前运算符的运算符，以实现运算符的优先级*/
                    while (priority(ch) >= priority(s[i]))
                    {
                        str += ch;
                        operate.pop();
                        if (operate.empty())	/*如果栈为空则结束循环*/
                        {
                            break;
                        }
                        else ch = operate.top();
                    }
                    operate.push(s[i]);		/*最后将当前运算符入栈*/
                }
                else {			/*如果栈为空，直接将运算符入栈*/
                    operate.push(s[i]);
                }
            }
        }
    }
    /*最后如果栈不为空，则出栈并输出到字符串*/
    while (!operate.empty()) {
        char ch = operate.top();
        operate.pop();
        str += ch;
    }

    return str;
}


// 后缀转NFA

NfaState NfaStates[MAX];    /*NFA状态数组*/
int nfaStateNum;        /*NFA状态总数*/

/*从状态n1到状态n2添加一条弧，弧上的值为ch*/
void NFAClass::add(NfaState* n1, NfaState* n2, char ch)
{
    n1->input = ch;
    n1->ch_index = n2->index;
}

/*从状态n1到状态n2添加一条弧，弧上的值为ε*/
void NFAClass::add(NfaState* n1, NfaState* n2)
{
    n1->ep_index.insert(n2->index);
}

/*新建一个NFA（即从NFA状态数组中取出两个状态）*/
NFA NFAClass::creatNFA(int sum)
{
    NFA n;
    n.head = &NfaStates[sum];
    n.tail = &NfaStates[sum + 1];
    return n;
}

//后缀表达式转nfa
NFA NFAClass::toNFA(string s)
{
    stack<NFA> NfaStack;		/*定义一个NFA栈*/

    for (int i = 0; i < s.size(); i++)		/*读取后缀表达式，每次读一个字符*/
    {
        if (s[i] >= 'a' && s[i] <= 'z')		/*遇到操作数*/
        {
            NFA n = creatNFA(nfaStateNum);		/*新建一个NFA*/
            nfaStateNum += 2;					/*NFA状态总数加2*/
            add(n.head, n.tail, s[i]);			/*NFA的头指向尾，弧上的值为s[i]*/
            NfaStack.push(n);					/*将该NFA入栈*/
        }
        else if (s[i] == '*')		/*遇到闭包运算符*/
        {
            NFA n1 = creatNFA(nfaStateNum);		/*新建一个NFA*/
            nfaStateNum += 2;					/*NFA状态总数加2*/
            NFA n2 = NfaStack.top();			/*从栈中弹出一个NFA*/
            NfaStack.pop();
            add(n2.tail, n2.head);				/*n2的尾通过ε指向n2的头*/
            add(n2.tail, n1.tail);				/*n2的尾通过ε指向n1的尾*/
            add(n1.head, n2.head);				/*n1的头通过ε指向n2的头*/
            add(n1.head, n1.tail);				/*n1的头通过ε指向n1的尾*/
            NfaStack.push(n1);					/*最后将新生成的NFA入栈*/
        }
        else if (s[i] == '|')		/*遇到或运算符*/
        {
            NFA n1, n2;							/*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
            n2 = NfaStack.top();
            NfaStack.pop();
            n1 = NfaStack.top();
            NfaStack.pop();
            NFA n = creatNFA(nfaStateNum);		/*新建一个NFA*/
            nfaStateNum += 2;					/*NFA状态总数加2*/
            add(n.head, n1.head);				/*n的头通过ε指向n1的头*/
            add(n.head, n2.head);				/*n的头通过ε指向n2的头*/
            add(n1.tail, n.tail);				/*n1的尾通过ε指向n的尾*/
            add(n2.tail, n.tail);				/*n2的尾通过ε指向n的尾*/
            NfaStack.push(n);					/*最后将新生成的NFA入栈*/
        }
        else if (s[i] == '&')		/*遇到连接运算符*/
        {
            NFA n1, n2, n;				/*定义一个新的NFA n*/
            n2 = NfaStack.top();				/*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
            NfaStack.pop();
            n1 = NfaStack.top();
            NfaStack.pop();
            add(n1.tail, n2.head);				/*n1的尾通过ε指向n2的头*/
            n.head = n1.head;					/*n的头为n1的头*/
            n.tail = n2.tail;					/*n的尾为n2的尾*/
            NfaStack.push(n);					/*最后将新生成的NFA入栈*/
        }
        else if (s[i] == '?') {             // 遇到可选运算符
            NFA n1 = creatNFA(nfaStateNum);        /*新建一个NFA*/
            nfaStateNum += 2;                    /*NFA状态总数加2*/
            NFA n2 = NfaStack.top();            /*从栈中弹出一个NFA*/
            NfaStack.pop();
            add(n2.tail, n1.tail);                /*n2的尾通过ε指向n1的尾*/
            add(n1.head, n2.head);                /*n1的头通过ε指向n2的头*/
            add(n1.head, n1.tail);                /*n1的头通过ε指向n1的尾*/
            NfaStack.push(n1);                    /*最后将新生成的NFA入栈*/
        }
        else if (s[i] == '+') {             // 遇到正闭包运算符
            NFA n1 = creatNFA(nfaStateNum);        /*新建一个NFA*/
            nfaStateNum += 2;                    /*NFA状态总数加2*/
            NFA n2 = NfaStack.top();            /*从栈中弹出一个NFA*/
            NfaStack.pop();
            add(n2.tail, n2.head);                /*n2的尾通过ε指向n2的头*/
            add(n2.tail, n1.tail);                /*n2的尾通过ε指向n1的尾*/
            add(n1.head, n2.head);                /*n1的头通过ε指向n2的头*/
            NfaStack.push(n1);                    /*最后将新生成的NFA入栈*/
        }
    }
    return NfaStack.top();		/*最后的栈顶元素即为生成好的NFA*/
}

unordered_map<char, int> ap;
unordered_map<int, char> ap_num;
vector<vector<string>> nfa_res;
/**
 * @brief NFAClass::printNFA cui输出
 * @param nfa
 */
void NFAClass::printNFA(NFA nfa)
{
    cout << "\n***************     NFA     ***************" << endl << endl;

    ap_num[0] = 'e';
    ap['e'] = 0;

    // 统计吸收字符
    int index_ap = 1;
    int m, n;
    for (int i = 0; i < nfaStateNum; i++)        /*遍历NFA状态数组*/
    {
        if (NfaStates[i].input != '#' && NfaStates[i].input != ' ')            /*如果弧上的值不是初始时的‘#’则输出*/
        {
            if (ap[NfaStates[i].input] == 0)
            {
                ap[NfaStates[i].input] = index_ap;
                ap_num[index_ap] = NfaStates[i].input;
                index_ap++;
            }
        }
        n = nfa.tail->index + 1; // NFA状态数
        m = index_ap; // 吸收字符个数
    }

    // 存入nfa结果集
    nfa_res = vector<vector<string>>(n, vector<string>(m, " "));
    for (int i = 0; i < nfaStateNum; i++) {              /*遍历NFA状态数组*/
        if (NfaStates[i].input != '#')            /*如果弧上的值不是初始时的‘#’则输出*/
        {

            if (nfa_res[NfaStates[i].index][ap[NfaStates[i].input]] == " ") {
                nfa_res[NfaStates[i].index][ap[NfaStates[i].input]] = to_string(NfaStates[i].ch_index);
            }
            else {
                nfa_res[NfaStates[i].index][ap[NfaStates[i].input]] += "," + to_string(NfaStates[i].ch_index);
            }


            //cout <<  << "-->'" <<  << "'-->" <<  << '\t';
        }
        set<int>::iterator it;                    /*输出该状态经过ε到达的状态*/
        for (it = NfaStates[i].ep_index.begin(); it != NfaStates[i].ep_index.end(); it++) {
            if (nfa_res[NfaStates[i].index][ap['e']] == " ") {
                nfa_res[NfaStates[i].index][ap['e']] = to_string(*it);
            }
            else {
                nfa_res[NfaStates[i].index][ap['e']] += "," + to_string(*it);
            }
            //cout << NfaStates[i].index << "-->'" << ' ' << "'-->" << *it << '\t';
        }
    }

    cout << setiosflags(ios::left) << setw(10) << " ";

    // 输出吸收字符
    for (int i = 0; i < index_ap; i++) {
        cout << setiosflags(ios::left) << setw(10) << ap_num[i];
    }

    cout << endl;
    // 输出nfa状态跳转
    for (int i = 0; i < n; i++) {
        if (i == nfa.head->index) { // 开始状态
            cout << "-";
            cout << setiosflags(ios::left) << setw(10) << i;
        }
        else if (i == nfa.tail->index) { // 接受状态
            cout << "+";
            cout << setiosflags(ios::left) << setw(10) << i;
        }
        else { // 一般状态
            cout << " ";
            cout << setiosflags(ios::left) << setw(10) << i;
        }

        for (int j = 0; j < m; j++) { // 输出当前状态下【空】及【吸收字符】对应的跳转状态
            cout << setiosflags(ios::left) << setw(10) << nfa_res[i][j];
        }
        cout << endl;
    }

/** 生成graph所需要的txt文件 */
    QFile file("nfa.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, "错误", "文件打开失败!");
    }

    QTextStream out(&file);
    out << QString::fromStdString("digraph abc{\n");

    for (int i = 0; i < n; i++) {
        out << i;
        out << "[fontcolor = white][shape = circle];\n";
    }
    for (int i = 0; i < n; i++) {
        if (i == nfa.head->index)
        {
            out << i;
            out << "[fontcolor = black][label = begin];\n";
        }
        else if (i == nfa.tail->index) {
            out << i;
            out << "[fontcolor = black][label = acc];\n";
        }

        for (int j = 0; j < m; j++)
        {
            if (nfa_res[i][j] != " ")
            {
                vector<string>nextStates = cutStringByComma(nfa_res[i][j]);
                for(auto nextState:nextStates)
                {
                    string label="";
                    label+=ap_num[j];
                    out << i << "->" << QString::fromStdString(nextState) << "[label=\"" << QString::fromStdString(letterTOword[label]) << "\"];\n";
                }
            }
        }
    }
    out << "}";
    file.close();
    cout << "printNFA---------------> end" << endl;
}



/********************NFA转DFA********************/

DfaState DfaStates[MAX];		/*DFA状态数组*/
int dfaStateNum = 0;			/*DFA状态总数*/
/*求一个状态集的ε-cloure*/
IntSet DFAClass::epclosure(IntSet s)
{
    stack<int> epStack;		/*(此处栈和队列均可)*/
    IntSet::iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        epStack.push(*it);
    }
    while (!epStack.empty())			/*只要栈不为空*/
    {
        int temp = epStack.top();		/*从栈中弹出一个元素*/
        epStack.pop();
        IntSet::iterator iter;
        /*遍历它通过ε能转换到的状态集*/
        for (iter = NfaStates[temp].ep_index.begin(); iter != NfaStates[temp].ep_index.end(); iter++)
        {
            if (!s.count(*iter))			/*如果当前元素没有在集合中出现*/
            {
                s.insert(*iter);			/*则把它加入集合中*/
                epStack.push(*iter);		/*同时压入栈中*/
            }
        }
    }
    return s;		/*最后的s即为ε-cloure*/
}


/*求一个状态集s的ε-cloure(move(ch))*/
IntSet DFAClass::moveEpclosure(IntSet s, char ch)
{
    IntSet temp;
    IntSet::iterator it;
    for (it = s.begin(); it != s.end(); it++)		/*遍历当前集合s中的每个元素*/
    {
        if (NfaStates[*it].input == ch)				/*如果对应转换弧上的值为ch*/
        {
            temp.insert(NfaStates[*it].ch_index);		/*则把该弧通过ch转换到的状态加入到集合temp中*/
        }
    }
    temp = epclosure(temp);			/*最后求temp的ε闭包*/
    return temp;
}


/*判断一个状态是否为终态*/
bool DFAClass::IsEnd(NFA n, IntSet s)
{
    IntSet::iterator it;
    for (it = s.begin(); it != s.end(); it++)	/*遍历该状态所包含的nfa状态集*/
    {
        if (*it == n.tail->index)				/*如果包含nfa的终态，则该状态为终态，返回true*/
        {
            return true;
        }
    }
    return false;		/*如果不包含，则不是终态，返回false*/
}

/*nfa转dfa主函数*/
DFA DFAClass::nfaToDfa(NFA n, string str)		/*参数为nfa和后缀表达式*/
{
    int i;
    DFA d;
    set<IntSet> states;
    memset(d.trans, -1, sizeof(d.trans));	/*初始化dfa的转移矩阵*/
    for (i = 0; i < str.size(); i++)			/*遍历后缀表达式*/
    {
        if (str[i] >= 'a' && str[i] <= 'z')		/*如果遇到操作数，则把它加入到dfa的终结符集中*/
        {
            d.terminator.insert(str[i]);
        }
    }
    d.startState = 0;		/*dfa的初态为0*/
    IntSet tempSet;
    tempSet.insert(n.head->index);		/*将nfa的初态加入到集合中*/
    DfaStates[0].closure = epclosure(tempSet);					/*求dfa的初态*/
    DfaStates[0].isEnd = IsEnd(n, DfaStates[0].closure);		/*判断初态是否为终态*/
    dfaStateNum++;			/*dfa数量加一*/
    queue<int> q;
    q.push(d.startState);		/*把dfa的初态存入队列中*/
    while (!q.empty())		/*只要队列不为空，就一直循环*/
    {
        int num = q.front();				/*取出队首元素*/
        q.pop();
        CharSet::iterator it;
        for (it = d.terminator.begin(); it != d.terminator.end(); it++)
        {// 逐一考察吸收字符
            IntSet temp = moveEpclosure(DfaStates[num].closure, *it);
            /*如果求出来的状态集不为空且与之前求出来的状态集不同，则新建一个DFA状态*/
            if (!states.count(temp) && !temp.empty())
            {// 新状态next_state
                states.insert(temp);
                DfaStates[dfaStateNum].closure = temp;
                DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;				/*该状态弧的输入即为当前终结符*/
                DfaStates[num].Edges[DfaStates[num].edgeNum].d_index = dfaStateNum;		/*弧转移到的状态为最后一个DFA状态*/
                DfaStates[num].edgeNum++;
                d.trans[num][*it - 'a'] = dfaStateNum;		/*更新转移矩阵*/
                DfaStates[dfaStateNum].isEnd = IsEnd(n, DfaStates[dfaStateNum].closure);
                q.push(dfaStateNum);	/*将新的状态号加入队列中*/
                dfaStateNum++;		/*DFA状态总数加一*/
            }
            else			/*求出来的状态集在之前求出的某个状态集相同*/
            {
                for (i = 0; i < dfaStateNum; i++)		/*遍历之前求出来的状态集合*/
                {
                    if (temp == DfaStates[i].closure)		/*找到与该集合相同的DFA状态*/
                    {
                        DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;		/*该状态弧的输入即为当前终结符*/
                        DfaStates[num].Edges[DfaStates[num].edgeNum].d_index = i;		/*该弧转移到的状态即为i*/
                        DfaStates[num].edgeNum++;										/*该状态弧的数目加一*/
                        d.trans[num][*it - 'a'] = i;		/*更新转移矩阵*/
                        break;
                    }
                }
            }
        }
    }

    /*计算dfa的终态集*/
    for (i = 0; i < dfaStateNum; i++)	/*遍历dfa的所有状态*/
    {
        if (DfaStates[i].isEnd == true)		/*如果该状态是终态*/
        {
            d.endStates.insert(i);		/*则将该状态号加入到dfa的终态集中*/
        }
    }
    return d;
}

/*打印dfa函数*/
void DFAClass::printDFA(DFA d)
{
    cout << "\n***************     DFA     ***************" << endl << endl;

    //生成graph所需要的txt文件
    QFile file("dfa.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, "错误", "文件打开失败!");
    }

    QTextStream out(&file);

    out << "digraph abc{\n";
    for (int i = 0; i < dfaStateNum; i++)
    {
        out << i << "[fontcolor=white][shape=circle];\n";
    }

    int i, j;

    // 打印横列
    cout << "\t\t";
    CharSet::iterator t;
    for (t = d.terminator.begin(); t != d.terminator.end(); t++)
    {
        cout << *t << "\t\t";
    }

    cout << endl;
    for (i = 0; i < dfaStateNum; i++)
    {
    // 1.起始列
        if (d.endStates.count(i))
        {// 是终止状态
            //cout << '<' << i << ">  ";
            cout << "+{" << i << "}\t\t";
            out << i << "[fontcolor=black][label=acc];\n";
        }
        else if (d.startState == i)
        {// 是起始状态
            //cout << ' ' << i << "   ";
            cout << "-{" << i << "}\t\t";
            out << i << "[fontcolor=black][label=begin];\n";
        }
        else
        { // 一般状态
            //cout << ' ' << i << "   ";
            cout << " {" << i << "}\t\t\t";
        }

    // 2.后续吸收字符列
        for (j = 0; j < 26; j++)
        {
            char label = j + 'a';
            string tmp = "";
            tmp += label;
            if (d.terminator.count(label))
            { // 当前字符是吸收字符
                if (d.trans[i][j] != -1)
                {
                    int ii = d.trans[i][j];
                    cout << "{" << ii << "}\t\t";

                    out << i << "->" << ii << "[label=\"" << QString::fromStdString(letterTOword[tmp]) << "\"];\n";
                }
                else
                {
                    cout << "\t\t\t";
                }
            }
        }
        cout << endl;
    }
    out << "}";
    file.close();

}


/******************DFA的最小化******************/
IntSet s[MAX];					/*划分出来的集合数组*/
DfaState minDfaStates[MAX];		/*minDfa状态数组*/
int minDfaStateNum = 0;			/*minDfa的状态总数，同时也是划分出的集合数*/

/*当前划分总数为count，返回状态n所属的状态集标号i*/
int MinDFAClass::findSetNum(int count, int n)
{
    for (int i = 0; i < count; i++)
    {
        if (s[i].count(n))
        {
            return i;
        }
    }

}


/********************最小化DFA********************/
DFA MinDFAClass::minDFA(DFA d)
{
    int i, j;
    cout << endl << "*************     minDFA     **************" << endl << endl;

    DFA minDfa;
    minDfa.terminator = d.terminator;		/*把dfa的终结符集赋给minDfa*/
    memset(minDfa.trans, -1, sizeof(minDfa.trans));		/*初始化minDfa转移矩阵*/
    /*做第一次划分，即将终态与非终态分开*/
    bool isAllEnd = true;					/*判断dfa的所有状态是否全为终态的标志*/
    for (i = 0; i < dfaStateNum; i++)	/*遍历dfa状态数组*/
    {
        if (DfaStates[i].isEnd == false)			/*如果该dfa状态不是终态*/
        {
            isAllEnd = false;						/*标志应为false*/
            minDfaStateNum = 2;						/*第一次划分应该有两个集合，一个终态一个非终态*/
            s[0].insert(DfaStates[i].index);		/*把非终态的状态号加入s[0]集合中*/
        }
        else									/*如果该dfa状态是终态*/
        {
            s[1].insert(DfaStates[i].index);		/*把终态的状态号加入s[1]集合中*/
        }
    }
    if (isAllEnd)					/*如果标志为真，则所有dfa状态都是终态*/
    {
        minDfaStateNum = 1;			/*第一次划分结束应只有一个集合*/
    }
    bool cutFlag = true;		/*上一次是否产生新的划分的标志*/
    while (cutFlag)				/*只要上一次产生新的划分就继续循环*/
    {
        int cutCount = 0;			/*需要产生新的划分的数量*/
        for (i = 0; i < minDfaStateNum; i++)			/*遍历每个划分集合*/
        {
            CharSet::iterator it;
            for (it = d.terminator.begin(); it != d.terminator.end(); it++)		/*遍历dfa的终结符集*/
            {
                int setNum = 0;				/*当前缓冲区中的状态集个数*/
                stateSet temp[20];			/*划分状态集“缓冲区”*/
                IntSet::iterator iter;
                for (iter = s[i].begin(); iter != s[i].end(); iter++)		/*遍历集合中的每个状态号*/
                {
                    bool epFlag = true;			/*判断该集合中是否存在没有该终结符对应的转换弧的状态*/
                    for (j = 0; j < DfaStates[*iter].edgeNum; j++)		/*遍历该状态的所有边*/
                    {
                        if (DfaStates[*iter].Edges[j].input == *it)		/*如果该边的输入为该终结符*/
                        {
                            epFlag = false;			/*则标志为false*/
                            /*计算该状态转换到的状态集的标号*/
                            int transNum = findSetNum(minDfaStateNum, DfaStates[*iter].Edges[j].d_index);
                            int curSetNum = 0;			/*遍历缓冲区，寻找是否存在到达这个标号的状态集*/
                            while ((temp[curSetNum].index != transNum) && (curSetNum < setNum))
                            {
                                curSetNum++;
                            }

                            if (curSetNum == setNum)		/*缓冲区中不存在到达这个标号的状态集*/
                            {
                                /*在缓冲区中新建一个状态集*/
                                temp[setNum].index = transNum;		/*该状态集所能转换到的状态集标号为transNum*/
                                temp[setNum].s.insert(*iter);		/*把当前状态添加到该状态集中*/

                                setNum++;		/*缓冲区中的状态集个数加一*/
                            }
                            else			/*缓冲区中存在到达这个标号的状态集*/
                            {
                                temp[curSetNum].s.insert(*iter);	/*把当前状态加入到该状态集中*/
                            }
                        }
                    }
                    if (epFlag)		/*如果该状态不存在与该终结符对应的转换弧*/
                    {
                        /*寻找缓冲区中是否存在转换到标号为-1的状态集,这里规定如果不存在转换弧，则它所到达的状态集标号为-1*/
                        int curSetNum = 0;
                        while ((temp[curSetNum].index != -1) && (curSetNum < setNum))
                        {
                            curSetNum++;
                        }
                        if (curSetNum == setNum)			/*如果不存在这样的状态集*/
                        {
                            /*在缓冲区中新建一个状态集*/
                            temp[setNum].index = -1;			/*该状态集转移到的状态集标号为-1*/
                            temp[setNum].s.insert(*iter);		/*把当前状态加入到该状态集中*/

                            setNum++;		/*缓冲区中的状态集个数加一*/
                        }
                        else			/*缓冲区中存在到达这个标号的状态集*/
                        {
                            temp[curSetNum].s.insert(*iter);	/*把当前状态加入到该状态集中*/
                        }
                    }
                }
                if (setNum > 1)	/*如果缓冲区中的状态集个数大于1，表示同一个状态集中的元素能转换到不同的状态集，则需要划分*/
                {
                    cutCount++;		/*划分次数加一*/
                    /*为每组划分创建新的dfa状态*/
                    for (j = 1; j < setNum; j++)		/*遍历缓冲区，这里从1开始是将第0组划分留在原集合中*/
                    {
                        IntSet::iterator t;
                        for (t = temp[j].s.begin(); t != temp[j].s.end(); t++)
                        {
                            s[i].erase(*t);						/*在原来的状态集中删除该状态*/
                            s[minDfaStateNum].insert(*t);		/*在新的状态集中加入该状态*/
                        }

                        minDfaStateNum++;		/*最小化DFA状态总数加一*/
                    }
                }
            }
        }
        if (cutCount == 0)		/*如果需要划分的次数为0，表示本次不需要进行划分*/
        {
            cutFlag = false;
        }
    }

    /*遍历每个划分好的状态集*/
    for (i = 0; i < minDfaStateNum; i++)
    {
        IntSet::iterator y;
        for (y = s[i].begin(); y != s[i].end(); y++)		/*遍历集合中的每个元素*/
        {
            if (*y == d.startState)			/*如果当前状态为dfa的初态，则该最小化DFA状态也为初态*/
            {
                minDfa.startState = i;
            }

            if (d.endStates.count(*y))		/*如果当前状态是终态，则该最小化DFA状态也为终态*/
            {
                minDfaStates[i].isEnd = true;
                minDfa.endStates.insert(i);		/*将该最小化DFA状态加入终态集中*/
            }

            for (j = 0; j < DfaStates[*y].edgeNum; j++)		/*遍历该DFA状态的每条弧，为最小化DFA创建弧*/
            {
                /*遍历划分好的状态集合，找出该弧转移到的状态现在属于哪个集合*/
                for (int t = 0; t < minDfaStateNum; t++)
                {
                    if (s[t].count(DfaStates[*y].Edges[j].d_index))
                    {
                        bool haveEdge = false;		/*判断该弧是否已经创建的标志*/
                        for (int l = 0; l < minDfaStates[i].edgeNum; l++)	/*遍历已创建的弧*/
                        {
                            /*如果该弧已经存在*/
                            if ((minDfaStates[i].Edges[l].input == DfaStates[*y].Edges[j].input) && (minDfaStates[i].Edges[l].d_index == t))
                            {
                                haveEdge = true;		/*标志为真*/
                            }
                        }
                        if (!haveEdge)		//如果该弧不存在，则创建一条新的弧
                        {
                            minDfaStates[i].Edges[minDfaStates[i].edgeNum].input = DfaStates[*y].Edges[j].input;	/*弧的值与DFA的相同*/
                            minDfaStates[i].Edges[minDfaStates[i].edgeNum].d_index = t;	/*该弧转移到的状态为这个状态集的标号*/

                            minDfa.trans[i][DfaStates[*y].Edges[j].input - 'a'] = t;	/*更新转移矩阵*/

                            minDfaStates[i].edgeNum++;		/*该状态的弧的数目加一*/
                        }
                        break;
                    }
                }
            }
        }
    }

    return minDfa;
}

/**
 * 输出最小化DFA cui
 * @brief MinDFAClass::printMinDFA
 * @param d
 */
void MinDFAClass::printMinDFA(DFA d)
{
    //生成graph所需要的txt文件
    QFile file("minidfa.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, "错误", "文件打开失败!");
    }

    QTextStream out(&file);


    out << "digraph abc{\n";
    for (int i = 0; i < minDfaStateNum; i++)
    {
        out << i << "[fontcolor=white][shape=circle];\n";
    }


    int i, j;

    cout << "     ";
    CharSet::iterator t;
    for (t = d.terminator.begin(); t != d.terminator.end(); t++)
    {
        cout << *t << "   ";
    }
    cout << endl;

    for (i = 0; i < minDfaStateNum; i++)
    {
        if (d.endStates.count(i))
        {
            cout << '+' << i << "  ";
            out << i << "[fontcolor=black][label=acc];\n";
        }
        else if (d.startState == i)
        {
            cout << '-' << i << "   ";
            out << i << "[fontcolor=black][label=begin];\n";
        }
        else
        {
            cout << " " << i << "   ";
        }

        for (j = 0; j < 26; j++)
        {
            char label = j + 'a';
            string tmp = "";
            tmp += label;
            if (d.terminator.count(label))
            {
                if (d.trans[i][j] != -1)
                {
                    cout << d.trans[i][j] << "   ";
                    out << i << "->" << d.trans[i][j] << "[label =\"" << QString::fromStdString(letterTOword[tmp]) << "\"];\n";

                }
                else
                {
                    cout << "    ";
                }
            }
        }

        cout << endl;
    }
    out << "}";
    file.close();
    cout << endl << "*******************************************";
}


/** 通用函数 */

//初始化函数
void initialize()
{
    //初始化所有的数组
    int i, j;
    for (i = 0; i < MAX; i++)
    {
        NfaStates[i].index = i;
        NfaStates[i].input = '#';
        NfaStates[i].ch_index = -1;
    }

    for (i = 0; i < MAX; i++)
    {
        DfaStates[i].index = i;
        DfaStates[i].isEnd = false;

        for (j = 0; j < 10; j++)
        {
            DfaStates[i].Edges[j].input = '#';
            DfaStates[i].Edges[j].d_index = -1;
        }
    }

    for (i = 0; i < MAX; i++)
    {
        minDfaStates[i].index = i;
        minDfaStates[i].isEnd = false;

        for (int j = 0; j < 10; j++)
        {
            minDfaStates[i].Edges[j].input = '#';
            minDfaStates[i].Edges[j].d_index = -1;
        }
    }
}



// 实现输入的映射
void reflect(string str)
{
    vector<string>expp = cutStringByOr(str);
    cout << expp.size() << endl;
    cout << "OK" << endl;
    string res = "";

    for(int i = 0; i < expp.size(); i ++)
    {
        string tmp = ""; // 当前轮的结果
        string input = expp[i];
        cout << input << endl;
        bool ys = true;
        for(int j = 0; j < input.size(); j ++)
        {
            string tmp_map = "";
            tmp_map += input[j];
            if (tmp_map == "\\")
            {
                string map_key = "";
                input = input.substr(1); // 把\消掉
                cout << "input: " << input << endl;
                for (int k = j; k < input.size(); k ++,j ++) {
                    map_key += input[k];
                    cout << "Map_key" << endl;
                    if (wordTOletter[map_key] != "")
                    {
                        tmp += wordTOletter[map_key];
                        ys = false;
                        break;
                    }
                }
            }
            else if (wordTOletter[tmp_map] != "" && ys) {
                //cout<<"YESYESYES"<<endl;
                if(tmp_map == "~")
                {
                    ys = false;
                }
                tmp += wordTOletter[tmp_map];
            }
            else
            {
                tmp += input[j];
                if(input[j] == '*') ys = true;
            }
        }
        res += tmp;
        res += "|";
    }
    res.pop_back();
    cout << "res: " << res << endl;
    regexp_str = res;
    cout << "regexp_str: " << regexp_str << endl;
}

/*
 * string字符串内容直接设置成编辑框文本
 * @brief strToTextEdit
 * @param str
 * @param textedit
 */
void strToTextEdit(string str, QTextEdit* textedit){
    //编辑框设置文本
    textedit->setText(QString::fromStdString(str.data()));
}

/*
 * 编辑框文本返回string字符串
 * @brief strToTextEdit
 * @param str
 * @param textedit
 */
string textEditToStr(QTextEdit* textedit){
    //获取编辑框内容
    QString qtxt = textedit->toPlainText();
    //QString转string类型
    string txt = qtxt.toStdString();
    return txt;
}


/* 读取文件内容
 * @brief fileToStr
 * @param qMain
 * @return
 */
//string fileToStr(MainWindow *main){
//    string res = "";
//    QString path = QFileDialog::getOpenFileName(main, QObject::tr("打开文件"),".",QObject::tr("文本文件(*txt)"));
//    if (!path.isEmpty()) {
//       QFile file(path);
//       if (file.open(QIODevice::ReadOnly)) {
//           while (!file.atEnd()) {
//               res.append(file.readLine());
//           }
//        }
//    }
//    return res;
//}

/*
 * 保存字符内容到文件
 */
//void strToFile(MainWindow *main,string sstr){
//    QFileDialog fileDialog;
//    QString str = fileDialog.getSaveFileName(main, QObject::tr("打开文件"),".",QObject::tr("文本文件(*txt)"));
//    if(str == "")
//    {
//        return;
//    }
//    QFile filename(str);
//    if(!filename.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        QMessageBox::warning(main,QObject::tr("错误"),QObject::tr("打开文件失败"),QMessageBox::Ok);
//        return;
//    }
//    else
//    {
//        QTextStream textStream(&filename);
//        textStream<<QString::fromStdString(sstr.data());
//    }
//    QMessageBox::information(main,"保存文件","保存文件成功",QMessageBox::Ok);
//}

/*
 * 填充表格
 * @brief fillTable
 */
void fillTable(QTableView* table,vector<string> head,vector<vector<string>> data,vector<string> row){
    //设置表头
    QStandardItemModel* m_tableModel = new  QStandardItemModel(table);      //创建模型指定父类


    m_tableModel->setHorizontalHeaderItem(0, new QStandardItem(" ")); ;

    for( int i = 0 ; i < head.size() ; i ++ ){
        m_tableModel->setHorizontalHeaderItem(i+1, new QStandardItem(head[i].data()));
    }
    m_tableModel->setRowCount(row.size());
    //设置正文
    table->setModel(m_tableModel);
    for( int i = 0 ; i < data.size() ; i++ ){
        m_tableModel->setItem(i, 0, new QStandardItem(row[i].data()));
        for( int j = 0 ; j < data[i].size() ; j++ ){
            m_tableModel->setItem(i, j+1, new QStandardItem(data[i][j].data()));
        }
    }
}
