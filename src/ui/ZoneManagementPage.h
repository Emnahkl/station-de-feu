#pragma once
#include "widgets/ZoneMapWidget.h"
#include <QWidget>

class ZoneRepository;
class ZoneTableModel;
class ZoneFilterProxy;
class StatsChartWidget;
class AlertPanel;
class ZoneDetailPanel;
class KpiCard;
class QTableView;
class QComboBox;
class QLineEdit;
class QPushButton;
class QLabel;

// Module « Gestion des zones de couverture » : carte + tableau CRUD + alertes + statistiques + exports
class ZoneManagementPage : public QWidget {
    Q_OBJECT
public:
    explicit ZoneManagementPage(ZoneRepository* repo, QWidget* parent = nullptr);

private slots:
    void refresh();
    void onFiltersChanged();
    void onTableSelectionChanged();
    void onMapModeChanged(ZoneMapWidget::Mode mode);

    void addZone();
    void addZoneAt(double lat, double lon);
    void editSelected();
    void deleteSelected();
    void recordIntervention();
    void exportPdf();
    void exportExcel();
    void loadBackground();
    void clearBackground();

private:
    void buildUi();
    void selectZone(int id);
    void updateActions();
    QVector<Zone> visibleZones() const;
    void editZone(int id);
    void openDialog(Zone z, bool isNew);

    ZoneRepository* m_repo;
    ZoneTableModel* m_model;
    ZoneFilterProxy* m_proxy;

    ZoneMapWidget* m_map;
    QTableView* m_table;
    StatsChartWidget* m_chart;
    AlertPanel* m_alerts;
    ZoneDetailPanel* m_detail;
    KpiCard *m_kZones, *m_kRisk, *m_kPop, *m_kTime, *m_kAlert;

    QLineEdit* m_search;
    QComboBox* m_riskFilter;
    QComboBox* m_sortCombo;
    QPushButton *m_btnAdd, *m_btnPlace, *m_btnMove, *m_btnEdit, *m_btnDelete, *m_btnInterv;
    QLabel* m_status;

    int m_selectedId = -1;
    bool m_syncing = false;
};
