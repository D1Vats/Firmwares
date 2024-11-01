#include "simulator.h"
#include "ui_simulator.h"

Simulator::Simulator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Simulator)
{
    ui->setupUi(this);
    setWindowTitle("Simulator");
}

Simulator::~Simulator()
{
    delete ui;
}


