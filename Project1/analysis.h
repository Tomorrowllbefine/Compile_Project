#pragma once
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>

const int MaxNum = 100;


// ���ݽṹ����
// �����
typedef struct Node {
    char type; // ���������
    char Char; // Ҷ�ӽ�������ַ�
    Node* lchild; // ������
    Node* rchild; // ������
    int start_S; // ��ʼ״̬
    int end_S; // �ս�״̬
}*Tree;


// ״̬��
typedef struct states_set {
    int st_set[MaxNum];
    int length = 0;
};

//ͼ�ľ����ʾ
typedef struct Skips_graph {
    int states_size; // ״̬��
    int ab_char_size; // �����ַ�������
    char ab_char[MaxNum]; // �����ַ�
    int Skips[MaxNum][MaxNum]; // ״̬��ת
    int Skips_empty[MaxNum][MaxNum]; // ��״̬��ת
    int start_s; // ���״̬
    int end_s; // �յ�״̬(NFA��һ���յ�)
    states_set begin_set; // �洢��ʼ��
    states_set end_set; // �յ㼯(DFA�ж���յ�) ��ֻ����sets�ı��û�д��Ӧ��״̬��Ԫ�ء�
    states_set sets[MaxNum]; // �洢���м���
};

// ������
class Analysis
{
public:
    Analysis() {}
    void analyse(std::string inputstr); // ������

    Tree Syntax_Tree(std::string regex); // �����﷨��
    std::string getRegex(std::string inputstr); // �������ڷ�����regex
    void getNFA(Tree tree, Skips_graph& NFA, std::string regex); // Tompson�㷨����NFA
    void createNFA(Tree tree, Skips_graph& NFA, std::string regex); // ����NFA�����㷨
    void getDFA(Skips_graph& NFA, Skips_graph& DFA); // �Ӽ������㷨����DFA
    void createDFA(Skips_graph& NFA, Skips_graph& DFA, states_set* sets); // ����DFA���ĺ���
    void minimizeDFA(Skips_graph DFA, Skips_graph& minDFA); // ��С��DFA

    void display_Graph(Skips_graph& S, std::string type); // չʾͼ

public:
    Node* synTree; // �﷨��
    Skips_graph NFA; // NFA
    Skips_graph DFA; // DFA
    Skips_graph minDFA; // ��С��DFA
};

#endif // ANALYSIS_H
