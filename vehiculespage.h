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
    void setSearch(const QString &text);

private:
    void loadVehicles();
    void refreshTable();
    void refreshDashboard();
    void resetForm();
    void editVehicle(const QString &immatriculation);
    void deleteVehicle(const QString &immatriculation);
    void saveVehicle();
    void showMaintenanceAlerts();
    void showVehicleCalendar();
    void showStatistics();
    void recommendVehicle();
    void exportCsv();
    void exportPdf();
    int indexForRegistration(const QString &immatriculation) const;
    bool isMaintenanceDue(const QJsonObject &vehicle) const;

    Ui::VehiculesPage *ui;
    QJsonArray vehicles;
};

#endif // VEHICULESPAGE_H