#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RegExp.h"
#include <QStringList>
#include <iostream>
#include <string>
#include<bits/stdc++.h>
#include <QDir>
#include <QFileInfo>
#include<QCoreApplication>
#include<QProcess>
#include<QString>
#include<QDebug>
#include <QPixmap>
#include <vector>
#include <QFileDialog>
#include <qdebug.h>
#include <QString>
#include <QStringList>
#include"global.h"
#include "getCode.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("XLEX by mmkk");
}
MainWindow::~MainWindow()
{
    delete ui;
}

FILE* source;	// 源代码文件

/**
 * 逐行获取正则文本
 * @brief MainWindow::getInputLins
 * @return
 */
vector<string> MainWindow::getInputLines()
{
    vector<string> inputLines;
    QString text = ui->textEdit->toPlainText(); // 对于 QTextEdit 或 QPlainTextEdit 都适用

    // 使用 QString 的 split 方法按行分割文本
    QStringList lines = text.split(QRegularExpression("\r\n|\n|\r"), Qt::SkipEmptyParts);
    // 获取lines中每个string存入inputLines中
    // 遍历QStringList，并将每个QString转换为std::string，然后添加到vector中
    for (const QString& line : lines) {
        inputLines.push_back(line.toStdString());
    }
    return inputLines;
}

/**
 * 分隔=号后面的内容并用'|'拼接
 * @brief MainWindow::gerRegex
 * @param inputLines
 * @return
 */
string MainWindow::getRegex(vector<string> inputLines)
{
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
            regex += line;
            regex += "|";
            cout << "转换类型后内容是: " << regex << endl;
        }
        regex.pop_back();
    }
    else {
        regex += regexList[0];
    }
    return regex;
}

/**
 * 转化函数按钮
 * @brief MainWindow::on_pushButton_6_clicked
 */
void MainWindow::on_pushButton_6_clicked()
{
    ui->label_graph->clear();
    // 获取输入行
    vector<string> inputLines = getInputLines();
    // 获取装配完的正则表达式
    string txt = getRegex(inputLines);

    qDebug()<<"decorated regex: "<< txt;


//    //获取编辑框内容
//    QString qtxt = ui->textEdit->toPlainText();
//    //QString转string类型
//    string txt = qtxt.toStdString();
    cout << endl << "txt:" << txt << endl;
    init_map();
    reflect(txt);
    initialize();
    qDebug()<<"regexp_str: "<< regexp_str;

    regexp_str = nfaClass.Postfix(regexp_str);
    qDebug()<<"Post_regexp_str: "<< regexp_str;


    // NFA
    NFA n = nfaClass.toNFA(regexp_str);
    this->nfa = &n; // 赋值成员变量
    nfaClass.printNFA(n);
    this->buildNfaTable();

    // DFA
    DFA d = dfaClass.nfaToDfa(n, regexp_str);
    this->dfa = &d;
    dfaClass.printDFA(d);
    this->buildDfaTable();

    // minDFA
    DFA minDfa = minDfaClass.minDFA(d);
    this->minDfa = &minDfa;
    minDfaClass.printMinDFA(minDfa);
    this->buildMinDfaTable();

//    AnalyseClass aa;
//    string codes = aa.getCode(minDfa);
//    ui->codeText->setText(QString::fromStdString(codes));
}


/**
 * 打印NFA表格
 * @brief MainWindow::buildNfaTable
 */
void MainWindow::buildNfaTable(){

    QTableWidget *nfaTable = ui->tableWidget_2;

    nfaTable->setRowCount(MAX); // 设置行
    nfaTable->setColumnCount(MAX); // 设置列
    NFA nfa = *this->nfa;

    qDebug()<<"Test nfa.states_size: " << nfaStateNum;

    // 打印横栏
    for(int i = 1; i < ap_num.size(); i++)
    {
        std::string colLabel = letterTOword[std::string(1, ap_num[i])];
        QString item = QString::fromStdString(colLabel);
        nfaTable->setHorizontalHeaderItem(i-1,new QTableWidgetItem(item));
    }
    nfaTable->setHorizontalHeaderItem(ap_num.size() - 1, new QTableWidgetItem("空跳转"));

    // 打印纵栏
    for(int i = 0; i < nfaStateNum; i++)
    {
        std::string rowLabel = "state: "+std::to_string(i);
        QString item = QString::fromStdString(rowLabel);
        nfaTable->setVerticalHeaderItem(i,new QTableWidgetItem(item));
    }

    // 打印NFA
    for(int i = 0; i < nfaStateNum; i++)
    {
        for(int j = 1; j < ap_num.size(); j++)
        {
            string context = "";
            context += nfa_res[i][j];
            context = (context == " ")? "-" : context;
            QString item = QString::fromStdString(context);
            nfaTable->setItem(i,j - 1, new QTableWidgetItem(item));
        }
        // 打印空跳转
        {
            string context = "";
            context += nfa_res[i][0];
            context = (context == " ")? "-" : context;
            QString item = QString::fromStdString(context);
            nfaTable->setItem(i, ap_num.size() - 1, new QTableWidgetItem(item));
        }
    }
    // 标出起点终点
    nfaTable->setItem(nfa.head->index, ap_num.size(), new QTableWidgetItem("开始状态"));
    nfaTable->setItem(nfa.tail->index, ap_num.size(), new QTableWidgetItem("接收状态"));

    // 展示下方栏
    if(nfaStateNum > 0)
    {
        ui->begin_label1->setText(QString::fromStdString(to_string(nfa.head->index)));
        ui->end_label1->setText(QString::fromStdString(to_string(nfa.tail->index)));
    }
    // 自适应宽度
    nfaTable->resizeColumnsToContents();
}



// 根据dfa状态返回对应的epclosure闭包 {1,2,3,4...,n} 格式
string MainWindow::dfaStateToEpclosure_string(int stateNum)
{
    IntSet set = DfaStates[stateNum].closure;
    std::string rowLabel = "{";
    for(auto it : set){
        rowLabel += to_string(it) + ",";
    }
    rowLabel.pop_back();
    rowLabel += "}";
    return rowLabel;
}

// 根据dfa状态返回对应的epclosure闭包 {1,2,3,4...,n} 格式
string MainWindow::minDfaStateToEpclosure_string(int stateNum)
{
    IntSet set = minDfaStates[stateNum].closure;
    std::string rowLabel = "{";
    for(auto it : set){
        rowLabel += to_string(it) + ",";
    }
    rowLabel.pop_back();
    rowLabel += "}";
    return rowLabel;
}

// 构造吸收字符映射
unordered_map<int,char> int_char_ab;
unordered_map<char,int> char_int_ab;


/**
 * 打印Dfa表格
 * @brief MainWindow::buildDfaTable
 */
void MainWindow::buildDfaTable(){

    // 显示DFA
    QTableWidget *dfaTable = ui->tableWidget_3;
    DFA dfa = *this->dfa;
    dfaTable->setRowCount(MAX);
    dfaTable->setColumnCount(MAX);

    // 打印横栏
    CharSet::iterator it;
    int index = 0;
    for(it = dfa.terminator.begin(); it != dfa.terminator.end(); it ++)
    {
        std::string colLabel = letterTOword[std::string(1,*it)];
        QString item = QString::fromStdString(colLabel);
        dfaTable->setHorizontalHeaderItem(index ++, new QTableWidgetItem(item));
    }


    int count = 0;
    for(CharSet::iterator it = dfa.terminator.begin(); it != dfa.terminator.end(); it ++)
    {
        int_char_ab[count++] = *it;
    }
    for(auto it : int_char_ab)
    {
        char_int_ab[it.second] = it.first;
    }

    // 打印纵栏
    for(int i = 0; i < dfaStateNum; i ++)
    {
        string rowLabel = dfaStateToEpclosure_string(i);
        QString item = QString::fromStdString(rowLabel);
        dfaTable->setVerticalHeaderItem(i,new QTableWidgetItem(item));
    }
    dfaTable->setHorizontalHeaderItem(dfa.terminator.size(), new QTableWidgetItem("空跳转"));

    // 打印DFA
    for(int i = 0; i < dfaStateNum; i ++)
    {
        for(int j = 0; j < 26; j++)
        {
            char label = j + 'a'; // 转成对应序号字符
            string tmp = std::string(1, label);
            if(dfa.terminator.count(label))
            { // 是吸收字符
                if(dfa.trans[i][j] != -1)
                {
                    int next_state = dfa.trans[i][j];
                    string str = dfaStateToEpclosure_string(next_state); // 状态集合字符串
                    QString item = QString::fromStdString(str);
                    int index = char_int_ab[label];
                    dfaTable->setItem(i, index, new QTableWidgetItem(item));
                }
                else{
                    //int next_state = dfa.trans[i][j];
                    int index = char_int_ab[label];
                    QString item = QString::fromStdString("-");
                    dfaTable->setItem(i, index, new QTableWidgetItem(item));
                }
            }
        }
    }


    // 标出起点终点
    dfaTable->setItem(dfa.startState, dfa.terminator.size(), new QTableWidgetItem("开始状态"));
    for(int state_num : dfa.endStates){
        dfaTable->setItem(state_num, dfa.terminator.size(), new QTableWidgetItem("接收状态"));
    }

    // 显示下方栏
    string endNodeText = "";
        // 显示初始集
    string startNodeText = dfaStateToEpclosure_string(dfa.startState);

        // 遍历所有接收集
    for(int end_state : dfa.endStates)
    {
        string str = dfaStateToEpclosure_string(end_state);
        endNodeText += str + " ";
    }
    ui->begin_label2->setText(QString::fromStdString(startNodeText));
    ui->end_label2->setText(QString::fromStdString(endNodeText));

    // 自适应宽度
    dfaTable->resizeColumnsToContents();

}

/**
 * 输出最小化Dfa表格
 * @brief MainWindow::buildMinDfaTable
 */
void MainWindow::buildMinDfaTable()
{
    // 打印最小DFA
    QTableWidget *minDfaTable = ui->tableWidget_4;
    minDfaTable->setRowCount(MAX);
    minDfaTable->setColumnCount(MAX);
    DFA minDfa = *this->minDfa;

    // 打印横栏
    CharSet::iterator it;
    int index = 0;
    for(it = minDfa.terminator.begin(); it != minDfa.terminator.end(); it ++)
    {
        std::string colLabel = letterTOword[std::string(1, *it)];
        QString item = QString::fromStdString(colLabel);
        minDfaTable->setHorizontalHeaderItem(index ++, new QTableWidgetItem(item));
    }

    // 打印纵栏
    for(int i = 0; i < minDfaStateNum; i ++)
    {
        std::string rowLabel = "state: "+std::to_string(i);
        QString item = QString::fromStdString(rowLabel);
        minDfaTable -> setVerticalHeaderItem(i,new QTableWidgetItem(item));
    }

    for(int i = 0; i < minDfaStateNum; i ++)
    {
        for(int j = 0; j < 26; j ++)
        {
            char label = j + 'a'; // 转成对应序号字符
            string tmp = std::string(1, label);
            if(minDfa.terminator.count(label))
            { // 是吸收字符
                if(minDfa.trans[i][j] != -1)
                {
                    int next_state = minDfa.trans[i][j];
                    string str = to_string(next_state);
                    QString item = QString::fromStdString(str);
                    int index = char_int_ab[label];
                    minDfaTable->setItem(i, index, new QTableWidgetItem(item));
                }
                else{
                    // int next_state = minDfa.trans[i][j];
                    int index = char_int_ab[label];
                    QString item = QString::fromStdString("-");
                    minDfaTable->setItem(i, index, new QTableWidgetItem(item));
                }
            }

        }
    }

    // 标出起点终点
    minDfaTable->setItem(minDfa.startState, minDfa.terminator.size(), new QTableWidgetItem("开始状态"));
    for(int state_num : minDfa.endStates){
        minDfaTable->setItem(state_num, minDfa.terminator.size(), new QTableWidgetItem("接收状态"));
    }


    // 显示下方栏
    string endNodeText = "";
        // 显示初始集
    string startNodeText = to_string(minDfa.startState);

    // 遍历所有接收集
    for(int end_state : minDfa.endStates)
    {
        string str = to_string(end_state);
        endNodeText += str + ",";
//        string str = dfaStateToEpclosure_string(end_state);
//        endNodeText += str + " ";
    }
    endNodeText.pop_back();

    ui->begin_label3->setText(QString::fromStdString(startNodeText));
    ui->end_label3->setText(QString::fromStdString(endNodeText));

    // 自适应宽度
    minDfaTable->resizeColumnsToContents();

}



/**
 * 转化为NFA图
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    QFile file("nfa.txt");
    QString filePath = QFileInfo(file).absoluteFilePath();
    //cout<<filePath.toStdString()<<endl;
    QString z= filePath;
    QStringList pathList = filePath.split("/");
    pathList.removeLast(); // 移除最后一个元素（文件名）
    pathList.removeLast();
    QString tmp_m = pathList.join("/") + "/";
    QString m = tmp_m+"Test1/util/graph/dot.exe"; // Todo 路径要调整
    //创建进程执行命令，根据dot.txt文件画出png图并保存在其目录下
    QStringList arguments;

    arguments << z <<"-Tpng"<<"-o"<< "nfa.png";
    QProcess *pro = new QProcess;
    pro->execute(m, arguments);

    //将生成的nfa图展示出来
    QPixmap image;
    image.load("nfa.png");
    ui->label_graph->clear();
    ui->label_graph->setPixmap(image);
    ui->label_graph->setScaledContents(true);    //根据label大小缩放图片
    ui->label_graph->show();

}



/**
 * 转化为DFA按钮
 * @brief MainWindow::on_pushButton_2_clicked
 */
void MainWindow::on_pushButton_2_clicked()
{
    QFile file("dfa.txt");
    QString filePath = QFileInfo(file).absoluteFilePath();
    //cout<<filePath.toStdString()<<endl;
    QString z= filePath;
    QStringList pathList = filePath.split("/");
    pathList.removeLast(); // 移除最后一个元素（文件名）
    pathList.removeLast();  //移除导数第二个元素（得到上一级目录）
    QString tmp_m = pathList.join("/") + "/";
    QString m = tmp_m+"Test1/util/graph/dot.exe"; // Todo 路径要调整
    //创建进程执行命令，根据dot.txt文件画出png图并保存在其目录下
    QStringList arguments;

    arguments << z <<"-Tpng"<<"-o"<< "dfa.png";
    QProcess *pro = new QProcess;
    pro->execute(m, arguments);

    //将生成的nfa图展示出来
    QPixmap image;
    image.load("dfa.png");
    ui->label_graph->clear();
    ui->label_graph->setPixmap(image);
    ui->label_graph->setScaledContents(true);    //根据label大小缩放图片
    ui->label_graph->show();
}

/**
 * DFA最小化
 * @brief MainWindow::on_pushButton_3_clicked
 */
void MainWindow::on_pushButton_3_clicked()
{
    QFile file("minidfa.txt");
    QString filePath = QFileInfo(file).absoluteFilePath();
    //cout<<filePath.toStdString()<<endl;
    QString z= filePath;
    QStringList pathList = filePath.split("/");
    pathList.removeLast(); // 移除最后一个元素（文件名）
    pathList.removeLast();  //移除导数第二个元素（得到上一级目录）
    QString tmp_m = pathList.join("/") + "/";
    QString m = tmp_m+"Test1/util/graph/dot.exe"; // Todo 路径要调整
    //创建进程执行命令，根据dot.txt文件画出png图并保存在其目录下
    QStringList arguments;

    arguments << z <<"-Tpng"<<"-o"<< "minidfa.png";
    QProcess *pro = new QProcess;
    pro->execute(m, arguments);

    //将生成的nfa图展示出来
    QPixmap image;
    image.load("minidfa.png");
    ui->label_graph->clear();
    ui->label_graph->setPixmap(image);
    ui->label_graph->setScaledContents(true);    //根据label大小缩放图片
    ui->label_graph->show();
}



///**
// * 生成分析程序
// * @brief MainWindow::on_btn_initscan_clicked
// * @todo 不能使用这种方式
// */
//void MainWindow::on_btn_initscan_clicked()
//{

//    QString filePath = ":/cpp/scan.cpp"; // 使用资源路径

//    QFile file(filePath);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qDebug() << "Failed to open file: " << filePath;
//        return;
//    }

//    QTextStream in(&file);
//    ui->textBrowser->clear(); // 待删除
//    ui->codeText->clear();
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        ui->textBrowser->append(line); // 待删除
//        ui->codeText->append(line);
//    }
//}



///**
// * 词法分析
// * @brief MainWindow::on_btn_scan_clicked
// */
//void MainWindow::on_btn_scan_clicked()
//{
//    if(ui->cbBox_src->currentText().isEmpty())
//    {
//        return;
//    }
//    if(ui->cbBox_scan->currentText().isEmpty())
//    {
//        QMessageBox::information(NULL, "提示", "词法分析文件未指定!");
//        return;
//    }
//    QString source_path = ui->cbBox_src->currentText();
//    QString scan_path = ui->cbBox_scan->currentText();

//    QByteArray tmp1 = source_path.toLocal8Bit();
//    const char* soure_p = tmp1.data();

//    QByteArray tmp2 = scan_path.toLocal8Bit();
//    const char* scan_p = tmp2.data();

//    source = fopen(soure_p,"r");
//    scan(source);
//    fclose(source);
//    printTokenList(scan_p);

//    QMessageBox::information(NULL, "提示", "词法分析完成!");
//    QMessageBox::information(NULL, "提示", "词法分析完成!");
//    QFile scan;
//    scan.setFileName(scan_path);
//    scan.open(QIODevice::ReadOnly|QIODevice::Text);
//    QByteArray scan_array = scan.readAll();
//    ui->text_token->setText(QString(scan_array));
//    //    ui->text_token->setText();
//    scan.close();
//}

/**
 * 保存词法分析文件
 * @brief MainWindow::on_tlbtn_scan_clicked
 */
void MainWindow::on_btn_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "保存文件", QDir::homePath(), "文本文件 (*.txt)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            QString fileContent = ui->codeText->toPlainText(); // 获取TextEdit文本框的内容
            out << fileContent; // 将内容写入文件
            file.close();
        } else {
            // 处理文件无法打开的情况
            QMessageBox::warning(this, "错误", "无法保存文件");
        }
    }
}

/**
 * 退出
 * @brief MainWindow::on_btn_exit_clicked
 */
void MainWindow::on_btn_exit_clicked()
{
    this->close();
}

/**
 * 打开正则文本
 * @brief MainWindow::on_btn_open_clicked
 */
void MainWindow::on_btn_open_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择正则文本文件", "", "Files (*.txt)");

    if (!filePath.isEmpty()) {
        // 打开用户选择的CPP文件
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent;

            // 逐行读取文件内容
            while (!in.atEnd()) {
                QString line = in.readLine();
                fileContent += line;
                if (!in.atEnd()) {
                    fileContent += "\n"; // 如果不是最后一行，添加换行符
                }
                //fileContent += line + "\n"; // 添加换行符
            }
            // 文件中文本不空才清楚原输入框内容
            if (!fileContent.isEmpty()) ui->textEdit->clear();
            // 将文件内容显示在文本框中
            ui->textEdit->setPlainText(fileContent);

            file.close();
        } else {
            ui->textEdit->setPlainText("无法打开文件");
        }
    }
}


extern NfaState NfaStates[MAX];    // NFA状态数组
extern int nfaStateNum;        // NFA状态总数
extern unordered_map<char, int> ap; // 吸收字符映射（字符->序号）
extern unordered_map<int, char> ap_num; // 吸收字符映射（序号->字符）
extern vector<vector<string>> nfa_res; // NFA结果集

extern DfaState DfaStates[MAX];        /*DFA状态数组*/
extern int dfaStateNum;            /*DFA状态总数*/

extern IntSet s[MAX];					/*划分出来的集合数组*/
extern DfaState minDfaStates[MAX];		/*minDfa状态数组*/
extern int minDfaStateNum;			/*minDfa的状态总数，同时也是划分出的集合数*/

/**
 * 重置
 * @brief MainWindow::on_btn_reset_clicked
 */
void MainWindow::on_btn_reset_clicked()
{
    // 控件内容清空初始化
    ui->codeText->clear();
    ui->tableWidget_2->clear();
    ui->tableWidget_3->clear();
    ui->tableWidget_4->clear();
    ui->begin_label1->clear();
    ui->begin_label2->clear();
    ui->begin_label3->clear();
    ui->end_label1->clear();
    ui->end_label2->clear();
    ui->end_label3->clear();
    // 输入框重置
    ui->textEdit->clear();

/** 重置全局变量 */
    initialize();

    nfaStateNum = 0; // 重置总数
    // 清除吸收字符映射
    ap.clear();
    ap_num.clear();
    nfa_res.clear();
    this->nfa->head = nullptr;
    this->nfa->tail = nullptr;

    dfaStateNum = 0; // 重置总数
//    dfa->startState = -1/* 初始状态的整数值 */; // 设置起始状态为初始值
//    dfa->endStates.clear(); // 清除终态集
//    dfa->terminator.clear(); // 清除终结符集

    // 清除转移矩阵中的所有条目
    //memset(dfa->trans, -1, sizeof(dfa->trans)); // 假设-1表示无转移

    // IntSet重置
    for(auto it : s){
        it.clear();
    }
    minDfaStateNum = 0; // 重置总数
//    minDfa->startState = -1/* 初始状态的整数值 */; // 设置起始状态为初始值
//    minDfa->endStates.clear(); // 清除终态集
//    minDfa->terminator.clear(); // 清除终结符集

    // 清除转移矩阵中的所有条目
    //memset(minDfa->trans, -1, sizeof(minDfa->trans)); // 假设-1表示无转移


}





