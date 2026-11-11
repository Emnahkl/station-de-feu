#include "appshell.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("integration_empl_interv");

    // Une seule connexion Oracle (base SAFESTATION) pour les deux modules
    Connection &c = Connection::createInstance();
    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, QObject::tr("Base de données non ouverte"),
            QObject::tr("Connexion à la base SAFESTATION échouée.\n\n%1\n\n"
                        "Vérifiez : le service Oracle, le DSN ODBC « SAFESTATION » "
                        "(ODBC 64 bits), l'utilisateur safestation / safestation123 "
                        "et que sql/safestation_bd.sql a été exécuté.")
                .arg(c.lastError()));
        return 1;
    }

    AppShell w;
    w.show();
    return a.exec();
}
