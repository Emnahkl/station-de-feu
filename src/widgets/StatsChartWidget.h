#pragma once
#include "core/Zone.h"
#include <QFrame>
#include <QVector>
class QComboBox;
class ChartCanvas;

// Statistiques des interventions par zone ou par niveau de risque (graphique en barres)
class StatsChartWidget : public QFrame {
    Q_OBJECT
public:
    explicit StatsChartWidget(QWidget* parent = nullptr);
    void setZones(const QVector<Zone>& zones);
private:
    QComboBox* m_mode;
    ChartCanvas* m_canvas;
    QVector<Zone> m_zonesCache;
};
