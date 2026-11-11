#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("firestation_groupe1");

    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, QObject::tr("Base de données non ouverte"),
            QObject::tr("Connexion à Oracle échouée.\n\n%1\n\n"
                        "Vérifiez : le service Oracle, le DSN ODBC « projet_bd » "
                        "(ODBC 64 bits) et l'utilisateur projet / projet123.")
                .arg(c.lastError()));
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
