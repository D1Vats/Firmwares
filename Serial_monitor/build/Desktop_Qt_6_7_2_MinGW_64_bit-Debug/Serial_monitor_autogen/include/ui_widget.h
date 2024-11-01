/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_6;
    QWidget *TitleBar_Widget;
    QHBoxLayout *horizontalLayout_9;
    QLabel *Icon_Label;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *checkBox;
    QRadioButton *radioButton;
    QHBoxLayout *horizontalLayout_8;
    QWidget *GroupBox_Widget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QGroupBox *SerialPortInfo_GroupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QComboBox *ComPort_ComboBox;
    QPushButton *Scan_Button;
    QLabel *Description_Label;
    QLabel *Manufacturer_Label;
    QLabel *SerialNumber_Label;
    QLabel *Location_Label;
    QLabel *VendorID_Label;
    QLabel *ProductID_Label;
    QGroupBox *Parameters_GroupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *BaudRate_Label;
    QComboBox *BaoudRate_ComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *DataBits_Label;
    QComboBox *DataBits_ComboBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *Parity_Label;
    QComboBox *Parity_ComboBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *StopBits_Label;
    QComboBox *StopBits_ComboBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *FlowControl_Label;
    QComboBox *FlowControl_ComboBox;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *Connect_Button;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *Apply_Button;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *Disconnect_Button;
    QSpacerItem *horizontalSpacer_2;
    QWidget *PushButton_Widget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *Monitor_Button;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(720, 420);
        Widget->setMinimumSize(QSize(720, 420));
        Widget->setMaximumSize(QSize(720, 420));
        Widget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_6 = new QVBoxLayout(Widget);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        TitleBar_Widget = new QWidget(Widget);
        TitleBar_Widget->setObjectName("TitleBar_Widget");
        TitleBar_Widget->setMinimumSize(QSize(0, 50));
        TitleBar_Widget->setMaximumSize(QSize(16777215, 50));
        TitleBar_Widget->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 139, 139);"));
        horizontalLayout_9 = new QHBoxLayout(TitleBar_Widget);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        Icon_Label = new QLabel(TitleBar_Widget);
        Icon_Label->setObjectName("Icon_Label");
        Icon_Label->setMinimumSize(QSize(32, 32));
        Icon_Label->setMaximumSize(QSize(32, 32));
        Icon_Label->setStyleSheet(QString::fromUtf8(""));
        Icon_Label->setFrameShape(QFrame::Shape::Box);
        Icon_Label->setFrameShadow(QFrame::Shadow::Plain);
        Icon_Label->setPixmap(QPixmap(QString::fromUtf8(":/Images/M_Im_1.jpg")));
        Icon_Label->setScaledContents(true);

        horizontalLayout_9->addWidget(Icon_Label);

        horizontalSpacer_5 = new QSpacerItem(583, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);

        checkBox = new QCheckBox(TitleBar_Widget);
        checkBox->setObjectName("checkBox");
        QFont font;
        font.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font.setPointSize(12);
        font.setBold(true);
        checkBox->setFont(font);
        checkBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_9->addWidget(checkBox);

        radioButton = new QRadioButton(TitleBar_Widget);
        radioButton->setObjectName("radioButton");
        radioButton->setFont(font);
        radioButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_9->addWidget(radioButton);


        verticalLayout_6->addWidget(TitleBar_Widget);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        GroupBox_Widget = new QWidget(Widget);
        GroupBox_Widget->setObjectName("GroupBox_Widget");
        verticalLayout_4 = new QVBoxLayout(GroupBox_Widget);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        SerialPortInfo_GroupBox = new QGroupBox(GroupBox_Widget);
        SerialPortInfo_GroupBox->setObjectName("SerialPortInfo_GroupBox");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font1.setPointSize(14);
        font1.setBold(true);
        SerialPortInfo_GroupBox->setFont(font1);
        SerialPortInfo_GroupBox->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_3 = new QVBoxLayout(SerialPortInfo_GroupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        ComPort_ComboBox = new QComboBox(SerialPortInfo_GroupBox);
        ComPort_ComboBox->setObjectName("ComPort_ComboBox");
        ComPort_ComboBox->setFont(font);
        ComPort_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_10->addWidget(ComPort_ComboBox);

        Scan_Button = new QPushButton(SerialPortInfo_GroupBox);
        Scan_Button->setObjectName("Scan_Button");
        Scan_Button->setMaximumSize(QSize(90, 30));
        Scan_Button->setFont(font);

        horizontalLayout_10->addWidget(Scan_Button);


        verticalLayout_3->addLayout(horizontalLayout_10);

        Description_Label = new QLabel(SerialPortInfo_GroupBox);
        Description_Label->setObjectName("Description_Label");
        Description_Label->setFont(font);
        Description_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(Description_Label);

        Manufacturer_Label = new QLabel(SerialPortInfo_GroupBox);
        Manufacturer_Label->setObjectName("Manufacturer_Label");
        Manufacturer_Label->setFont(font);
        Manufacturer_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(Manufacturer_Label);

        SerialNumber_Label = new QLabel(SerialPortInfo_GroupBox);
        SerialNumber_Label->setObjectName("SerialNumber_Label");
        SerialNumber_Label->setFont(font);
        SerialNumber_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(SerialNumber_Label);

        Location_Label = new QLabel(SerialPortInfo_GroupBox);
        Location_Label->setObjectName("Location_Label");
        Location_Label->setFont(font);
        Location_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(Location_Label);

        VendorID_Label = new QLabel(SerialPortInfo_GroupBox);
        VendorID_Label->setObjectName("VendorID_Label");
        VendorID_Label->setFont(font);
        VendorID_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(VendorID_Label);

        ProductID_Label = new QLabel(SerialPortInfo_GroupBox);
        ProductID_Label->setObjectName("ProductID_Label");
        ProductID_Label->setEnabled(true);
        ProductID_Label->setFont(font);
        ProductID_Label->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(ProductID_Label);


        horizontalLayout_6->addWidget(SerialPortInfo_GroupBox);

        Parameters_GroupBox = new QGroupBox(GroupBox_Widget);
        Parameters_GroupBox->setObjectName("Parameters_GroupBox");
        Parameters_GroupBox->setFont(font1);
        Parameters_GroupBox->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(Parameters_GroupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        BaudRate_Label = new QLabel(Parameters_GroupBox);
        BaudRate_Label->setObjectName("BaudRate_Label");
        BaudRate_Label->setFont(font);
        BaudRate_Label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(BaudRate_Label);

        BaoudRate_ComboBox = new QComboBox(Parameters_GroupBox);
        BaoudRate_ComboBox->setObjectName("BaoudRate_ComboBox");
        BaoudRate_ComboBox->setFont(font);
        BaoudRate_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(BaoudRate_ComboBox);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        DataBits_Label = new QLabel(Parameters_GroupBox);
        DataBits_Label->setObjectName("DataBits_Label");
        DataBits_Label->setFont(font);
        DataBits_Label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(DataBits_Label);

        DataBits_ComboBox = new QComboBox(Parameters_GroupBox);
        DataBits_ComboBox->setObjectName("DataBits_ComboBox");
        DataBits_ComboBox->setFont(font);
        DataBits_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(DataBits_ComboBox);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        Parity_Label = new QLabel(Parameters_GroupBox);
        Parity_Label->setObjectName("Parity_Label");
        Parity_Label->setFont(font);
        Parity_Label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(Parity_Label);

        Parity_ComboBox = new QComboBox(Parameters_GroupBox);
        Parity_ComboBox->setObjectName("Parity_ComboBox");
        Parity_ComboBox->setFont(font);
        Parity_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(Parity_ComboBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        StopBits_Label = new QLabel(Parameters_GroupBox);
        StopBits_Label->setObjectName("StopBits_Label");
        StopBits_Label->setFont(font);
        StopBits_Label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(StopBits_Label);

        StopBits_ComboBox = new QComboBox(Parameters_GroupBox);
        StopBits_ComboBox->setObjectName("StopBits_ComboBox");
        StopBits_ComboBox->setFont(font);
        StopBits_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(StopBits_ComboBox);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        FlowControl_Label = new QLabel(Parameters_GroupBox);
        FlowControl_Label->setObjectName("FlowControl_Label");
        FlowControl_Label->setFont(font);
        FlowControl_Label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(FlowControl_Label);

        FlowControl_ComboBox = new QComboBox(Parameters_GroupBox);
        FlowControl_ComboBox->setObjectName("FlowControl_ComboBox");
        FlowControl_ComboBox->setFont(font);
        FlowControl_ComboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(FlowControl_ComboBox);


        verticalLayout_2->addLayout(horizontalLayout_5);


        horizontalLayout_6->addWidget(Parameters_GroupBox);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        Connect_Button = new QPushButton(GroupBox_Widget);
        Connect_Button->setObjectName("Connect_Button");
        Connect_Button->setFont(font);
        Connect_Button->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_7->addWidget(Connect_Button);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        Apply_Button = new QPushButton(GroupBox_Widget);
        Apply_Button->setObjectName("Apply_Button");
        Apply_Button->setFont(font);

        horizontalLayout_7->addWidget(Apply_Button);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        Disconnect_Button = new QPushButton(GroupBox_Widget);
        Disconnect_Button->setObjectName("Disconnect_Button");
        Disconnect_Button->setFont(font);
        Disconnect_Button->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_7->addWidget(Disconnect_Button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_7);


        horizontalLayout_8->addWidget(GroupBox_Widget);

        PushButton_Widget = new QWidget(Widget);
        PushButton_Widget->setObjectName("PushButton_Widget");
        verticalLayout_5 = new QVBoxLayout(PushButton_Widget);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        Monitor_Button = new QPushButton(PushButton_Widget);
        Monitor_Button->setObjectName("Monitor_Button");
        Monitor_Button->setMinimumSize(QSize(64, 64));
        Monitor_Button->setMaximumSize(QSize(64, 64));
        Monitor_Button->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(Monitor_Button);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        verticalLayout_5->addLayout(verticalLayout);


        horizontalLayout_8->addWidget(PushButton_Widget);


        verticalLayout_6->addLayout(horizontalLayout_8);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        Icon_Label->setText(QString());
        checkBox->setText(QCoreApplication::translate("Widget", "Local Echo", nullptr));
        radioButton->setText(QCoreApplication::translate("Widget", "Dark", nullptr));
        SerialPortInfo_GroupBox->setTitle(QCoreApplication::translate("Widget", "Serial Port Info", nullptr));
        Scan_Button->setText(QCoreApplication::translate("Widget", "Scan", nullptr));
        Description_Label->setText(QCoreApplication::translate("Widget", "Description:", nullptr));
        Manufacturer_Label->setText(QCoreApplication::translate("Widget", "Manufacturer:", nullptr));
        SerialNumber_Label->setText(QCoreApplication::translate("Widget", "Serial Number:", nullptr));
        Location_Label->setText(QCoreApplication::translate("Widget", "Location:", nullptr));
        VendorID_Label->setText(QCoreApplication::translate("Widget", "Vendor ID:", nullptr));
        ProductID_Label->setText(QCoreApplication::translate("Widget", "Product ID:", nullptr));
        Parameters_GroupBox->setTitle(QCoreApplication::translate("Widget", "Serial Port Parameters", nullptr));
        BaudRate_Label->setText(QCoreApplication::translate("Widget", "BaudRate:", nullptr));
        DataBits_Label->setText(QCoreApplication::translate("Widget", "Data Bits:", nullptr));
        Parity_Label->setText(QCoreApplication::translate("Widget", "Parity:", nullptr));
        StopBits_Label->setText(QCoreApplication::translate("Widget", "Stop Bits", nullptr));
        FlowControl_Label->setText(QCoreApplication::translate("Widget", "Flow control", nullptr));
        Connect_Button->setText(QCoreApplication::translate("Widget", "Connect", nullptr));
        Apply_Button->setText(QCoreApplication::translate("Widget", "Apply", nullptr));
        Disconnect_Button->setText(QCoreApplication::translate("Widget", "Disconnect", nullptr));
        Monitor_Button->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
