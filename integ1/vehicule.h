#ifndef VEHICULE_H
#define VEHICULE_H

#include <QDate>
#include <QDateTime>
#include <QList>
#include <QString>

struct EvenementVehicule
{
    QString immatriculation;
    QString type;
    QString details;
    QDateTime date;
};

// Entité mappée sur VEHICULES; IMMATRICULATION est sa clé primaire.
class Vehicule
{
public:
    QString immatriculation;
    QString type;
    QString etat;
    QString modele;
    int kilometrage = 0;
    int kilometrageDerniereMaintenance = -1; // -1 means unknown (stored as SQL NULL).
    QDate derniereMaintenance;

public:
    static bool ajouter(const Vehicule &vehicule, QString *erreur = nullptr);
    static bool modifier(const QString &ancienneImmatriculation, const Vehicule &vehicule,
                         QString *erreur = nullptr);
    static bool supprimer(const QString &immatriculation, QString *erreur = nullptr);
    static QList<Vehicule> afficher(const QString &recherche = QString(),
                                    const QString &type = QString(),
                                    const QString &etat = QString(),
                                    const QString &tri = QStringLiteral("IMMATRICULATION ASC"),
                                    QString *erreur = nullptr);
    static bool obtenir(const QString &immatriculation, Vehicule *vehicule,
                        QString *erreur = nullptr);
    static QList<EvenementVehicule> evenementsEntre(const QDate &debut, const QDate &fin,
                                                    QString *erreur = nullptr);

private:
    static bool valider(const Vehicule &vehicule, QString *erreur);
};

#endif // VEHICULE_H
