#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QPalette>
#include <QStyleFactory>

#include "connection.h"
#include "logindialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Fire Station");
    QCoreApplication::setApplicationName("FireStation Manager");

    // Thème clair identique sur Windows / macOS / Linux (même en mode sombre)
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#eef0f4"));
    pal.setColor(QPalette::WindowText, QColor("#1f2937"));
    pal.setColor(QPalette::Base, QColor("#ffffff"));
    pal.setColor(QPalette::AlternateBase, QColor("#f5f6f8"));
    pal.setColor(QPalette::Text, QColor("#1f2937"));
    pal.setColor(QPalette::Button, QColor("#ffffff"));
    pal.setColor(QPalette::ButtonText, QColor("#1f2937"));
    pal.setColor(QPalette::ToolTipBase, QColor("#ffffff"));
    pal.setColor(QPalette::ToolTipText, QColor("#1f2937"));
    pal.setColor(QPalette::Highlight, QColor("#fde2e2"));
    pal.setColor(QPalette::HighlightedText, QColor("#1f2937"));
    pal.setColor(QPalette::PlaceholderText, QColor("#9ca3af"));
    app.setPalette(pal);

    QFile qss(":/style.qss");
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text))
        app.setStyleSheet(QString::fromUtf8(qss.readAll()));

    // Base de données commune (modules Équipements et Campagnes)
    Connection connexion;
    if (!connexion.createConnection()) {
        QMessageBox::critical(nullptr, "FireStation Manager",
                              "Connexion à la base de données impossible.\n" + connexion.lastError());
        return 1;
    }

    // 1) Fenêtre de connexion, 2) menu principal
    LoginDialog login;
    if (login.exec() != QDialog::Accepted)
        return 0;

    MainWindow w;
    w.setUtilisateur(login.nomUtilisateur());
    w.showMaximized();
    return app.exec();
}
