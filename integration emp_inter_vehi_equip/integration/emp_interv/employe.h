#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QDate>
#include <QMap>
#include <QPair>
#include <QSqlQueryModel>
#include <QString>
#include <QVector>

// Entite EMPLOYE : attributs du MLD (id, nom, prenom, photo, grade, specialite, telephone)
// + statut de presence, emploi du temps (garde) et indicateur volontaire.
class Employe
{
public:
    Employe() = default;
    Employe(int id, const QString &nom, const QString &prenom, const QString &photo,
            int grade, const QString &specialite, const QString &telephone,
            const QString &statut = "En service", const QString &garde = "Aucune",
            bool volontaire = false);

    // getters
    int     getId() const         { return id; }
    QString getNom() const        { return nom; }
    QString getPrenom() const     { return prenom; }
    QString getPhoto() const      { return photo; }
    int     getGrade() const      { return grade; }
    QString getSpecialite() const { return specialite; }
    QString getTelephone() const  { return telephone; }
    QString getStatut() const     { return statut; }
    QString getGarde() const      { return garde; }
    bool    isVolontaire() const  { return volontaire; }
    QString nomComplet() const    { return (nom + " " + prenom).trimmed(); }

    // setters
    void setId(int v)                     { id = v; }
    void setNom(const QString &v)         { nom = v; }
    void setPrenom(const QString &v)      { prenom = v; }
    void setPhoto(const QString &v)       { photo = v; }
    void setGrade(int v)                  { grade = v; }
    void setSpecialite(const QString &v)  { specialite = v; }
    void setTelephone(const QString &v)   { telephone = v; }
    void setStatut(const QString &v)      { statut = v; }
    void setGarde(const QString &v)       { garde = v; }
    void setVolontaire(bool v)            { volontaire = v; }

    // ----- CRUD -----
    bool            ajouter();              // INSERT
    QSqlQueryModel *afficher();             // SELECT (l'appelant supprime le modele)
    bool            supprimer(int id);      // DELETE
    bool            modifier();             // UPDATE

    // ----- outils -----
    static bool            changerStatut(int id, const QString &statut);
    static bool            changerGarde(int id, const QString &garde);
    static bool            changerGardes(const QVector<QPair<int, QString>> &liste);
    static QVector<Employe> chargerTous();
    static QString         derniereErreur();

    // ----- calendrier de travail (table PLANNING_JOUR) -----
    static bool                 assurerTablePlanning();
    static QMap<QDate, QString> chargerPlanning(int idEmploye);
    static bool                 affecterJour(int idEmploye, const QDate &jour, const QString &garde);
    static bool                 retirerJour(int idEmploye, const QDate &jour);

private:
    int     id = 0;
    QString nom, prenom, photo;
    int     grade = 1;                 // 1..3 etoiles (stocke en texte dans la colonne GRADE)
    QString specialite = "Fire";       // Fire | Medic | Logistics
    QString telephone;
    QString statut = "En service";     // En service | En pause | En formation
    QString garde = "Aucune";          // colonne EMPLOI_TEMPS
    bool    volontaire = false;
};

#endif // EMPLOYE_H
