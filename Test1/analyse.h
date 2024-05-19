#ifndef ANALYSE_H
#define ANALYSE_H

#include <QMainWindow>

namespace Ui {class analyse;}


class analyse : public QMainWindow
{
    Q_OBJECT

public:
    explicit analyse(QWidget *parent = nullptr);
    ~analyse();

private:
    Ui::analyse *ui;

    QWidget *QMainWindow;
private slots:

    void on_btn_choose_src_clicked();

    void on_btn_show_clicked();
};

#endif // ANALYSE_H
