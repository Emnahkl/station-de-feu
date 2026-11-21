#ifndef MENULATERAL_H
#define MENULATERAL_H

#include <QFrame>
#include <QMap>

class QPushButton;

// Menu lateral commun aux pages ajoutees a l'integration finale
// (Zones de couverture, Equipements, Campagnes de sensibilisation).
// Les modules Employes et Interventions gardent leur propre menu, branche sur les memes pages.
class MenuLateral : public QFrame
{
    Q_OBJECT
public:
    enum Page { Employes, Interventions, Zones, Equipements, Campagnes };

    explicit MenuLateral(Page courante, QWidget *parent = nullptr);
    void marquer();   // remet le bouton de la page courante en surbrillance

signals:
    void naviguer(MenuLateral::Page page);
    void quitter();

private:
    Page m_courante;
    QMap<Page, QPushButton *> m_boutons;
};

#endif // MENULATERAL_H
