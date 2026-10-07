#pragma once
#include "ZoneRepository.h"
#include <QAbstractTableModel>

class ZoneTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column { ColId, ColNom, ColRegion, ColSuperficie, ColPopulation,
                  ColRisque, ColTemps, ColInterventions, ColCount };

    explicit ZoneTableModel(ZoneRepository* repo, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation o, int role) const override;

    const Zone& zoneAt(int row) const { return m_zones.at(row); }
    void setThreshold(double minutes);

private:
    void reload();
    ZoneRepository* m_repo;
    QVector<Zone> m_zones;
    double m_threshold = 12.0;
};
