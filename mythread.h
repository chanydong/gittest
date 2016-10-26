#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QString>

// --------------------------------- MyThreadBG (Console)
// for running on the background
class MyThreadBG : public QThread
{
public:
    // constructor
    // set name using initializer
    explicit MyThreadBG(QString s);

    // overriding the QThread's run() method
    void run();
private:
    QString name;
};

// -------------------------------------------- MyThread2(UI update)
// for sending signal to the UI
class MyThreadUI : public QThread
{
    Q_OBJECT
public:
    explicit MyThreadUI(QObject *parent = 0, bool b = false);
    void run();

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;

signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void valueChanged(int);

public slots:
    // none
};



#endif // MYTHREAD_H
