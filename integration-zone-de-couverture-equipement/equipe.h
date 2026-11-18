#pragma once
// equipe.h : equipes disponibles (donnees + carte du tableau de bord)
#include <QFrame>
#include <QList>
#include <QString>
class QVBoxLayout;

struct Equipe {
    QString nom, composition;
    bool disponible = true;
};

class EquipesCard : public QFrame {
    Q_OBJECT
public:
    explicit EquipesCard(QWidget *parent = nullptr);
private:
    void rebuild();
    QList<Equipe> m_equipes;
    QVBoxLayout *m_list;
};
