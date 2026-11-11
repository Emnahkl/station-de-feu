#include "affectation.h"

#include <QDate>
#include <QDebug>
#include <QObject>
#include <QMap>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <algorithm>

namespace {

QString g_erreur;

bool executer(QSqlQuery &q)
{
    if (!q.exec()) {
        g_erreur = q.lastError().text();
        qDebug() << "Erreur SQL (affectation) :" << g_erreur;
        return false;
    }
    g_erreur.clear();
    return true;
}

// minuscules sans accents : « Écharpe » -> « echarpe » (pour comparer des libellés saisis librement)
QString norm(const QString &s)
{
    const QString d = s.normalized(QString::NormalizationForm_D);
    QString r;
    for (int i = 0; i < d.size(); ++i)
        if (d.at(i).category() != QChar::Mark_NonSpacing)
            r += d.at(i);
    return r.toLower();
}

bool contientUn(const QString &texte, const QStringList &mots)
{
    const QString t = norm(texte);
    for (int i = 0; i < mots.size(); ++i)
        if (t.contains(mots[i]))
            return true;
    return false;
}

// Mots-clés cherchés dans les libellés de la base (TYPE / MODELE des véhicules, SPECIALITE des
// employés, CATEGORIE des équipements) pour chaque type d'intervention.
struct Regle {
    QStringList vehPrincipal;   // véhicule principal
    QStringList vehSecours;     // 2e véhicule (renfort / secours à personne)
    QStringList specialites;    // spécialités d'employé recherchées
    QStringList equipements;    // catégories d'équipement nécessaires (une par mot-clé)
};

Regle regleDuType(const QString &type)
{
    Regle r;
    if (type == "Incendie") {
        r.vehPrincipal << "incendie" << "fpt" << "citerne" << "pompier" << "echelle" << "camion";
        r.vehSecours   << "ambulance" << "vsav" << "secours";
        r.specialites  << "incendie" << "feu" << "sapeur" << "pompier" << "fire";
        r.equipements  << "extincteur" << "lance" << "tuyau" << "respirat" << "casque" << "echelle" << "hache";
    } else if (type == "Accident de la route") {
        r.vehPrincipal << "secours routier" << "vsr" << "desincarc" << "ambulance" << "secours";
        r.vehSecours   << "incendie" << "fpt" << "camion";
        r.specialites  << "routier" << "desincarc" << "secours" << "ambulancier" << "paramedic" << "medic";
        r.equipements  << "desincarc" << "civiere" << "brancard" << "trousse" << "defibrillateur" << "collier" << "cone";
    } else if (type == "Secours à personne") {
        r.vehPrincipal << "ambulance" << "vsav" << "sanitaire" << "secours";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "secours" << "ambulancier" << "infirmier" << "medic" << "paramedic";
        r.equipements  << "defibrillateur" << "oxygene" << "brancard" << "civiere" << "trousse" << "bouteille";
    } else if (type == "Fuite de gaz") {
        r.vehPrincipal << "incendie" << "fpt" << "technique" << "intervention" << "camion";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "gaz" << "technique" << "risque" << "incendie" << "fire";
        r.equipements  << "detecteur" << "gaz" << "respirat" << "masque" << "combinaison" << "extincteur";
    } else if (type == "Inondation") {
        r.vehPrincipal << "pompe" << "tout" << "citerne" << "utilitaire" << "intervention" << "camion";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "sauvetage" << "plong" << "nautique" << "pompage" << "secours" << "logistic";
        r.equipements  << "motopompe" << "pompe" << "bateau" << "gilet" << "corde" << "tuyau";
    } else {
        r.vehPrincipal << "camion" << "intervention" << "secours";
        r.vehSecours   << "ambulance";
        r.specialites  << "secours" << "incendie" << "fire" << "medic";
        r.equipements  << "trousse" << "extincteur";
    }
    return r;
}

// Besoins estimés d'après la gravité : Faible, Moyenne, Élevée, Critique
int nbVehicules(const QString &g)   { return g == "Critique" ? 3 : g == "Élevée" ? 2 : 1; }
int nbEmployes(const QString &g)    { return g == "Critique" ? 8 : g == "Élevée" ? 6 : g == "Moyenne" ? 4 : 3; }
int nbEquipements(const QString &g) { return g == "Critique" ? 5 : g == "Élevée" ? 4 : g == "Moyenne" ? 3 : 2; }

// Grade du module Employes : "1".."3" -> "1 étoile" / "2 étoiles" / "3 étoiles" (autre texte inchangé)
QString libelleGrade(const QString &g)
{
    const QString t = g.trimmed();
    if (t == "1") return QObject::tr("1 étoile");
    if (t == "2" || t == "3") return QObject::tr("%1 étoiles").arg(t);
    return g;
}

// Plus le rang est petit, plus le grade est élevé (grade inconnu : milieu de l'échelle)
int rangGrade(const QString &grade)
{
    static const char *ordre[] = { "colonel", "commandant", "capitaine", "lieutenant",
                                   "adjudant", "sergent", "caporal", "sapeur", "agent" };
    const QString g = norm(grade);
    if (g.contains("etoile")) {                       // grade du module Employes : 3 étoiles = le plus haut
        if (g.startsWith("3")) return 1;
        if (g.startsWith("2")) return 4;
        return 7;
    }
    for (int i = 0; i < 9; ++i)
        if (g.contains(ordre[i]))
            return i;
    return 5;
}

QSet<QString> chaines(const QString &sql, int idIntervention)
{
    QSet<QString> s;
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":id", idIntervention);
    q.bindValue(":t", QString("Terminée"));
    if (executer(q))
        while (q.next())
            s.insert(q.value(0).toString());
    return s;
}

} // namespace

QString Affectation::derniereErreur()
{
    return g_erreur;
}

// Nombre de {employés, véhicules, équipements} déjà liés à l'intervention
QVector<int> Affectation::compter(int idIntervention)
{
    QVector<int> n(3, 0);
    const char *sql[3] = {
        "SELECT COUNT(*) FROM PARTICIPER_UTILISER WHERE ID_INTERVENTION = :id",
        "SELECT COUNT(DISTINCT IMMATRICULATION) FROM PARTICIPER_UTILISER WHERE ID_INTERVENTION = :id",
        "SELECT COUNT(*) FROM NECESSITER WHERE ID_INTERVENTION = :id" };
    for (int i = 0; i < 3; ++i) {
        QSqlQuery q;
        q.prepare(sql[i]);
        q.bindValue(":id", idIntervention);
        if (executer(q) && q.next())
            n[i] = q.value(0).toInt();
    }
    return n;
}

PropositionAffectation Affectation::proposer(const Intervention &it)
{
    PropositionAffectation p;
    const Regle regle = regleDuType(it.getType());
    p.besoinVehicules = nbVehicules(it.getGravite());
    p.besoinEmployes = nbEmployes(it.getGravite());
    p.besoinEquipements = nbEquipements(it.getGravite());

    // Ressources déjà prises par une AUTRE intervention non terminée
    const QString occVeh = "SELECT DISTINCT PU.IMMATRICULATION FROM PARTICIPER_UTILISER PU "
                           "JOIN INTERVENTIONS I ON I.ID_INTERVENTION = PU.ID_INTERVENTION "
                           "WHERE PU.ID_INTERVENTION <> :id AND (I.STATUT IS NULL OR I.STATUT <> :t)";
    const QString occEmp = "SELECT DISTINCT PU.ID_EMPLOYE FROM PARTICIPER_UTILISER PU "
                           "JOIN INTERVENTIONS I ON I.ID_INTERVENTION = PU.ID_INTERVENTION "
                           "WHERE PU.ID_INTERVENTION <> :id AND (I.STATUT IS NULL OR I.STATUT <> :t)";
    const QSet<QString> vehOccupes = chaines(occVeh, it.getId());
    const QSet<QString> empOccupes = chaines(occEmp, it.getId());

    // ------------------------------------------------------------ véhicules
    QVector<PropositionAffectation::Vehicule> libres;
    {
        QSqlQuery q;
        q.prepare("SELECT IMMATRICULATION, TYPE, MODELE, ETAT FROM VEHICULES ORDER BY KILOMETRAGE, IMMATRICULATION");
        if (executer(q)) {
            static const QStringList horsService = QStringList() << "panne" << "maintenance" << "hors"
                                                                 << "reparation" << "indisponible" << "accident";
            while (q.next()) {
                PropositionAffectation::Vehicule v;
                v.immat = q.value(0).toString();
                v.type = q.value(1).toString();
                v.modele = q.value(2).toString();
                v.etat = q.value(3).toString();
                if (vehOccupes.contains(v.immat) || contientUn(v.etat, horsService))
                    continue;
                libres.push_back(v);
            }
        }
    }
    auto correspond = [](const PropositionAffectation::Vehicule &v, const QStringList &mots) {
        return contientUn(v.type + " " + v.modele, mots);
    };
    auto dejaChoisi = [&p](const QString &immat) {
        for (int i = 0; i < p.vehicules.size(); ++i)
            if (p.vehicules[i].immat == immat) return true;
        return false;
    };
    bool generique = false;
    for (int i = 0; i < libres.size() && p.vehicules.isEmpty(); ++i)          // 1) véhicule principal
        if (correspond(libres[i], regle.vehPrincipal)) p.vehicules.push_back(libres[i]);
    if (p.vehicules.isEmpty() && !libres.isEmpty()) {                         //    sinon : n'importe lequel
        p.vehicules.push_back(libres[0]);
        generique = true;
    }
    if (p.besoinVehicules >= 2) {                                             // 2) véhicule de secours / renfort
        for (int i = 0; i < libres.size() && p.vehicules.size() < 2; ++i)
            if (!dejaChoisi(libres[i].immat) && correspond(libres[i], regle.vehSecours))
                p.vehicules.push_back(libres[i]);
    }
    for (int pass = 0; pass < 2; ++pass)                                      // 3) compléter : principaux puis autres
        for (int i = 0; i < libres.size() && p.vehicules.size() < p.besoinVehicules; ++i)
            if (!dejaChoisi(libres[i].immat) && (pass == 1 || correspond(libres[i], regle.vehPrincipal)))
                p.vehicules.push_back(libres[i]);
    if (p.vehicules.isEmpty())
        p.avertissements << QObject::tr("Aucun véhicule disponible (tous sont en intervention, en panne ou en maintenance).");
    else if (generique)
        p.avertissements << QObject::tr("Aucun véhicule de type adapté à « %1 » n'est libre : un véhicule quelconque est proposé.")
                                .arg(it.getType());

    // ------------------------------------------------------------ employés
    struct Candidat { PropositionAffectation::Employe e; int score; };
    QVector<Candidat> candidats;
    {
        QSqlQuery q;
        q.prepare("SELECT ID_EMPLOYE, TRIM(NOM || ' ' || PRENOM), GRADE, SPECIALITE FROM EMPLOYE ORDER BY ID_EMPLOYE");   // table du module Employes
        if (executer(q)) {
            while (q.next()) {
                Candidat c;
                c.e.id = q.value(0).toInt();
                c.e.nom = q.value(1).toString();
                c.e.grade = libelleGrade(q.value(2).toString());
                c.e.specialite = q.value(3).toString();
                c.e.chef = false;
                if (empOccupes.contains(QString::number(c.e.id)))
                    continue;
                c.score = contientUn(c.e.specialite, regle.specialites) ? 0 : 1;   // spécialité adaptée d'abord
                candidats.push_back(c);
            }
        }
    }
    std::stable_sort(candidats.begin(), candidats.end(), [](const Candidat &a, const Candidat &b) {
        if (a.score != b.score) return a.score < b.score;
        return rangGrade(a.e.grade) < rangGrade(b.e.grade);
    });
    QVector<PropositionAffectation::Employe> choisis;
    for (int i = 0; i < candidats.size() && choisis.size() < p.besoinEmployes; ++i)
        choisis.push_back(candidats[i].e);
    if (choisis.isEmpty())
        p.avertissements << QObject::tr("Aucun employé disponible (tous participent déjà à une intervention non terminée).");

    // un véhicule sans équipage ne peut pas être enregistré (PARTICIPER_UTILISER lie employé + véhicule)
    while (p.vehicules.size() > choisis.size() && p.vehicules.size() > 0)
        p.vehicules.removeLast();
    if (!p.vehicules.isEmpty()) {
        // chefs d'équipe = les plus hauts gradés (un par véhicule), les autres sont répartis à tour de rôle
        std::stable_sort(choisis.begin(), choisis.end(),
                         [](const PropositionAffectation::Employe &a, const PropositionAffectation::Employe &b) {
                             return rangGrade(a.grade) < rangGrade(b.grade);
                         });
        for (int i = 0; i < choisis.size(); ++i) {
            choisis[i].immat = p.vehicules[i % p.vehicules.size()].immat;
            choisis[i].chef = (i < p.vehicules.size());
        }
        std::stable_sort(choisis.begin(), choisis.end(),
                         [](const PropositionAffectation::Employe &a, const PropositionAffectation::Employe &b) {
                             if (a.immat != b.immat) return a.immat < b.immat;
                             if (a.chef != b.chef) return a.chef;
                             return a.nom < b.nom;
                         });
        p.employes = choisis;
        if (p.employes.size() < p.besoinEmployes)
            p.avertissements << QObject::tr("Effectif insuffisant : %1 employé(s) disponible(s) pour %2 nécessaire(s).")
                                    .arg(p.employes.size()).arg(p.besoinEmployes);
        if (p.vehicules.size() < p.besoinVehicules)
            p.avertissements << QObject::tr("Véhicules insuffisants : %1 proposé(s) pour %2 nécessaire(s).")
                                    .arg(p.vehicules.size()).arg(p.besoinVehicules);
    }

    // ------------------------------------------------------------ équipements
    QMap<int, int> utilisations;     // équipement -> nombre d'interventions non terminées qui l'utilisent
    {
        QSqlQuery q;
        q.prepare("SELECT N.ID_EQUIPEMENT, COUNT(*) FROM NECESSITER N JOIN INTERVENTIONS I "
                  "ON I.ID_INTERVENTION = N.ID_INTERVENTION "
                  "WHERE N.ID_INTERVENTION <> :id AND (I.STATUT IS NULL OR I.STATUT <> :t) GROUP BY N.ID_EQUIPEMENT");
        q.bindValue(":id", it.getId());
        q.bindValue(":t", QString("Terminée"));
        if (executer(q))
            while (q.next())
                utilisations.insert(q.value(0).toInt(), q.value(1).toInt());
    }
    QSet<QString> vehChoisis;
    for (int i = 0; i < p.vehicules.size(); ++i)
        vehChoisis.insert(p.vehicules[i].immat);

    if (!p.vehicules.isEmpty()) {
        static const QStringList mauvaisEtat = QStringList() << "hors" << "defect" << "mauvais" << "perim"
                                                             << "casse" << "reparation";
        QVector<PropositionAffectation::Equipement> libresEq;
        QSqlQuery q;
        q.prepare("SELECT ID_EQUIPEMENT, CATEGORIE, QUANTITE, ETAT, TO_CHAR(DATE_PEREMPTION, 'YYYY-MM-DD'), IMMATRICULATION "
                  "FROM EQUIPEMENTS ORDER BY ID_EQUIPEMENT");
        if (executer(q)) {
            while (q.next()) {
                PropositionAffectation::Equipement e;
                e.id = q.value(0).toInt();
                e.categorie = q.value(1).toString();
                e.etat = q.value(3).toString();
                e.immat = q.value(5).toString();
                const QDate peremption = QDate::fromString(q.value(4).toString(), "yyyy-MM-dd");
                e.disponible = q.value(2).toInt() - utilisations.value(e.id, 0);
                if (e.disponible <= 0 || contientUn(e.etat, mauvaisEtat))
                    continue;
                if (peremption.isValid() && peremption <= QDate::currentDate())
                    continue;                                          // périmé
                if (!e.immat.isEmpty() && !vehChoisis.contains(e.immat))
                    continue;                                          // rangé dans un véhicule non affecté
                libresEq.push_back(e);
            }
        }
        // privilégie le matériel déjà embarqué dans un véhicule affecté
        std::stable_sort(libresEq.begin(), libresEq.end(),
                         [](const PropositionAffectation::Equipement &a, const PropositionAffectation::Equipement &b) {
                             return !a.immat.isEmpty() && b.immat.isEmpty();
                         });
        QSet<int> pris;
        for (int k = 0; k < regle.equipements.size() && p.equipements.size() < p.besoinEquipements; ++k) {
            for (int i = 0; i < libresEq.size(); ++i) {
                if (pris.contains(libresEq[i].id) || !contientUn(libresEq[i].categorie, QStringList(regle.equipements[k])))
                    continue;
                pris.insert(libresEq[i].id);
                p.equipements.push_back(libresEq[i]);
                break;
            }
        }
        if (p.equipements.isEmpty())
            p.avertissements << QObject::tr("Aucun équipement adapté n'est disponible (stock vide, périmé ou hors service).");
        else if (p.equipements.size() < p.besoinEquipements)
            p.avertissements << QObject::tr("Équipements insuffisants : %1 trouvé(s) pour %2 souhaité(s).")
                                    .arg(p.equipements.size()).arg(p.besoinEquipements);
    }
    return p;
}

bool Affectation::enregistrer(int idIntervention, const PropositionAffectation &p)
{
    if (idIntervention <= 0 || p.vehicules.isEmpty() || p.employes.isEmpty()) {
        g_erreur = QObject::tr("Affectation incomplète : il faut au moins un véhicule et un employé.");
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database();
    const bool transaction = db.transaction();        // tout ou rien (si le pilote le permet)
    auto echec = [&]() { if (transaction) db.rollback(); return false; };

    QSqlQuery q;
    q.prepare("DELETE FROM PARTICIPER_UTILISER WHERE ID_INTERVENTION = :id");
    q.bindValue(":id", idIntervention);
    if (!executer(q)) return echec();
    q.prepare("DELETE FROM NECESSITER WHERE ID_INTERVENTION = :id");
    q.bindValue(":id", idIntervention);
    if (!executer(q)) return echec();

    for (int i = 0; i < p.employes.size(); ++i) {
        q.prepare("INSERT INTO PARTICIPER_UTILISER (ID_EMPLOYE, IMMATRICULATION, ID_INTERVENTION) VALUES (:e, :v, :i)");
        q.bindValue(":e", p.employes[i].id);
        q.bindValue(":v", p.employes[i].immat);
        q.bindValue(":i", idIntervention);
        if (!executer(q)) return echec();
    }
    for (int i = 0; i < p.equipements.size(); ++i) {
        q.prepare("INSERT INTO NECESSITER (ID_EQUIPEMENT, ID_INTERVENTION) VALUES (:e, :i)");
        q.bindValue(":e", p.equipements[i].id);
        q.bindValue(":i", idIntervention);
        if (!executer(q)) return echec();
    }
    if (transaction && !db.commit()) {
        g_erreur = db.lastError().text();
        db.rollback();
        return false;
    }
    return true;
}
