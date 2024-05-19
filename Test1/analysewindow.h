#ifndef ANALYSEWINDOW_H
#define ANALYSEWINDOW_H

#include <QMainWindow>

namespace Ui {
class AnalyseWindow;
}

class AnalyseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalyseWindow(QWidget *parent = nullptr);
    ~AnalyseWindow();

private:
    Ui::AnalyseWindow *ui;

private slots:
    void on_btn_choose_src_clicked();

    void on_btn_show_clicked();
};

#endif // ANALYSEWINDOW_H
