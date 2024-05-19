#ifndef REGEXP_H
#define REGEXP_H

#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include <QString>

#include <QMessageBox>
#include <QTextStream>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTextEdit>
#include <QTableView>
// #include"graph.h"
// #include <mainwindow.h>

#define MAX 128

using namespace std;


typedef set<int> IntSet;
typedef set<char> CharSet;
extern string regexp_str;	//输入的正则表达式

extern map<string,string> letterTOword;  //实现abc和tiny单词的映射
extern map<string,string> wordTOletter;  //实现tiny单词和abc的映射

/********************regex表达式转NFA********************/
//一个圆圈
struct NfaState				/*定义NFA状态*/
{
    int index;				/*NFA状态的状态号*/
    char input;				/*NFA状态弧上的值*/
    int ch_index;			/*NFA状态弧转移到的状态号*/
    IntSet ep_index;		/*当前状态通过ε转移到的状态号集合*/
};

struct NFA
{
    NfaState* head;			/*NFA的头指针*/
    NfaState* tail;			/*NFA的尾指针*/
};

extern NfaState NfaStates[MAX];    // NFA状态数组
extern int nfaStateNum;        // NFA状态总数
extern unordered_map<char, int> ap; // 吸收字符映射（字符->序号）
extern unordered_map<int, char> ap_num; // 吸收字符映射（序号->字符）
extern vector<vector<string>> nfa_res; // NFA结果集


class NFAClass{
public:
    /*从状态n1到状态n2添加一条弧，弧上的值为ch*/
    void add(NfaState *n1, NfaState *n2, char ch);

    /*从状态n1到状态n2添加一条弧，弧上的值为ε*/
    void add(NfaState *n1, NfaState *n2);

    //新建一个NFA（即从NFA状态数组中取出两个状态）
    NFA creatNFA(int sum);

    /**
     * @brief 对字符串s进行预处理，在第一位是操作数、‘*’或‘)’且第二位是操作数或‘(’之间加入连接符‘&’
     * @param s
     */
    void preprocess(string &s);

    /*中缀转后缀时用到的优先级比较，即为每个操作符赋一个权重，通过权重大小比较优先级*/
    int priority(char ch);
    // 后缀表达式转nfa
    NFA toNFA(string s);

    // 中缀正则转后缀
    string Postfix(string s);

    /*打印NFA函数*/
    void printNFA(NFA nfa);

};

/*在字符串s第n位后面插入字符ch*/
void insert(string &s, int n, char ch);
//分割字符串函数(根据“,”进行分割)
vector<string> cutStringByComma(const string& str);
//分割字符串函数(根据“|”进行分割)
vector<string> cutStringByOr(const string& str);





/********************NFA转DFA********************/
struct dfa_Edge			/*定义DFA的转换弧*/
{
    char input;			/*弧上的值*/
    int d_index;		/*弧所指向的状态号*/
};
struct DfaState		/*定义DFA状态*/
{
    int index;			/*DFA状态的状态号*/
    IntSet closure;		/*NFA的ε-move()闭包*/
    int edgeNum;		/*DFA状态上的射出弧数*/
    dfa_Edge Edges[30];		/*DFA状态上的射出弧*/
    bool isEnd;			/*是否为终态*/
};
struct DFA			/*定义DFA结构*/
{
    int startState;				/*DFA的初态*/
    IntSet endStates;			/*DFA的终态集*/
    CharSet terminator;			/*DFA的终结符集*/
    int trans[MAX][26];			/*DFA的转移矩阵*/
};

extern DfaState DfaStates[MAX];        /*DFA状态数组*/
extern int dfaStateNum;            /*DFA状态总数*/

class DFAClass{
public:
    /*求一个状态集的ε-cloure*/
    set<int> epclosure(IntSet s);
    /*求一个状态集s的ε-cloure(move(ch))*/
    set<int> moveEpclosure(set<int> s, char ch);

    /*判断一个状态是否为终态*/
    bool IsEnd(NFA n, set<int> s);
    /*nfa转dfa主函数*/
    DFA nfaToDfa(NFA n, string str);
    /*打印dfa函数*/
    void printDFA(DFA d);
};



/********************DFA最小化********************/
struct stateSet			/*划分状态集*/
{
    int index;			/*该状态集所能转换到的状态集标号*/
    IntSet s;			/*该状态集中的dfa状态号*/
};
extern IntSet s[MAX];					/*划分出来的集合数组*/
extern DfaState minDfaStates[MAX];		/*minDfa状态数组*/
extern int minDfaStateNum;			/*minDfa的状态总数，同时也是划分出的集合数*/

class MinDFAClass{

public:
    int findSetNum(int count, int n);
    DFA minDFA(DFA d);
    void printMinDFA(DFA d);
};


class AnalyseClass{
public:
    string getCode(DFA minDfa);
};


void initialize();
void reflect(string str);
void init_map();
void strToTextEdit(string str, QTextEdit* textedit);
string textEditToStr(QTextEdit* textedit);
//string fileToStr(MainWindow *main);
//void strToFile(MainWindow *main,string sstr);
void fillTable(QTableView* table,vector<string> head,vector<vector<string>> data,vector<string> row);


#endif

