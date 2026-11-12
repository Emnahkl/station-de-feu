#include "connection.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QSqlError>

namespace {
const QString kConnectionName = QStringLiteral("firestation_oracle");

QString configurationFile()
{
    const QString explicitPath = qEnvironmentVariable("FIRESTATION_CONFIG");
    if (!explicitPath.isEmpty())
        return explicitPath;

    // In Qt Creator, the executable is under build/<kit>/debug. Keep the
    // local credentials file beside the project, outside the source files.
    return QDir::cleanPath(QDir(QCoreApplication::applicationDirPath())
                               .filePath(QStringLiteral("../../../database/connection.ini")));
}

QString settingOrEnvironment(const char *environmentName, const QString &settingName)
{
    const QString environmentValue = qEnvironmentVariable(environmentName);
    if (!environmentValue.isEmpty())
        return environmentValue;

    QSettings settings(configurationFile(), QSettings::IniFormat);
    return settings.value(settingName).toString().trimmed();
}
}

Connection::Connection()
{
    if (QSqlDatabase::contains(kConnectionName))
        m_db = QSqlDatabase::database(kConnectionName, false);
    else
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QODBC"), kConnectionName);
}

Connection::~Connection()
{
    if (m_db.isValid() && m_db.isOpen())
        m_db.close();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(kConnectionName);
}

Connection &Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::isConfigured() const
{
    return !settingOrEnvironment("FIRESTATION_DB_USER", QStringLiteral("oracle/user")).isEmpty()
        && !settingOrEnvironment("FIRESTATION_DB_PASSWORD", QStringLiteral("oracle/password")).isEmpty();
}

bool Connection::createConnection()
{
    if (m_db.isOpen())
        return true;
    if (!m_db.isValid()) {
        m_lastError = QStringLiteral("Le pilote Qt QODBC est indisponible.");
        qWarning().noquote() << m_lastError;
        return false;
    }

    // When no named DSN is supplied, connect directly through the installed
    // 64-bit Oracle XE ODBC driver. This avoids requiring a separately-created
    // Windows ODBC alias on every development machine.
    const QString dsn = settingOrEnvironment("FIRESTATION_DSN", QStringLiteral("oracle/dsn"));
    const QString user = settingOrEnvironment("FIRESTATION_DB_USER", QStringLiteral("oracle/user"));
    const QString password = settingOrEnvironment("FIRESTATION_DB_PASSWORD", QStringLiteral("oracle/password"));
    if (user.isEmpty() || password.isEmpty()) {
        m_lastError = QStringLiteral("Identifiants absents : configurez database/connection.ini ou les variables FIRESTATION_DB_USER et FIRESTATION_DB_PASSWORD.");
        qWarning().noquote() << m_lastError;
        return false;
    }

    const QString driver = settingOrEnvironment("FIRESTATION_ODBC_DRIVER", QStringLiteral("oracle/driver"));
    const QString service = settingOrEnvironment("FIRESTATION_DB_SERVICE", QStringLiteral("oracle/service"));
    const QString databaseName = dsn.isEmpty()
        ? QStringLiteral("Driver={%1};DBQ=%2;").arg(driver.isEmpty() ? QStringLiteral("Oracle in XE") : driver,
                                                   service.isEmpty() ? QStringLiteral("XE") : service)
        : dsn;
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qWarning().noquote() << "Connexion Oracle impossible:" << m_lastError;
        return false;
    }

    m_lastError.clear();
    qDebug() << "Connexion Oracle établie via ODBC, cible:" << databaseName;
    return true;
}
