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
        r.specialites  << "incendie" << "feu" << "sapeur" << "pompier";
        r.equipements  << "extincteur" << "lance" << "tuyau" << "respirat" << "casque" << "echelle" << "hache";
    } else if (type == "Accident de la route") {
        r.vehPrincipal << "secours routier" << "vsr" << "desincarc" << "ambulance" << "secours";
        r.vehSecours   << "incendie" << "fpt" << "camion";
        r.specialites  << "routier" << "desincarc" << "secours" << "ambulancier" << "paramedic";
        r.equipements  << "desincarc" << "civiere" << "brancard" << "trousse" << "defibrillateur" << "collier" << "cone";
    } else if (type == "Secours à personne") {
        r.vehPrincipal << "ambulance" << "vsav" << "sanitaire" << "secours";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "secours" << "ambulancier" << "infirmier" << "medic" << "paramedic";
        r.equipements  << "defibrillateur" << "oxygene" << "brancard" << "civiere" << "trousse" << "bouteille";
    } else if (type == "Fuite de gaz") {
        r.vehPrincipal << "incendie" << "fpt" << "technique" << "intervention" << "camion";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "gaz" << "technique" << "risque" << "incendie";
        r.equipements  << "detecteur" << "gaz" << "respirat" << "masque" << "combinaison" << "extincteur";
    } else if (type == "Inondation") {
        r.vehPrincipal << "pompe" << "tout" << "citerne" << "utilitaire" << "intervention" << "camion";
        r.vehSecours   << "ambulance" << "secours";
        r.specialites  << "sauvetage" << "plong" << "nautique" << "pompage" << "secours";
        r.equipements  << "motopompe" << "pompe" << "bateau" << "gilet" << "corde" << "tuyau";
    } else {
        r.vehPrincipal << "camion" << "intervention" << "secours";
        r.vehSecours   << "ambulance";
        r.specialites  << "secours" << "incendie";
        r.equipements  << "trousse" << "extincteur";
    }
    return r;
}

// Besoins estimés d'après la gravité : Faible, Moyenne, Élevée, Critique
int nbVehicules(const QString &g)   { return g == "Critique" ? 3 : g == "Élevée" ? 2 : 1; }
int nbEmployes(const QString &g)    { return g == "Critique" ? 8 : g == "Élevée" ? 6 : g == "Moyenne" ? 4 : 3; }
int nbEquipements(const QString &g) { return g == "Critique" ? 5 : g == "Élevée" ? 4 : g == "Moyenne" ? 3 : 2; }

// Plus le rang est petit, plus le grade est élevé (grade inconnu : milieu de l'échelle)
int rangGrade(const QString &grade)
{
    static const char *ordre[] = { "colonel", "commandant", "capitaine", "lieutenant",
                                   "adjudant", "sergent", "caporal", "sapeur", "agent" };
    const QString g = norm(grade);
    for (int i = 0; i < 9; ++i)
        if (g.contains(ordre[i]))
            return i;
    return 5;
}

int rangGravite(const QString &g)
{
    if (g == "Critique") return 4;
    if (g == "Élevée")   return 3;
    if (g == "Moyenne")  return 2;
    return 1;
}

// Ressource occupée par une AUTRE intervention non terminée (sert au plan B : on ne l'affecte pas
// automatiquement, on la propose seulement si cette autre intervention est moins grave).
struct Occupe { QString ressource; int idInter; QString gravite; };

QVector<Occupe> occupees(const QString &sql, int idIntervention);

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

QVector<Occupe> occupees(const QString &sql, int idIntervention)
{
    QVector<Occupe> r;
    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":id", idIntervention);
    q.bindValue(":t", QString("Terminée"));
    if (executer(q)) {
        while (q.next()) {
            Occupe o;
            o.ressource = q.value(0).toString();
            o.idInter = q.value(1).toInt();
            o.gravite = q.value(2).toString();
            r.push_back(o);
        }
    }
    return r;
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
    // Score de pertinence : principal 100, renfort / secours 75, remplaçant 40
    for (int i = 0; i < p.vehicules.size(); ++i) {
        PropositionAffectation::Vehicule &v = p.vehicules[i];
        if (correspond(v, regle.vehPrincipal))      { v.score = 100; }
        else if (correspond(v, regle.vehSecours))   { v.score = 75; v.note = QObject::tr("renfort / secours"); }
        else                                        { v.score = 40; v.note = QObject::tr("remplaçant"); }
    }
    if (p.vehicules.isEmpty())
        p.avertissements << QObject::tr("Aucun véhicule disponible (tous sont en intervention, en panne ou en maintenance).");
    else if (generique)
        p.planB << QObject::tr("aucun véhicule adapté à « %1 » n'est libre : %2 (%3) est proposé à la place, "
                               "car c'est le véhicule libre le moins kilométré.")
                       .arg(it.getType(), p.vehicules[0].immat, p.vehicules[0].type);

    // ------------------------------------------------------------ employés
    struct Candidat { PropositionAffectation::Employe e; int score; };
    QVector<Candidat> candidats;
    QMap<int, int> charges;          // employé -> nombre de missions sur les 7 derniers jours
    {
        QSqlQuery qc;
        qc.prepare("SELECT PU.ID_EMPLOYE, COUNT(DISTINCT PU.ID_INTERVENTION) FROM PARTICIPER_UTILISER PU "
                   "JOIN INTERVENTIONS I ON I.ID_INTERVENTION = PU.ID_INTERVENTION "
                   "WHERE PU.ID_INTERVENTION <> :id AND I.DATE_INTERVENTION >= TRUNC(SYSDATE) - 7 "
                   "GROUP BY PU.ID_EMPLOYE");
        qc.bindValue(":id", it.getId());
        if (executer(qc))
            while (qc.next())
                charges.insert(qc.value(0).toInt(), qc.value(1).toInt());
    }
    {
        QSqlQuery q;
        q.prepare("SELECT ID_EMPLOYE, TRIM(NOM || ' ' || PRENOM), GRADE, SPECIALITE FROM EMPLOYE ORDER BY ID_EMPLOYE");
        if (executer(q)) {
            while (q.next()) {
                Candidat c;
                c.e.id = q.value(0).toInt();
                c.e.nom = q.value(1).toString();
                c.e.grade = q.value(2).toString();
                c.e.specialite = q.value(3).toString();
                c.e.chef = false;
                if (empOccupes.contains(QString::number(c.e.id)))
                    continue;
                // score 0..100 = spécialité adaptée (55) + grade (25) + repos récent (20)
                c.e.charge = charges.value(c.e.id, 0);
                const int pSpec  = contientUn(c.e.specialite, regle.specialites) ? 55 : 0;
                const int pGrade = (8 - rangGrade(c.e.grade)) * 25 / 8;
                const int pRepos = 20 - 5 * qMin(4, c.e.charge);
                c.e.score = pSpec + pGrade + pRepos;
                c.score = c.e.score;
                candidats.push_back(c);
            }
        }
    }
    // Équipe équilibrée : on prend le meilleur score, avec un petit bonus (+8) pour une spécialité
    // pas encore présente dans l'équipe (mélange des compétences).
    QVector<PropositionAffectation::Employe> choisis;
    QSet<QString> specialitesVues;
    while (choisis.size() < p.besoinEmployes && !candidats.isEmpty()) {
        int meilleur = 0, meilleurScore = -1;
        for (int i = 0; i < candidats.size(); ++i) {
            const int sc = candidats[i].score + (specialitesVues.contains(norm(candidats[i].e.specialite)) ? 0 : 8);
            if (sc > meilleurScore) { meilleurScore = sc; meilleur = i; }
        }
        specialitesVues.insert(norm(candidats[meilleur].e.specialite));
        choisis.push_back(candidats[meilleur].e);
        candidats.remove(meilleur);
    }
    if (choisis.isEmpty())
        p.avertissements << QObject::tr("Aucun employé disponible (tous participent déjà à une intervention non terminée).");

    // un véhicule sans équipage ne peut pas être enregistré (PARTICIPER lie employé + véhicule)
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
        {
            QSet<QString> specs, grades;
            int totalCharge = 0;
            for (int i = 0; i < choisis.size(); ++i) {
                specs.insert(norm(choisis[i].specialite));
                grades.insert(norm(choisis[i].grade));
                totalCharge += choisis[i].charge;
            }
            p.equilibre = QObject::tr("équipe de %1 : %2 spécialité(s), %3 grade(s), %4 mission(s) récente(s) au total (7 j)")
                              .arg(choisis.size()).arg(specs.size()).arg(grades.size()).arg(totalCharge);
        }
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
        QStringList manquants;                                            // mots-clés sans équipement trouvé
        for (int k = 0; k < regle.equipements.size() && p.equipements.size() < p.besoinEquipements; ++k) {
            bool trouve = false;
            for (int i = 0; i < libresEq.size(); ++i) {
                if (pris.contains(libresEq[i].id) || !contientUn(libresEq[i].categorie, QStringList(regle.equipements[k])))
                    continue;
                pris.insert(libresEq[i].id);
                libresEq[i].score = libresEq[i].immat.isEmpty() ? 85 : 100;   // embarqué = prêt à partir
                p.equipements.push_back(libresEq[i]);
                trouve = true;
                break;
            }
            if (!trouve)
                manquants << regle.equipements[k];
        }
        // Plan B : un équipement d'une autre famille d'usage, sinon un équipement polyvalent
        for (int m = 0; m < manquants.size() && p.equipements.size() < p.besoinEquipements; ++m) {
            int choix = -1;
            bool memeUsage = false;
            for (int i = 0; i < libresEq.size() && choix < 0; ++i)
                if (!pris.contains(libresEq[i].id) && contientUn(libresEq[i].categorie, regle.equipements)) {
                    choix = i; memeUsage = true;
                }
            for (int i = 0; i < libresEq.size() && choix < 0; ++i)
                if (!pris.contains(libresEq[i].id))
                    choix = i;
            if (choix < 0)
                break;
            pris.insert(libresEq[choix].id);
            libresEq[choix].score = memeUsage ? 60 : 35;
            libresEq[choix].note = QObject::tr("remplaçant");
            p.equipements.push_back(libresEq[choix]);
            p.planB << QObject::tr("aucun équipement « %1 » disponible : « %2 » est proposé à la place (%3).")
                           .arg(manquants[m], libresEq[choix].categorie,
                                memeUsage ? QObject::tr("même famille d'usage, libre et en bon état")
                                          : QObject::tr("équipement polyvalent, libre et en bon état"));
        }
        if (p.equipements.isEmpty())
            p.avertissements << QObject::tr("Aucun équipement adapté n'est disponible (stock vide, périmé ou hors service).");
        else if (p.equipements.size() < p.besoinEquipements)
            p.avertissements << QObject::tr("Équipements insuffisants : %1 trouvé(s) pour %2 souhaité(s).")
                                    .arg(p.equipements.size()).arg(p.besoinEquipements);
    }
    // ------------------------------------------------------------ plan B : ressources à rappeler
    // Si l'effectif ou les véhicules manquent, on cite ceux retenus par une intervention MOINS grave.
    const int rg = rangGravite(it.getGravite());
    if (p.employes.size() < p.besoinEmployes) {
        const int manque = p.besoinEmployes - p.employes.size();
        const QVector<Occupe> occ = occupees(
            "SELECT DISTINCT TRIM(E.NOM || ' ' || E.PRENOM), I.ID_INTERVENTION, I.GRAVITE FROM PARTICIPER_UTILISER PU "
            "JOIN INTERVENTIONS I ON I.ID_INTERVENTION = PU.ID_INTERVENTION "
            "JOIN EMPLOYE E ON E.ID_EMPLOYE = PU.ID_EMPLOYE "
            "WHERE PU.ID_INTERVENTION <> :id AND (I.STATUT IS NULL OR I.STATUT <> :t)", it.getId());
        QStringList noms;
        for (int i = 0; i < occ.size() && noms.size() < manque; ++i)
            if (rangGravite(occ[i].gravite) < rg)
                noms << QObject::tr("%1 (intervention n° %2, gravité %3)").arg(occ[i].ressource).arg(occ[i].idInter).arg(occ[i].gravite);
        if (!noms.isEmpty())
            p.planB << QObject::tr("il manque %1 employé(s) : on peut rappeler %2, car leur intervention est moins grave.")
                           .arg(manque).arg(noms.join(", "));
    }
    if (p.vehicules.size() < p.besoinVehicules) {
        const int manque = p.besoinVehicules - p.vehicules.size();
        const QVector<Occupe> occ = occupees(
            "SELECT DISTINCT PU.IMMATRICULATION, I.ID_INTERVENTION, I.GRAVITE FROM PARTICIPER_UTILISER PU "
            "JOIN INTERVENTIONS I ON I.ID_INTERVENTION = PU.ID_INTERVENTION "
            "WHERE PU.ID_INTERVENTION <> :id AND (I.STATUT IS NULL OR I.STATUT <> :t)", it.getId());
        QStringList l;
        for (int i = 0; i < occ.size() && l.size() < manque; ++i)
            if (rangGravite(occ[i].gravite) < rg)
                l << QObject::tr("%1 (intervention n° %2, gravité %3)").arg(occ[i].ressource).arg(occ[i].idInter).arg(occ[i].gravite);
        if (!l.isEmpty())
            p.planB << QObject::tr("il manque %1 véhicule(s) : on peut rappeler %2, car leur intervention est moins grave.")
                           .arg(manque).arg(l.join(", "));
    }

    // ------------------------------------------------------------ pertinence globale
    int somme = 0, nb = 0;
    for (int i = 0; i < p.vehicules.size(); ++i)   { somme += p.vehicules[i].score;   ++nb; }
    for (int i = 0; i < p.employes.size(); ++i)    { somme += p.employes[i].score;    ++nb; }
    for (int i = 0; i < p.equipements.size(); ++i) { somme += p.equipements[i].score; ++nb; }
    p.scoreGlobal = nb ? somme / nb : 0;
    return p;
}

QVector<RessourceDispo> Affectation::disponibilites()
{
    QVector<RessourceDispo> r;
    // clé de la ressource -> numéros des interventions NON terminées qui l'utilisent
    auto occupations = [](const QString &sql) {
        QMap<QString, QStringList> m;
        QSqlQuery q;
        q.prepare(sql);
        q.bindValue(":t", QString("Terminée"));
        if (executer(q)) {
            while (q.next()) {
                const QString cle = q.value(0).toString();
                const QString n = q.value(1).toString();
                if (!m[cle].contains(n))
                    m[cle] << n;
            }
        }
        return m;
    };
    const QString from = " FROM %1 X JOIN INTERVENTIONS I ON I.ID_INTERVENTION = X.ID_INTERVENTION "
                         "WHERE (I.STATUT IS NULL OR I.STATUT <> :t)";
    const QMap<QString, QStringList> empOcc = occupations("SELECT X.ID_EMPLOYE, X.ID_INTERVENTION" + from.arg("PARTICIPER_UTILISER"));
    const QMap<QString, QStringList> vehOcc = occupations("SELECT X.IMMATRICULATION, X.ID_INTERVENTION" + from.arg("PARTICIPER_UTILISER"));
    const QMap<QString, QStringList> eqOcc  = occupations("SELECT X.ID_EQUIPEMENT, X.ID_INTERVENTION" + from.arg("NECESSITER"));

    auto trier = [&r](int debut) {          // libres d'abord, dans la famille commençant à « debut »
        std::stable_sort(r.begin() + debut, r.end(), [](const RessourceDispo &a, const RessourceDispo &b) {
            return a.libre && !b.libre;
        });
    };
    auto occupe = [](const QStringList &l) {
        return QObject::tr("Occupé – intervention n° %1").arg(l.join(", "));
    };

    // ---- employés
    int debut = r.size();
    {
        QSqlQuery q;
        q.prepare("SELECT ID_EMPLOYE, TRIM(NOM || ' ' || PRENOM), GRADE, SPECIALITE FROM EMPLOYE ORDER BY NOM, PRENOM");
        if (executer(q)) {
            while (q.next()) {
                RessourceDispo d;
                d.genre = QObject::tr("Employé");
                d.designation = q.value(1).toString();
                d.detail = q.value(2).toString() + (q.value(3).toString().isEmpty() ? QString() : " · " + q.value(3).toString());
                const QString cle = q.value(0).toString();
                d.libre = !empOcc.contains(cle);
                d.statut = d.libre ? QObject::tr("Libre") : occupe(empOcc.value(cle));
                r.push_back(d);
            }
        }
    }
    trier(debut);

    // ---- véhicules
    debut = r.size();
    {
        static const QStringList horsService = QStringList() << "panne" << "maintenance" << "hors"
                                                             << "reparation" << "indisponible" << "accident";
        QSqlQuery q;
        q.prepare("SELECT IMMATRICULATION, TYPE, MODELE, ETAT FROM VEHICULES ORDER BY IMMATRICULATION");
        if (executer(q)) {
            while (q.next()) {
                RessourceDispo d;
                d.genre = QObject::tr("Véhicule");
                const QString immat = q.value(0).toString();
                const QString etat = q.value(3).toString();
                d.designation = immat + " – " + q.value(1).toString();
                d.detail = (q.value(2).toString().isEmpty() ? QString() : q.value(2).toString() + " · ")
                           + QObject::tr("état : ") + (etat.isEmpty() ? "—" : etat);
                if (contientUn(etat, horsService)) {
                    d.libre = false;
                    d.statut = QObject::tr("Indisponible – %1").arg(etat);
                } else if (vehOcc.contains(immat)) {
                    d.libre = false;
                    d.statut = occupe(vehOcc.value(immat));
                } else {
                    d.libre = true;
                    d.statut = QObject::tr("Libre");
                }
                r.push_back(d);
            }
        }
    }
    trier(debut);

    // ---- équipements
    debut = r.size();
    {
        static const QStringList mauvaisEtat = QStringList() << "hors" << "defect" << "mauvais" << "perim"
                                                             << "casse" << "reparation";
        QSqlQuery q;
        q.prepare("SELECT ID_EQUIPEMENT, CATEGORIE, QUANTITE, ETAT, TO_CHAR(DATE_PEREMPTION, 'YYYY-MM-DD'), IMMATRICULATION "
                  "FROM EQUIPEMENTS ORDER BY CATEGORIE, ID_EQUIPEMENT");
        if (executer(q)) {
            while (q.next()) {
                RessourceDispo d;
                d.genre = QObject::tr("Équipement");
                const QString cle = q.value(0).toString();
                const int quantite = q.value(2).toInt();
                const QString etat = q.value(3).toString();
                const QDate peremption = QDate::fromString(q.value(4).toString(), "yyyy-MM-dd");
                const QString immat = q.value(5).toString();
                const int utilise = eqOcc.value(cle).size();
                d.designation = q.value(1).toString();
                d.detail = QObject::tr("quantité : %1").arg(quantite) + " · "
                           + (immat.isEmpty() ? QObject::tr("dépôt") : QObject::tr("dans le véhicule %1").arg(immat));
                if (contientUn(etat, mauvaisEtat)) {
                    d.statut = QObject::tr("Indisponible – état : %1").arg(etat);
                } else if (peremption.isValid() && peremption <= QDate::currentDate()) {
                    d.statut = QObject::tr("Périmé depuis le %1").arg(peremption.toString("dd/MM/yyyy"));
                } else if (quantite - utilise <= 0) {
                    d.statut = eqOcc.contains(cle) ? QObject::tr("Épuisé – utilisé par l'intervention n° %1").arg(eqOcc.value(cle).join(", "))
                                                   : QObject::tr("Stock vide");
                } else {
                    d.libre = true;
                    d.statut = QObject::tr("Libre – %1 disponible(s)").arg(quantite - utilise);
                }
                r.push_back(d);
            }
        }
    }
    trier(debut);
    return r;
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
