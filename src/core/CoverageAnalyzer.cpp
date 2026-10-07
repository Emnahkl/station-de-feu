#include "CoverageAnalyzer.h"

namespace CoverageAnalyzer {

bool isUnderCovered(const Zone& z, double thresholdMin)
{
    return z.tempsMoyen > thresholdMin;
}

QVector<int> underCoveredIds(const QVector<Zone>& zones, double thresholdMin)
{
    QVector<int> ids;
    for (const Zone& z : zones)
        if (isUnderCovered(z, thresholdMin)) ids.append(z.id);
    return ids;
}

RiskStats statsForRisk(const QVector<Zone>& zones, RiskLevel r)
{
    RiskStats s;
    for (const Zone& z : zones) {
        if (z.risque != r) continue;
        s.zones++;
        s.interventions += z.nbInterventions;
        s.population += z.population;
    }
    return s;
}

double averageResponse(const QVector<Zone>& zones)
{
    double sum = 0; long long n = 0;
    for (const Zone& z : zones) {
        sum += z.tempsMoyen * z.nbInterventions;
        n += z.nbInterventions;
    }
    if (n > 0) return sum / double(n);
    if (zones.isEmpty()) return 0.0;
    double s = 0;
    for (const Zone& z : zones) s += z.tempsMoyen;
    return s / zones.size();
}

long long totalPopulation(const QVector<Zone>& zones)
{
    long long p = 0;
    for (const Zone& z : zones) p += z.population;
    return p;
}

}
