#include "connection.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

Connection::Connection() {}

bool Connection::createconnect()
{
    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        m_err = "Le pilote Qt « QODBC » est introuvable (module Qt SQL / ODBC).";
        qDebug() << m_err;
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet_bd");   // nom du DSN ODBC (Source de donnees ODBC 64 bits)
    db.setUserName("projet");
    db.setPassword("projet123");

    if (db.open())
        return true;

    m_err = db.lastError().text();
    qDebug() << "Erreur de connexion :" << m_err;
    return false;
}
