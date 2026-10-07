#pragma once
#include "Zone.h"
#include <QVector>

// Logique métier « innovante » : alertes de zones sous-couvertes + statistiques.
namespace CoverageAnalyzer {

struct RiskStats {
    int zones = 0;
    int interventions = 0;
    long long population = 0;
};

bool isUnderCovered(const Zone& z, double thresholdMin);
QVector<int> underCoveredIds(const QVector<Zone>& zones, double thresholdMin);
RiskStats statsForRisk(const QVector<Zone>& zones, RiskLevel r);
double averageResponse(const QVector<Zone>& zones);   // moyenne pondérée par interventions
long long totalPopulation(const QVector<Zone>& zones);

}
