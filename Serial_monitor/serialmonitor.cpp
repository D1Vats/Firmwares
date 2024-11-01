#include "serialmonitor.h"
#include "ui_serialmonitor.h"

#include <QDebug>
#include <QFileDialog>

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
    ui->BPS_Value->setDigitCount(8);

    connect(ui->Send_Button, &QPushButton::clicked, this, &SerialMonitor::sendData);
    connect(ui->Start_Button, &QPushButton::clicked, this, &SerialMonitor::toggleDataPrinting);
    connect(ui->Browse_Button, &QPushButton::clicked, this, &SerialMonitor::browseFileSlot);
    connect(ui->Read_Button, &QPushButton::clicked, this, &SerialMonitor::readFromFlash);
    connect(ui->Write_Button, &QPushButton::clicked, this, &SerialMonitor::readFileSlot);

    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &SerialMonitor::insertData);
    connect(dataTimer, &QTimer::timeout, this, &SerialMonitor::BPS_Display);
    dataTimer->start(10);

    newDataAvailable = false;
}

SerialMonitor::~SerialMonitor()
{
    delete ui;
}

void SerialMonitor::getData(const QByteArray data, qint64 BPS_Value)
{
    this->data = data;
    bps_value = BPS_Value;
}

// void SerialMonitor::getData(const QByteArray data)
// {
//     // ui->plainTextEdit->insertPlainText(data);
//     // QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();
//     // bar->setValue(bar->maximum());
//     this->data = data;
//     //qDebug() << "This Data is: " << this->data;
// }

void SerialMonitor::sendData()
{
    emit dataReady(ui->lineEdit->text().toLocal8Bit());
    ui->lineEdit->clear();
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
        QString receivedData = QString::fromUtf8(data);
        QByteArray sizeArray = QByteArray::number(receivedData.size());
        ui->Bytes_Rcv_linEdit->setText(sizeArray);
        ui->plainTextEdit->insertPlainText(receivedData);
        QScrollBar *bar = ui->plainTextEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
        newDataAvailable = false;
    }
}

void SerialMonitor::browseFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;TAP Files (*.tap)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->File_LineEdit->setText(fileName);
}

void SerialMonitor::readFileSlot()
{
    QString src_file_name = ui->File_LineEdit->text();
    if(src_file_name.isEmpty())
    {
        return;
    }

    QFile file(src_file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QByteArray fileData;
    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        fileData.append(line.toUtf8());
    }
    file.close();
    QByteArray sizeArray = QByteArray::number(fileData.size());
    qDebug() << sizeArray;
    ui->Bytes_Sent_lineEdit->setText(sizeArray);
    emit dataReady(fileData);
}

void SerialMonitor::BPS_Display()
{
    ui->BPS_Value->display(static_cast<int>(bps_value));
}

void SerialMonitor::readFromFlash()
{
    emit dataReady("R");
}

void SerialMonitor::on_Clear_Button_clicked()
{
    ui->plainTextEdit->clear();
    ui->lineEdit->clear();
    ui->BPS_Value->display("");
}
