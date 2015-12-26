#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //file control
    void fnew();
    void fopen();
    void fopenRecent();
    void fclose();
    void fsave();
    void frename();

    //Edit control
    void eundo();
    void eredo();
    void edelete();
    void efind();

    //view control
    void vlf_1_1();
    void vlf_2_1();
    void vlf_1_2();

    //help control
    void help();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
