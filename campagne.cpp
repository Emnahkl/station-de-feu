#include "campagne.h"
#include <QSqlQuery>
#include <QVariant>

Campagne::Campagne()
    : id(0), nbParticipants(0), budget(0), scoreImpact(0), idEmploye(0), idZone(0)
{
}

Campagne::Campagne(int id, QString titre, QString theme, QString publicCible, QString description,
                   QDate dateDebut, QDate dateFin, int nbParticipants, double budget,
                   QString statut, double scoreImpact, int idEmploye, int idZone)
    : id(id), titre(titre), theme(theme), publicCible(publicCible), description(description),
      statut(statut), dateDebut(dateDebut), dateFin(dateFin), nbParticipants(nbParticipants),
      budget(budget), scoreImpact(scoreImpact), idEmploye(idEmploye), idZone(idZone)
{
}

// Colonnes : 0 ID, 1 Titre, 2 Theme, 3 Public cible, 4 Description, 5 Date debut, 6 Date fin,
// 7 Participants, 8 Budget, 9 Statut, 10 Score impact, 11 Organisateur, 12 Zone,
// 13 ID_EMPLOYE (cache), 14 ID_ZONE (cache)
QString Campagne::selectColonnes()
{
    return "SELECT c.ID_CAMPAGNE AS \"ID\", c.TITRE AS \"Titre\", c.THEME AS \"Theme\", "
           "c.PUBLIC_CIBLE AS \"Public cible\", c.DESCRIPTION AS \"Description\", "
           "TO_CHAR(c.DATE_DEBUT,'YYYY-MM-DD') AS \"Date debut\", "
           "TO_CHAR(c.DATE_FIN,'YYYY-MM-DD') AS \"Date fin\", "
           "c.NB_PARTICIPANTS AS \"Participants\", c.BUDGET AS \"Budget (DT)\", "
           "c.STATUT AS \"Statut\", c.SCORE_IMPACT AS \"Score impact\", "
           "e.NOM || ' ' || e.PRENOM AS \"Organisateur\", z.NOM_ZONE AS \"Zone ciblee\", "
           "c.ID_EMPLOYE, c.ID_ZONE "
           "FROM CAMPAGNE c "
           "JOIN EMPLOYE e ON e.ID_EMPLOYE = c.ID_EMPLOYE "
           "JOIN ZONE z ON z.ID_ZONE = c.ID_ZONE ";
}

bool Campagne::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CAMPAGNE (ID_CAMPAGNE, TITRE, THEME, PUBLIC_CIBLE, DESCRIPTION, "
                  "DATE_DEBUT, DATE_FIN, NB_PARTICIPANTS, BUDGET, STATUT, SCORE_IMPACT, "
                  "ID_EMPLOYE, ID_ZONE) "
                  "VALUES (:id, :titre, :theme, :pc, :descr, TO_DATE(:dd,'YYYY-MM-DD'), "
                  "TO_DATE(:df,'YYYY-MM-DD'), :nb, :budget, :statut, :score, :emp, :zone)");
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":theme", theme);
    query.bindValue(":pc", publicCible);
    query.bindValue(":descr", description);
    query.bindValue(":dd", dateDebut.toString("yyyy-MM-dd"));
    query.bindValue(":df", dateFin.toString("yyyy-MM-dd"));
    query.bindValue(":nb", nbParticipants);
    query.bindValue(":budget", budget);
    query.bindValue(":statut", statut);
    query.bindValue(":score", scoreImpact);
    query.bindValue(":emp", idEmploye);
    query.bindValue(":zone", idZone);
    return query.exec();
}

QSqlQueryModel *Campagne::afficher(const QString &orderBy)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(selectColonnes() + "ORDER BY " + orderBy);
    return model;
}

bool Campagne::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CAMPAGNE WHERE ID_CAMPAGNE = :id");
    query.bindValue(":id", id);
    return query.exec() && query.numRowsAffected() > 0;
}

bool Campagne::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE CAMPAGNE SET TITRE = :titre, THEME = :theme, PUBLIC_CIBLE = :pc, "
                  "DESCRIPTION = :descr, DATE_DEBUT = TO_DATE(:dd,'YYYY-MM-DD'), "
                  "DATE_FIN = TO_DATE(:df,'YYYY-MM-DD'), NB_PARTICIPANTS = :nb, "
                  "BUDGET = :budget, STATUT = :statut, SCORE_IMPACT = :score, "
                  "ID_EMPLOYE = :emp, ID_ZONE = :zone WHERE ID_CAMPAGNE = :id");
    query.bindValue(":titre", titre);
    query.bindValue(":theme", theme);
    query.bindValue(":pc", publicCible);
    query.bindValue(":descr", description);
    query.bindValue(":dd", dateDebut.toString("yyyy-MM-dd"));
    query.bindValue(":df", dateFin.toString("yyyy-MM-dd"));
    query.bindValue(":nb", nbParticipants);
    query.bindValue(":budget", budget);
    query.bindValue(":statut", statut);
    query.bindValue(":score", scoreImpact);
    query.bindValue(":emp", idEmploye);
    query.bindValue(":zone", idZone);
    query.bindValue(":id", id);
    return query.exec() && query.numRowsAffected() > 0;
}

bool Campagne::existe(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CAMPAGNE WHERE ID_CAMPAGNE = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next())
        return query.value(0).toInt() > 0;
    return false;
}

QSqlQueryModel *Campagne::rechercher(const QString &critere, const QString &valeur,
                                     const QString &orderBy)
{
    QString colonne;
    if (critere == "ID") colonne = "TO_CHAR(c.ID_CAMPAGNE)";
    else if (critere == "Titre") colonne = "UPPER(c.TITRE)";
    else if (critere == "Theme") colonne = "UPPER(c.THEME)";
    else if (critere == "Zone") colonne = "UPPER(z.NOM_ZONE)";
    else if (critere == "Organisateur") colonne = "UPPER(e.NOM || ' ' || e.PRENOM)";
    else colonne = "UPPER(c.STATUT)";

    QSqlQuery query;
    query.prepare(selectColonnes() + "WHERE " + colonne + " LIKE :v ORDER BY " + orderBy);
    query.bindValue(":v", "%" + valeur.trimmed().toUpper() + "%");
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    return model;
}

QMap<QString, int> Campagne::statistiques(const QString &critere)
{
    QString col;
    if (critere == "STATUT") col = "c.STATUT";
    else if (critere == "PUBLIC_CIBLE") col = "c.PUBLIC_CIBLE";
    else if (critere == "ZONE") col = "z.NOM_ZONE";
    else col = "c.THEME";

    QMap<QString, int> resultat;
    QSqlQuery query("SELECT " + col + ", COUNT(*) FROM CAMPAGNE c "
                    "JOIN ZONE z ON z.ID_ZONE = c.ID_ZONE GROUP BY " + col);
    while (query.next())
        resultat.insert(query.value(0).toString(), query.value(1).toInt());
    return resultat;
}

QList<QPair<int, QString>> Campagne::listeEmployes()
{
    QList<QPair<int, QString>> liste;
    QSqlQuery query("SELECT ID_EMPLOYE, NOM || ' ' || PRENOM || ' (' || ROLE || ')' "
                    "FROM EMPLOYE ORDER BY NOM");
    while (query.next())
        liste.append(qMakePair(query.value(0).toInt(), query.value(1).toString()));
    return liste;
}

QList<QPair<int, QString>> Campagne::listeZones()
{
    QList<QPair<int, QString>> liste;
    QSqlQuery query("SELECT ID_ZONE, NOM_ZONE || ' - ' || GOUVERNORAT || ' (risque ' "
                    "|| NIVEAU_RISQUE || ')' FROM ZONE ORDER BY NOM_ZONE");
    while (query.next())
        liste.append(qMakePair(query.value(0).toInt(), query.value(1).toString()));
    return liste;
}
