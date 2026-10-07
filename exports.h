#ifndef EXPORTS_H
#define EXPORTS_H

#include <QString>
#include <QAbstractItemModel>

// Fonctions d'export : PDF (liste ou bon de commande) et CSV (ouvrable avec Excel)
namespace Exports
{
    QString htmlDepuisModele(QAbstractItemModel *model, const QString &titre);
    bool    ecrirePdf(const QString &fichier, const QString &html);
    bool    ecrireCsv(const QString &fichier, QAbstractItemModel *model);
}

#endif // EXPORTS_H
