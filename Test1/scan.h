#ifndef SCAN_H
#define SCAN_H
#include"global.h"
//#include<iostream>
using namespace std;

/**判断是否为关键字**/
bool IsKeyword(string word);

/**判断是否为运算符**/
bool IsOperator2(string op);

/**判断是否为分隔符**/
bool IsSeparater(char ch);

/**判断是否为过滤符**/
bool IsFilter(char ch);


bool IsLetter(char ch);

/**判断是否为数字**/
bool IsDigit(char ch);

bool IsNote(char ch,FILE* sc_file);

/**词法分析**/
void scan(FILE *file);

token getToken();

void printTokenList(string file);
#endif // SCAN_H
