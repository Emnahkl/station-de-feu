#include "employe.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtGlobal>

namespace {

QString g_erreur;

bool executer(QSqlQuery &q)
{
    if (q.exec())
        return true;
    g_erreur = q.lastError().text();
    qDebug() << "Erreur SQL :" << g_erreur;
    return false;
}

} // namespace

Employe::Employe(int id, const QString &nom, const QString &prenom, const QString &photo,
                 int grade, const QString &specialite, const QString &telephone,
                 const QString &statut, const QString &garde, bool volontaire)
    : id(id), nom(nom), prenom(prenom), photo(photo), grade(grade),
      specialite(specialite), telephone(telephone), statut(statut),
      garde(garde), volontaire(volontaire)
{
}

QString Employe::derniereErreur() { return g_erreur; }

// ---------------------------------------------------------------- Ajouter
bool Employe::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO EMPLOYE (id_employe, nom, prenom, photo, grade, specialite, "
              "telephone, statut, emploi_temps, volontaire) "
              "VALUES (seq_employe.NEXTVAL, :nom, :prenom, :photo, :grade, :specialite, "
              ":tel, :statut, :garde, :vol)");
    q.bindValue(":nom", nom);
    q.bindValue(":prenom", prenom);
    q.bindValue(":photo", photo);
    q.bindValue(":grade", QString::number(grade));
    q.bindValue(":specialite", specialite);
    q.bindValue(":tel", telephone);
    q.bindValue(":statut", statut);
    q.bindValue(":garde", garde);
    q.bindValue(":vol", volontaire ? 1 : 0);
    return executer(q);
}

// ---------------------------------------------------------------- Afficher
QSqlQueryModel *Employe::afficher()
{
    auto *model = new QSqlQueryModel;
    model->setQuery("SELECT id_employe, nom, prenom, photo, grade, specialite, telephone, "
                    "statut, emploi_temps, volontaire FROM EMPLOYE ORDER BY id_employe");
    if (model->lastError().isValid()) {
        g_erreur = model->lastError().text();
        qDebug() << "Erreur SQL :" << g_erreur;
    }
    // le pilote ODBC charge les lignes par paquets : on recupere tout
    while (model->canFetchMore())
        model->fetchMore();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Photo"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Grade"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Spécialité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Emploi du temps"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Volontaire"));
    return model;
}

// ---------------------------------------------------------------- Supprimer
bool Employe::supprimer(int idEmp)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    // On retire d'abord les liens (cles etrangeres) avec les autres modules.
    // Si une de ces tables n'existe pas encore, l'erreur est ignoree.
    QSqlQuery q;
    q.prepare("DELETE FROM PARTICIPER_UTILISER WHERE id_employe = :id");
    q.bindValue(":id", idEmp);
    q.exec();

    q.prepare("DELETE FROM ANIMER WHERE id_employe = :id");
    q.bindValue(":id", idEmp);
    q.exec();

    q.prepare("DELETE FROM PLANNING_JOUR WHERE id_employe = :id");
    q.bindValue(":id", idEmp);
    q.exec();

    q.prepare("DELETE FROM EMPLOYE WHERE id_employe = :id");
    q.bindValue(":id", idEmp);
    bool ok = executer(q);
    if (ok && q.numRowsAffected() == 0) {
        g_erreur = "Aucun employé avec cet identifiant.";
        ok = false;
    }

    if (ok) db.commit(); else db.rollback();
    return ok;
}

// ---------------------------------------------------------------- Modifier
bool Employe::modifier()
{
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYE SET nom = :nom, prenom = :prenom, photo = :photo, "
              "grade = :grade, specialite = :specialite, telephone = :tel, "
              "statut = :statut, emploi_temps = :garde, volontaire = :vol "
              "WHERE id_employe = :id");
    q.bindValue(":nom", nom);
    q.bindValue(":prenom", prenom);
    q.bindValue(":photo", photo);
    q.bindValue(":grade", QString::number(grade));
    q.bindValue(":specialite", specialite);
    q.bindValue(":tel", telephone);
    q.bindValue(":statut", statut);
    q.bindValue(":garde", garde);
    q.bindValue(":vol", volontaire ? 1 : 0);
    q.bindValue(":id", id);
    return executer(q);
}

// ---------------------------------------------------------------- Outils
bool Employe::changerStatut(int id, const QString &statut)
{
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYE SET statut = :statut WHERE id_employe = :id");
    q.bindValue(":statut", statut);
    q.bindValue(":id", id);
    return executer(q);
}

bool Employe::changerGarde(int id, const QString &garde)
{
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYE SET emploi_temps = :garde WHERE id_employe = :id");
    q.bindValue(":garde", garde);
    q.bindValue(":id", id);
    return executer(q);
}

bool Employe::changerGardes(const QVector<QPair<int, QString>> &liste)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    bool ok = true;
    for (const auto &p : liste) {
        QSqlQuery q;
        q.prepare("UPDATE EMPLOYE SET emploi_temps = :garde WHERE id_employe = :id");
        q.bindValue(":garde", p.second);
        q.bindValue(":id", p.first);
        if (!executer(q)) { ok = false; break; }
    }
    if (ok) db.commit(); else db.rollback();
    return ok;
}

QVector<Employe> Employe::chargerTous()
{
    QVector<Employe> liste;
    QSqlQueryModel *m = Employe().afficher();

    for (int r = 0; r < m->rowCount(); ++r) {
        auto d = [&](int c) { return m->data(m->index(r, c)); };

        Employe e;
        e.id         = d(0).toInt();
        e.nom        = d(1).toString();
        e.prenom     = d(2).toString();
        e.photo      = d(3).toString();
        e.grade      = qBound(1, d(4).toString().trimmed().toInt(), 3);
        e.specialite = d(5).toString().isEmpty() ? QString("Fire") : d(5).toString();
        e.telephone  = d(6).toString();
        e.statut     = d(7).toString().isEmpty() ? QString("En service") : d(7).toString();
        e.garde      = d(8).toString().isEmpty() ? QString("Aucune") : d(8).toString();
        e.volontaire = d(9).toInt() == 1;
        liste.append(e);
    }
    delete m;
    return liste;
}

// ---------------------------------------------------------------- Calendrier de travail
// Table PLANNING_JOUR (id_employe, jour, garde) : creee automatiquement si elle n'existe pas.
bool Employe::assurerTablePlanning()
{
    static bool fait = false;
    if (fait)
        return true;
    QSqlQuery q;
    // si la table existe deja, Oracle repond ORA-00955 : c'est normal, on ignore
    q.exec("CREATE TABLE PLANNING_JOUR ("
           "id_employe NUMBER(10) NOT NULL, "
           "jour DATE NOT NULL, "
           "garde VARCHAR2(100), "
           "CONSTRAINT pk_planning_jour PRIMARY KEY (id_employe, jour), "
           "CONSTRAINT fk_plan_emp FOREIGN KEY (id_employe) REFERENCES EMPLOYE(id_employe))");
    fait = true;
    return true;
}

QMap<QDate, QString> Employe::chargerPlanning(int idEmploye)
{
    assurerTablePlanning();
    QMap<QDate, QString> res;
    QSqlQuery q;
    q.prepare("SELECT TO_CHAR(jour, 'YYYY-MM-DD'), garde FROM PLANNING_JOUR WHERE id_employe = :id");
    q.bindValue(":id", idEmploye);
    if (!executer(q))
        return res;
    while (q.next()) {
        const QDate d = QDate::fromString(q.value(0).toString(), "yyyy-MM-dd");
        if (d.isValid())
            res.insert(d, q.value(1).toString());
    }
    return res;
}

bool Employe::affecterJour(int idEmploye, const QDate &jour, const QString &garde)
{
    assurerTablePlanning();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
    q.prepare("DELETE FROM PLANNING_JOUR WHERE id_employe = :id AND jour = TO_DATE(:j, 'YYYY-MM-DD')");
    q.bindValue(":id", idEmploye);
    q.bindValue(":j", jour.toString("yyyy-MM-dd"));
    bool ok = executer(q);

    if (ok) {
        QSqlQuery ins;
        ins.prepare("INSERT INTO PLANNING_JOUR (id_employe, jour, garde) "
                    "VALUES (:id, TO_DATE(:j, 'YYYY-MM-DD'), :g)");
        ins.bindValue(":id", idEmploye);
        ins.bindValue(":j", jour.toString("yyyy-MM-dd"));
        ins.bindValue(":g", garde);
        ok = executer(ins);
    }
    if (ok) db.commit(); else db.rollback();
    return ok;
}

bool Employe::retirerJour(int idEmploye, const QDate &jour)
{
    assurerTablePlanning();
    QSqlQuery q;
    q.prepare("DELETE FROM PLANNING_JOUR WHERE id_employe = :id AND jour = TO_DATE(:j, 'YYYY-MM-DD')");
    q.bindValue(":id", idEmploye);
    q.bindValue(":j", jour.toString("yyyy-MM-dd"));
    return executer(q);
}
