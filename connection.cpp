#include "connection.h"

#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QVariant>

Connection::Connection() {}

bool Connection::createConnection()
{
    const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dir + "/firestation.db");

    if (!m_db.open()) {
        m_error = m_db.lastError().text();
        return false;
    }
    QSqlQuery("PRAGMA foreign_keys = ON");

    if (!createSchema())
        return false;
    return seedData();
}

void Connection::closeConnection()
{
    m_db.close();
}

bool Connection::createSchema()
{
    const QStringList ddl = {
        "CREATE TABLE IF NOT EXISTS ZONE_COUVERTURE ("
        " ID_ZONE INTEGER PRIMARY KEY,"
        " NOM TEXT NOT NULL,"
        " REGION TEXT,"
        " SUPERFICIE REAL,"
        " POPULATION INTEGER,"
        " NIVEAU_RISQUE TEXT CHECK (NIVEAU_RISQUE IN ('Faible','Moyen','Élevé')),"
        " TEMPS_INTERVENTION_MOYEN INTEGER)",

        "CREATE TABLE IF NOT EXISTS EMPLOYE ("
        " ID_EMPLOYE INTEGER PRIMARY KEY,"
        " NOM TEXT NOT NULL,"
        " PRENOM TEXT NOT NULL,"
        " GRADE TEXT,"
        " SPECIALITE TEXT)",

        "CREATE TABLE IF NOT EXISTS INTERVENTION ("
        " ID_INTERVENTION INTEGER PRIMARY KEY AUTOINCREMENT,"
        " TYPE TEXT NOT NULL,"
        " ADRESSE TEXT,"
        " DATE_HEURE TEXT NOT NULL,"
        " ID_ZONE INTEGER REFERENCES ZONE_COUVERTURE(ID_ZONE),"
        " GRAVITE TEXT)",

        "CREATE TABLE IF NOT EXISTS CAMPAGNE ("
        " ID_CAMPAGNE INTEGER PRIMARY KEY,"
        " TITRE TEXT NOT NULL,"
        " THEME TEXT NOT NULL,"
        " DATE_CAMPAGNE TEXT NOT NULL,"
        " LIEU TEXT NOT NULL,"
        " PUBLIC_CIBLE TEXT NOT NULL,"
        " NB_PARTICIPANTS INTEGER NOT NULL DEFAULT 0 CHECK (NB_PARTICIPANTS >= 0),"
        " ID_ZONE INTEGER NOT NULL REFERENCES ZONE_COUVERTURE(ID_ZONE),"
        " ID_EMPLOYE INTEGER NOT NULL REFERENCES EMPLOYE(ID_EMPLOYE))",

        "CREATE INDEX IF NOT EXISTS IDX_INTER_ZONE_DATE ON INTERVENTION(ID_ZONE, DATE_HEURE)",
        "CREATE INDEX IF NOT EXISTS IDX_CAMP_ZONE ON CAMPAGNE(ID_ZONE)"
    };

    QSqlQuery q;
    for (const QString &s : ddl) {
        if (!q.exec(s)) {
            m_error = q.lastError().text();
            return false;
        }
    }
    return true;
}

// Données de démonstration (uniquement si la base est vide) :
// zones du Grand Tunis, quelques employés, un historique d'interventions
// et des campagnes, pour pouvoir tester le ciblage et l'évaluation d'impact.
bool Connection::seedData()
{
    QSqlQuery q("SELECT COUNT(*) FROM ZONE_COUVERTURE");
    if (q.next() && q.value(0).toInt() > 0)
        return true;

    m_db.transaction();

    struct Z { int id; const char *nom; const char *region; double sup; int pop; const char *risque; int temps; int tauxMensuel; };
    const Z zones[] = {
        {1, "Bab El Bhar", "Tunis", 3.2, 45000, "Moyen", 7, 4},
        {2, "La Marsa", "Tunis Nord", 25.0, 92000, "Élevé", 12, 6},
        {3, "Ariana Ville", "Ariana", 18.5, 114000, "Élevé", 11, 7},
        {4, "Le Bardo", "Tunis Ouest", 12.0, 73000, "Moyen", 9, 4},
        {5, "Ben Arous", "Ben Arous", 22.0, 88000, "Élevé", 14, 6},
        {6, "La Goulette", "Tunis Est", 9.5, 45000, "Faible", 10, 2},
        {7, "El Menzah", "Tunis Nord", 7.8, 60000, "Faible", 8, 2},
        {8, "Sidi Hassine", "Tunis Sud", 30.0, 110000, "Élevé", 16, 8}
    };

    q.prepare("INSERT INTO ZONE_COUVERTURE VALUES (?,?,?,?,?,?,?)");
    for (const Z &z : zones) {
        q.addBindValue(z.id);
        q.addBindValue(QString::fromUtf8(z.nom));
        q.addBindValue(QString::fromUtf8(z.region));
        q.addBindValue(z.sup);
        q.addBindValue(z.pop);
        q.addBindValue(QString::fromUtf8(z.risque));
        q.addBindValue(z.temps);
        if (!q.exec()) { m_error = q.lastError().text(); m_db.rollback(); return false; }
    }

    const char *emp[][4] = {
        {"Ben Ali", "Ahmed", "Lieutenant", "Prévention"},
        {"Trabelsi", "Youssef", "Adjudant", "Premiers secours"},
        {"Mabrouk", "Sana", "Capitaine", "Prévention"},
        {"Jaziri", "Amine", "Sergent", "Feux de forêt"}
    };
    q.prepare("INSERT INTO EMPLOYE VALUES (?,?,?,?,?)");
    for (int i = 0; i < 4; ++i) {
        q.addBindValue(i + 1);
        for (int j = 0; j < 4; ++j) q.addBindValue(QString::fromUtf8(emp[i][j]));
        if (!q.exec()) { m_error = q.lastError().text(); m_db.rollback(); return false; }
    }

    // Campagnes de démonstration (id, titre, thème, date, lieu, public, participants, zone, employé)
    struct C { int id; const char *titre; const char *theme; const char *date; const char *lieu; const char *pub; int nb; int zone; int emp; };
    const C camps[] = {
        {1, "Écoles sans risque", "Sécurité incendie", "2026-02-10", "École primaire Rue de Marseille", "Écoles", 180, 1, 1},
        {2, "Forêt protégée", "Feux de forêt", "2026-03-15", "Parc de Gammarth", "Habitants", 240, 2, 4},
        {3, "Gestes qui sauvent", "Premiers secours", "2026-04-22", "Zone industrielle Ben Arous", "Entreprises", 95, 5, 2},
        {4, "Cuisine en sécurité", "Accidents domestiques", "2026-05-18", "Maison de la culture Le Bardo", "Habitants", 60, 4, 3},
        {5, "Au travail, prévenir", "Sécurité incendie", "2026-06-05", "Technopôle El Ghazala", "Entreprises", 130, 3, 1}
    };
    q.prepare("INSERT INTO CAMPAGNE VALUES (?,?,?,?,?,?,?,?,?)");
    for (const C &c : camps) {
        q.addBindValue(c.id);
        q.addBindValue(QString::fromUtf8(c.titre));
        q.addBindValue(QString::fromUtf8(c.theme));
        q.addBindValue(QString::fromUtf8(c.date));
        q.addBindValue(QString::fromUtf8(c.lieu));
        q.addBindValue(QString::fromUtf8(c.pub));
        q.addBindValue(c.nb);
        q.addBindValue(c.zone);
        q.addBindValue(c.emp);
        if (!q.exec()) { m_error = q.lastError().text(); m_db.rollback(); return false; }
    }

    // Historique d'interventions (générateur déterministe) : baisse après
    // une campagne dans la zone, pour illustrer l'évaluation d'impact.
    const char *types[] = {"Incendie", "Accident de la route", "Secours à personne", "Feu de forêt", "Fuite de gaz"};
    const char *grav[] = {"Faible", "Moyenne", "Élevée"};
    unsigned int seed = 12345;
    auto rnd = [&seed](int n) { seed = seed * 1103515245u + 12345u; return int((seed >> 16) % unsigned(n)); };

    q.prepare("INSERT INTO INTERVENTION (TYPE, ADRESSE, DATE_HEURE, ID_ZONE, GRAVITE) VALUES (?,?,?,?,?)");
    const QDate debut(2025, 1, 1), fin(2026, 9, 30);
    for (const Z &z : zones) {
        QDate dateCampagne;
        for (const C &c : camps)
            if (c.zone == z.id) dateCampagne = QDate::fromString(QString::fromLatin1(c.date), Qt::ISODate);

        for (QDate m(debut.year(), debut.month(), 1); m <= fin; m = m.addMonths(1)) {
            int taux = z.tauxMensuel;
            if (dateCampagne.isValid() && m > dateCampagne)
                taux = qMax(1, taux * 55 / 100);      // effet de la prévention
            const int n = qMax(0, taux - 1 + rnd(3));
            for (int k = 0; k < n; ++k) {
                const QDate d(m.year(), m.month(), 1 + rnd(m.daysInMonth()));
                if (d > fin) continue;
                const QDateTime dt(d, QTime(rnd(24), rnd(60)));
                q.addBindValue(QString::fromUtf8(types[rnd(5)]));
                q.addBindValue(QString::fromUtf8(z.nom));
                q.addBindValue(dt.toString(Qt::ISODate));
                q.addBindValue(z.id);
                q.addBindValue(QString::fromUtf8(grav[rnd(3)]));
                if (!q.exec()) { m_error = q.lastError().text(); m_db.rollback(); return false; }
            }
        }
    }

    return m_db.commit();
}
