#include "assigimentseletionwindow.h"
#include "mainwindow.h"
#include "analysewindow.h"
#include "ui_assigimentseletionwindow.h"

AssigimentSeletionWindow::AssigimentSeletionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssigimentSeletionWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("XLEX by mmkk");
}

AssigimentSeletionWindow::~AssigimentSeletionWindow()
{
    delete ui;
}

/**
 * 词法生成
 * @brief AssigimentSeletionWindow::on_pushButton_2_clicked
 */
void AssigimentSeletionWindow::on_pushButton_2_clicked()
{

    MainWindow *m = new MainWindow(this);
    m->show();
}

/**
 * 词法分析测试
 * @brief AssigimentSeletionWindow::on_pushButton_3_clicked
 * @todo 待完善
 */
void AssigimentSeletionWindow::on_pushButton_3_clicked()
{
    // Todo
    AnalyseWindow *a = new AnalyseWindow(this);
    a->show();
}
