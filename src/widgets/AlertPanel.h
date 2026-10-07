#pragma once
#include "core/Zone.h"
#include <QFrame>
class QDoubleSpinBox;
class QLabel;
class QListWidget;

// Alertes de zones sous-couvertes (temps moyen > seuil)
class AlertPanel : public QFrame {
    Q_OBJECT
public:
    explicit AlertPanel(QWidget* parent = nullptr);
    double threshold() const;
    void setThreshold(double minutes);
    void setAlerts(const QVector<Zone>& underCovered, double threshold);

signals:
    void thresholdChanged(double minutes);
    void zoneClicked(int id);

private:
    QDoubleSpinBox* m_spin;
    QListWidget* m_list;
    QLabel* m_title;
};
