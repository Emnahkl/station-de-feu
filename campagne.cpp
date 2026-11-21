#include "campagne.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

QString Campagne::s_erreur;

Campagne::Campagne() = default;

Campagne::Campagne(int id, const QString &titre, const QString &theme, const QDate &date,
                   const QString &lieu, const QString &publicCible, int nbParticipants,
                   int idZone, const QList<int> &animateurs)
    : m_id(id), m_titre(titre), m_theme(theme), m_lieu(lieu), m_publicCible(publicCible),
      m_date(date), m_nbParticipants(nbParticipants), m_idZone(idZone), m_animateurs(animateurs)
{
}

QString Campagne::derniereErreur()
{
    return s_erreur;
}

// Colonnes : 0 ID, 1 Titre, 2 Theme, 3 Date, 4 Lieu, 5 Public cible, 6 Participants,
//            7 Zone, 8 Animateurs, 9 ID_ZONE (cachee)
QString Campagne::selectColonnes()
{
    return "SELECT c.id_campagne AS \"ID\", c.titre AS \"Titre\", c.theme AS \"Theme\", "
           "TO_CHAR(c.date_campagne,'YYYY-MM-DD') AS \"Date\", c.lieu AS \"Lieu\", "
           "c.public_cible AS \"Public cible\", c.nb_participants AS \"Participants\", "
           "z.NOM AS \"Zone\", "
           "(SELECT LISTAGG(e.nom || ' ' || e.prenom, ', ') WITHIN GROUP (ORDER BY e.nom) "
           "   FROM ANIMER a JOIN EMPLOYE e ON e.id_employe = a.id_employe "
           "  WHERE a.id_campagne = c.id_campagne) AS \"Animateurs\", "
           "c.id_zone AS \"ID_ZONE\" "
           "FROM CAMPAGNES_SENSIBILISATION c "
           "LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.id_zone ";
}

// ---------------------------------------------------------------- CRUD

bool Campagne::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery seq;
    if (!seq.exec("SELECT seq_campagne.NEXTVAL FROM DUAL") || !seq.next()) {
        s_erreur = seq.lastError().text();
        db.rollback();
        return false;
    }
    m_id = seq.value(0).toInt();

    QSqlQuery q;
    q.prepare("INSERT INTO CAMPAGNES_SENSIBILISATION "
              "(id_campagne, titre, theme, date_campagne, lieu, public_cible, nb_participants, id_zone) "
              "VALUES (:id, :titre, :theme, TO_DATE(:d,'YYYY-MM-DD'), :lieu, :pc, :nb, :zone)");
    q.bindValue(":id", m_id);
    q.bindValue(":titre", m_titre);
    q.bindValue(":theme", m_theme);
    q.bindValue(":d", m_date.toString("yyyy-MM-dd"));
    q.bindValue(":lieu", m_lieu);
    q.bindValue(":pc", m_publicCible);
    q.bindValue(":nb", m_nbParticipants);
    q.bindValue(":zone", m_idZone);
    if (!q.exec()) {
        s_erreur = q.lastError().text();
        db.rollback();
        return false;
    }
    if (!enregistrerAnimateurs()) {   // s_erreur est rempli par enregistrerAnimateurs()
        db.rollback();
        return false;
    }
    return db.commit();
}

bool Campagne::modifier()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
    q.prepare("UPDATE CAMPAGNES_SENSIBILISATION SET titre = :titre, theme = :theme, "
              "date_campagne = TO_DATE(:d,'YYYY-MM-DD'), lieu = :lieu, public_cible = :pc, "
              "nb_participants = :nb, id_zone = :zone WHERE id_campagne = :id");
    q.bindValue(":titre", m_titre);
    q.bindValue(":theme", m_theme);
    q.bindValue(":d", m_date.toString("yyyy-MM-dd"));
    q.bindValue(":lieu", m_lieu);
    q.bindValue(":pc", m_publicCible);
    q.bindValue(":nb", m_nbParticipants);
    q.bindValue(":zone", m_idZone);
    q.bindValue(":id", m_id);
    if (!q.exec() || q.numRowsAffected() <= 0) {
        s_erreur = q.lastError().text();
        db.rollback();
        return false;
    }
    if (!enregistrerAnimateurs()) {
        db.rollback();
        return false;
    }
    return db.commit();
}

// Remplace les animateurs de la campagne (table d'association ANIMER)
bool Campagne::enregistrerAnimateurs()
{
    QSqlQuery del;
    del.prepare("DELETE FROM ANIMER WHERE id_campagne = :id");
    del.bindValue(":id", m_id);
    if (!del.exec()) {
        s_erreur = del.lastError().text();
        return false;
    }
    for (int idEmp : m_animateurs) {
        QSqlQuery ins;
        ins.prepare("INSERT INTO ANIMER (id_employe, id_campagne) VALUES (:e, :c)");
        ins.bindValue(":e", idEmp);
        ins.bindValue(":c", m_id);
        if (!ins.exec()) {
            s_erreur = ins.lastError().text();
            return false;
        }
    }
    return true;
}

bool Campagne::supprimer(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery del;
    del.prepare("DELETE FROM ANIMER WHERE id_campagne = :id");
    del.bindValue(":id", id);
    QSqlQuery q;
    q.prepare("DELETE FROM CAMPAGNES_SENSIBILISATION WHERE id_campagne = :id");
    q.bindValue(":id", id);

    if (!del.exec() || !q.exec() || q.numRowsAffected() <= 0) {
        s_erreur = del.lastError().isValid() ? del.lastError().text() : q.lastError().text();
        db.rollback();
        return false;
    }
    return db.commit();
}

QSqlQueryModel *Campagne::afficher(const QString &orderBy)
{
    auto *model = new QSqlQueryModel();
    model->setQuery(selectColonnes() + "ORDER BY " + orderBy);
    return model;
}

bool Campagne::existe(int id)
{
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM CAMPAGNES_SENSIBILISATION WHERE id_campagne = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next() && q.value(0).toInt() > 0;
}

// ---------------------------------------------------------------- Metiers

QSqlQueryModel *Campagne::rechercher(const QString &critere, const QString &valeur,
                                     const QString &orderBy)
{
    QString condition;
    if (critere == "ID")
        condition = "TO_CHAR(c.id_campagne) LIKE :v";
    else if (critere == "Theme")
        condition = "UPPER(c.theme) LIKE :v";
    else if (critere == "Lieu")
        condition = "UPPER(c.lieu) LIKE :v";
    else if (critere == "Zone")
        condition = "UPPER(z.NOM) LIKE :v";
    else if (critere == "Animateur")
        condition = "EXISTS (SELECT 1 FROM ANIMER a JOIN EMPLOYE e ON e.id_employe = a.id_employe "
                    "WHERE a.id_campagne = c.id_campagne "
                    "AND UPPER(e.nom || ' ' || e.prenom) LIKE :v)";
    else
        condition = "UPPER(c.titre) LIKE :v";

    QSqlQuery q;
    q.prepare(selectColonnes() + "WHERE " + condition + " ORDER BY " + orderBy);
    q.bindValue(":v", "%" + valeur.trimmed().toUpper() + "%");
    q.exec();

    auto *model = new QSqlQueryModel();
    model->setQuery(std::move(q));
    return model;
}

QMap<QString, int> Campagne::statistiques(const QString &critere)
{
    QString col;
    if (critere == "PUBLIC_CIBLE")
        col = "c.public_cible";
    else if (critere == "ZONE")
        col = "NVL(z.NOM, 'Sans zone')";
    else if (critere == "MOIS")
        col = "TO_CHAR(c.date_campagne, 'YYYY-MM')";
    else
        col = "c.theme";

    QMap<QString, int> resultat;
    QSqlQuery q("SELECT " + col + ", COUNT(*) FROM CAMPAGNES_SENSIBILISATION c "
                "LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.id_zone GROUP BY " + col);
    while (q.next())
        resultat.insert(q.value(0).toString(), q.value(1).toInt());
    return resultat;
}

// ---------------------------------------------------------------- Listes

QList<QPair<int, QString>> Campagne::listeEmployes()
{
    QList<QPair<int, QString>> liste;
    QSqlQuery q("SELECT id_employe, nom || ' ' || prenom || NVL2(grade, ' (' || grade || ')', '') "
                "FROM EMPLOYE ORDER BY nom, prenom");
    while (q.next())
        liste.append(qMakePair(q.value(0).toInt(), q.value(1).toString()));
    return liste;
}

QList<QPair<int, QString>> Campagne::listeZones()
{
    QList<QPair<int, QString>> liste;
    QSqlQuery q("SELECT ID_ZONE, NOM || NVL2(REGION, ' - ' || REGION, '') "
                "|| NVL2(NIVEAU_RISQUE, ' (risque ' || NIVEAU_RISQUE || ')', '') "
                "FROM ZONE_COUVERTURE ORDER BY NOM");
    while (q.next())
        liste.append(qMakePair(q.value(0).toInt(), q.value(1).toString()));
    return liste;
}

QList<int> Campagne::animateursDe(int idCampagne)
{
    QList<int> ids;
    QSqlQuery q;
    q.prepare("SELECT id_employe FROM ANIMER WHERE id_campagne = :id");
    q.bindValue(":id", idCampagne);
    if (q.exec())
        while (q.next())
            ids.append(q.value(0).toInt());
    return ids;
}
