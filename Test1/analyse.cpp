#include <QFileDialog>
#include <analyse.h>
#include "analyse.h"

analyse::analyse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::analyse)
{
    ui->setupUi(this);
}

analyse::~analyse()
{
    delete ui;
}

/**
 * 选择源程序并展示
 * @brief analyse::on_btn_choose_src_clicked
 */
void analyse::on_btn_choose_src_clicked()
{
    QString dir = QFileDialog::getOpenFileName(this,"请选择源文件的文件","./");
    if(!dir.isEmpty())
    {
        if(ui->cbBox_src->findText(dir)==-1)
        {
            ui->cbBox_src->addItem(dir);
        }
        ui->cbBox_src->setCurrentIndex(ui->cbBox_src->findText(dir));
    }
}

void analyse::on_btn_show_clicked()
{

}


