#ifndef VEHICLEWINDOW_H
#define VEHICLEWINDOW_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class VehicleWindow; }
QT_END_NAMESPACE

class VehicleWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VehicleWindow(QWidget *parent = nullptr);
    ~VehicleWindow();
    void showDashboard();
    void showVehicles();

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

    Ui::VehicleWindow *ui;
    QJsonArray vehicles;
};

#endif // VEHICLEWINDOW_H



