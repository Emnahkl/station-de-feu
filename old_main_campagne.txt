#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>
#include <QPalette>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("FireStation Manager");

    // Thème clair identique sur Windows / Linux / macOS (même en mode sombre)
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#F4F4F4"));
    pal.setColor(QPalette::WindowText, QColor("#2B2B2B"));
    pal.setColor(QPalette::Base, QColor("#FFFFFF"));
    pal.setColor(QPalette::AlternateBase, QColor("#FFF3E0"));
    pal.setColor(QPalette::Text, QColor("#2B2B2B"));
    pal.setColor(QPalette::Button, QColor("#FFFFFF"));
    pal.setColor(QPalette::ButtonText, QColor("#2B2B2B"));
    pal.setColor(QPalette::ToolTipBase, QColor("#FFFFFF"));
    pal.setColor(QPalette::ToolTipText, QColor("#2B2B2B"));
    pal.setColor(QPalette::Highlight, QColor("#FFE0B2"));
    pal.setColor(QPalette::HighlightedText, QColor("#2B2B2B"));
    pal.setColor(QPalette::PlaceholderText, QColor("#9E9E9E"));
    a.setPalette(pal);

    Connection c;
    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, "FireStation Manager",
                              "Connexion à la base de données impossible.\n" + c.lastError());
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
