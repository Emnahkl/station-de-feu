#ifndef INTEGRATEDWINDOW_H
#define INTEGRATEDWINDOW_H

#include <QMainWindow>
#include <QList>

class QStackedWidget;
class QEvent;
class VehicleWindow;
class EmployesWindow;
class GestionInterventions;
class EquipementsWindow;
class QPushButton;

class IntegratedWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit IntegratedWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void afficherPage(int index);
    void selectionnerMenu(int index);
    QStackedWidget *pages = nullptr;
    VehicleWindow *vehicules = nullptr;
    EmployesWindow *employes = nullptr;
    GestionInterventions *interventions = nullptr;
    EquipementsWindow *equipements = nullptr;
    QList<QPushButton *> menuButtons;
};

#endif
