#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class Connection
{
public:
    Connection();
    bool createconnect();
    QString lastError() const { return erreur; }

private:
    QString erreur;
};

#endif
