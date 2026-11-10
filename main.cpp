#include "mainwindow.h"
#include "integ1/connexion.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Fire Station");
    QCoreApplication::setApplicationName("GestionVehicules");
    app.setStyle("Fusion");

    if (!Connexion::ouvrir(nullptr, true))
        return 1;

    MainWindow window;
    window.showMaximized();
    return app.exec();
}
