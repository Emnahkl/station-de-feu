#include "connection.h"
#include <QSqlError>
#include <QSettings>
#include <QStringList>

Connection::Connection() {}

bool Connection::createconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // Recherche du driver ODBC Oracle installe sur le PC
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Drivers",
                  QSettings::NativeFormat);
    QString driver = "Oracle dans OraDB21Home1";
    for (const QString &d : reg.childKeys())
        if (d.startsWith("Oracle", Qt::CaseInsensitive))
            driver = d;

    // Connexion directe a la base XEPDB1 (utilisateur FIRESTATION)
    db.setDatabaseName("DRIVER={" + driver + "};DBQ=localhost:1521/XEPDB1;"
                       "UID=FIRESTATION;PWD=esprit2026;");

    if (db.open())
        return true;

    erreur = db.lastError().text();
    return false;
}
