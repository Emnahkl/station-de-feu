#include "connexion.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Connexion::ouvrir()
{
    // ---- Version SQLite : aucun serveur à installer, fichier "safestation.db" ----
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("safestation.db");

    // ---- Version Oracle (ODBC) : à utiliser si votre prof l'exige ----
    // QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    // db.setDatabaseName("Source_Projet2A");   // nom de votre source ODBC
    // db.setUserName("votre_user");
    // db.setPassword("votre_mdp");

    if (!db.open()) {
        qDebug() << "Connexion échouée :" << db.lastError().text();
        return false;
    }

    QSqlQuery q;
    return q.exec(
        "CREATE TABLE IF NOT EXISTS EQUIPEMENT ("
        " ID INTEGER PRIMARY KEY,"
        " NOM TEXT NOT NULL,"
        " TYPE_EQUIP TEXT,"
        " QUANTITE INTEGER,"
        " DATE_ACQUISITION TEXT,"
        " ETAT TEXT,"
        " DATE_CONTROLE TEXT,"
        " AFFECTATION TEXT,"
        " SEUIL_ALERTE INTEGER DEFAULT 5)");
}
