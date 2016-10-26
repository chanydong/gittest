#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include "led-test.h"
#include <QColor>
#include <QColorDialog>

// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Styling
    // QColor col = QColorDialog::getColor(Qt::green, this);
    QColor col = Qt::green;
    if(col.isValid()) {
        QString qss = QString("background-color: %1").arg(col.name());
        ui->btnLedLoopTest->setStyleSheet(qss);
    }
    ui->btnGetState->setStyleSheet("background-color: rgb(255,255,0);");

    // create mThread from MyThread2
     mThreadUI = new MyThreadUI(this);

    // signal & slot connect!
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            ui->progressBar, SLOT(setValue(int)));

    // signal & slot connect!
    connect(mThreadUI, SIGNAL(valueChanged(int)),
                this, SLOT(onValueChanged(int)));
    connect(mThreadUI, SIGNAL(valueChanged(int)),
            ui->progressBar, SLOT(setValue(int)));

    // POINT: led device open
    device_init();
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
    // POINT: led device close
    device_close();
}

// POINT: custom function for the SLOT
void MainWindow::onValueChanged(int count)
{
    ui->label->setText(QString::number(count));
    ui->lcdNumber->display(count);
}

// for the Thread Test --------
// POINT: button events!
void MainWindow::on_startButton_clicked()
{
    mThreadUI->Stop = false;
    mThreadUI->start();
}

void MainWindow::on_stopButton_clicked()
{
    mThreadUI->Stop = true;
}

// for LED Test from the library --------
void MainWindow::on_btnLedOff_clicked()
{
    std::cout << "led on test\n";
    led_off();
}
// --------------------------------------
void MainWindow::on_btnLedOn_clicked()
{
    std::cout << "led on test\n";
    led_on();
}
// --------------------------------------
void MainWindow::on_btnLedLoopTest_clicked()
{
    std::cout << "led on/off for loop test(5times)\n";
    for(int i=0;i<5;i++) {
        led_on();
        sleep(1);
        led_off();
        sleep(1);
    }
}
// --------------------------------------
void MainWindow::on_btnGetState_clicked()
{
    int state;
    state = getLed3State();
    ui->lineEdit->setText(QString::number(state));
}
