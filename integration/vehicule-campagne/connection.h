#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

// Connexion à la base de données de FireStation Manager.
// Par défaut : SQLite local (firestation.db) pour que le projet tourne
// sans configuration. Pour Oracle (ODBC), remplacer le driver "QSQLITE"
// par "QODBC" et renseigner la source de données, l'utilisateur et le mot de passe.
class Connection
{
public:
    Connection();
    bool createConnection();
    void closeConnection();
    QString lastError() const { return m_error; }

private:
    bool createSchema();
    bool seedData();

    QSqlDatabase m_db;
    QString m_error;
};

#endif // CONNECTION_H
