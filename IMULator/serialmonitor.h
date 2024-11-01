#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QWidget>
#include <QByteArray>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QTimer>

namespace Ui {
class SerialMonitor;
}

class SerialMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit SerialMonitor(QWidget *parent = nullptr);
    ~SerialMonitor();
    void getData(const QByteArray data);
    bool newDataAvailable;

signals:
    void dataReady(const QByteArray &data);

private slots:
    void sendData();
    void rawGyroSlot();
    void rawAccelSlot();
    void rawMagSlot();
    void calGyroSlot();
    void calAccelSlot();
    void calMagSlot();
    void Eulers_Slot();
    void Quternion_Slot();
    void toggleDataPrinting();
    void insertData();

    void on_Clear_Button_clicked();

private:
    Ui::SerialMonitor *ui;
    bool is_Running;
    QTimer *dataTimer;
    QByteArray data;
    QByteArray buffer;
};

#endif // SERIALMONITOR_H
