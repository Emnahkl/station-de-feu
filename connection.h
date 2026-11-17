#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

// One Oracle connection shared by every module. Connection details are read
// from the process environment so credentials never live in source control.
class Connection
{
public:
    Connection();
    bool createConnection();
    void closeConnection();
    QString lastError() const { return m_error; }

private:
    QSqlDatabase m_db;
    QString m_error;
};

#endif // CONNECTION_H
