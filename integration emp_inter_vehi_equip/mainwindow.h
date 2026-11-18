#ifndef VehicleWindow_H
#define VehicleWindow_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>

class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class VehicleWindow; }
QT_END_NAMESPACE

class VehicleWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VehicleWindow(QWidget *parent = nullptr);
    ~VehicleWindow();
    void afficherVehicules();

signals:
    void ouvrirTableauDeBord();
    void ouvrirInterventions();

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

    Ui::VehicleWindow *ui;
    QJsonArray vehicles;
    QPushButton *interventionsButton = nullptr;
};

#endif // VehicleWindow_H
