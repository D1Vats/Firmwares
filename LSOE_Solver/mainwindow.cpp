#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->Submit_Button, SIGNAL(clicked()), this, SLOT(respond()));
    connect(ui->Submit_Button, SIGNAL(clicked()), this, SLOT(answer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::respond()
{
    float temp_1;
    float eq_arr_1[4];
    float eq_arr_2[4];
    float eq_arr_3[4];


    QString equation_1[4];

    equation_1[0] = ui->X_1_lineEdit->text();
    equation_1[1] = ui->Y_1_lineEdit->text();
    equation_1[2] = ui->Z_1_lineEdit->text();
    equation_1[3] = ui->K_1_lineEdit->text();

    QString equation_2[4];

    equation_2[0] = ui->X_2_lineEdit->text();
    equation_2[1] = ui->Y_2_lineEdit->text();
    equation_2[2] = ui->Z_2_lineEdit->text();
    equation_2[3] = ui->K_2_lineEdit->text();

    QString equation_3[4];

    equation_3[0] = ui->X_3_lineEdit->text();
    equation_3[1] = ui->Y_3_lineEdit->text();
    equation_3[2] = ui->Z_3_lineEdit->text();
    equation_3[3] = ui->K_3_lineEdit->text();

    temp_1 = equation_1[0].toFloat();

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_1[i] = equation_1[i].toFloat();
        eq_arr_1[i] = eq_arr_1[i] / temp_1;
    }

    temp_1 = equation_2[0].toFloat();

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_2[i] = equation_2[i].toFloat();
        eq_arr_2[i] = eq_arr_2[i] - (temp_1 * eq_arr_1[i]);
    }

    temp_1 = equation_3[0].toFloat();

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_3[i] = equation_3[i].toFloat();
        eq_arr_3[i] = eq_arr_3[i] - (temp_1 * eq_arr_1[i]);
    }

    temp_1 = eq_arr_2[1];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_2[i] = eq_arr_2[i] / temp_1;
    }

    temp_1 = eq_arr_3[1];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_3[i] = eq_arr_3[i] - (temp_1 * eq_arr_2[i]);
    }

    temp_1 = eq_arr_3[2];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_3[i] = eq_arr_3[i] / temp_1;
    }

    temp_1 = eq_arr_1[2];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_1[i] = eq_arr_1[i] - (temp_1 * eq_arr_3[i]);
    }

    temp_1 = eq_arr_2[2];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_2[i] = eq_arr_2[i] - (temp_1 * eq_arr_3[i]);
    }

    temp_1 = eq_arr_1[1];

    for(int8_t i = 0; i < 4; i++)
    {
        eq_arr_1[i] = eq_arr_1[i] - (temp_1 * eq_arr_2[i]);
    }

    arr[0] = eq_arr_1[3];
    arr[1] = eq_arr_2[3];
    arr[2] = eq_arr_3[3];

}

void MainWindow::answer()
{

    double ans[3];
    for(int8_t i = 0; i < 3; i++)
    {
        ans[i] = double(arr[i]);
    }

    ui->ValX_Label->setNum(ans[0]);
    ui->ValY_Label->setNum(ans[1]);
    ui->ValZ_Label->setNum(ans[2]);
}


