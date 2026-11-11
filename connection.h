#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

// Ouvre la connexion Oracle via le DSN ODBC "projet_bd"
class Connection
{
public:
    Connection();
    bool createconnect();
    QString lastError() const { return m_err; }

private:
    QString m_err;
};

#endif // CONNECTION_H
