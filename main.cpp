#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Fire Station");
    QCoreApplication::setApplicationName("GestionVehicules");
    app.setStyle("Fusion");

    MainWindow window;
    window.showMaximized();
    return app.exec();
}
