#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QSqlQuery>

// Colonnes de la requête de liste (utile pour l'affichage coloré)
enum ColEquipement { COL_ID = 0, COL_NOM, COL_TYPE, COL_QTE, COL_DATE_ACQ,
                     COL_ETAT, COL_DATE_CTRL, COL_AFFECT, COL_SEUIL };

class Equipement
{
public:
    Equipement() : id(0), quantite(0), seuilAlerte(5) {}
    Equipement(const QString &nom, const QString &type, int quantite,
               const QDate &dateAcq, const QString &etat, const QDate &dateCtrl,
               const QString &affectation, int seuilAlerte);

    // --- Getters / Setters ---
    int     getId() const            { return id; }
    void    setId(int i)             { id = i; }
    QString getNom() const           { return nom; }
    QString getType() const          { return type; }
    int     getQuantite() const      { return quantite; }
    QDate   getDateAcquisition() const { return dateAcquisition; }
    QString getEtat() const          { return etat; }
    QDate   getDateControle() const  { return dateControle; }
    QString getAffectation() const   { return affectation; }
    int     getSeuilAlerte() const   { return seuilAlerte; }

    // --- CRUD ---
    bool ajouter();
    bool modifier();
    bool supprimer(int idASupprimer);

    // --- Fonctionnalités avancées ---
    // Liste avec recherche (champRech + texte) ET tri (champTri + ordre)
    static QSqlQuery lister(const QString &champRech = QString(),
                            const QString &texte = QString(),
                            const QString &champTri = "ID",
                            bool croissant = true);
    static QMap<QString, int> statsParEtat();
    static QMap<QString, int> statsParType();

    // --- Métiers innovants ---
    static QSqlQuery stockEnAlerte();               // QUANTITE <= SEUIL_ALERTE
    static QSqlQuery controlesProches(int jours);   // contrôle en retard ou dans <= N jours

private:
    int     id;
    QString nom;
    QString type;
    int     quantite;
    QDate   dateAcquisition;
    QString etat;
    QDate   dateControle;
    QString affectation;
    int     seuilAlerte;
};

#endif // EQUIPEMENT_H
