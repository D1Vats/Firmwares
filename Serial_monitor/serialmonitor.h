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
    void getData(const QByteArray data, qint64 BSP_VAlue);
    bool newDataAvailable;

signals:
    void dataReady(const QByteArray &data);

private slots:
    void sendData();

    void toggleDataPrinting();
    void insertData();
    void browseFileSlot();
    void readFileSlot();
    void BPS_Display();
    void readFromFlash();

    void on_Clear_Button_clicked();

private:
    Ui::SerialMonitor *ui;
    bool is_Running;
    QTimer *dataTimer;
    QByteArray data;
    QByteArray buffer;
    qint64 bps_value;
};

#endif // SERIALMONITOR_H
