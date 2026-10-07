#include "equipementmodel.h"
#include "equipement.h"
#include <QColor>
#include <QBrush>
#include <QDate>

QVariant EquipementModel::data(const QModelIndex &idx, int role) const
{
    if (idx.isValid() && (role == Qt::BackgroundRole || role == Qt::ForegroundRole)) {
        const int r = idx.row();
        const QDate ctrl = QDate::fromString(
            QSqlQueryModel::data(index(r, COL_DATE_CTRL)).toString(), Qt::ISODate);
        const int qte   = QSqlQueryModel::data(index(r, COL_QTE)).toInt();
        const int seuil = QSqlQueryModel::data(index(r, COL_SEUIL)).toInt();

        QColor fond;
        if (idx.column() == COL_QTE && qte <= seuil)
            fond = QColor("#fff3a0");                           // stock bas
        else if (ctrl.isValid() && ctrl < QDate::currentDate())
            fond = QColor("#f8d7da");                           // contrôle en retard
        else if (ctrl.isValid() && ctrl <= QDate::currentDate().addDays(30))
            fond = QColor("#ffe5b4");                           // contrôle bientôt

        if (fond.isValid())
            return role == Qt::BackgroundRole ? QVariant(QBrush(fond))
                                              : QVariant(QBrush(Qt::black));
    }
    return QSqlQueryModel::data(idx, role);
}
