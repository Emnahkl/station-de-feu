#include "ZoneTableModel.h"
#include <QFont>
#include <QLocale>

ZoneTableModel::ZoneTableModel(ZoneRepository* repo, QObject* parent)
    : QAbstractTableModel(parent), m_repo(repo), m_zones(repo->zones())
{
    connect(repo, &ZoneRepository::changed, this, &ZoneTableModel::reload);
}

void ZoneTableModel::reload()
{
    beginResetModel();
    m_zones = m_repo->zones();
    endResetModel();
}

void ZoneTableModel::setThreshold(double minutes)
{
    m_threshold = minutes;
    if (!m_zones.isEmpty())
        emit dataChanged(index(0, 0), index(m_zones.size() - 1, ColCount - 1));
}

int ZoneTableModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_zones.size();
}

int ZoneTableModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : ColCount;
}

QVariant ZoneTableModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid() || idx.row() >= m_zones.size()) return {};
    const Zone& z = m_zones.at(idx.row());
    const QLocale fr(QLocale::French);

    if (role == Qt::UserRole) {   // valeur brute pour le tri
        switch (idx.column()) {
        case ColId: return z.id;
        case ColNom: return z.nom;
        case ColRegion: return z.region;
        case ColSuperficie: return z.superficie;
        case ColPopulation: return z.population;
        case ColRisque: return static_cast<int>(z.risque);
        case ColTemps: return z.tempsMoyen;
        case ColInterventions: return z.nbInterventions;
        }
    }
    if (role == Qt::DisplayRole) {
        switch (idx.column()) {
        case ColId: return z.idLabel();
        case ColNom: return z.nom;
        case ColRegion: return z.region;
        case ColSuperficie: return QStringLiteral("%1 km²").arg(fr.toString(z.superficie, 'f', 1));
        case ColPopulation: return fr.toString(z.population);
        case ColRisque: return RiskUtil::label(z.risque);
        case ColTemps: return QStringLiteral("%1 min").arg(fr.toString(z.tempsMoyen, 'f', 1));
        case ColInterventions: return z.nbInterventions;
        }
    }
    if (role == Qt::ForegroundRole) {
        if (idx.column() == ColRisque) return RiskUtil::color(z.risque);
        if (idx.column() == ColTemps && z.tempsMoyen > m_threshold) return QColor("#B91C1C");
    }
    if (role == Qt::FontRole) {
        if (idx.column() == ColRisque || (idx.column() == ColTemps && z.tempsMoyen > m_threshold)) {
            QFont f; f.setBold(true); return f;
        }
    }
    if (role == Qt::TextAlignmentRole) {
        if (idx.column() >= ColSuperficie && idx.column() != ColRisque)
            return int(Qt::AlignRight | Qt::AlignVCenter);
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return {};
}

QVariant ZoneTableModel::headerData(int section, Qt::Orientation o, int role) const
{
    if (o != Qt::Horizontal || role != Qt::DisplayRole) return {};
    static const char* heads[] = {"ID zone", "Nom", "Région / quartier", "Superficie",
                                  "Population", "Niveau de risque", "Temps moyen", "Interventions"};
    return QString::fromUtf8(heads[section]);
}
