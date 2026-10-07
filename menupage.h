#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>

class QLabel;

// Menu principal affiché après la connexion : une tuile par module.
// Pour le moment tous les modules sont accessibles ; plus tard, setModulesAutorises()
// permettra de n'activer que les modules liés au poste de l'employé connecté.
class MenuPage : public QWidget
{
    Q_OBJECT
public:
    struct Module {
        int page;               // index de la page dans le QStackedWidget
        QString titre;
        QString description;
        QString icone;          // icône blanche (ressource)
    };

    explicit MenuPage(const QList<Module> &modules, QWidget *parent = nullptr);
    void setUtilisateur(const QString &nom);
    void setModulesAutorises(const QList<int> &pages);

signals:
    void moduleChoisi(int page);

private:
    QLabel *m_bienvenue = nullptr;
    QList<QWidget *> m_tuiles;
    QList<int> m_pages;
};

#endif // MENUPAGE_H
