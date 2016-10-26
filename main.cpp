#include "mainwindow.h"
#include <mythread.h>
#include <QApplication>
#include <QDebug>
#include "led-test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    qDebug() << "Hello Debug Message!";

    // POINT: to create background threads from MyThreadBG and start!!
    //MyThreadBG mThreadBG1("A"), mThreadBG2("B"), mThreadBG3("C");

    //mThreadBG1.start();  // --> call run() method!!
    //mThreadBG2.start();
    //mThreadBG3.start();


    // UI loop handler
    return a.exec();
}
