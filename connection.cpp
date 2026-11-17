#include "connection.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace {
QString setting(const char *name)
{
    return QString::fromLocal8Bit(qgetenv(name)).trimmed();
}
}

Connection::Connection() = default;

bool Connection::createConnection()
{
    const QString driver = setting("FIRESTATION_QT_SQL_DRIVER").isEmpty()
        ? QStringLiteral("QODBC") : setting("FIRESTATION_QT_SQL_DRIVER");
    const QString dsn = setting("FIRESTATION_ORACLE_DSN");
    const QString user = setting("FIRESTATION_ORACLE_USER");
    const QString password = setting("FIRESTATION_ORACLE_PASSWORD");

    if (dsn.isEmpty() || user.isEmpty() || password.isEmpty()) {
        m_error = QStringLiteral(
            "Configure FIRESTATION_ORACLE_DSN, FIRESTATION_ORACLE_USER and "
            "FIRESTATION_ORACLE_PASSWORD before launching the application.");
        return false;
    }
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        m_error = QStringLiteral("Qt SQL driver '%1' is not available.").arg(driver);
        return false;
    }

    // All module models use Qt's default connection. Keeping one database
    // prevents each page from opening a separate Oracle session.
    const QString connectionName = QSqlDatabase::defaultConnection;
    if (QSqlDatabase::contains(connectionName)) {
        m_db = QSqlDatabase::database(connectionName, false);
    } else {
        m_db = QSqlDatabase::addDatabase(driver, connectionName);
    }

    m_db.setDatabaseName(dsn);
    m_db.setUserName(user);
    m_db.setPassword(password);
    if (!m_db.open()) {
        m_error = m_db.lastError().text();
        return false;
    }

    QSqlQuery check(m_db);
    if (!check.exec(QStringLiteral("SELECT 1 FROM DUAL"))) {
        m_error = check.lastError().text();
        m_db.close();
        return false;
    }

    m_error.clear();
    return true;
}

void Connection::closeConnection()
{
    if (m_db.isValid() && m_db.isOpen())
        m_db.close();
}
