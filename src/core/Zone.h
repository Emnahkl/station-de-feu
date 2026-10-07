#pragma once
#include <QColor>
#include <QJsonObject>
#include <QString>

enum class RiskLevel { Faible = 0, Moyen = 1, Eleve = 2 };

namespace RiskUtil {
QString label(RiskLevel r);
QColor color(RiskLevel r);
RiskLevel fromInt(int v);
}

// Entité « Zone de couverture » du cahier des charges
struct Zone {
    int id = 0;                       // ID zone
    QString nom;                      // Nom
    QString region;                   // Région / quartier
    double superficie = 0.0;          // Superficie (km²)
    int population = 0;               // Population
    RiskLevel risque = RiskLevel::Faible;  // Niveau de risque
    double tempsMoyen = 0.0;          // Temps d'intervention moyen (min)

    // Liaison avec les interventions / équipes / véhicules
    int nbInterventions = 0;
    int nbEquipes = 1;
    int nbVehicules = 1;

    // Fonction territoriale : position sur la carte
    double latitude = 36.8065;
    double longitude = 10.1815;
    double rayonKm = 2.0;

    QString idLabel() const;
    QJsonObject toJson() const;
    static Zone fromJson(const QJsonObject& o);
};
