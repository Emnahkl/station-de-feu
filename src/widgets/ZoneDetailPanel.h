#pragma once
#include "core/Zone.h"
#include <QFrame>
class QLabel;

// Fiche de la zone sélectionnée (liaison employés / véhicules / interventions)
class ZoneDetailPanel : public QFrame {
    Q_OBJECT
public:
    explicit ZoneDetailPanel(QWidget* parent = nullptr);
    void setZone(const Zone* z, double threshold);
private:
    QLabel* m_title;
    QLabel* m_badge;
    QLabel* m_status;
    QLabel* m_info;
    QLabel* m_empty;
};
