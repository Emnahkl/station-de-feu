#ifndef GESTIONINTERVENTIONS_H
#define GESTIONINTERVENTIONS_H

#include <QMap>
#include <QVector>
#include <QWidget>

#include "affectation.h"
#include "intervention.h"

class QComboBox;
class QDateEdit;
class QLabel;
class QLineEdit;
class QTimeEdit;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class GestionInterventions; }
QT_END_NAMESPACE

// Une seule page, sans formulaire ni fenêtre :
//  - « Ajouter » insère une ligne vide EN HAUT du tableau, « Modifier » (ou double-clic) transforme
//    la ligne choisie en ligne éditable : on saisit directement dans les cellules ;
//  - un bandeau placé juste au-dessus du tableau porte les boutons Enregistrer / Annuler
//    (Entrée / Échap au clavier), la confirmation de suppression, la fiche « Consulter »
//    et les messages ;
//  - les statistiques remplacent le tableau, avec un bouton « Retour à la liste ».
// Seul l'export PDF ouvre une boîte de dialogue (choix du fichier).
class GestionInterventions : public QWidget
{
    Q_OBJECT

public:
    explicit GestionInterventions(QWidget *parent = nullptr);
    ~GestionInterventions();

    void marquerMenu();             // remet « Interventions » en surbrillance dans le menu lateral

signals:
    void ouvrirEmployes();          // clic sur « Employés » du menu lateral (integration)

private slots:
    void rafraichir();              // recharge le tableau depuis Oracle (filtres + recherche + tri)
    void ajouter();                 // ligne vide en haut du tableau
    void modifier();                // ligne sélectionnée -> éditable
    void supprimer();               // demande la confirmation dans le bandeau
    void consulter();               // fiche de lecture dans le bandeau
    void bandeauValider();          // bouton principal du bandeau (selon le contexte)
    void bandeauAnnuler();          // bouton secondaire du bandeau (selon le contexte)
    void enregistrerEdition();
    void annulerEdition();
    void confirmerSuppression();
    void majBoutons();              // active / désactive les boutons selon la sélection et le mode
    void exporterPdf();
    void trierParDate();
    void trierParId();
    void afficherStats();           // bascule tableau <-> statistiques
    void afficherParcours();        // bascule tableau <-> parcours recommandé
    void afficherAffectation();     // propose l'affectation de l'intervention sélectionnée
    void validerAffectation();      // enregistre la proposition affichée
    void fonctionnaliteAVenir();

private:
    enum Tri { TriId, TriDate };
    enum Mode { ModeListe, ModeAjout, ModeModif };                       // édition dans le tableau
    enum Contexte { CtxAucun, CtxEdition, CtxSuppression, CtxDetail };   // contenu du bandeau
    enum Etat { EtatInfo, EtatOk, EtatErreur, EtatEdition, EtatDanger };
    enum Champ { ChType = 1, ChAdresse = 2, ChDate = 4, ChZone = 8 };    // champs déjà touchés

    // Widgets de la ligne en cours d'édition (valides seulement hors ModeListe)
    struct LigneEdition {
        QComboBox *type = nullptr;
        QLineEdit *adresse = nullptr;
        QDateEdit *date = nullptr;
        QTimeEdit *heure = nullptr;
        QComboBox *zone = nullptr;
        QComboBox *gravite = nullptr;
        QComboBox *statut = nullptr;
    };

    int  idCourant() const;
    void chargerZones();
    QString libelleZone(int idZone) const;
    void montrerListe();
    void mettreAJourCartes();
    void mettreAJourStats(const QVector<Intervention> &toutes);
    void mettreAJourBoutonsTri();
    int  mettreAJourParcours();     // construit le parcours ; renvoie le nombre d'étapes

    // Édition dans le tableau
    void demarrerEdition(Mode mode, const Intervention &modele);
    void terminerEdition();
    QWidget *envelopper(QWidget *a, QWidget *b = nullptr);
    bool verifierEdition(bool strict = false);
    Intervention depuisEdition() const;
    void definirErreur(QWidget *champ, const QString &erreur, bool afficher);

    // Bandeau (messages, boutons Enregistrer / Annuler, confirmation, fiche)
    void bandeau(const QString &texte, Etat etat, const QString &boutonOk = QString(),
                 const QString &boutonAnnuler = QString(), bool permanent = false);
    void message(const QString &texte, Etat etat = EtatInfo);   // message temporaire
    void masquerBandeau();

    Ui::GestionInterventions *ui;
    Tri m_tri;
    bool m_idCroissant;
    bool m_dateCroissante;          // tri par date : false = du plus récent au plus ancien
    QVector<Intervention> m_liste;
    QMap<int, QString> m_zones;     // id -> nom (affichage du tableau et de la fiche)

    PropositionAffectation m_proposition;   // affectation affichée (page Affectation)
    int  m_idAffectation;                   // intervention concernée

    Mode m_mode;
    Contexte m_contexte;
    int  m_idEdition;               // id de l'intervention en cours de modification (-1 = ajout)
    int  m_ligneEdition;            // ligne du tableau en cours d'édition
    quint32 m_touches;              // champs que l'utilisateur a déjà modifiés (erreurs affichées)
    LigneEdition m_ed;
    QTimer *m_timerMessage;
};

#endif // GESTIONINTERVENTIONS_H
