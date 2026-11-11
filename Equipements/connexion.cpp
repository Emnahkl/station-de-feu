#include "connexion.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace {
const QString ODBC_DSN = QStringLiteral("SafeStationOracle");
QString connectionError;
}

QString Connexion::dernierErreur()
{
    return connectionError;
}

bool Connexion::ouvrir()
{
    connectionError.clear();

    if (!QSqlDatabase::drivers().contains(QStringLiteral("QODBC"))) {
        connectionError = QStringLiteral(
            "Le pilote Qt QODBC n'est pas installé dans le kit Qt choisi. "
            "Ajoute le pilote SQL ODBC avec Qt Maintenance Tool.");
        return false;
    }

    bool ok = false;
    const QString username = QInputDialog::getText(
        nullptr, QStringLiteral("Connexion à Oracle"),
        QStringLiteral("Utilisateur Oracle :"), QLineEdit::Normal,
        QStringLiteral("SAFESTATION"), &ok).trimmed();
    if (!ok) {
        connectionError = QStringLiteral("Connexion annulée par l'utilisateur.");
        return false;
    }
    if (username.isEmpty()) {
        connectionError = QStringLiteral("Le nom d'utilisateur Oracle est obligatoire.");
        return false;
    }

    const QString password = QInputDialog::getText(
        nullptr, QStringLiteral("Connexion à Oracle"),
        QStringLiteral("Mot de passe Oracle :"), QLineEdit::Password,
        QString(), &ok);
    if (!ok) {
        connectionError = QStringLiteral("Connexion annulée par l'utilisateur.");
        return false;
    }

    // Keep the default Qt connection: the CRUD class creates QSqlQuery objects
    // without an explicit connection, so they must use this same connection.
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QODBC"));
    // The user creates this 64-bit Oracle DSN once in Windows ODBC settings.
    db.setDatabaseName(ODBC_DSN);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open()) {
        connectionError = db.lastError().text();
        qWarning() << "Oracle connection failed:" << connectionError;
        return false;
    }

    // Verify the shared MLD table and all columns used by this CRUD module.
    const QString probeSql = QStringLiteral(
        "SELECT ID_EQUIPEMENT, DESIGNATION, CATEGORIE, QUANTITE, DATE_ACQUISITION, "
        "ETAT, DATE_PROCHAIN_CONTROLE, DATE_PEREMPTION, IMMATRICULATION, SEUIL_ALERTE "
        "FROM EQUIPEMENTS WHERE 1 = 0");
    QSqlQuery probe(db);
    if (!probe.exec(probeSql)) {
        connectionError = QStringLiteral(
            "Connexion Oracle réussie, mais la table EQUIPEMENTS ou ses colonnes sont absentes. "
            "Exécute sql/setup_equipements_oracle.sql dans SQL Developer connecté en SAFESTATION.\n"
            "Détail Oracle: %1").arg(probe.lastError().text());
        qWarning() << connectionError;
        db.close();
        return false;
    }
    return true;
}
