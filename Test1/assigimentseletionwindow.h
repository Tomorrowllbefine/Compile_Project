#ifndef ASSIGIMENTSELETIONWINDOW_H
#define ASSIGIMENTSELETIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class AssigimentSeletionWindow;
}

class AssigimentSeletionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssigimentSeletionWindow(QWidget *parent = 0);
    ~AssigimentSeletionWindow();

private slots:

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::AssigimentSeletionWindow *ui;
};

#endif // ASSIGIMENTSELETIONWINDOW_H
