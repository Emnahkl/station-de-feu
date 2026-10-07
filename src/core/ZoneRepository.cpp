#include "ZoneRepository.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <algorithm>

ZoneRepository::ZoneRepository(QObject* parent) : QObject(parent) {}

const Zone* ZoneRepository::find(int id) const
{
    for (const Zone& z : m_zones)
        if (z.id == id) return &z;
    return nullptr;
}

int ZoneRepository::add(Zone z)
{
    z.id = m_nextId++;
    m_zones.append(z);
    emit changed();
    return z.id;
}

bool ZoneRepository::update(const Zone& z)
{
    for (Zone& e : m_zones) {
        if (e.id == z.id) {
            e = z;
            emit changed();
            return true;
        }
    }
    return false;
}

bool ZoneRepository::remove(int id)
{
    for (int i = 0; i < m_zones.size(); ++i) {
        if (m_zones[i].id == id) {
            m_zones.removeAt(i);
            emit changed();
            return true;
        }
    }
    return false;
}

bool ZoneRepository::recordIntervention(int id, double minutes)
{
    for (Zone& z : m_zones) {
        if (z.id == id) {
            z.tempsMoyen = (z.tempsMoyen * z.nbInterventions + minutes) / (z.nbInterventions + 1);
            z.nbInterventions += 1;
            emit changed();
            return true;
        }
    }
    return false;
}

QString ZoneRepository::defaultPath()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dir + "/zones.json";
}

bool ZoneRepository::save(const QString& path) const
{
    QDir().mkpath(QFileInfo(path).absolutePath());
    QJsonArray arr;
    for (const Zone& z : m_zones) arr.append(z.toJson());
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    f.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    return true;
}

bool ZoneRepository::load(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray()) return false;

    m_zones.clear();
    m_nextId = 1;
    for (const QJsonValue& v : doc.array()) {
        Zone z = Zone::fromJson(v.toObject());
        m_nextId = std::max(m_nextId, z.id + 1);
        m_zones.append(z);
    }
    emit changed();
    return !m_zones.isEmpty();
}

void ZoneRepository::loadSamples()
{
    m_zones.clear();
    m_nextId = 1;
    auto mk = [this](const QString& nom, const QString& reg, double sup, int pop, RiskLevel r,
                     double t, int n, int eq, int veh, double lat, double lon, double ray) {
        Zone z;
        z.nom = nom; z.region = reg; z.superficie = sup; z.population = pop; z.risque = r;
        z.tempsMoyen = t; z.nbInterventions = n; z.nbEquipes = eq; z.nbVehicules = veh;
        z.latitude = lat; z.longitude = lon; z.rayonKm = ray;
        z.id = m_nextId++;
        m_zones.append(z);
    };
    mk("Zone industrielle de Radès", "Radès / Ben Arous", 18.5, 85000, RiskLevel::Eleve, 14.5, 42, 2, 3, 36.7686, 10.2745, 2.2);
    mk("Berges du Lac", "Tunis", 9.2, 60000, RiskLevel::Moyen, 9.8, 17, 2, 2, 36.8330, 10.2370, 1.6);
    mk("El Menzah", "Ariana / Tunis", 7.4, 70000, RiskLevel::Moyen, 8.9, 14, 1, 2, 36.8420, 10.1730, 1.5);
    mk("Carthage", "Carthage", 12.4, 22000, RiskLevel::Faible, 7.2, 8, 1, 1, 36.8528, 10.3233, 1.7);
    mk("La Marsa", "La Marsa", 20.0, 92000, RiskLevel::Moyen, 11.4, 21, 2, 2, 36.8782, 10.3247, 2.2);
    mk("Médina de Tunis", "Tunis", 6.8, 130000, RiskLevel::Eleve, 8.6, 55, 3, 4, 36.7983, 10.1710, 1.4);
    mk("Ariana Centre", "Ariana", 15.0, 114000, RiskLevel::Moyen, 10.1, 24, 2, 2, 36.8625, 10.1956, 1.8);
    mk("Ben Arous", "Ben Arous", 14.0, 88000, RiskLevel::Eleve, 13.7, 38, 2, 2, 36.7533, 10.2189, 2.0);
    mk("Hammam-Lif", "Ben Arous", 11.0, 54000, RiskLevel::Faible, 9.3, 9, 1, 1, 36.7300, 10.3400, 1.6);
    emit changed();
}
