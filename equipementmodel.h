#ifndef EQUIPEMENTMODEL_H
#define EQUIPEMENTMODEL_H

#include <QSqlQueryModel>

// Modèle du tableau : identique à QSqlQueryModel, mais colore les lignes en alerte.
//   rouge  = contrôle en retard
//   orange = contrôle dans les 30 jours
//   jaune  = cellule Quantité <= seuil d'alerte
class EquipementModel : public QSqlQueryModel
{
public:
    explicit EquipementModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // EQUIPEMENTMODEL_H
