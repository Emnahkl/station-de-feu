#pragma once

#include <QSqlDatabase>
#include <QString>

// Connexion Oracle unique, accessible depuis tous les modules.
class Connection final
{
public:
    static Connection &createInstance();
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

    bool isConfigured() const;
    bool createConnection();
    QSqlDatabase database() const { return m_db; }
    QString lastError() const { return m_lastError; }

private:
    Connection();
    ~Connection();

    QSqlDatabase m_db;
    QString m_lastError;
};
