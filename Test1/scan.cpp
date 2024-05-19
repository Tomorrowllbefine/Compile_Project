#include "scan.h"
#include "global.h"
#include<iostream>
using namespace std;
int lineno;

/**判断是否为关键字**/
int p_tk = 0;
const char* tokenString;//存储当前token的串
int currentLine = 1;//当前行号
bool IsError = false;//本次分析是否报错
token errorToken;
int num_tk = 0;//token数组的序号
token tokenList[max_token];//存储识别到的token

string KEYWORD[num_kw] = {"if","then","else","end","repeat","until","read","write"};//关键字
char OPERATOR1[num_op1] = {'+','-','*','/','<','='};     //运算符1
string OPERATOR2[num_op2] = {":="};                      //运算符2(赋值)
char SEPARATER[num_sp] = {';'};//分隔符
char FILTER[num_ft] = {' ','\t','\n'};                    //过滤符

/**判断是否为关键字**/
bool IsKeyword(string word){
    for(int i=0;i<num_kw;i++){
        if(KEYWORD[i]==word){
            token tmp;
            tmp.name = word;
            switch(i)
            {
            case 0:tmp.type = IF; break;
            case 1:tmp.type = THEN; break;
            case 2:tmp.type = ELSE; break;
            case 3:tmp.type = END; break;
            case 4:tmp.type = REPEAT; break;
            case 5:tmp.type = UNTIL; break;
            case 6:tmp.type = READ; break;
            case 7:tmp.type = WRITE; break;
            }
            tmp.lineNum =currentLine;
            tokenList[num_tk++] = tmp;
            return true;
        }
    }
    return false;
}

/**判断是否为运算符(赋值)**/
bool IsOperator2(string op){
    for(int i=0;i<num_op2;i++){
        if(OPERATOR2[i]==op){
            token tmp;
            tmp.name = op;
            switch(i)
            {
            case 0:tmp.type = ASSIGN;break;
            }
            tmp.lineNum =currentLine;
            tokenList[num_tk++] = tmp;
            return true;
        }
    }
    return false;
}
/**判断是否为分隔符**/
bool IsSeparater(char ch){
    string tmparr="";
    tmparr += ch;
    for(int i=0;i<num_sp;i++){
        if(SEPARATER[i]==ch){
            token tmp;
            tmp.name = tmparr;
            switch(ch)
            {
            case ';':tmp.type = SEMI;break;
            }
            tmp.lineNum =currentLine;
            tokenList[num_tk++] = tmp;

            return true;
        }
    }
    return false;
}
/**判断是否为过滤符**/
bool IsFilter(char ch){
    for(int i=0;i<num_ft;i++){
        if(FILTER[i]==ch){
            return true;
        }
    }
    return false;
}

bool IsLetter(char ch)
{
    if( (ch>='a' && ch<='z')||(ch>='A' && ch<='Z') ) return true;
    return false;
}

/**判断是否为数字**/
bool IsDigit(char ch){
    if(ch>='0' && ch<='9') return true;
    return false;
}

/**判断是否为注释**/
bool IsNote(char ch, FILE* sc_file)
{
    int state = 1;
    ch = fgetc(sc_file);
    while(state != 3)
    {
        switch(state){
        case 1:
            if(ch  == '{')
            {
                ch = fgetc(sc_file);
                state = 2;
                break;
            }
        case 2:
            if( ch == '}')
            { // 右闭注释
                ch = fgetc(sc_file);
                state = 3;
                break;
            }
            else if(ch == '\n')
            { // 换行
                currentLine++;
                ch = fgetc(sc_file);
                state = 2;
            }
            else
            { // 正常字符
                ch = fgetc(sc_file);
                state = 2;
                break;
            }
        }
    }
    if(ch == '\n')
        currentLine++;
}

/**词法分析**/
void scan(FILE *file){

    char ch=' ';
    string arr="";

    while((ch=fgetc(file))!=EOF){
        arr = "";

        if(ch == '{')   //判断是否为注释
        {
            fseek(file,-1L,SEEK_CUR);
            IsNote(ch,file);
        }
        else if(IsFilter(ch)){//判断是否为过滤符
            if(ch == '\n')
                currentLine++;

        }

        else if(IsLetter(ch)){    //判断是否为关键字或标识符
            while(IsLetter(ch)){
                arr += ch;

                ch=fgetc(file);
            }
            if(IsKeyword(arr)){

                fseek(file,-1L,SEEK_CUR);
            }
            else
            {
                token tmp;
                tmp.type = ID;
                tmp.name = arr;
                tmp.lineNum = currentLine;
                tokenList[num_tk++] = tmp;
                fseek(file,-1L,SEEK_CUR);
            }
        }
        else if(IsDigit(ch)){           //判断是否为数字
            while(IsDigit(ch)){
                arr += ch;

                ch=fgetc(file);
            }
            fseek(file,-1L,SEEK_CUR);

            token tmp;
            tmp.type = NUM;
            int n = atoi(arr.c_str());
            tmp.val = n;
            tmp.name = arr;
            tmp.lineNum = currentLine;
            tokenList[num_tk++] = tmp;

        }


        else switch(ch){
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
            case '<':{
                token tmp;
                arr += ch;
                if(ch == '+')
                    tmp.type = ADD;
                else if(ch == '-')
                    tmp.type = SUB;
                else if(ch == '*')
                    tmp.type = MUL;
                else if(ch == '/'){
                    tmp.type = DIV;
                }
                else if(ch == '='){
                    tmp.type = EQ;
                }
                else{
                    tmp.type = LT;
                }

                tmp.name = arr;
                tmp.lineNum = currentLine;
                tokenList[num_tk++] = tmp;
                break;
            }
            case ':':{
                arr += ch;
                char tmp = fgetc(file);
                if(tmp == '=')
                {
                    arr += tmp;
                    IsOperator2(arr);
                }
                else{
                    token tmptk;
                    tmptk.type = ERROR;//后续输出出错处的符号以及行号
                    IsError = true;
                    tmptk.name = arr;
                    errorToken = tmptk;
                    break;
                }
                break;
            }

            case ';':{
                IsSeparater(ch);
                break;
            }
            default :{
                token tmptk;
                tmptk.type = ERROR;//后续输出出错处的符号以及行号
                IsError = true;
                arr += ch;
                tmptk.name = arr;
                errorToken = tmptk;
                break;
            }

            }

        if(IsError)
        {
            //cout<<"在"<<currentLine<<"行处出现非法符号:"<<errorToken.name<<endl;
            break;
        }


    }
    if(!IsError)
    {
        token tmptk;
        tmptk.name = "EOF";
        tmptk.type = ENDFILE;
    }

}


token getToken()
{
    tokenString = tokenList[p_tk].name.c_str();
    lineno = tokenList[p_tk].lineNum;
    return tokenList[p_tk++];

}

void printTokenList(string file)
{
    FILE* tokenlist = fopen(file.c_str(),"w");
    for(int i = 0;i < num_tk;i++)
    {
        fprintf(tokenlist,tokenList[i].name.c_str());
        if(tokenList[i].type >= 2 && tokenList[i].type <= 9)
            fprintf(tokenlist,"    keyword");
        else if(tokenList[i].type  == 10)
            fprintf(tokenlist,"    ID");
        else if(tokenList[i].type  == 11)
            fprintf(tokenlist,"    NUM");
        else
            fprintf(tokenlist,"    symbol");
        fprintf(tokenlist,"\n");
    }
    fclose(tokenlist);
}
