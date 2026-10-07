#include "ui/MainWindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("TheInnovators");
    QApplication::setApplicationName("FireStationManager");
    QApplication::setStyle("Fusion");

    // Connexion a la base de donnees (necessaire pour la page Campagnes).
    // La partie Carte / zones fonctionne sans base : on avertit seulement.
    Connection c;
    if (!c.createConnection()) {
        QMessageBox::warning(nullptr, "FireStation Manager",
                             "Connexion a la base de donnees impossible.\n"
                             "La page Campagnes ne pourra pas charger ses donnees.\n\n" + c.lastError());
    }

    MainWindow w;
    w.show();
    return app.exec();
}
