#include "equipement.h"

#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <QVariant>
#include <QBrush>
#include <QColor>

static const QString SELECT_BASE =
    "SELECT ID_EQUIPEMENT AS ID, DESIGNATION AS \"Nom\", CATEGORIE AS \"Type\", "
    "QUANTITE AS \"Quantité\", TO_CHAR(DATE_ACQUISITION, 'YYYY-MM-DD') AS \"Date d'acquisition\", "
    "ETAT AS \"État\", TO_CHAR(DATE_PROCHAIN_CONTROLE, 'YYYY-MM-DD') AS \"Prochain contrôle\", "
    "TO_CHAR(DATE_PEREMPTION, 'YYYY-MM-DD') AS \"Date de péremption\", "
    "NVL(IMMATRICULATION, '-') AS \"Véhicule affecté\", "
    "SEUIL_ALERTE AS \"Seuil d'alerte\" FROM EQUIPEMENTS";

Equipement::Equipement(const QString &nom, const QString &type, int quantite,
                       const QDate &dateAcq, const QString &etat, const QDate &dateCtrl,
                       const QString &affectation, int seuilAlerte, const QDate &datePeremption)
    : id(0), nom(nom), type(type), quantite(quantite), dateAcquisition(dateAcq),
      etat(etat), dateControle(dateCtrl), affectation(affectation.trimmed()),
      seuilAlerte(seuilAlerte), datePeremption(datePeremption)
{
}

bool Equipement::ajouter()
{
    QSqlQuery q;
    q.prepare("INSERT INTO EQUIPEMENTS "
              "(DESIGNATION, CATEGORIE, QUANTITE, DATE_ACQUISITION, ETAT, "
              "DATE_PROCHAIN_CONTROLE, DATE_PEREMPTION, IMMATRICULATION, SEUIL_ALERTE) "
              "VALUES (:nom, :type, :qte, :dacq, :etat, :dctrl, :dexp, :aff, :seuil)");
    q.bindValue(":nom", nom);
    q.bindValue(":type", type);
    q.bindValue(":qte", quantite);
    q.bindValue(":dacq", dateAcquisition);
    q.bindValue(":etat", etat);
    q.bindValue(":dctrl", dateControle);
    q.bindValue(":dexp", datePeremption);
    q.bindValue(":aff", affectation.isEmpty() ? QVariant() : QVariant(affectation));
    q.bindValue(":seuil", seuilAlerte);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return true;
}

bool Equipement::modifier()
{
    QSqlQuery q;
    q.prepare("UPDATE EQUIPEMENTS SET DESIGNATION=:nom, CATEGORIE=:type, QUANTITE=:qte, "
              "DATE_ACQUISITION=:dacq, ETAT=:etat, DATE_PROCHAIN_CONTROLE=:dctrl, "
              "DATE_PEREMPTION=:dexp, IMMATRICULATION=:aff, SEUIL_ALERTE=:seuil "
              "WHERE ID_EQUIPEMENT=:id");
    q.bindValue(":nom", nom);
    q.bindValue(":type", type);
    q.bindValue(":qte", quantite);
    q.bindValue(":dacq", dateAcquisition);
    q.bindValue(":etat", etat);
    q.bindValue(":dctrl", dateControle);
    q.bindValue(":dexp", datePeremption);
    q.bindValue(":aff", affectation.isEmpty() ? QVariant() : QVariant(affectation));
    q.bindValue(":seuil", seuilAlerte);
    q.bindValue(":id", id);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return q.numRowsAffected() > 0;
}

bool Equipement::supprimer(int idASupprimer)
{
    QSqlQuery q;
    q.prepare("DELETE FROM EQUIPEMENTS WHERE ID_EQUIPEMENT=:id");
    q.bindValue(":id", idASupprimer);
    if (!q.exec()) { qDebug() << q.lastError().text(); return false; }
    return q.numRowsAffected() > 0;
}

QSqlQuery Equipement::lister(const QString &champRech, const QString &texte,
                             const QString &champTri, bool croissant)
{
    static const QStringList colonnesOk = {
        "ID_EQUIPEMENT", "DESIGNATION", "CATEGORIE", "QUANTITE", "DATE_ACQUISITION",
        "ETAT", "DATE_PROCHAIN_CONTROLE", "DATE_PEREMPTION", "IMMATRICULATION", "SEUIL_ALERTE"
    };
    QString sql = SELECT_BASE;
    const bool filtre = !texte.trimmed().isEmpty() && colonnesOk.contains(champRech);
    if (filtre) sql += " WHERE UPPER(" + champRech + ") LIKE UPPER(:txt)";
    const QString tri = colonnesOk.contains(champTri) ? champTri : QString("ID_EQUIPEMENT");
    sql += " ORDER BY " + tri + (croissant ? " ASC" : " DESC");
    QSqlQuery q;
    q.prepare(sql);
    if (filtre) q.bindValue(":txt", "%" + texte.trimmed() + "%");
    if (!q.exec()) qDebug() << q.lastError().text();
    return q;
}

static QMap<QString, int> compterPar(const QString &colonne)
{
    QMap<QString, int> result;
    QSqlQuery q("SELECT " + colonne + ", COUNT(*) FROM EQUIPEMENTS GROUP BY " + colonne);
    while (q.next()) result[q.value(0).toString()] = q.value(1).toInt();
    return result;
}

QMap<QString, int> Equipement::statsParEtat() { return compterPar("ETAT"); }
QMap<QString, int> Equipement::statsParType() { return compterPar("CATEGORIE"); }

QSqlQuery Equipement::stockEnAlerte()
{
    QSqlQuery q;
    q.exec(SELECT_BASE + " WHERE QUANTITE <= SEUIL_ALERTE ORDER BY QUANTITE ASC");
    return q;
}

QSqlQuery Equipement::controlesProches(int jours)
{
    QSqlQuery q;
    q.prepare(SELECT_BASE + " WHERE DATE_PROCHAIN_CONTROLE <= :limite "
              "AND DATE_PROCHAIN_CONTROLE IS NOT NULL ORDER BY DATE_PROCHAIN_CONTROLE ASC");
    q.bindValue(":limite", QDate::currentDate().addDays(jours));
    q.exec();
    return q;
}

QSqlQuery Equipement::peremptionsProches(int jours)
{
    QSqlQuery q;
    q.prepare(SELECT_BASE + " WHERE DATE_PEREMPTION <= :limite "
              "AND DATE_PEREMPTION IS NOT NULL ORDER BY DATE_PEREMPTION ASC");
    q.bindValue(":limite", QDate::currentDate().addDays(jours));
    q.exec();
    return q;
}

// Couleurs d'alerte du tableau : stock bas, contrôle/péremption dépassé,
// ou contrôle/péremption dans les 30 jours.
QVariant EquipementModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && (role == Qt::BackgroundRole || role == Qt::ForegroundRole)) {
        const QDate control = QDate::fromString(
            QSqlQueryModel::data(index(idx.row(), COL_DATE_CTRL)).toString(), Qt::ISODate);
        const QDate expiry = QDate::fromString(
            QSqlQueryModel::data(index(idx.row(), COL_DATE_EXP)).toString(), Qt::ISODate);
        const int quantity = QSqlQueryModel::data(index(idx.row(), COL_QTE)).toInt();
        const int threshold = QSqlQueryModel::data(index(idx.row(), COL_SEUIL)).toInt();
        QColor background;
        if (idx.column() == COL_QTE && quantity <= threshold)
            background = QColor("#fff3a0");
        else if ((control.isValid() && control < QDate::currentDate())
                 || (expiry.isValid() && expiry < QDate::currentDate()))
            background = QColor("#f8d7da");
        else if ((control.isValid() && control <= QDate::currentDate().addDays(30))
                 || (expiry.isValid() && expiry <= QDate::currentDate().addDays(30)))
            background = QColor("#ffe5b4");

        if (background.isValid())
            return role == Qt::BackgroundRole ? QVariant(QBrush(background))
                                              : QVariant(QBrush(Qt::black));
    }
    return QSqlQueryModel::data(idx, role);
}
