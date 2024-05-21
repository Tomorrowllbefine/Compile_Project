#include "analysewindow.h"
#include <QFileDialog>
#include <ui_analysewindow.h>

AnalyseWindow::AnalyseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyseWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("XLEX by mmkk");
}

AnalyseWindow::~AnalyseWindow()
{
    delete ui;
}

/**
 * 打开文件
 * @brief AnalyseWindow::on_btn_choose_src_clicked
 */
void AnalyseWindow::on_btn_choose_src_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "Files (*.cpp *.txt)");

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
            if (!fileContent.isEmpty()) ui->textEdit_src->clear();
            // 将文件内容显示在文本框中
            ui->textEdit_src->setPlainText(fileContent);

            file.close();
        } else {
            ui->textEdit_src->setPlainText("无法打开文件");
        }
    }

}

void AnalyseWindow::on_btn_show_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "Files (*.txt)");

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
            if (!fileContent.isEmpty()) ui->textEdit_res->clear();
            // 将文件内容显示在文本框中
            ui->textEdit_res->setPlainText(fileContent);

            file.close();
        } else {
            ui->textEdit_res->setPlainText("无法打开文件");
        }
    }
}
