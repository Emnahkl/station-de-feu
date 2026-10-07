#include "campagne.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <algorithm>

Campagne::Campagne() {}

Campagne::Campagne(int id, const QString &titre, const QString &theme, const QDate &date,
                   const QString &lieu, const QString &publicCible, int nbParticipants,
                   int idZone, int idEmploye)
    : m_id(id), m_titre(titre), m_theme(theme), m_date(date), m_lieu(lieu),
      m_publicCible(publicCible), m_nbParticipants(nbParticipants),
      m_idZone(idZone), m_idEmploye(idEmploye)
{
}

QString Campagne::valider() const
{
    QStringList e;
    if (m_id <= 0) e << "L'ID doit être un entier positif.";
    if (m_titre.trimmed().size() < 3) e << "Le titre doit contenir au moins 3 caractères.";
    if (m_titre.size() > 80) e << "Le titre ne doit pas dépasser 80 caractères.";
    if (m_theme.trimmed().isEmpty()) e << "Choisissez un thème.";
    if (!m_date.isValid()) e << "La date est invalide.";
    if (m_lieu.trimmed().size() < 3) e << "Le lieu doit contenir au moins 3 caractères.";
    if (m_publicCible.trimmed().isEmpty()) e << "Choisissez un public cible.";
    if (m_nbParticipants < 0) e << "Le nombre de participants ne peut pas être négatif.";
    if (m_idZone <= 0) e << "Choisissez une zone de couverture.";
    if (m_idEmploye <= 0) e << "Choisissez l'employé animateur.";
    return e.join("\n");
}

static void bindAll(QSqlQuery &q, const Campagne &c)
{
    q.bindValue(":titre", c.getTitre().trimmed());
    q.bindValue(":theme", c.getTheme());
    q.bindValue(":date", c.getDate().toString(Qt::ISODate));
    q.bindValue(":lieu", c.getLieu().trimmed());
    q.bindValue(":public", c.getPublicCible());
    q.bindValue(":nb", c.getNbParticipants());
    q.bindValue(":zone", c.getIdZone());
    q.bindValue(":emp", c.getIdEmploye());
    q.bindValue(":id", c.getId());
}

bool Campagne::ajouter(QString *erreur)
{
    const QString v = valider();
    if (!v.isEmpty()) { if (erreur) *erreur = v; return false; }
    if (existe(m_id)) { if (erreur) *erreur = "Une campagne avec cet ID existe déjà."; return false; }

    QSqlQuery q;
    q.prepare("INSERT INTO CAMPAGNE (ID_CAMPAGNE, TITRE, THEME, DATE_CAMPAGNE, LIEU, PUBLIC_CIBLE,"
              " NB_PARTICIPANTS, ID_ZONE, ID_EMPLOYE)"
              " VALUES (:id, :titre, :theme, :date, :lieu, :public, :nb, :zone, :emp)");
    bindAll(q, *this);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Campagne::modifier(QString *erreur)
{
    const QString v = valider();
    if (!v.isEmpty()) { if (erreur) *erreur = v; return false; }
    if (!existe(m_id)) { if (erreur) *erreur = "Aucune campagne avec cet ID."; return false; }

    QSqlQuery q;
    q.prepare("UPDATE CAMPAGNE SET TITRE=:titre, THEME=:theme, DATE_CAMPAGNE=:date, LIEU=:lieu,"
              " PUBLIC_CIBLE=:public, NB_PARTICIPANTS=:nb, ID_ZONE=:zone, ID_EMPLOYE=:emp"
              " WHERE ID_CAMPAGNE=:id");
    bindAll(q, *this);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Campagne::supprimer(int id, QString *erreur)
{
    if (!existe(id)) { if (erreur) *erreur = "Aucune campagne avec cet ID."; return false; }
    QSqlQuery q;
    q.prepare("DELETE FROM CAMPAGNE WHERE ID_CAMPAGNE = :id");
    q.bindValue(":id", id);
    if (!q.exec()) { if (erreur) *erreur = q.lastError().text(); return false; }
    return true;
}

bool Campagne::existe(int id)
{
    QSqlQuery q;
    q.prepare("SELECT 1 FROM CAMPAGNE WHERE ID_CAMPAGNE = :id");
    q.bindValue(":id", id);
    return q.exec() && q.next();
}

bool Campagne::charger(int id, Campagne &c)
{
    QSqlQuery q;
    q.prepare("SELECT ID_CAMPAGNE, TITRE, THEME, DATE_CAMPAGNE, LIEU, PUBLIC_CIBLE,"
              " NB_PARTICIPANTS, ID_ZONE, ID_EMPLOYE FROM CAMPAGNE WHERE ID_CAMPAGNE = :id");
    q.bindValue(":id", id);
    if (!q.exec() || !q.next()) return false;
    c = Campagne(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(),
                 QDate::fromString(q.value(3).toString(), Qt::ISODate), q.value(4).toString(),
                 q.value(5).toString(), q.value(6).toInt(), q.value(7).toInt(), q.value(8).toInt());
    return true;
}

int Campagne::prochainId()
{
    QSqlQuery q("SELECT COALESCE(MAX(ID_CAMPAGNE), 0) + 1 FROM CAMPAGNE");
    return q.next() ? q.value(0).toInt() : 1;
}

QSqlQueryModel *Campagne::afficher(const QString &critere, const QString &texte, const QString &tri)
{
    // Liste blanche : on n'insère jamais du texte libre dans la requête
    static const QStringList criteres = {"TITRE", "THEME", "PUBLIC_CIBLE", "LIEU"};
    static const QStringList tris = {"DATE_CAMPAGNE DESC", "DATE_CAMPAGNE ASC",
                                     "NB_PARTICIPANTS DESC", "NB_PARTICIPANTS ASC",
                                     "TITRE ASC", "ID_CAMPAGNE ASC"};

    QString sql = "SELECT c.ID_CAMPAGNE, c.TITRE, c.THEME, c.DATE_CAMPAGNE, c.LIEU, c.PUBLIC_CIBLE,"
                  " c.NB_PARTICIPANTS, z.NOM, e.PRENOM || ' ' || e.NOM"
                  " FROM CAMPAGNE c"
                  " LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.ID_ZONE"
                  " LEFT JOIN EMPLOYE e ON e.ID_EMPLOYE = c.ID_EMPLOYE";

    const bool filtre = criteres.contains(critere) && !texte.trimmed().isEmpty();
    if (filtre)
        sql += " WHERE c." + critere + " LIKE :txt";
    sql += " ORDER BY c." + (tris.contains(tri) ? tri : QString("DATE_CAMPAGNE DESC"));

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
    const QStringList entetes = {"ID", "Titre", "Thème", "Date", "Lieu", "Public cible",
                                 "Participants", "Zone", "Animateur"};
    for (int i = 0; i < entetes.size(); ++i)
        model->setHeaderData(i, Qt::Horizontal, entetes.at(i));
    return model;
}

static QList<QPair<QString, int>> statsDepuis(const QString &sql)
{
    QList<QPair<QString, int>> res;
    QSqlQuery q(sql);
    while (q.next())
        res.append(qMakePair(q.value(0).toString(), q.value(1).toInt()));
    return res;
}

QList<QPair<QString, int>> Campagne::statistiquesParTheme()
{
    return statsDepuis("SELECT THEME, COUNT(*) FROM CAMPAGNE GROUP BY THEME ORDER BY COUNT(*) DESC");
}

QList<QPair<QString, int>> Campagne::statistiquesParZone()
{
    return statsDepuis("SELECT z.NOM, COUNT(c.ID_CAMPAGNE) FROM ZONE_COUVERTURE z"
                       " LEFT JOIN CAMPAGNE c ON c.ID_ZONE = z.ID_ZONE"
                       " GROUP BY z.ID_ZONE, z.NOM ORDER BY COUNT(c.ID_CAMPAGNE) DESC, z.NOM");
}

// Score de priorité = interventions sur 12 mois × poids du risque ÷ (1 + campagnes déjà faites sur 12 mois)
QList<CiblageZone> Campagne::ciblageAutomatique(const QDate &reference)
{
    const QString debut = reference.addMonths(-12).toString(Qt::ISODate);
    const QString fin = reference.addDays(1).toString(Qt::ISODate);

    QSqlQuery q;
    q.prepare("SELECT z.ID_ZONE, z.NOM, z.NIVEAU_RISQUE,"
              " (SELECT COUNT(*) FROM INTERVENTION i WHERE i.ID_ZONE = z.ID_ZONE"
              "   AND i.DATE_HEURE >= :d1 AND i.DATE_HEURE < :f1),"
              " (SELECT COUNT(*) FROM CAMPAGNE c WHERE c.ID_ZONE = z.ID_ZONE"
              "   AND c.DATE_CAMPAGNE >= :d2 AND c.DATE_CAMPAGNE < :f2)"
              " FROM ZONE_COUVERTURE z");
    q.bindValue(":d1", debut);
    q.bindValue(":f1", fin);
    q.bindValue(":d2", debut);
    q.bindValue(":f2", fin);

    QList<CiblageZone> res;
    if (!q.exec()) return res;

    while (q.next()) {
        CiblageZone z;
        z.idZone = q.value(0).toInt();
        z.nom = q.value(1).toString();
        z.niveauRisque = q.value(2).toString();
        z.interventions12Mois = q.value(3).toInt();
        z.campagnes12Mois = q.value(4).toInt();
        double poids = 1.0;
        if (z.niveauRisque == QString::fromUtf8("Moyen")) poids = 1.5;
        else if (z.niveauRisque == QString::fromUtf8("Élevé")) poids = 2.0;
        z.score = z.interventions12Mois * poids / (1.0 + z.campagnes12Mois);
        res.append(z);
    }

    std::sort(res.begin(), res.end(), [](const CiblageZone &a, const CiblageZone &b) {
        return a.score > b.score;
    });

    const double max = res.isEmpty() ? 0.0 : res.first().score;
    for (CiblageZone &z : res) {
        if (max > 0 && z.score >= 0.66 * max)
            z.recommandation = QString::fromUtf8("Priorité haute : planifier une campagne");
        else if (max > 0 && z.score >= 0.33 * max)
            z.recommandation = QString::fromUtf8("Priorité moyenne : à programmer");
        else
            z.recommandation = QString::fromUtf8("Couverture suffisante");
    }
    return res;
}

ImpactCampagne Campagne::evaluerImpact(int idCampagne, int fenetreMois)
{
    ImpactCampagne r;
    Campagne c;
    if (!charger(idCampagne, c) || fenetreMois <= 0) return r;

    const QDate d = c.getDate();
    const QDate avantDebut = d.addMonths(-fenetreMois);
    const QDate apresFin = d.addMonths(fenetreMois);
    r.periodeComplete = apresFin <= QDate::currentDate();

    auto compter = [&](const QDate &de, const QDate &a) {
        QSqlQuery q;
        q.prepare("SELECT COUNT(*) FROM INTERVENTION WHERE ID_ZONE = :z"
                  " AND DATE_HEURE >= :de AND DATE_HEURE < :a");
        q.bindValue(":z", c.getIdZone());
        q.bindValue(":de", de.toString(Qt::ISODate));
        q.bindValue(":a", a.toString(Qt::ISODate));
        return (q.exec() && q.next()) ? q.value(0).toInt() : 0;
    };

    r.avant = compter(avantDebut, d);
    r.apres = compter(d.addDays(1), apresFin.addDays(1));
    r.valide = true;

    if (r.avant == 0) {
        r.variation = 0.0;
        r.verdict = QString::fromUtf8("Pas d'interventions avant la campagne : impact non mesurable.");
        return r;
    }
    r.variation = 100.0 * (r.apres - r.avant) / r.avant;

    if (r.variation <= -20)
        r.verdict = QString::fromUtf8("Impact positif : baisse nette des interventions dans la zone.");
    else if (r.variation < 0)
        r.verdict = QString::fromUtf8("Impact modéré : légère baisse des interventions.");
    else if (r.variation == 0)
        r.verdict = QString::fromUtf8("Pas d'effet visible sur les interventions.");
    else
        r.verdict = QString::fromUtf8("Aucune baisse : renforcer ou recibler la campagne.");

    if (!r.periodeComplete)
        r.verdict += QString::fromUtf8(" (période après campagne encore en cours)");
    return r;
}
