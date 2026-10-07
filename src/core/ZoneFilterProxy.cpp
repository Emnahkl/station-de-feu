#include "ZoneFilterProxy.h"
#include "ZoneTableModel.h"

ZoneFilterProxy::ZoneFilterProxy(QObject* parent) : QSortFilterProxyModel(parent)
{
    setSortRole(Qt::UserRole);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

void ZoneFilterProxy::setSearchText(const QString& text)
{
    m_search = text.trimmed();
    invalidateFilter();
}

void ZoneFilterProxy::setRiskFilter(int risk)
{
    m_risk = risk;
    invalidateFilter();
}

bool ZoneFilterProxy::filterAcceptsRow(int row, const QModelIndex&) const
{
    auto* m = qobject_cast<ZoneTableModel*>(sourceModel());
    if (!m || row < 0 || row >= m->rowCount()) return false;
    const Zone& z = m->zoneAt(row);

    if (m_risk >= 0 && static_cast<int>(z.risque) != m_risk) return false;
    if (m_search.isEmpty()) return true;
    return z.nom.contains(m_search, Qt::CaseInsensitive)
        || z.region.contains(m_search, Qt::CaseInsensitive)
        || z.idLabel().contains(m_search, Qt::CaseInsensitive);
}
