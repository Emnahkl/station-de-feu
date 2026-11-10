#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    Ui::MainWindow *ui;
    QJsonArray vehicles;
};

#endif // MAINWINDOW_H
