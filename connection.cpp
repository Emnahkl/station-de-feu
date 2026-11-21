#include "connection.h"

#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QSqlError>
#include <QStringList>
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
    if (m_db.isOpen())   // deja ouverte (appelee aussi par le module Zones)
        return true;

    const QString dsn  = qEnvironmentVariable("SAFESTATION_DSN",      SOURCE_ODBC);
    const QString user = qEnvironmentVariable("SAFESTATION_USER",     UTILISATEUR);
    const QString pwd  = qEnvironmentVariable("SAFESTATION_PASSWORD", MOT_DE_PASSE);

    // 1) Source ODBC « SAFESTATION » si elle existe sur le PC
    m_db.setDatabaseName(dsn);
    m_db.setUserName(user);
    m_db.setPassword(pwd);
    if (m_db.open()) {
        qDebug() << "Connexion établie (DSN" << dsn << ")";
        return true;
    }
    const QString erreurDsn = m_db.lastError().text();

    // 2) Sans DSN : pilote ODBC Oracle installe + base XEPDB1 (Oracle 21c XE)
    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBCINST.INI\\ODBC Drivers",
                  QSettings::NativeFormat);
    QStringList pilotes;
    for (const QString &d : reg.childKeys())
        if (d.startsWith("Oracle", Qt::CaseInsensitive))
            pilotes << d;
    if (pilotes.isEmpty())
        pilotes << "Oracle dans OraDB21Home1" << "Oracle in OraDB21Home1";

    // Identifiants uniquement dans la chaine de connexion : sinon QODBC les ajoute
    // une seconde fois a la fin et le pilote Oracle refuse (ORA-01017).
    m_db.setUserName(QString());
    m_db.setPassword(QString());
    QStringList essais;
    const QStringList services = { "localhost:1521/XEPDB1", "localhost:1521/XE" };
    for (const QString &pilote : pilotes) {
        for (const QString &service : services) {
            m_db.setDatabaseName(QString("DRIVER={%1};DBQ=%2;UID=%3;PWD=%4;")
                                     .arg(pilote, service, user, pwd));
            if (m_db.open()) {
                qDebug() << "Connexion établie (" << pilote << service << ")";
                return true;
            }
            essais << service + " : " + m_db.lastError().text();
        }
    }
    m_error = "DSN " + dsn + " : " + erreurDsn + "\nSans DSN (" + pilotes.join(", ") + ") :\n"
              + essais.join("\n");
    QFile log(QCoreApplication::applicationDirPath() + "/connexion_log.txt");
    if (log.open(QIODevice::WriteOnly | QIODevice::Text))
        QTextStream(&log) << m_error << "\n";
    qDebug() << "Échec de la connexion :" << m_error;
    return false;
}

void Connection::closeConnection()
{
    if (m_db.isOpen())
        m_db.close();
}
