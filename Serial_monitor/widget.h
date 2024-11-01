#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QApplication>
#include <QSerialPort>
#include <QIntValidator>
#include <QSerialPortInfo>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QElapsedTimer>

//Other Widgets header files
#include "serialmonitor.h"

#define STM_HAL_DELAY  1

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString style;

    struct Settings
    {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings() const;

private slots:
    void radioButton_toggled();
    void monitorButton_Clicked();

    void apply();
    void showPortInfo(int idx);
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    void on_Scan_Button_clicked();

    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void handleBytesWritten(qint64 bytes);
    void handleWriteTimeout();
    //void timer();

private:
    Ui::Widget *ui;
    SerialMonitor *monitor = nullptr;
    QIntValidator *m_IntValidator = nullptr;
    Settings m_CurrentSettings;
    QLabel *m_Status = nullptr;
    qint64 m_BytesToWrite = 0;
    qint64 bytesReceived = 0;
    qint64 lastBytesReceived = 0;
    QTimer *m_Timer = nullptr;
    QElapsedTimer speedTimer;;
    QSerialPort *m_Serial = nullptr;
    bool m_localEChoEnabled = false;
    float bitsPerSecond = 0;

private:
    QString readFile(QString path);
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void showStatusMessage(const QString &message);
    void showWriteError(const QString &message);
    void setLocalEchoEnabled(bool set);
};
#endif // WIDGET_H
