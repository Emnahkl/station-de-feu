#include "equipement.h"
#include <QSqlError>
#include <QStringList>
#include <QDebug>

// Requête de base : les alias ("Nom", "Quantité"...) deviennent les titres du tableau.
static const char *SELECT_BASE =
    "SELECT ID, NOM AS \"Nom\", TYPE_EQUIP AS \"Type\", QUANTITE AS \"Quantité\", "
    "DATE_ACQUISITION AS \"Date d'acquisition\", ETAT AS \"État\", "
    "DATE_CONTROLE AS \"Prochain contrôle\", AFFECTATION AS \"Affectation\", "
    "SEUIL_ALERTE AS \"Seuil d'alerte\" FROM EQUIPEMENT";

Equipement::Equipement(const QString &nom, const QString &type, int quantite,
                       const QDate &dateAcq, const QString &etat, const QDate &dateCtrl,
                       const QString &affectation, int seuilAlerte)
    : id(0), nom(nom), type(type), quantite(quantite), dateAcquisition(dateAcq),
      etat(etat), dateControle(dateCtrl), affectation(affectation), seuilAlerte(seuilAlerte)
{
}

// ---------------------------------------------------------------- CRUD
bool Equipement::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO EQUIPEMENT (NOM, TYPE_EQUIP, QUANTITE, DATE_ACQUISITION, ETAT, "
              "DATE_CONTROLE, AFFECTATION, SEUIL_ALERTE) "
              "VALUES (:nom, :type, :qte, :dacq, :etat, :dctrl, :aff, :seuil)");
    q.bindValue(":nom",   nom);
    q.bindValue(":type",  type);
    q.bindValue(":qte",   quantite);
    q.bindValue(":dacq",  dateAcquisition.toString(Qt::ISODate));
    q.bindValue(":etat",  etat);
    q.bindValue(":dctrl", dateControle.toString(Qt::ISODate));
    q.bindValue(":aff",   affectation);
    q.bindValue(":seuil", seuilAlerte);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return true;
}

bool Equipement::modifier()
{
    QSqlQuery q;
    q.prepare("UPDATE EQUIPEMENT SET NOM=:nom, TYPE_EQUIP=:type, QUANTITE=:qte, "
              "DATE_ACQUISITION=:dacq, ETAT=:etat, DATE_CONTROLE=:dctrl, "
              "AFFECTATION=:aff, SEUIL_ALERTE=:seuil WHERE ID=:id");
    q.bindValue(":nom",   nom);
    q.bindValue(":type",  type);
    q.bindValue(":qte",   quantite);
    q.bindValue(":dacq",  dateAcquisition.toString(Qt::ISODate));
    q.bindValue(":etat",  etat);
    q.bindValue(":dctrl", dateControle.toString(Qt::ISODate));
    q.bindValue(":aff",   affectation);
    q.bindValue(":seuil", seuilAlerte);
    q.bindValue(":id",    id);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return q.numRowsAffected() > 0;
}

bool Equipement::supprimer(int idASupprimer)
{
    QSqlQuery q;
    q.prepare("DELETE FROM EQUIPEMENT WHERE ID=:id");
    q.bindValue(":id", idASupprimer);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return q.numRowsAffected() > 0;
}

// ------------------------------------------------- Recherche + Tri
QSqlQuery Equipement::lister(const QString &champRech, const QString &texte,
                             const QString &champTri, bool croissant)
{
    // Liste blanche : on n'insère JAMAIS un nom de colonne venant de l'utilisateur sans le vérifier
    static const QStringList colonnesOk = { "ID", "NOM", "TYPE_EQUIP", "QUANTITE",
        "DATE_ACQUISITION", "ETAT", "DATE_CONTROLE", "AFFECTATION" };

    QString sql = SELECT_BASE;
    const bool filtre = !texte.trimmed().isEmpty() && colonnesOk.contains(champRech);
    if (filtre)
        sql += " WHERE UPPER(" + champRech + ") LIKE UPPER(:txt)";
    const QString tri = colonnesOk.contains(champTri) ? champTri : QString("ID");
    sql += " ORDER BY " + tri + (croissant ? " ASC" : " DESC");

    QSqlQuery q;
    q.prepare(sql);
    if (filtre)
        q.bindValue(":txt", "%" + texte.trimmed() + "%");
    q.exec();
    return q;
}

// ------------------------------------------------------ Statistiques
static QMap<QString, int> compterPar(const QString &colonne)
{
    QMap<QString, int> res;
    QSqlQuery q("SELECT " + colonne + ", COUNT(*) FROM EQUIPEMENT GROUP BY " + colonne);
    while (q.next())
        res[q.value(0).toString()] = q.value(1).toInt();
    return res;
}
QMap<QString, int> Equipement::statsParEtat() { return compterPar("ETAT"); }
QMap<QString, int> Equipement::statsParType() { return compterPar("TYPE_EQUIP"); }

// ------------------------------------------------- Métiers innovants
QSqlQuery Equipement::stockEnAlerte()
{
    QSqlQuery q;
    q.exec(QString(SELECT_BASE) + " WHERE QUANTITE <= SEUIL_ALERTE ORDER BY QUANTITE ASC");
    return q;
}

QSqlQuery Equipement::controlesProches(int jours)
{
    QSqlQuery q;
    q.prepare(QString(SELECT_BASE) + " WHERE DATE_CONTROLE <= :limite ORDER BY DATE_CONTROLE ASC");
    q.bindValue(":limite", QDate::currentDate().addDays(jours).toString(Qt::ISODate));
    q.exec();
    return q;
}
