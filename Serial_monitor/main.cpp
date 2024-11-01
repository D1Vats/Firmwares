#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget w;
    w.setWindowTitle("Serial Monitor");
    w.show();
    return app.exec();
}
