#include "ui/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("TheInnovators");
    QApplication::setApplicationName("FireStationManager");
    QApplication::setStyle("Fusion");

    MainWindow w;
    w.show();
    return app.exec();
}
