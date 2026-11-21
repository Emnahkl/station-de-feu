#ifndef CAMPAGNE_H
#define CAMPAGNE_H

// Module « Gestion des campagnes de sensibilisation » (Faouzene)
// Conforme au MCD/MLD de l'equipe (base commune SAFESTATION) :
//   CAMPAGNES_SENSIBILISATION (id_campagne, titre, theme, date_campagne, lieu,
//                              public_cible, nb_participants, #id_zone)
//   ANIMER (#id_employe, #id_campagne)   -> une campagne est animee par 1..N employes

#include <QDate>
#include <QList>
#include <QMap>
#include <QPair>
#include <QSqlQueryModel>
#include <QString>

class Campagne
{
public:
    Campagne();
    Campagne(int id, const QString &titre, const QString &theme, const QDate &date,
             const QString &lieu, const QString &publicCible, int nbParticipants,
             int idZone, const QList<int> &animateurs);

    int getId() const { return m_id; }
    QString getTitre() const { return m_titre; }

    // CRUD
    bool ajouter();                       // l'ID vient de la sequence seq_campagne
    bool modifier();
    bool supprimer(int id);               // supprime aussi les lignes ANIMER
    QSqlQueryModel *afficher(const QString &orderBy = "c.id_campagne ASC");
    bool existe(int id);

    // Metiers
    QSqlQueryModel *rechercher(const QString &critere, const QString &valeur,
                               const QString &orderBy = "c.id_campagne ASC");
    QMap<QString, int> statistiques(const QString &critere);

    // Listes pour les combos / associations
    static QList<QPair<int, QString>> listeEmployes();     // association ANIMER
    static QList<QPair<int, QString>> listeZones();        // cle etrangere id_zone
    static QList<int> animateursDe(int idCampagne);

    static QString derniereErreur();

private:
    bool enregistrerAnimateurs();

    int m_id = 0;
    QString m_titre, m_theme, m_lieu, m_publicCible;
    QDate m_date;
    int m_nbParticipants = 0;
    int m_idZone = 0;
    QList<int> m_animateurs;

    static QString selectColonnes();
    static QString s_erreur;
};

#endif // CAMPAGNE_H
