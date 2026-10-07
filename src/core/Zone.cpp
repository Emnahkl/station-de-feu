#include "Zone.h"

namespace RiskUtil {
QString label(RiskLevel r)
{
    switch (r) {
    case RiskLevel::Faible: return QStringLiteral("Faible");
    case RiskLevel::Moyen:  return QStringLiteral("Moyen");
    case RiskLevel::Eleve:  return QStringLiteral("Élevé");
    }
    return {};
}

QColor color(RiskLevel r)
{
    switch (r) {
    case RiskLevel::Faible: return QColor("#2E9E5B");
    case RiskLevel::Moyen:  return QColor("#F59E0B");
    case RiskLevel::Eleve:  return QColor("#D32F2F");
    }
    return Qt::gray;
}

RiskLevel fromInt(int v)
{
    if (v <= 0) return RiskLevel::Faible;
    if (v == 1) return RiskLevel::Moyen;
    return RiskLevel::Eleve;
}
} // namespace RiskUtil

QString Zone::idLabel() const
{
    return QStringLiteral("Z-%1").arg(id, 3, 10, QLatin1Char('0'));
}

QJsonObject Zone::toJson() const
{
    QJsonObject o;
    o["id"] = id;
    o["nom"] = nom;
    o["region"] = region;
    o["superficie"] = superficie;
    o["population"] = population;
    o["risque"] = static_cast<int>(risque);
    o["tempsMoyen"] = tempsMoyen;
    o["nbInterventions"] = nbInterventions;
    o["nbEquipes"] = nbEquipes;
    o["nbVehicules"] = nbVehicules;
    o["latitude"] = latitude;
    o["longitude"] = longitude;
    o["rayonKm"] = rayonKm;
    return o;
}

Zone Zone::fromJson(const QJsonObject& o)
{
    Zone z;
    z.id = o["id"].toInt();
    z.nom = o["nom"].toString();
    z.region = o["region"].toString();
    z.superficie = o["superficie"].toDouble();
    z.population = o["population"].toInt();
    z.risque = RiskUtil::fromInt(o["risque"].toInt());
    z.tempsMoyen = o["tempsMoyen"].toDouble();
    z.nbInterventions = o["nbInterventions"].toInt();
    z.nbEquipes = o["nbEquipes"].toInt(1);
    z.nbVehicules = o["nbVehicules"].toInt(1);
    z.latitude = o["latitude"].toDouble(36.8065);
    z.longitude = o["longitude"].toDouble(10.1815);
    z.rayonKm = o["rayonKm"].toDouble(2.0);
    return z;
}
