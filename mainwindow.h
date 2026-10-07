#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QButtonGroup;
class QFrame;
class QLabel;
class QLineEdit;
class QStackedWidget;
class MenuPage;
class EmployesPage;
class InterventionsPage;
class VehiculesPage;
class EquipementsPage;
class ZonesPage;
class CampagnesPage;

// Fenêtre principale : sidebar + barre du haut communes, et une zone centrale
// (QStackedWidget) qui affiche le menu ou le module choisi.
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setUtilisateur(const QString &nom);

private:
    // L'ordre doit suivre l'ordre d'ajout des pages dans le QStackedWidget
    enum Page { Menu, Interventions, Equipe, Vehicules, Equipements, Carte, Sensibilisation };

    QFrame *buildSidebar();
    QFrame *buildTopBar();
    void afficherPage(int page);

    QFrame *m_sidebar = nullptr;
    QLineEdit *m_search = nullptr;
    QStackedWidget *m_stack = nullptr;
    QButtonGroup *m_navGroup = nullptr;
    QLabel *m_labelUtilisateur = nullptr;

    MenuPage *m_menu = nullptr;
    EmployesPage *m_employes = nullptr;
    InterventionsPage *m_interventions = nullptr;
    VehiculesPage *m_vehicules = nullptr;
    EquipementsPage *m_equipements = nullptr;
    ZonesPage *m_zones = nullptr;
    CampagnesPage *m_campagnes = nullptr;
};

#endif // MAINWINDOW_H
