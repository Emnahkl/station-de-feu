#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logo_icone.png"));

    if (!Connexion::ouvrir()) {
        QMessageBox::critical(nullptr, "Base de données",
                              "Impossible d'ouvrir la base de données.");
        return 1;
    }

    MainWindow w;
    w.showMaximized();
    w.verifierAlertes(true);   // popup au démarrage s'il y a des alertes
    return a.exec();
}
