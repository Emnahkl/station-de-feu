#ifndef CAMPAGNE_H
#define CAMPAGNE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QMap>
#include <QList>
#include <QPair>

class Campagne
{
public:
    Campagne();
    Campagne(int id, QString titre, QString theme, QString publicCible, QString description,
             QDate dateDebut, QDate dateFin, int nbParticipants, double budget,
             QString statut, double scoreImpact, int idEmploye, int idZone);

    int getId() const { return id; }

    // CRUD
    bool ajouter();
    QSqlQueryModel *afficher(const QString &orderBy = "c.ID_CAMPAGNE ASC");
    bool supprimer(int id);
    bool modifier();
    bool existe(int id);

    // Metiers
    QSqlQueryModel *rechercher(const QString &critere, const QString &valeur,
                               const QString &orderBy = "c.ID_CAMPAGNE ASC");
    QMap<QString, int> statistiques(const QString &critere);

    // Listes pour les associations Organiser (Employe) et Cibler (Zone)
    static QList<QPair<int, QString>> listeEmployes();
    static QList<QPair<int, QString>> listeZones();

private:
    int id;
    QString titre, theme, publicCible, description, statut;
    QDate dateDebut, dateFin;
    int nbParticipants;
    double budget, scoreImpact;
    int idEmploye, idZone;

    static QString selectColonnes();
};

#endif
