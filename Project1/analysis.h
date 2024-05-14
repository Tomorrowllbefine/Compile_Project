#pragma once
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>

const int MaxNum = 100;


// 数据结构定义
// 树结点
typedef struct Node {
    char type; // 根结点类型
    char Char; // 叶子结点代表的字符
    Node* lchild; // 左子树
    Node* rchild; // 右子树
    int start_S; // 起始状态
    int end_S; // 终结状态
}*Tree;


// 状态集
typedef struct states_set {
    int st_set[MaxNum];
    int length = 0;
};

//图的矩阵表示
typedef struct Skips_graph {
    int states_size; // 状态数
    int ab_char_size; // 吸收字符种类数
    char ab_char[MaxNum]; // 吸收字符
    int Skips[MaxNum][MaxNum]; // 状态跳转
    int Skips_empty[MaxNum][MaxNum]; // 空状态跳转
    int start_s; // 起点状态
    int end_s; // 终点状态(NFA有一个终点)
    states_set begin_set; // 存储初始集
    states_set end_set; // 终点集(DFA有多个终点) 【只存了sets的编号没有存对应的状态集元素】
    states_set sets[MaxNum]; // 存储所有集合
};

// 分析类
class Analysis
{
public:
    Analysis() {}
    void analyse(std::string inputstr); // 主函数

    Tree Syntax_Tree(std::string regex); // 生成语法树
    std::string getRegex(std::string inputstr); // 配置用于分析的regex
    void getNFA(Tree tree, Skips_graph& NFA, std::string regex); // Tompson算法构造NFA
    void createNFA(Tree tree, Skips_graph& NFA, std::string regex); // 构造NFA核心算法
    void getDFA(Skips_graph& NFA, Skips_graph& DFA); // 子集构造算法构造DFA
    void createDFA(Skips_graph& NFA, Skips_graph& DFA, states_set* sets); // 构造DFA核心函数
    void minimizeDFA(Skips_graph DFA, Skips_graph& minDFA); // 最小化DFA

    void display_Graph(Skips_graph& S, std::string type); // 展示图

public:
    Node* synTree; // 语法树
    Skips_graph NFA; // NFA
    Skips_graph DFA; // DFA
    Skips_graph minDFA; // 最小化DFA
};

#endif // ANALYSIS_H
