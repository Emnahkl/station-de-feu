#ifndef AFFECTATION_H
#define AFFECTATION_H

#include <QString>
#include <QStringList>
#include <QVector>

#include "intervention.h"

// Proposition d'affectation pour UNE intervention :
//   véhicules + équipes (employés et véhicules liés dans PARTICIPER_UTILISER)
//   + équipements nécessaires (table NECESSITER).
struct PropositionAffectation
{
    // score = pertinence de la ressource pour CETTE intervention (0 à 100)
    struct Vehicule   { QString immat, type, modele, etat; int score = 0; QString note; };
    struct Employe    { int id; QString nom, grade, specialite, immat; bool chef;
                        int score = 0; int charge = 0; };      // charge = missions des 7 derniers jours
    struct Equipement { int id; QString categorie, etat, immat; int disponible;
                        int score = 0; QString note; };

    QVector<Vehicule>   vehicules;
    QVector<Employe>    employes;
    QVector<Equipement> equipements;
    QStringList         avertissements;       // ce qui manque / les choix par défaut
    QStringList         planB;                // remplaçants proposés quand une ressource manque (+ pourquoi)
    int                 scoreGlobal;          // pertinence moyenne de la proposition (0 à 100)
    QString             equilibre;            // résumé de l'équilibre de l'équipe
    int besoinVehicules;                      // besoin estimé d'après le type et la gravité
    int besoinEmployes;
    int besoinEquipements;

    PropositionAffectation() : scoreGlobal(0), besoinVehicules(0), besoinEmployes(0), besoinEquipements(0) {}
};

// Une ressource (employé, véhicule ou équipement) et sa disponibilité actuelle
struct RessourceDispo
{
    QString genre;          // « Employé », « Véhicule », « Équipement »
    QString designation;
    QString detail;
    bool    libre;
    QString statut;         // « Libre », « Occupé – intervention n° 4 », « Indisponible – en panne »...
    RessourceDispo() : libre(false) {}
};

// Affectation automatique des ressources (employés, véhicules, équipements) à une intervention.
class Affectation
{
public:
    // Calcule la meilleure affectation avec les ressources LIBRES (non utilisées par une autre
    // intervention non terminée), sans rien écrire en base.
    static PropositionAffectation proposer(const Intervention &it);

    // Enregistre la proposition (remplace l'affectation existante de l'intervention).
    static bool enregistrer(int idIntervention, const PropositionAffectation &p);

    // Nombre de {employés, véhicules, équipements} déjà affectés à l'intervention
    static QVector<int> compter(int idIntervention);

    // Disponibilité de TOUTES les ressources (libres d'abord dans chaque famille)
    static QVector<RessourceDispo> disponibilites();

    static QString derniereErreur();
};

#endif // AFFECTATION_H
