#pragma once
#include <QSortFilterProxyModel>

// Recherche par nom / région + filtre niveau de risque + tri (rôle UserRole)
class ZoneFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ZoneFilterProxy(QObject* parent = nullptr);
    void setSearchText(const QString& text);
    void setRiskFilter(int risk);   // -1 = tous

protected:
    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;

private:
    QString m_search;
    int m_risk = -1;
};
