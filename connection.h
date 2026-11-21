#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

// Connexion UNIQUE a la base Oracle "SAFESTATION" (patron Singleton), partagee par les deux
// modules : Employes, Interventions, Zones de couverture, Equipements et Campagnes.
// Les deux modules utilisent la connexion par defaut de Qt (QSqlDatabase::database()).
class Connection
{
public:
    static Connection& createInstance();   // point d'acces global a l'instance unique
    bool createConnection();               // ouvre la connexion Oracle via ODBC
    bool createconnect() { return createConnection(); }   // ancien nom (module Employes)
    void closeConnection();
    bool isConfigured() const { return true; }        // (module Zones) identifiants fixes ci-dessus
    QSqlDatabase database() const { return m_db; }   // (modules Zones / Equipements)
    QString lastError() const { return m_error; }

private:
    Connection();
    ~Connection();
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    QSqlDatabase m_db;
    QString m_error;
};

#endif // CONNECTION_H
