#include "connection.h"

#include <QDebug>
#include <QSqlError>
#include <QtGlobal>

// ======= A MODIFIER SI BESOIN =======
// Source de donnees ODBC 64 bits a creer sur chaque PC (voir LISEZMOI_INTEGRATION.txt) :
//   Data Source Name = SAFESTATION | TNS Service Name = XE | User ID = safestation
// On peut aussi les remplacer sans recompiler avec les variables d'environnement
//   SAFESTATION_DSN, SAFESTATION_USER, SAFESTATION_PASSWORD.
static const char *SOURCE_ODBC  = "SAFESTATION";
static const char *UTILISATEUR  = "safestation";
static const char *MOT_DE_PASSE = "safestation123";   // ne jamais pousser un vrai mot de passe sur GitHub
// ====================================

Connection::Connection()
{
    m_db = QSqlDatabase::addDatabase("QODBC");   // connexion par defaut
}

Connection::~Connection()
{
    closeConnection();
}

Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::createConnection()
{
    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        m_error = "Le pilote Qt « QODBC » est introuvable (module Qt SQL / ODBC).";
        qDebug() << m_error;
        return false;
    }
    m_db.setDatabaseName("SAFESTATION");
    m_db.setUserName("safestation");
    m_db.setPassword("safestation123");
    if (m_db.open()) {
        qDebug() << "Connexion établie (SAFESTATION)";
        return true;
    }
    m_error = m_db.lastError().text();
    qDebug() << "Échec de la connexion :" << m_error;
    return false;
}

void Connection::closeConnection()
{
    if (m_db.isOpen())
        m_db.close();
}
