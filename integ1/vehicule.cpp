#include "vehicule.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

namespace {
QSqlDatabase base()
{
    return QSqlDatabase::database(QStringLiteral("firestation-main"));
}

void definirErreur(QString *erreur, const QString &message)
{
    if (erreur) *erreur = message;
}

void lierVehicule(QSqlQuery &q, const Vehicule &v)
{
    q.bindValue(QStringLiteral(":immat"), v.immatriculation.trimmed().toUpper());
    q.bindValue(QStringLiteral(":type"), v.type.trimmed());
    q.bindValue(QStringLiteral(":etat"), v.etat.trimmed());
    q.bindValue(QStringLiteral(":modele"), v.modele.trimmed());
    q.bindValue(QStringLiteral(":km"), v.kilometrage);
    q.bindValue(QStringLiteral(":kmMaintenance"), v.kilometrageDerniereMaintenance >= 0
                    ? QVariant(v.kilometrageDerniereMaintenance)
                    : QVariant(QMetaType::fromType<int>()));
    q.bindValue(QStringLiteral(":maintenance"), v.derniereMaintenance.isValid()
                    ? QVariant(v.derniereMaintenance)
                    : QVariant(QMetaType::fromType<QDate>()));
}

bool ajouterEvenement(QSqlDatabase &db, const QString &immatriculation,
                      const QString &type, const QString &details, QString *erreur)
{
    QSqlQuery q(db);
    q.prepare(QStringLiteral("INSERT INTO VEHICULE_EVENEMENTS (IMMATRICULATION, TYPE_EVENEMENT, DETAILS) VALUES (:immat, :type, :details)"));
    q.bindValue(QStringLiteral(":immat"), immatriculation.trimmed().toUpper());
    q.bindValue(QStringLiteral(":type"), type);
    q.bindValue(QStringLiteral(":details"), details);
    if (q.exec()) return true;
    definirErreur(erreur, q.lastError().text());
    return false;
}

Vehicule depuisRequete(const QSqlQuery &q)
{
    Vehicule v;
    v.immatriculation = q.value(QStringLiteral("IMMATRICULATION")).toString();
    v.type = q.value(QStringLiteral("TYPE")).toString();
    v.etat = q.value(QStringLiteral("ETAT")).toString();
    v.modele = q.value(QStringLiteral("MODELE")).toString();
    v.kilometrage = q.value(QStringLiteral("KILOMETRAGE")).toInt();
    if (!q.value(QStringLiteral("KM_DERNIERE_MAINTENANCE")).isNull())
        v.kilometrageDerniereMaintenance = q.value(QStringLiteral("KM_DERNIERE_MAINTENANCE")).toInt();
    v.derniereMaintenance = q.value(QStringLiteral("DERNIERE_MAINTENANCE")).toDate();
    return v;
}

const QString colonnes = QStringLiteral(
    "IMMATRICULATION, TYPE, ETAT, MODELE, KILOMETRAGE, DERNIERE_MAINTENANCE, KM_DERNIERE_MAINTENANCE");
}

bool Vehicule::valider(const Vehicule &v, QString *erreur)
{
    if (v.immatriculation.trimmed().isEmpty() || v.immatriculation.trimmed().size() > 20) {
        definirErreur(erreur, QStringLiteral("L’immatriculation est obligatoire (20 caractères maximum)."));
        return false;
    }
    if (v.type.size() > 50 || v.etat.size() > 30 || v.modele.size() > 50) {
        definirErreur(erreur, QStringLiteral("Le type, l’état ou le modèle dépasse la longueur autorisée."));
        return false;
    }
    if (v.kilometrage < 0 || v.kilometrage > 99999999) {
        definirErreur(erreur, QStringLiteral("Le kilométrage doit être compris entre 0 et 99 999 999."));
        return false;
    }
    if (v.kilometrageDerniereMaintenance > v.kilometrage) {
        definirErreur(erreur, QStringLiteral("Le kilométrage de la dernière maintenance ne peut pas dépasser le kilométrage actuel."));
        return false;
    }
    return true;
}

bool Vehicule::ajouter(const Vehicule &v, QString *erreur)
{
    if (!valider(v, erreur)) return false;
    QSqlDatabase db = base();
    if (!db.transaction()) {
        definirErreur(erreur, db.lastError().text());
        return false;
    }
    QSqlQuery q(db);
    q.prepare(QStringLiteral("INSERT INTO VEHICULES (IMMATRICULATION, TYPE, ETAT, MODELE, KILOMETRAGE, DERNIERE_MAINTENANCE, KM_DERNIERE_MAINTENANCE) VALUES (:immat, :type, :etat, :modele, :km, :maintenance, :kmMaintenance)"));
    lierVehicule(q, v);
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        db.rollback();
        return false;
    }
    if (!ajouterEvenement(db, v.immatriculation, QStringLiteral("Ajout"),
                          QStringLiteral("Véhicule ajouté au parc."), erreur)) {
        db.rollback();
        return false;
    }
    if (!db.commit()) {
        definirErreur(erreur, db.lastError().text());
        db.rollback();
        return false;
    }
    return true;
}

bool Vehicule::modifier(const QString &ancienneImmatriculation, const Vehicule &v, QString *erreur)
{
    if (ancienneImmatriculation.trimmed().isEmpty() || !valider(v, erreur)) return false;
    QSqlDatabase db = base();
    if (!db.transaction()) {
        definirErreur(erreur, db.lastError().text());
        return false;
    }
    QSqlQuery before(db);
    before.prepare(QStringLiteral("SELECT ETAT, DERNIERE_MAINTENANCE, KM_DERNIERE_MAINTENANCE FROM VEHICULES WHERE IMMATRICULATION=:immat FOR UPDATE"));
    before.bindValue(QStringLiteral(":immat"), ancienneImmatriculation.trimmed().toUpper());
    if (!before.exec() || !before.next()) {
        definirErreur(erreur, before.lastError().text().isEmpty()
            ? QStringLiteral("Véhicule introuvable.") : before.lastError().text());
        db.rollback();
        return false;
    }
    const QString ancienEtat = before.value(0).toString();
    const QDate ancienneDateMaintenance = before.value(1).toDate();
    const QVariant ancienKmMaintenance = before.value(2);
    before.finish();

    QSqlQuery q(db);
    q.prepare(QStringLiteral("UPDATE VEHICULES SET IMMATRICULATION=:immat, TYPE=:type, ETAT=:etat, MODELE=:modele, KILOMETRAGE=:km, DERNIERE_MAINTENANCE=:maintenance, KM_DERNIERE_MAINTENANCE=:kmMaintenance WHERE IMMATRICULATION=:ancienne_immat"));
    lierVehicule(q, v);
    q.bindValue(QStringLiteral(":ancienne_immat"), ancienneImmatriculation.trimmed().toUpper());
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        db.rollback();
        return false;
    }
    if (!ajouterEvenement(db, v.immatriculation, QStringLiteral("Modification"),
                          QStringLiteral("Informations du véhicule modifiées."), erreur)) {
        db.rollback();
        return false;
    }
    if (ancienEtat != QStringLiteral("En maintenance") && v.etat == QStringLiteral("En maintenance")) {
        if (!ajouterEvenement(db, v.immatriculation, QStringLiteral("Mise en maintenance"),
                              QStringLiteral("Le véhicule est passé en maintenance."), erreur)) {
            db.rollback();
            return false;
        }
    } else if (ancienEtat == QStringLiteral("En maintenance") && v.etat != QStringLiteral("En maintenance")) {
        if (!ajouterEvenement(db, v.immatriculation, QStringLiteral("Fin de maintenance"),
                              QStringLiteral("Le véhicule est sorti de maintenance."), erreur)) {
            db.rollback();
            return false;
        }
    }
    const bool dateMaintenanceChangee = ancienneDateMaintenance != v.derniereMaintenance;
    const bool kmMaintenanceChange = ancienKmMaintenance.isNull()
        ? v.kilometrageDerniereMaintenance >= 0
        : (v.kilometrageDerniereMaintenance < 0
            || ancienKmMaintenance.toInt() != v.kilometrageDerniereMaintenance);
    if ((dateMaintenanceChangee || kmMaintenanceChange)
        && (v.derniereMaintenance.isValid() || v.kilometrageDerniereMaintenance >= 0)) {
        const QString details = QStringLiteral("Dernière maintenance : %1, %2.")
            .arg(v.derniereMaintenance.isValid() ? v.derniereMaintenance.toString(QStringLiteral("dd/MM/yyyy"))
                                                : QStringLiteral("date inconnue"),
                 v.kilometrageDerniereMaintenance >= 0
                     ? QStringLiteral("%1 km au compteur").arg(v.kilometrageDerniereMaintenance)
                     : QStringLiteral("kilométrage inconnu"));
        if (!ajouterEvenement(db, v.immatriculation, QStringLiteral("Maintenance enregistrée"), details, erreur)) {
            db.rollback();
            return false;
        }
    }
    if (!db.commit()) {
        definirErreur(erreur, db.lastError().text());
        db.rollback();
        return false;
    }
    return true;
}

bool Vehicule::supprimer(const QString &immatriculation, QString *erreur)
{
    if (immatriculation.trimmed().isEmpty()) {
        definirErreur(erreur, QStringLiteral("Immatriculation invalide."));
        return false;
    }
    QSqlDatabase db = base();
    if (!db.transaction()) {
        definirErreur(erreur, db.lastError().text());
        return false;
    }
    QSqlQuery q(db);
    q.prepare(QStringLiteral("SELECT TYPE, MODELE, ETAT FROM VEHICULES WHERE IMMATRICULATION=:immat"));
    q.bindValue(QStringLiteral(":immat"), immatriculation.trimmed().toUpper());
    if (!q.exec() || !q.next()) {
        definirErreur(erreur, q.lastError().text().isEmpty()
            ? QStringLiteral("Véhicule introuvable.") : q.lastError().text());
        db.rollback();
        return false;
    }
    const QString details = QStringLiteral("%1 — %2 (%3)")
        .arg(q.value(0).toString(), q.value(1).toString(), q.value(2).toString());
    q.finish();
    if (!ajouterEvenement(db, immatriculation, QStringLiteral("Suppression"),
                          QStringLiteral("Véhicule supprimé du parc. ") + details, erreur)) {
        db.rollback();
        return false;
    }
    q.prepare(QStringLiteral("DELETE FROM VEHICULES WHERE IMMATRICULATION=:immat"));
    q.bindValue(QStringLiteral(":immat"), immatriculation.trimmed().toUpper());
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        db.rollback();
        return false;
    }
    if (!db.commit()) {
        definirErreur(erreur, db.lastError().text());
        db.rollback();
        return false;
    }
    return true;
}

QList<EvenementVehicule> Vehicule::evenementsEntre(const QDate &debut, const QDate &fin, QString *erreur)
{
    QList<EvenementVehicule> evenements;
    if (!debut.isValid() || !fin.isValid() || fin < debut) {
        definirErreur(erreur, QStringLiteral("Période de calendrier invalide."));
        return evenements;
    }
    QSqlQuery q(base());
    q.prepare(QStringLiteral(
        "SELECT IMMATRICULATION, TYPE_EVENEMENT, DETAILS, "
        "TO_CHAR(DATE_EVENEMENT, 'YYYY-MM-DD HH24:MI:SS') AS DATE_TEXTE "
        "FROM VEHICULE_EVENEMENTS "
        "WHERE DATE_EVENEMENT >= TO_DATE(:debut, 'YYYY-MM-DD') "
        "AND DATE_EVENEMENT < TO_DATE(:fin, 'YYYY-MM-DD') "
        "ORDER BY DATE_EVENEMENT, ID_EVENEMENT"));
    q.bindValue(QStringLiteral(":debut"), debut.toString(Qt::ISODate));
    q.bindValue(QStringLiteral(":fin"), fin.addDays(1).toString(Qt::ISODate));
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        return evenements;
    }
    while (q.next()) {
        EvenementVehicule event;
        event.immatriculation = q.value(0).toString();
        event.type = q.value(1).toString();
        event.details = q.value(2).toString();
        event.date = QDateTime::fromString(q.value(3).toString(), QStringLiteral("yyyy-MM-dd HH:mm:ss"));
        evenements.append(event);
    }
    return evenements;
}

QList<Vehicule> Vehicule::afficher(const QString &recherche, const QString &type,
                                         const QString &etat, const QString &tri, QString *erreur)
{
    const QStringList trisAutorises = {QStringLiteral("IMMATRICULATION ASC"),
        QStringLiteral("IMMATRICULATION DESC"), QStringLiteral("KILOMETRAGE ASC"),
        QStringLiteral("KILOMETRAGE DESC"), QStringLiteral("ETAT ASC")};
    const QString ordre = trisAutorises.contains(tri) ? tri : trisAutorises.first();
    QSqlQuery q(base());
    QStringList conditions;
    if (!recherche.trimmed().isEmpty())
        conditions << QStringLiteral("(UPPER(IMMATRICULATION) LIKE UPPER(:motif1) OR UPPER(TYPE) LIKE UPPER(:motif2) OR UPPER(MODELE) LIKE UPPER(:motif3))");
    if (!type.isEmpty())
        conditions << QStringLiteral("TYPE=:type");
    if (!etat.isEmpty())
        conditions << QStringLiteral("ETAT=:etat");
    QString sql = QStringLiteral("SELECT %1 FROM VEHICULES").arg(colonnes);
    if (!conditions.isEmpty())
        sql += QStringLiteral(" WHERE ") + conditions.join(QStringLiteral(" AND "));
    sql += QStringLiteral(" ORDER BY ") + ordre;
    q.prepare(sql);
    const QString texte = recherche.trimmed();
    if (!texte.isEmpty()) {
        const QString motif = QStringLiteral("%") + texte + QStringLiteral("%");
        q.bindValue(QStringLiteral(":motif1"), motif);
        q.bindValue(QStringLiteral(":motif2"), motif);
        q.bindValue(QStringLiteral(":motif3"), motif);
    }
    if (!type.isEmpty()) q.bindValue(QStringLiteral(":type"), type);
    if (!etat.isEmpty()) q.bindValue(QStringLiteral(":etat"), etat);

    QList<Vehicule> liste;
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        return liste;
    }
    while (q.next()) liste.append(depuisRequete(q));
    return liste;
}

bool Vehicule::obtenir(const QString &immatriculation, Vehicule *v, QString *erreur)
{
    if (!v || immatriculation.trimmed().isEmpty()) {
        definirErreur(erreur, QStringLiteral("Immatriculation invalide."));
        return false;
    }
    QSqlQuery q(base());
    q.prepare(QStringLiteral("SELECT %1 FROM VEHICULES WHERE IMMATRICULATION=:immat").arg(colonnes));
    q.bindValue(QStringLiteral(":immat"), immatriculation.trimmed().toUpper());
    if (!q.exec()) {
        definirErreur(erreur, q.lastError().text());
        return false;
    }
    if (!q.next()) {
        definirErreur(erreur, QStringLiteral("Véhicule introuvable."));
        return false;
    }
    *v = depuisRequete(q);
    return true;
}
