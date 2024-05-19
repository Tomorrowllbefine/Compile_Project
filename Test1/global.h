#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include<string>
using namespace std;
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

extern FILE* source;	// 源代码文件
extern FILE* listing;	// 输出文本文件列表
extern FILE* symlisting;  //符号表列表
extern FILE* code;		// 中间码
extern int lineno;		// 源代码行号

// 保留字的个数
#define MAXRESERVED 8

#define HASHSIZE 200//哈希表大小
#define MAX_SCP 8

typedef struct minLineList
{
    int lineno; // 变量在源代码中的行数
    struct minLineList* next;
} *LineList;

typedef struct minBucketList
{
    char* name;
    int isParam;  // 是否参数
    int scope;   // 变量的作用域
    int memloc; // 变量的内存位置
    int funloc; // 函数的内存位置
    LineList lines;
    struct minBucketList* next;
} *BucketList;


// Token类型
typedef enum
{
    ENDFILE, ERROR,

    // 关键字
    IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,

    ID, NUM,
    // 专用符号
    ADD, SUB, MUL, DIV, LT, EQ, ASSIGN,
    SEMI
} TokenType;

typedef struct token
{
    string name;//ID|keyword|op
    int val;    //NUM
    TokenType type;
    int lineNum;
};


const int num_kw = 8; // keyword个数
const int num_op1 = 6; // 普通运算符
const int num_op2 = 1; // 赋值运算符
const int num_sp = 1; // 分隔符
const int num_ft = 3; // 过滤符

#define max_token 300
extern int p_tk;
extern const char* tokenString;//存储当前token的串
extern int currentLine;//当前行号
extern bool IsError;//本次分析是否报错
extern token errorToken;
extern int num_tk;//token数组的序号
extern token tokenList[max_token];//存储识别到的token

extern string KEYWORD[num_kw];//关键字

extern char OPERATOR1[num_op1];     //运算符1
extern string OPERATOR2[num_op2];//运算符2

extern char SEPARATER[num_sp];//分隔符
extern char FILTER[num_ft];   //过滤符




//extern int EOF_flag; // 尾标志

// 定义三类节点类型，分别表示语句结点、表达式结点和声明结点
//typedef enum { StmtK, ExpK, DeclK } NodeKind;
//typedef enum { Selection_StmtK, Iteration_StmtK, Return_StmtK, Compound_StmtK, CallK } StmtKind;
//typedef enum { ConstK, OpK, IdK, AssignK } ExpKind;
//typedef enum { Var_DeclK, Array_DeclK, Fun_DeclK } DeclKind;

//// ExpType用以类型检查
//typedef enum { Void, Int, Bool, Array, Func} ExpType;

//#define MAXCHILDREN 4

//typedef struct treeNode
//{
//    struct treeNode* child[MAXCHILDREN];
//    struct treeNode* sibling;//兄弟节点
//    int lineno;
//    NodeKind nodekind;
//    union {
//        StmtKind stmt;
//        ExpKind exp;
//        DeclKind declare;
//    } kind;

//    union {
//        TokenType op;
//        int val;
//        char* name;
//    } attr;

//    int value; //数组下标
//    int isArray;
//    int isParameter;
//    int isGlobal;
//    int paramSize;
//    int localSize;
//    int scope;
//    ExpType type;

//} TreeNode;


// EchoSource=TRUE时，解析期间可用行号将源程序回显到列表文件
extern int EchoSource;

// TraceScan=TRUE时，当scanner识别每个Token将打印其信息到列表文件中
extern int TraceScan;

// TraceParse=TRUE时，语法树以线性方式打印到列表文件（对子级使用缩进）
extern int TraceParse;

// TraceAnalyse=TRUE时，向列表文件报告符号表插入和查找
extern int TraceAnalyze;

// TraceCode=TRUE时会在生成代码时将注释写入TM代码文件
extern int TraceCode;

// Error=TRUE可以在发生错误时阻止进一步传递
extern int Error;

extern int HighScope;
#endif // GLOBAL_H
