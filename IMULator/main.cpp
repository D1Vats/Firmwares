#include "widget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("IMUlator");
    w.show();
    return a.exec();
}
