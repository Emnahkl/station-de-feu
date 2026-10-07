#ifndef VEHICULESPAGE_H
#define VEHICULESPAGE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class VehiculesPage; }
QT_END_NAMESPACE

class VehiculesPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit VehiculesPage(QWidget *parent = nullptr);
    ~VehiculesPage();

private:
    void loadVehicles();
    void saveVehicles() const;
    void refreshTable();
    void refreshDashboard();
    void resetForm();
    void editVehicle(int id);
    void deleteVehicle(int id);
    void saveVehicle();
    void showMaintenanceAlerts();
    void showStatistics();
    void recommendVehicle();
    void exportCsv();
    void exportPdf();
    int indexForId(int id) const;
    bool isMaintenanceDue(const QJsonObject &vehicle) const;

    Ui::VehiculesPage *ui;
    QJsonArray vehicles;
};

#endif // VEHICULESPAGE_H
