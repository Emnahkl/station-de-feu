#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    Connection connection;
    if (!connection.createConnection()) {
        QMessageBox::critical(nullptr, "Base de données", "Connexion à la base des campagnes impossible.\n" + connection.lastError());
        return 1;
    }
    MainWindow window;
    window.showMaximized();
    return app.exec();
}
