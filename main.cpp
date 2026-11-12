#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool ok = c.createconnect();
    MainWindow w;

    if (ok) {
        w.show();
        QMessageBox::information(nullptr, "Base de donnees",
                                 "Connexion reussie a la base Oracle.");
    } else {
        QMessageBox::critical(nullptr, "Base de donnees",
                              "Connexion echouee :\n" + c.lastError());
    }
    return a.exec();
}
