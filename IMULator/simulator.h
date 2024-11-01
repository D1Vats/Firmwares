#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QWidget>

namespace Ui {
class Simulator;
}

class Simulator : public QWidget
{
    Q_OBJECT

public:
    explicit Simulator(QWidget *parent = nullptr);
    ~Simulator();

private:
    Ui::Simulator *ui;
};

#endif // SIMULATOR_H
