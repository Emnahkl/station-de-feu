#ifndef AFFECTATION_H
#define AFFECTATION_H

#include <QString>
#include <QStringList>
#include <QVector>

#include "intervention.h"

// Proposition d'affectation pour UNE intervention :
//   véhicules + équipes (employés rattachés à un véhicule, table PARTICIPER_UTILISER)
//   + équipements nécessaires (table NECESSITER).
struct PropositionAffectation
{
    struct Vehicule   { QString immat, type, modele, etat; };
    struct Employe    { int id; QString nom, grade, specialite, immat; bool chef; };
    struct Equipement { int id; QString categorie, etat, immat; int disponible; };

    QVector<Vehicule>   vehicules;
    QVector<Employe>    employes;
    QVector<Equipement> equipements;
    QStringList         avertissements;       // ce qui manque / les choix par défaut
    int besoinVehicules;                      // besoin estimé d'après le type et la gravité
    int besoinEmployes;
    int besoinEquipements;

    PropositionAffectation() : besoinVehicules(0), besoinEmployes(0), besoinEquipements(0) {}
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

    static QString derniereErreur();
};

#endif // AFFECTATION_H
