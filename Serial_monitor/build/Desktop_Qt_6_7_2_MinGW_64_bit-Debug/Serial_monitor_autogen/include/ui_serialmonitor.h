/********************************************************************************
** Form generated from reading UI file 'serialmonitor.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALMONITOR_H
#define UI_SERIALMONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialMonitor
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QPlainTextEdit *plainTextEdit;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *Bytes_Sent_Label;
    QLineEdit *Bytes_Sent_lineEdit;
    QVBoxLayout *verticalLayout;
    QLabel *Bytes_Rcv_Label;
    QLineEdit *Bytes_Rcv_linEdit;
    QVBoxLayout *verticalLayout_3;
    QLabel *BPS_Label;
    QLCDNumber *BPS_Value;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Write_Button;
    QPushButton *Read_Button;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *File_LineEdit;
    QPushButton *Browse_Button;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *Send_Button;
    QPushButton *Clear_Button;
    QPushButton *Start_Button;

    void setupUi(QWidget *SerialMonitor)
    {
        if (SerialMonitor->objectName().isEmpty())
            SerialMonitor->setObjectName("SerialMonitor");
        SerialMonitor->resize(610, 483);
        verticalLayout_5 = new QVBoxLayout(SerialMonitor);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        plainTextEdit = new QPlainTextEdit(SerialMonitor);
        plainTextEdit->setObjectName("plainTextEdit");
        QFont font;
        font.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font.setPointSize(10);
        font.setBold(true);
        plainTextEdit->setFont(font);

        horizontalLayout_5->addWidget(plainTextEdit);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        Bytes_Sent_Label = new QLabel(SerialMonitor);
        Bytes_Sent_Label->setObjectName("Bytes_Sent_Label");
        Bytes_Sent_Label->setFont(font);
        Bytes_Sent_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(Bytes_Sent_Label);

        Bytes_Sent_lineEdit = new QLineEdit(SerialMonitor);
        Bytes_Sent_lineEdit->setObjectName("Bytes_Sent_lineEdit");
        Bytes_Sent_lineEdit->setMaximumSize(QSize(90, 16777215));
        Bytes_Sent_lineEdit->setFont(font);
        Bytes_Sent_lineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(Bytes_Sent_lineEdit);


        horizontalLayout_4->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        Bytes_Rcv_Label = new QLabel(SerialMonitor);
        Bytes_Rcv_Label->setObjectName("Bytes_Rcv_Label");
        Bytes_Rcv_Label->setFont(font);
        Bytes_Rcv_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(Bytes_Rcv_Label);

        Bytes_Rcv_linEdit = new QLineEdit(SerialMonitor);
        Bytes_Rcv_linEdit->setObjectName("Bytes_Rcv_linEdit");
        Bytes_Rcv_linEdit->setMaximumSize(QSize(90, 16777215));
        Bytes_Rcv_linEdit->setFont(font);
        Bytes_Rcv_linEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(Bytes_Rcv_linEdit);


        horizontalLayout_4->addLayout(verticalLayout);


        verticalLayout_4->addLayout(horizontalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        BPS_Label = new QLabel(SerialMonitor);
        BPS_Label->setObjectName("BPS_Label");
        BPS_Label->setFont(font);
        BPS_Label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(BPS_Label);

        BPS_Value = new QLCDNumber(SerialMonitor);
        BPS_Value->setObjectName("BPS_Value");
        BPS_Value->setMinimumSize(QSize(150, 75));

        verticalLayout_3->addWidget(BPS_Value);


        verticalLayout_4->addLayout(verticalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        Write_Button = new QPushButton(SerialMonitor);
        Write_Button->setObjectName("Write_Button");
        Write_Button->setMaximumSize(QSize(90, 16777215));
        Write_Button->setFont(font);

        horizontalLayout_2->addWidget(Write_Button);

        Read_Button = new QPushButton(SerialMonitor);
        Read_Button->setObjectName("Read_Button");
        Read_Button->setMaximumSize(QSize(90, 16777215));
        Read_Button->setFont(font);

        horizontalLayout_2->addWidget(Read_Button);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        horizontalLayout_5->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label = new QLabel(SerialMonitor);
        label->setObjectName("label");
        label->setFont(font);

        horizontalLayout_3->addWidget(label);

        File_LineEdit = new QLineEdit(SerialMonitor);
        File_LineEdit->setObjectName("File_LineEdit");

        horizontalLayout_3->addWidget(File_LineEdit);

        Browse_Button = new QPushButton(SerialMonitor);
        Browse_Button->setObjectName("Browse_Button");
        Browse_Button->setFont(font);

        horizontalLayout_3->addWidget(Browse_Button);


        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lineEdit = new QLineEdit(SerialMonitor);
        lineEdit->setObjectName("lineEdit");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font1.setBold(true);
        lineEdit->setFont(font1);

        horizontalLayout->addWidget(lineEdit);

        Send_Button = new QPushButton(SerialMonitor);
        Send_Button->setObjectName("Send_Button");
        Send_Button->setFont(font);

        horizontalLayout->addWidget(Send_Button);

        Clear_Button = new QPushButton(SerialMonitor);
        Clear_Button->setObjectName("Clear_Button");
        Clear_Button->setFont(font);

        horizontalLayout->addWidget(Clear_Button);

        Start_Button = new QPushButton(SerialMonitor);
        Start_Button->setObjectName("Start_Button");
        Start_Button->setFont(font);

        horizontalLayout->addWidget(Start_Button);


        verticalLayout_5->addLayout(horizontalLayout);


        retranslateUi(SerialMonitor);

        QMetaObject::connectSlotsByName(SerialMonitor);
    } // setupUi

    void retranslateUi(QWidget *SerialMonitor)
    {
        SerialMonitor->setWindowTitle(QCoreApplication::translate("SerialMonitor", "Form", nullptr));
        Bytes_Sent_Label->setText(QCoreApplication::translate("SerialMonitor", "Bytes Sent", nullptr));
        Bytes_Rcv_Label->setText(QCoreApplication::translate("SerialMonitor", "Bytes Rcv", nullptr));
        BPS_Label->setText(QCoreApplication::translate("SerialMonitor", "Bits Per Sec", nullptr));
        Write_Button->setText(QCoreApplication::translate("SerialMonitor", "F_Write", nullptr));
        Read_Button->setText(QCoreApplication::translate("SerialMonitor", "F_Read", nullptr));
        label->setText(QCoreApplication::translate("SerialMonitor", "Source File", nullptr));
        Browse_Button->setText(QCoreApplication::translate("SerialMonitor", "Browse", nullptr));
        Send_Button->setText(QCoreApplication::translate("SerialMonitor", "Send", nullptr));
        Clear_Button->setText(QCoreApplication::translate("SerialMonitor", "Clear", nullptr));
        Start_Button->setText(QCoreApplication::translate("SerialMonitor", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialMonitor: public Ui_SerialMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALMONITOR_H
