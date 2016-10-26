#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <strstream>
#include <iostream>

#include <mythread.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    // define a thread from the MyThreadUI
    MyThreadUI *mThreadUI;

private slots:
    // POINT: start and stop event handler
    void on_startButton_clicked();
    void on_stopButton_clicked();
    // add a function for the thread
    void onValueChanged(int);

    // for LED Tests
    void on_btnLedOff_clicked();
    void on_btnLedOn_clicked();
    void on_btnLedLoopTest_clicked();

    void on_btnGetState_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
