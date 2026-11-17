#include "zone.h"

#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

Zone::Zone(int id, const QString &nom, const QString &region, double superficie, int population,
           const QString &niveauRisque, int tempsIntervention, double latitude, double longitude)
    : m_id(id), m_nom(nom), m_region(region), m_superficie(superficie), m_population(population),
      m_niveauRisque(niveauRisque), m_tempsIntervention(tempsIntervention),
      m_latitude(latitude), m_longitude(longitude)
{
}

QString Zone::valider() const
{
    QStringList e;
    if (m_id <= 0) e << "L'ID doit être un entier positif.";
    if (m_nom.trimmed().size() < 2) e << "Le nom doit contenir au moins 2 caractères.";
    if (m_region.trimmed().isEmpty()) e << "La région / le quartier est obligatoire.";
    if (m_superficie <= 0) e << "La superficie doit être supérieure à 0.";
    if (m_population <= 0) e << "La population doit être supérieure à 0.";
    if (!QStringList({"Faible", "Moyen", "Élevé"}).contains(m_niveauRisque))
        e << "Choisissez un niveau de risque.";
    if (m_tempsIntervention <= 0 || m_tempsIntervention > 180)
        e << "Le temps d'intervention moyen doit être compris entre 1 et 180 minutes.";
    // Grand Tunis (contrôle de cohérence des coordonnées)
    if (m_latitude < 36.5 || m_latitude > 37.2 || m_longitude < 9.8 || m_longitude > 10.6)
        e << "Les coordonnées doivent se situer dans le Grand Tunis (lat. 36,5–37,2 ; long. 9,8–10,6).";
    return e.join("\n");
}

static void bindAll(QSqlQuery &q, const Zone &z)
{
    q.bindValue(":id", z.getId());
    q.bindValue(":nom", z.getNom().trimmed());
    q.bindValue(":region", z.getRegion().trimmed());
    q.bindValue(":sup", z.getSuperficie());
    q.bindValue(":pop", z.getPopulation());
    q.bindValue(":risque", z.getNiveauRisque());
    q.bindValue(":temps", z.getTempsIntervention());
    q.bindValue(":lat", z.getLatitude());
    q.bindValue(":lon", z.getLongitude());
}

bool Zone::ajouter(QString *erreur)
{
    const QString v = valider();
    if (!v.isEmpty()) { if (erreur) *erreur = v; return false; }
    if (existe(m_id)) { if (erreur) *erreur = "Une zone avec cet ID existe déjà."; return false; }
    QSqlQuery q;
    q.prepare("INSERT INTO ZONE_COUVERTURE (ID_ZONE, NOM, REGION, SUPERFICIE, POPULATION, NIVEAU_RISQUE,"
              " TEMPS_INTERVENTION_MOYEN, LATITUDE, LONGITUDE)"
              " VALUES (:id, :nom, :region, :sup, :pop, :risque, :temps, :lat, :lon)");
    bindAll(q, *this);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Zone::modifier(QString *erreur)
{
    const QString v = valider();
    if (!v.isEmpty()) { if (erreur) *erreur = v; return false; }
    if (!existe(m_id)) { if (erreur) *erreur = "Aucune zone avec cet ID."; return false; }
    QSqlQuery q;
    q.prepare("UPDATE ZONE_COUVERTURE SET NOM=:nom, REGION=:region, SUPERFICIE=:sup, POPULATION=:pop,"
              " NIVEAU_RISQUE=:risque, TEMPS_INTERVENTION_MOYEN=:temps, LATITUDE=:lat, LONGITUDE=:lon"
              " WHERE ID_ZONE=:id");
    bindAll(q, *this);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Zone::supprimer(int id, QString *erreur)
{
    if (!existe(id)) { if (erreur) *erreur = "Aucune zone avec cet ID."; return false; }

    // Une zone ciblée par des campagnes ne peut pas être supprimée (clé étrangère)
    QSqlQuery c;
    c.prepare("SELECT COUNT(*) FROM CAMPAGNE WHERE ID_ZONE = :id");
    c.bindValue(":id", id);
    if (c.exec() && c.next() && c.value(0).toInt() > 0) {
        if (erreur)
            *erreur = QString("Cette zone est ciblée par %1 campagne(s) de sensibilisation : "
                              "modifiez ou supprimez d'abord ces campagnes.").arg(c.value(0).toInt());
        return false;
    }

    QSqlQuery q;
    q.prepare("DELETE FROM ZONE_COUVERTURE WHERE ID_ZONE = :id");
    q.bindValue(":id", id);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Zone::existe(int id)
{
    QSqlQuery q;
    q.prepare("SELECT 1 FROM ZONE_COUVERTURE WHERE ID_ZONE = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next();
}

static Zone depuisRequete(const QSqlQuery &q)
{
    return Zone(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(3).toDouble(),
                q.value(4).toInt(), q.value(5).toString(), q.value(6).toInt(),
                q.value(7).isNull() ? 36.80 : q.value(7).toDouble(),
                q.value(8).isNull() ? 10.18 : q.value(8).toDouble());
}

static const char *kColonnes = "ID_ZONE, NOM, REGION, SUPERFICIE, POPULATION, NIVEAU_RISQUE,"
                               " TEMPS_INTERVENTION_MOYEN, LATITUDE, LONGITUDE";

bool Zone::charger(int id, Zone &z)
{
    QSqlQuery q;
    q.prepare(QString("SELECT %1 FROM ZONE_COUVERTURE WHERE ID_ZONE = :id").arg(kColonnes));
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) return false;
    z = depuisRequete(q);
    return true;
}

int Zone::prochainId()
{
    QSqlQuery q("SELECT COALESCE(MAX(ID_ZONE), 0) + 1 FROM ZONE_COUVERTURE");
    return q.next() ? q.value(0).toInt() : 1;
}

QList<Zone> Zone::toutes()
{
    QList<Zone> res;
    QSqlQuery q(QString("SELECT %1 FROM ZONE_COUVERTURE ORDER BY NOM").arg(kColonnes));
    while (q.next())
        res.append(depuisRequete(q));
    return res;
}

QSqlQueryModel *Zone::afficher(const QString &critere, const QString &texte, const QString &tri)
{
    // Listes blanches : jamais de texte libre dans la requête
    static const QStringList criteres = {"NOM", "REGION"};
    static const QStringList tris = {
        "CASE NIVEAU_RISQUE WHEN 'Élevé' THEN 0 WHEN 'Moyen' THEN 1 ELSE 2 END, NOM",
        "CASE NIVEAU_RISQUE WHEN 'Élevé' THEN 0 WHEN 'Moyen' THEN 1 ELSE 2 END DESC, NOM",
        "TEMPS_INTERVENTION_MOYEN DESC", "TEMPS_INTERVENTION_MOYEN ASC", "NOM ASC", "ID_ZONE ASC"};

    QString sql = "SELECT ID_ZONE, NOM, REGION, SUPERFICIE, POPULATION, NIVEAU_RISQUE,"
                  " TEMPS_INTERVENTION_MOYEN FROM ZONE_COUVERTURE";
    const bool filtre = criteres.contains(critere) && !texte.trimmed().isEmpty();
    if (filtre)
        sql += " WHERE " + critere + " LIKE :txt";
    sql += " ORDER BY " + (tris.contains(tri) ? tri : tris.first());

    QSqlQuery q;
    q.prepare(sql);
    if (filtre)
        q.bindValue(":txt", "%" + texte.trimmed() + "%");
    q.exec();

    auto *model = new QSqlQueryModel();
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    model->setQuery(std::move(q));
#else
    model->setQuery(q);
#endif
    const QStringList entetes = {"ID", "Nom", "Région / quartier", "Superficie (km²)", "Population",
                                 "Niveau de risque", "Temps d'intervention (min)"};
    for (int i = 0; i < entetes.size(); ++i)
        model->setHeaderData(i, Qt::Horizontal, entetes.at(i));
    return model;
}

static QString depuisMois(int mois)
{
    return QDate::currentDate().addMonths(-mois).toString(Qt::ISODate);
}

QList<QPair<QString, int>> Zone::interventionsParZone(int mois)
{
    QList<QPair<QString, int>> res;
    QSqlQuery q;
    q.prepare("SELECT z.NOM, (SELECT COUNT(*) FROM INTERVENTION i WHERE i.ID_ZONE = z.ID_ZONE"
              " AND i.DATE_HEURE >= :d) AS N FROM ZONE_COUVERTURE z ORDER BY N DESC, z.NOM");
    q.bindValue(":d", depuisMois(mois));
    if (q.exec())
        while (q.next())
            res.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return res;
}

QList<QPair<QString, int>> Zone::interventionsParNiveauRisque(int mois)
{
    QList<QPair<QString, int>> res;
    for (const QString &niveau : {QString("Élevé"), QString("Moyen"), QString("Faible")}) {
        QSqlQuery q;
        q.prepare("SELECT COUNT(*) FROM INTERVENTION i JOIN ZONE_COUVERTURE z ON z.ID_ZONE = i.ID_ZONE"
                  " WHERE z.NIVEAU_RISQUE = :n AND i.DATE_HEURE >= :d");
        q.bindValue(":n", niveau);
        q.bindValue(":d", depuisMois(mois));
        res.append(qMakePair(niveau, (q.exec() && q.next()) ? q.value(0).toInt() : 0));
    }
    return res;
}

int Zone::interventionsRecentes(int idZone, int mois)
{
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM INTERVENTION WHERE ID_ZONE = :z AND DATE_HEURE >= :d");
    q.bindValue(":z", idZone);
    q.bindValue(":d", depuisMois(mois));
    return (q.exec() && q.next()) ? q.value(0).toInt() : 0;
}
