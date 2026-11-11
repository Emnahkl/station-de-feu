#include "intervention.h"

#include <QDebug>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

namespace {

QString g_erreur;

// Traduit les erreurs Oracle les plus fréquentes (l'erreur ORA-02292 est gardée telle quelle :
// l'interface la reconnaît pour expliquer pourquoi une suppression est impossible).
QString messageLisible(const QString &brut)
{
    if (brut.contains("ORA-02291"))
        return QObject::tr("La zone choisie n'existe pas (ou plus) dans la base de données.");
    if (brut.contains("ORA-12899"))
        return QObject::tr("Une valeur saisie est trop longue pour la base de données.");
    if (brut.contains("ORA-01400"))
        return QObject::tr("Un champ obligatoire est vide.");
    return brut;
}

bool executer(QSqlQuery &q)
{
    if (!q.exec()) {
        qDebug() << "Erreur SQL :" << q.lastError().text();
        g_erreur = messageLisible(q.lastError().text());
        return false;
    }
    g_erreur.clear();
    return true;
}

const char *FORMAT_DATE_SQL = "yyyy-MM-dd";
const char *FORMAT_HEURE    = "HH:mm";

} // namespace

Intervention::Intervention()
    : m_id(0), m_date(QDate::currentDate()), m_heure(QTime::currentTime()), m_idZone(0)
{
}

Intervention::Intervention(int id, const QString &type, const QString &adresse,
                           const QDate &date, const QTime &heure,
                           const QString &gravite, const QString &statut, int idZone)
    : m_id(id), m_type(type), m_adresse(adresse), m_date(date), m_heure(heure),
      m_gravite(gravite), m_statut(statut), m_idZone(idZone)
{
}

QString Intervention::derniereErreur()
{
    return g_erreur;
}

QStringList Intervention::types()
{
    return QStringList() << "Incendie" << "Accident de la route" << "Secours à personne"
                         << "Fuite de gaz" << "Inondation";
}

QStringList Intervention::gravites()
{
    return QStringList() << "Faible" << "Moyenne" << "Élevée" << "Critique";
}

QStringList Intervention::statuts()
{
    return QStringList() << "En cours" << "En attente" << "Terminée";
}


// ------------------------------------------------------- Contrôle de saisie
// Type : lettres (accents compris), espaces, apostrophe et tiret ; 3 à 40 caractères.
QString Intervention::erreurType(const QString &type)
{
    const QString t = type.simplified();
    if (t.isEmpty())
        return QObject::tr("Le type est obligatoire.");
    if (t.size() < 3)
        return QObject::tr("Le type doit contenir au moins 3 caractères.");
    if (t.size() > 40)
        return QObject::tr("Le type ne doit pas dépasser 40 caractères.");
    static const QRegularExpression ok("^\\p{L}[\\p{L} '\u2019-]*$");
    if (!ok.match(t).hasMatch())
        return QObject::tr("Le type ne doit contenir que des lettres (pas de chiffres ni de symboles).");
    return QString();
}

// Adresse : format « [numéro] type de voie + nom [, ville] »
//   ex. « 12 Rue de la Liberté, Tunis », « Avenue Habib Bourguiba », « Rue 8 Mars 1934, Tunis »
// - 8 à 100 caractères, lettres/chiffres et , . ' - / seulement
// - numéro optionnel (1 à 4 chiffres + bis/ter/quater/A-D), placé au début
// - un type de voie obligatoire (rue, avenue, boulevard, route, cité, place...)
// - un nom de voie d'au moins 3 lettres (les particules de, du, la, el... ne comptent pas) ;
//   le nom peut contenir un nombre (« Avenue du 14 Janvier », « Rue 9 Avril »)
// - ville optionnelle après une virgule (lettres uniquement, 3 caractères minimum)
QString Intervention::erreurAdresse(const QString &adresse)
{
    const QString a = adresse.simplified();
    if (a.isEmpty())
        return QObject::tr("L'adresse est obligatoire.");
    if (a.size() < 8)
        return QObject::tr("Adresse trop courte (ex. : 12 Rue de la Liberté, Tunis).");
    if (a.size() > 100)
        return QObject::tr("L'adresse est trop longue (100 caractères maximum).");
    static const QRegularExpression caracteres("^[\\p{L}0-9][\\p{L}0-9 ,.'\\x{2019}/-]*$");
    if (!caracteres.match(a).hasMatch())
        return QObject::tr("Caractères non autorisés (lettres, chiffres, espace , . ' - / seulement).");
    static const QRegularExpression repete("(.)\\1{3,}");
    if (repete.match(a).hasMatch())
        return QObject::tr("Adresse invalide (caractère répété trop de fois).");

    // partie avant la virgule = voie ; après = ville
    const QStringList parties = a.split(',');
    if (parties.size() > 2)
        return QObject::tr("Une seule virgule autorisée : « voie, ville ».");
    const QString voie = parties[0].trimmed();
    if (parties.size() == 2) {
        const QString ville = parties[1].trimmed();
        static const QRegularExpression villeOk("^\\p{L}[\\p{L} '\\x{2019}-]{2,}$");
        if (!villeOk.match(ville).hasMatch())
            return QObject::tr("Ville invalide (lettres uniquement, 3 caractères minimum).");
    }

    static const QRegularExpression format(
        "^(?:\\d{1,4}\\s?(?:bis|ter|quater|[a-d])?[ -]*)?"                                  // numéro (optionnel)
        "(?:rue|ruelle|r|avenue|ave|av|boulevard|bd|bld|route|rte|impasse|place|cit[eé]|"
        "passage|all[eé]e|chemin|r[eé]sidence|r[eé]s|quartier|lotissement|lot|zone|hay|centre)"
        "\\.?\\s+(.+)$",
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::UseUnicodePropertiesOption);
    const QRegularExpressionMatch m = format.match(voie);
    if (!m.hasMatch())
        return QObject::tr("Format attendu : [numéro] type de voie + nom (ex. : 12 Rue de la Liberté, Tunis).");

    // nom de la voie : au moins 3 lettres hors particules
    QString nom = m.captured(1);
    static const QRegularExpression particules(
        "(^|\\s)(?:de|du|des|d['\\x{2019}]?|la|le|les|el|l['\\x{2019}]?)(?=\\s|$)",
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::UseUnicodePropertiesOption);
    nom.remove(particules);
    int lettres = 0;
    for (int i = 0; i < nom.size(); ++i)
        if (nom.at(i).isLetter())
            ++lettres;
    if (lettres < 3)
        return QObject::tr("Le nom de la voie est trop court ou manquant (ex. : 12 Rue de la Liberté, Tunis).");
    return QString();
}

// Date/heure : pas dans le futur, pas avant 2026.
QString Intervention::erreurDateHeure(const QDate &date, const QTime &heure)
{
    if (!date.isValid() || !heure.isValid())
        return QObject::tr("Date ou heure invalide.");
    if (date < dateMinimale())
        return QObject::tr("La date doit être à partir du %1.").arg(dateMinimale().toString("dd/MM/yyyy"));
    if (QDateTime(date, heure) > QDateTime::currentDateTime())
        return QObject::tr("La date et l'heure ne peuvent pas être dans le futur.");
    return QString();
}

QString Intervention::erreurGravite(const QString &gravite)
{
    if (!gravites().contains(gravite))
        return QObject::tr("Choisissez une gravité dans la liste.");
    return QString();
}

QString Intervention::erreurStatut(const QString &statut)
{
    if (!statuts().contains(statut))
        return QObject::tr("Choisissez un statut dans la liste.");
    return QString();
}

QStringList Intervention::erreurs() const
{
    QStringList l;
    const QString e1 = erreurType(m_type);
    const QString e2 = erreurAdresse(m_adresse);
    const QString e3 = erreurDateHeure(m_date, m_heure);
    const QString e4 = erreurGravite(m_gravite);
    const QString e5 = erreurStatut(m_statut);
    if (!e1.isEmpty()) l << e1;
    if (!e2.isEmpty()) l << e2;
    if (!e3.isEmpty()) l << e3;
    if (!e4.isEmpty()) l << e4;
    if (!e5.isEmpty()) l << e5;
    if (e3.isEmpty() && e5.isEmpty() && estPassee(m_date) && m_statut != "Terminée")
        l << QObject::tr("Une intervention dont la date est passée doit avoir le statut « Terminée ».");
    if (m_idZone <= 0) l << QObject::tr("Choisissez une zone.");
    return l;
}

// Une autre intervention identique existe-t-elle déjà ? (évite les doublons : double clic, ressaisie...)
int Intervention::idDoublon() const
{
    QSqlQuery q;
    q.prepare("SELECT ID_INTERVENTION FROM INTERVENTIONS "
              "WHERE UPPER(TYPE_INTERVENTION) = UPPER(:t) AND UPPER(ADRESSE) = UPPER(:a) "
              "AND DATE_INTERVENTION = TO_DATE(:d, 'YYYY-MM-DD') AND HEURE = :h "
              "AND ID_INTERVENTION <> :id");
    q.bindValue(":t", m_type.simplified());
    q.bindValue(":a", m_adresse.simplified());
    q.bindValue(":d", m_date.toString(FORMAT_DATE_SQL));
    q.bindValue(":h", m_heure.toString(FORMAT_HEURE));
    q.bindValue(":id", m_id);
    if (!executer(q) || !q.next())
        return 0;
    return q.value(0).toInt();
}

// Clôture automatique : date dépassée => statut « Terminée »
bool Intervention::cloturerPassees()
{
    QSqlQuery q;
    q.prepare("UPDATE INTERVENTIONS SET STATUT = 'Terminée' "
              "WHERE DATE_INTERVENTION < TRUNC(SYSDATE) AND (STATUT IS NULL OR STATUT <> 'Terminée')");
    return executer(q);
}

// ---------------------------------------------------------------- CREATE
// L'identifiant est généré par Oracle (séquence + trigger), on ne l'envoie pas.
bool Intervention::ajouter()
{
    const QStringList err = erreurs();
    if (!err.isEmpty()) {
        g_erreur = err.join("\n");
        return false;
    }
    m_type = m_type.simplified();
    m_adresse = m_adresse.simplified();
    QSqlQuery q;
    q.prepare("INSERT INTO INTERVENTIONS "
              "(TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE) "
              "VALUES (:t, :a, TO_DATE(:d, 'YYYY-MM-DD'), :h, :g, :s, :z)");
    q.bindValue(":t", m_type);
    q.bindValue(":a", m_adresse);
    q.bindValue(":d", m_date.toString(FORMAT_DATE_SQL));
    q.bindValue(":h", m_heure.toString(FORMAT_HEURE));
    q.bindValue(":g", m_gravite);
    q.bindValue(":s", m_statut);
    q.bindValue(":z", m_idZone);
    return executer(q);
}

// ---------------------------------------------------------------- UPDATE
bool Intervention::modifier()
{
    const QStringList err = erreurs();
    if (!err.isEmpty()) {
        g_erreur = err.join("\n");
        return false;
    }
    m_type = m_type.simplified();
    m_adresse = m_adresse.simplified();
    QSqlQuery q;
    q.prepare("UPDATE INTERVENTIONS SET "
              "TYPE_INTERVENTION = :t, ADRESSE = :a, "
              "DATE_INTERVENTION = TO_DATE(:d, 'YYYY-MM-DD'), HEURE = :h, "
              "GRAVITE = :g, STATUT = :s, ID_ZONE = :z "
              "WHERE ID_INTERVENTION = :id");
    q.bindValue(":t", m_type);
    q.bindValue(":a", m_adresse);
    q.bindValue(":d", m_date.toString(FORMAT_DATE_SQL));
    q.bindValue(":h", m_heure.toString(FORMAT_HEURE));
    q.bindValue(":g", m_gravite);
    q.bindValue(":s", m_statut);
    q.bindValue(":z", m_idZone);
    q.bindValue(":id", m_id);
    if (!executer(q))
        return false;
    if (q.numRowsAffected() == 0) {
        g_erreur = "Intervention introuvable.";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------- DELETE
bool Intervention::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM INTERVENTIONS WHERE ID_INTERVENTION = :id");
    q.bindValue(":id", id);
    if (!executer(q))
        return false;
    if (q.numRowsAffected() == 0) {
        g_erreur = "Intervention introuvable.";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------- READ
QVector<Intervention> Intervention::afficher(const QString &gravite,
                                             const QString &statut,
                                             const QString &recherche)
{
    QString sql =
        "SELECT ID_INTERVENTION, TYPE_INTERVENTION, ADRESSE, "
        "TO_CHAR(DATE_INTERVENTION, 'YYYY-MM-DD'), HEURE, GRAVITE, STATUT, ID_ZONE "
        "FROM INTERVENTIONS";

    QStringList conditions;
    if (!gravite.isEmpty())
        conditions << "GRAVITE = :g";
    if (!statut.isEmpty())
        conditions << "STATUT = :s";
    const QString r = recherche.trimmed();
    if (!r.isEmpty())
        conditions << "(UPPER(TYPE_INTERVENTION) LIKE UPPER(:r1) ESCAPE '\\' "
                      "OR UPPER(ADRESSE) LIKE UPPER(:r2) ESCAPE '\\' "
                      "OR TO_CHAR(ID_INTERVENTION) LIKE :r3 ESCAPE '\\')";
    if (!conditions.isEmpty())
        sql += " WHERE " + conditions.join(" AND ");
    sql += " ORDER BY ID_INTERVENTION";

    QSqlQuery q;
    q.prepare(sql);
    if (!gravite.isEmpty())
        q.bindValue(":g", gravite);
    if (!statut.isEmpty())
        q.bindValue(":s", statut);
    if (!r.isEmpty()) {
        // % et _ tapés par l'utilisateur sont cherchés tels quels (pas comme jokers SQL)
        QString texte = r;
        texte.replace("\\", "\\\\").replace("%", "\\%").replace("_", "\\_");
        const QString motif = "%" + texte + "%";
        q.bindValue(":r1", motif);
        q.bindValue(":r2", motif);
        q.bindValue(":r3", motif);
    }

    QVector<Intervention> liste;
    if (!executer(q))
        return liste;

    while (q.next()) {
        liste.push_back(Intervention(
            q.value(0).toInt(),
            q.value(1).toString(),
            q.value(2).toString(),
            QDate::fromString(q.value(3).toString(), FORMAT_DATE_SQL),
            QTime::fromString(q.value(4).toString(), FORMAT_HEURE),
            q.value(5).toString(),
            q.value(6).toString(),
            q.value(7).toInt()));
    }
    return liste;
}

// ---------------------------------------------------------------- Zones
QVector<QPair<int, QString> > Intervention::zones()
{
    QVector<QPair<int, QString> > liste;
    QSqlQuery q;
    q.prepare("SELECT ID_ZONE, NOM FROM ZONE_COUVERTURE ORDER BY ID_ZONE");
    if (!executer(q))
        return liste;
    while (q.next())
        liste.push_back(qMakePair(q.value(0).toInt(), q.value(1).toString()));
    return liste;
}

// Temps moyen d'intervention de chaque zone (colonne TEMPS_INTERVENTION, 10 min si non renseigné)
QMap<int, int> Intervention::tempsZones()
{
    QMap<int, int> temps;
    QSqlQuery q;
    q.prepare("SELECT ID_ZONE, COALESCE(TEMPS_INTERVENTION, 10) FROM ZONE_COUVERTURE");
    if (!executer(q))
        return temps;
    while (q.next())
        temps.insert(q.value(0).toInt(), q.value(1).toInt());
    return temps;
}
