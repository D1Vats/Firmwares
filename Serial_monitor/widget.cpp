#include "widget.h"
#include "./ui_widget.h"

#include <QDebug>

#include <chrono>

static constexpr std::chrono::seconds kWriteTimeout = std::chrono::seconds{5};

static const char blankString[] = QT_TRANSLATE_NOOP("Settings", "N/A");

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,monitor(new SerialMonitor())
    ,m_IntValidator(new QIntValidator(0, 4000000, this))
    ,m_Serial(new QSerialPort(this))
    ,m_Status(new QLabel(this))
    ,m_Timer(new QTimer(this))
{
    ui->setupUi(this);
    qApp->setStyleSheet(readFile(":/Styles/DarkTheme.css"));

    ui->BaoudRate_ComboBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->radioButton, &QRadioButton::toggled, this, &Widget::radioButton_toggled);
    connect(ui->Monitor_Button, &QPushButton::clicked, this, &Widget::monitorButton_Clicked);
    // connect(ui->Plotter_Button, &QPushButton::clicked, this, &Widget::plotterButton_Clicked);
    // connect(ui->Modbus_Button, &QPushButton::clicked, this, &Widget::modbusButton_Clicked);

    connect(ui->Apply_Button, &QPushButton::clicked, this, &Widget::apply);
    connect(ui->ComPort_ComboBox, &QComboBox::currentIndexChanged, this, &Widget::showPortInfo);
    connect(ui->BaoudRate_ComboBox, &QComboBox::currentIndexChanged, this, &Widget::checkCustomBaudRatePolicy);
    connect(ui->ComPort_ComboBox, &QComboBox::currentIndexChanged, this, &Widget::checkCustomDevicePathPolicy);
    connect(ui->Connect_Button, &QPushButton::clicked, this, &Widget::openSerialPort);
    connect(ui->Disconnect_Button, &QPushButton::clicked, this, &Widget::closeSerialPort);


    connect(m_Serial, &QSerialPort::errorOccurred, this, &Widget::handleError);
    connect(m_Timer, &QTimer::timeout, this, &Widget::handleWriteTimeout);
    m_Timer->setSingleShot(true);

    //connect(m_Serial, &QSerialPort::readyRead, this, &Widget::timer);
    connect(m_Serial, &QSerialPort::readyRead, this, &Widget::readData);
    connect(m_Serial, &QSerialPort::bytesWritten, this, &Widget::handleBytesWritten);
    connect(monitor, &SerialMonitor::dataReady, this, &Widget::writeData);
    // connect(plotter, &SerialPlotter::plotterDataReady, this, &Widget::writeData);
    //connect(&speedTimer, &QTimer::timeout, this, &Widget::calculateSpeed);
    speedTimer.start();

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
}

Widget::~Widget()
{
    delete monitor;
    delete ui;
}

Widget::Settings Widget::settings() const
{
    return m_CurrentSettings;
}

QString Widget::readFile(QString path)
{
    QFile file(path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}

void Widget::fillPortsParameters()
{
    // BaudRate Parameter setUp
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->BaoudRate_ComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->BaoudRate_ComboBox->addItem("Custom");
    ui->BaoudRate_ComboBox->setCurrentIndex(2);

    // Data Bits Parameter setUp
    ui->DataBits_ComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->DataBits_ComboBox->addItem(QStringLiteral("6"), QSerialPort::Data7);
    ui->DataBits_ComboBox->addItem(QStringLiteral("7"), QSerialPort::Data6);
    ui->DataBits_ComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->DataBits_ComboBox->setCurrentIndex(3);

    // Parity Parameter setUp
    ui->Parity_ComboBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->Parity_ComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->Parity_ComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->Parity_ComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->Parity_ComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    // Stop Bits Parameter setUp
    ui->StopBits_ComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->StopBits_ComboBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->StopBits_ComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    // Flow Control Parameter setUp
    ui->FlowControl_ComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->FlowControl_ComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->FlowControl_ComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

}

void Widget::fillPortsInfo()
{
    // Auto scanning of the serial ports connected to the device
    ui->ComPort_ComboBox->clear();

    const QString blankString = tr(::blankString);
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QStringList list;
        const QString description = info.description();
        const QString manufacturer = info.manufacturer();
        const QString serialNumber = info.serialNumber();
        const auto vendorID = info.vendorIdentifier();
        const auto productID = info.productIdentifier();

        list << info.portName()
             <<(!description.isEmpty() ? description : blankString)
             <<(!manufacturer.isEmpty() ? manufacturer : blankString)
             <<(!serialNumber.isEmpty() ? serialNumber :blankString)
             <<info.systemLocation()
             <<(vendorID ? QString::number(vendorID, 16) : blankString)
             <<(productID ? QString::number(productID, 16) :blankString);

        ui->ComPort_ComboBox->addItem(list.constFirst(), list);
    }
}

void Widget::updateSettings()
{
    // Port Info
    m_CurrentSettings.name = ui->ComPort_ComboBox->currentText();

    // Baudrate info
    if(ui->BaoudRate_ComboBox->currentIndex() == 7)
    {
        m_CurrentSettings.baudRate = ui->BaoudRate_ComboBox->currentText().toInt();
    }
    else
    {
        const auto baudRateData = ui->BaoudRate_ComboBox->currentData();
        m_CurrentSettings.baudRate = baudRateData.value<QSerialPort::BaudRate>();
    }
    m_CurrentSettings.stringBaudRate = QString::number(m_CurrentSettings.baudRate);

    //Data bits Info
    const auto dataBitsData = ui->DataBits_ComboBox->currentData();
    m_CurrentSettings.dataBits = dataBitsData.value<QSerialPort::DataBits>();
    m_CurrentSettings.stringDataBits = ui->DataBits_ComboBox->currentText();

    //Parity bit Info
    const auto parityData = ui->Parity_ComboBox->currentData();
    m_CurrentSettings.parity = parityData.value<QSerialPort::Parity>();
    m_CurrentSettings.stringParity = ui->Parity_ComboBox->currentText();


    //Stop Bit Info
    const auto stopBitsData = ui->StopBits_ComboBox->currentData();
    m_CurrentSettings.stopBits = stopBitsData.value<QSerialPort::StopBits>();
    m_CurrentSettings.stringStopBits = ui->StopBits_ComboBox->currentText();

    //Flow Control Info
    const auto flowControlData = ui->FlowControl_ComboBox->currentData();
    m_CurrentSettings.flowControl = flowControlData.value<QSerialPort::FlowControl>();
    m_CurrentSettings.stringFlowControl = ui->FlowControl_ComboBox->currentText();

    m_CurrentSettings.localEchoEnabled = ui->checkBox->isChecked();

}

void Widget::openSerialPort()
{
    const  Settings p = settings();
    m_Serial->setPortName(p.name);
    m_Serial->setBaudRate(p.baudRate);
    m_Serial->setDataBits(p.dataBits);
    m_Serial->setParity(p.parity);
    m_Serial->setStopBits(p.stopBits);
    m_Serial->setFlowControl(p.flowControl);
    if(m_Serial->open(QIODevice::ReadWrite))
    {
        setLocalEchoEnabled(p.localEchoEnabled);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name, p.stringBaudRate, p.stringDataBits, p.stringParity,
                                   p.stringStopBits, p.stringFlowControl));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), m_Serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void Widget::closeSerialPort()
{
    if(m_Serial->isOpen())
    {
        m_Serial->close();
        monitor->newDataAvailable = false;
    }
    showStatusMessage(tr("Disconnected"));
}

void Widget::showStatusMessage(const QString &message)
{
    m_Status->setText(message);
}

void Widget::showWriteError(const QString &message)
{
    QMessageBox::warning(this, tr("Warning"), message);
}

void Widget::setLocalEchoEnabled(bool set)
{
    m_localEChoEnabled = set;
}



void Widget::monitorButton_Clicked()
{
    monitor->show();
    monitor->raise();
    monitor->activateWindow();
    qDebug() << "Hello there!";
}

void Widget::radioButton_toggled()
{
    if(ui->radioButton->isChecked())
    {
        style = readFile(":/Styles/LightTheme.css");
        qApp->setStyleSheet(style);
    }
    else
    {
        style = readFile(":/Styles/DarkTheme.css");
        qApp->setStyleSheet(style);
    }
}

void Widget::apply()
{
    updateSettings();
}

void Widget::showPortInfo(int idx)
{
    if(idx == -1)
    {
        return;
    }
    const QString blankString = tr(::blankString);

    const QStringList list = ui->ComPort_ComboBox->itemData(idx).toStringList();
    ui->Description_Label->setText(tr("Description: %1").arg(list.value(1, blankString)));
    ui->Manufacturer_Label->setText(tr("Manufacturer: %1").arg(list.value(2, blankString)));
    ui->SerialNumber_Label->setText(tr("Serial Number: %1").arg(list.value(3, blankString)));
    ui->Location_Label->setText(tr("Location: %1").arg(list.value(4, blankString)));
    ui->VendorID_Label->setText(tr("Vendor ID: %1").arg(list.value(5, blankString)));
    ui->ProductID_Label->setText(tr("Product ID: %1").arg(list.value(6, blankString)));
}

void Widget::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !ui->BaoudRate_ComboBox->itemData(idx).isValid();

    ui->BaoudRate_ComboBox->setEditable(isCustomBaudRate);

    if(isCustomBaudRate)
    {
        ui->BaoudRate_ComboBox->clearEditText();
        QLineEdit *edit = ui->BaoudRate_ComboBox->lineEdit();
        edit->setValidator(m_IntValidator);
    }
}

void Widget::checkCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !ui->ComPort_ComboBox->itemData(idx).isValid();

    ui->ComPort_ComboBox->setEditable(isCustomPath);
    if(isCustomPath)
    {
        ui->ComPort_ComboBox->clearEditText();
    }
}


void Widget::on_Scan_Button_clicked()
{
    fillPortsInfo();
}

void Widget::handleError(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError)
    {
        QMessageBox::critical(this, tr("Critical Error"), m_Serial->errorString());
        closeSerialPort();
    }
}
void Widget::handleBytesWritten(qint64 bytes)
{
    m_BytesToWrite -= bytes;
    if(m_BytesToWrite == 0)
    {
        m_Timer->stop();
    }
}
void Widget::handleWriteTimeout()
{
    const QString error = tr("Write operation timed out for port %1.\n").arg(m_Serial->portName(), m_Serial->errorString());
    showWriteError(error);
}

// void Widget::timer()
// {
//     speedTimer.start();
// }

void Widget::writeData(const QByteArray &data)
{
    const qint64 written = m_Serial->write(data);
    if(written == data.size())
    {
        m_BytesToWrite +=written;
        m_Timer->start(kWriteTimeout);
    }
    else
    {
        const QString error = tr("Failed to write all data to port %1.\n"
                                 "Error: %2").arg(m_Serial->portName(),
                                       m_Serial->errorString());
        showWriteError(error);
    }
    qDebug() << "The data is: " << data;
}
void Widget::readData()
{
    while(m_Serial->canReadLine())
    {
        QByteArray data = m_Serial->readLine();
        bytesReceived += data.size();
        qDebug() << "The data coming from serial port: " << data.size();
        // Calculate the speed based on elapsed time
        qint64 elapsed = speedTimer.elapsed();
        elapsed /= STM_HAL_DELAY;
        if (elapsed > 0) {  // Avoid division by zero
            bitsPerSecond = (bytesReceived - lastBytesReceived) * 8 * 1000 / elapsed;
            lastBytesReceived = bytesReceived;
            qDebug() << "Current data speed:" << bitsPerSecond << "bits per second";

            // Reset the timer for the next burst
            speedTimer.restart();
        }
        monitor->getData(data, bitsPerSecond);
        monitor->newDataAvailable = true;
        //qDebug() << data ;
        //     QString line = QString::fromLatin1(data);
        //     QStringList value = line.trimmed().split('\t');

        //     if(value.size()  == 3)
        //     {
        //         bool ok1, ok2, ok3;
        //         float x_Value = value[0].toFloat(&ok1);
        //         float y_Value = value[1].toFloat(&ok2);
        //         float z_Value = value[2].toFloat(&ok3);
        //         if(ok1 && ok2 && ok3)
        //         {
        //             plotter->updateData(x_Value, y_Value, z_Value);
        //             plotter->newDataAvailable = true;
        //         }
        //         else
        //         {
        //             qDebug() << "There is something wrong";
        //         }
        //     }
        //     else if(value.size() == 1)
        //     {
        //         float singleValue = bitsPerSecond;
        //         plotter->updateData(singleValue);
        //         plotter->newDataAvailable = true;
        //     }
    }
}
