#ifndef APPSHELL_H
#define APPSHELL_H

#include <QWidget>

class QStackedWidget;
class MainWindow;
class GestionInterventions;

// Fenetre unique de l'application : contient les deux modules, chacun tel quel (meme interface,
// memes fonctions), et bascule de l'un a l'autre avec le menu lateral :
//   - bouton « Interventions » du module Employes  -> page Interventions
//   - bouton « Employes »      du module Interventions -> page Employes
class AppShell : public QWidget
{
    Q_OBJECT
public:
    explicit AppShell(QWidget *parent = nullptr);

    void afficherEmployes();
    void afficherInterventions();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    QStackedWidget *m_stack = nullptr;
    MainWindow *m_employes = nullptr;
    GestionInterventions *m_interventions = nullptr;
};

#endif // APPSHELL_H
