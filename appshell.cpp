#include "appshell.h"
#include "mainwindow.h"
#include "gestioninterventions.h"

#include <QCloseEvent>
#include <QEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

AppShell::AppShell(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1280, 720);
    resize(1440, 800);

    m_stack = new QStackedWidget(this);
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(m_stack);

    m_employes = new MainWindow(m_stack);
    m_employes->setWindowFlags(Qt::Widget);   // QMainWindow reste une fenetre meme avec un parent : on l'integre comme une page
    m_interventions = new GestionInterventions(m_stack);
    m_stack->addWidget(m_employes);
    m_stack->addWidget(m_interventions);

    connect(m_employes, &MainWindow::ouvrirInterventions, this, &AppShell::afficherInterventions);
    connect(m_interventions, &GestionInterventions::ouvrirEmployes, this, &AppShell::afficherEmployes);

    // Le bouton « quitter » de chaque module ferme l'application entiere
    m_employes->installEventFilter(this);
    m_interventions->installEventFilter(this);

    afficherEmployes();   // page de depart = module Employes
}

void AppShell::afficherEmployes()
{
    m_stack->setCurrentWidget(m_employes);
    m_employes->marquerMenu();
    setWindowTitle(QStringLiteral("Fire Station – Gestion des employés"));
}

void AppShell::afficherInterventions()
{
    m_stack->setCurrentWidget(m_interventions);
    m_interventions->marquerMenu();
    setWindowTitle(QStringLiteral("Fire Station – Gestion des interventions"));
}

bool AppShell::eventFilter(QObject *obj, QEvent *ev)
{
    // close() sur un module (bouton ⏻) -> on ferme la fenetre principale
    if (ev->type() == QEvent::Close && (obj == m_employes || obj == m_interventions)) {
        QMetaObject::invokeMethod(this, [this] { close(); }, Qt::QueuedConnection);
        return true;   // la page elle-meme reste intacte
    }
    return QWidget::eventFilter(obj, ev);
}
