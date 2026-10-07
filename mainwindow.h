#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QButtonGroup;
class QFrame;
class QLineEdit;
class QStackedWidget;
class EmployesPage;
class InterventionsPage;

// Fenêtre principale : une seule sidebar + barre du haut communes,
// et une zone centrale (QStackedWidget) qui change de page.
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    enum Page { Dashboard, Interventions, Equipe, Vehicules, Carte, Sensibilisation, Volontaires, Parametres };

    QFrame *buildSidebar();
    QFrame *buildTopBar();
    QWidget *placeholder(const QString &title) const;

    QFrame *m_sidebar = nullptr;
    QLineEdit *m_search = nullptr;
    QStackedWidget *m_stack = nullptr;
    QButtonGroup *m_navGroup = nullptr;
    EmployesPage *m_employes = nullptr;
    InterventionsPage *m_interventions = nullptr;
};

#endif // MAINWINDOW_H
