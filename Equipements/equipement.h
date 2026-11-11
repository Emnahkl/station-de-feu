#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QSqlQuery>
#include <QSqlQueryModel>

enum ColEquipement {
    COL_ID = 0, COL_NOM, COL_TYPE, COL_QTE, COL_DATE_ACQ, COL_ETAT,
    COL_DATE_CTRL, COL_DATE_EXP, COL_AFFECT, COL_SEUIL
};

class Equipement
{
public:
    Equipement() : id(0), quantite(0), seuilAlerte(5) {}
    Equipement(const QString &nom, const QString &type, int quantite,
               const QDate &dateAcq, const QString &etat, const QDate &dateCtrl,
               const QString &affectation, int seuilAlerte, const QDate &datePeremption);

    int getId() const { return id; }
    void setId(int value) { id = value; }

    bool ajouter();
    bool modifier();
    bool supprimer(int idASupprimer);

    static QSqlQuery lister(const QString &champRech = QString(),
                            const QString &texte = QString(),
                            const QString &champTri = "ID_EQUIPEMENT",
                            bool croissant = true);
    static QMap<QString, int> statsParEtat();
    static QMap<QString, int> statsParType();
    static QSqlQuery stockEnAlerte();
    static QSqlQuery controlesProches(int jours);
    static QSqlQuery peremptionsProches(int jours);

private:
    int id;
    QString nom;
    QString type;
    int quantite;
    QDate dateAcquisition;
    QString etat;
    QDate dateControle;
    QString affectation;
    int seuilAlerte;
    QDate datePeremption;
};

// Modèle du tableau Qt : inclus avec l'entité pour garder un seul fichier .h.
class EquipementModel : public QSqlQueryModel
{
public:
    explicit EquipementModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // EQUIPEMENT_H
