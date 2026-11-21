// Module « Gestion des campagnes de sensibilisation » - version finale autonome (Faouzene)
// Meme code que dans l'integration finale du projet Fire Station (base Oracle SAFESTATION).
#include "campagneswindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("campagnes_sensibilisation");

    Connection &c = Connection::createInstance();
    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, "Base de données non ouverte",
                              "Connexion à la base SAFESTATION échouée.\n\n" + c.lastError());
        return 1;
    }

    CampagnesWindow w;
    w.setWindowTitle("Fire Station - Campagnes de sensibilisation");
    // Version autonome : les autres pages du menu sont dans le projet d'integration
    QObject::connect(w.menu(), &MenuLateral::naviguer, &w, [&w](MenuLateral::Page p) {
        if (p != MenuLateral::Campagnes)
            QMessageBox::information(&w, "Fire Station",
                                     "Cette page fait partie de l'application intégrée (branche d'intégration).");
        w.marquerMenu();
    });
    QObject::connect(w.menu(), &MenuLateral::quitter, &a, &QApplication::quit);
    w.resize(1366, 800);
    w.show();
    return a.exec();
}
