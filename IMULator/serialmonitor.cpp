#include "serialmonitor.h"
#include "ui_serialmonitor.h"
#include <QDebug>

SerialMonitor::SerialMonitor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialMonitor)
    ,is_Running(false)
{
    ui->setupUi(this);
    setWindowTitle("Serial Monitor");
    resize(1080, 520);

    ui->Start_Button->setText("Stop");
    ui->Start_Button->setStyleSheet("background-color:rgb(250, 20, 20)");

    connect(ui->Send_Button, &QPushButton::clicked, this, &SerialMonitor::sendData);
    connect(ui->Gyro_radioButton, &QRadioButton::clicked, this, &SerialMonitor::rawGyroSlot);
    connect(ui->Accel_radioButton, &QRadioButton::clicked, this, &SerialMonitor::rawAccelSlot);
    connect(ui->Mag_radioButton, &QRadioButton::clicked, this, &SerialMonitor::rawMagSlot);
    connect(ui->CalGyro_radioButton, &QRadioButton::clicked, this, &SerialMonitor::calGyroSlot);
    connect(ui->CalAccel_radioBUtton, &QRadioButton::clicked, this, &SerialMonitor::calAccelSlot);
    connect(ui->CalMag_radioButton, &QRadioButton::clicked, this, &SerialMonitor::calMagSlot);
    connect(ui->Eulers_radioButton, &QRadioButton::clicked, this, &SerialMonitor::Eulers_Slot);
    connect(ui->Quaternion_radioButton, &QRadioButton::clicked, this, &SerialMonitor::Quternion_Slot);
    connect(ui->Start_Button, &QPushButton::clicked, this, &SerialMonitor::toggleDataPrinting);

    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &SerialMonitor::insertData);
    dataTimer->start(10);

    newDataAvailable = false;
}

SerialMonitor::~SerialMonitor()
{
    delete ui;
}

void SerialMonitor::getData(const QByteArray data)
{
    // ui->plainTextEdit->insertPlainText(data);
    // QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();
    // bar->setValue(bar->maximum());
    this->data = data;
    //qDebug() << "This Data is: " << this->data;
}

void SerialMonitor::sendData()
{
    emit dataReady(ui->lineEdit->text().toLocal8Bit());
    ui->lineEdit->clear();
}

void SerialMonitor::rawGyroSlot()
{
    emit dataReady("Gr");
}

void SerialMonitor::rawAccelSlot()
{
    emit dataReady("Ar");
}

void SerialMonitor::rawMagSlot()
{
    emit dataReady("Mr");
}

void SerialMonitor::calGyroSlot()
{
    emit dataReady("Gc");
}

void SerialMonitor::calAccelSlot()
{
    emit dataReady("Ac");
}

void SerialMonitor::calMagSlot()
{
    emit dataReady("Mc");
}

void SerialMonitor::Eulers_Slot()
{
    emit dataReady("Eu");
}

void SerialMonitor::Quternion_Slot()
{
    emit dataReady("Qr");
}

void SerialMonitor::toggleDataPrinting()
{
    if(is_Running)
    {
        dataTimer->start(10);
        ui->Start_Button->setText("Stop");
        ui->Start_Button->setStyleSheet("background-color:rgb(250, 20, 20)");
    }
    else
    {
        dataTimer->stop();
        ui->Start_Button->setText("Start");
        ui->Start_Button->setStyleSheet("background-color:rgb(0, 139, 139)");
    }
    is_Running = !is_Running;
}

void SerialMonitor::insertData()
{
    if(newDataAvailable)
    {
        buffer.append(data);

        int lastIndex = buffer.lastIndexOf('\n');
        if(lastIndex != -1)
        {
            QByteArray completeData = buffer.left(lastIndex + 1);
            buffer.remove(0, lastIndex + 1);
            ui->plainTextEdit->appendPlainText(completeData);
        }
        else
        {
            ui->plainTextEdit->moveCursor(QTextCursor::End);
            ui->plainTextEdit->appendPlainText(buffer);
            buffer.clear();
        }
        QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
        //qDebug() << "Plain data is: " << data;
        newDataAvailable = false;
    }
}

void SerialMonitor::on_Clear_Button_clicked()
{
    ui->plainTextEdit->clear();
    ui->lineEdit->clear();
}

