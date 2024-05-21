#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RegExp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // regex->nfa
    void on_pushButton_clicked();
    // nfa->dfa
    void on_pushButton_2_clicked();
    // 最小化dfa
    void on_pushButton_3_clicked();
    // 生成
    void on_pushButton_6_clicked();
    // 保存词法分析程序
    void on_btn_save_clicked();
    // 退出
    void on_btn_exit_clicked();
    // 打开正则文本
    void on_btn_open_clicked();
    // 重置
    void on_btn_reset_clicked();

private:
    Ui::MainWindow *ui;
    NFA* nfa;
    DFA* dfa;
    DFA* minDfa;

    NFAClass nfaClass;
    DFAClass dfaClass;
    MinDFAClass minDfaClass;
    AnalyseClass analyseClass;

    // 输出nfa表格
    void buildNfaTable();
    // 输出dfa表格
    void buildDfaTable();
    string dfaStateToEpclosure_string(int stateNum);
    // 输出minDfa表格
    void buildMinDfaTable();
    string minDfaStateToEpclosure_string(int stateNum);

    // 逐行获取正则文本
    vector<string> getInputLines();
    // 分隔=号后面的内容并用'|'拼接
    string getRegex(vector<string> inputLines);
};
#endif // MAINWINDOW_H
