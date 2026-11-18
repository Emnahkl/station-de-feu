#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Fire Station");
    MainWindow w;
    w.resize(1480, 920);
    w.show();
    return app.exec();
}
