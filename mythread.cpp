//
// mythread.cpp
// Author: Youngdeok
// Desc: implementation of the MyThreadBG/UI class
//

#include "mythread.h"
#include "led-test.h"
#include <QDebug>
#include <QMutex>

// Thread1 implementation -------------------------------------
MyThreadBG::MyThreadBG(QString s) : name(s)
{
}

// Override function
// POINT: We overrides the QThread's run() method here
// run() will be called when a thread1 starts
// the code will be shared by all threads
void MyThreadBG::run()
{
    for(int i = 0; i <= 10000; i++) // or infinite loop here!!
    {
        qDebug() << "thread Test" << this->name << " " << i;
    }
}

// Thread2 implementation -------------------------------------
MyThreadUI::MyThreadUI(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}

// Override function
// POINT: run() will be called when a thread2 starts
void MyThreadUI::run()
{
    for(int i = 0; i <= 100; i++) // or infinite loop here!!
    {
        QMutex mutex;
        // prevent other threads from changing the "Stop" value
        mutex.lock();
        if(this->Stop) break;
        mutex.unlock();

        // POINT: emit the signal for the count label
        emit valueChanged(i);

        // for LED .. slowdown the count change, msec
        led_on();
        this->msleep(500);
        led_off();
        this->msleep(500);

    }
}

