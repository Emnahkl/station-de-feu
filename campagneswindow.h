#ifndef CAMPAGNESWINDOW_H
#define CAMPAGNESWINDOW_H

#include <QImage>
#include <QList>
#include <QWidget>
#include "campagne.h"
#include "menulateral.h"

class QLineEdit;
class QComboBox;
class QSpinBox;
class QDateEdit;
class QTableView;
class QListWidget;
class QPushButton;
class QChartView;
class QTabWidget;
class QLabel;
class QTableWidget;
class QTimer;

// Page « Campagnes de sensibilisation » integree dans AppShell.
// (Ancien MainWindow du module, renomme pour ne pas entrer en conflit avec le MainWindow
//  du module Employes.)
class CampagnesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CampagnesWindow(QWidget *parent = nullptr);

    void marquerMenu();
    void recharger();          // relit employes / zones / campagnes (appele a chaque affichage)

    MenuLateral *menu() const { return m_menu; }   // AppShell branche la navigation dessus

private slots:
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_supprimer_clicked();
    void on_vider_clicked();
    void on_table_clicked(const QModelIndex &index);
    void on_exportPdf_clicked();
    void on_exportExcel_clicked();
    void on_stats_refresh();
    void rafraichir();

    // Metiers avances (branche gestion-campagnes-sensibilisation)
    void on_lancerCiblage_clicked();
    void on_planifierZone_clicked();
    void on_evaluerImpact_clicked();
    void on_genererAffiche_clicked();
    void on_enregistrerAffiche_clicked();
    void on_apercuAnime_clicked();
    void on_enregistrerGif_clicked();
    void imageSuivante();

private:
    bool controleSaisie(bool ajout);
    Campagne campagneDuFormulaire() const;
    QList<int> animateursCoches() const;
    QString ordreCourant() const;
    void construireInterface();
    void chargerListes();
    void chargerCampagnesCombos();
    QWidget *construireOngletCiblage();
    QWidget *construireOngletImpact();
    QWidget *construireOngletAffiche();
    void afficherApercu(const QImage &image);
    void arreterAnimation();

    Campagne c;

    QSpinBox *idSpin = nullptr;
    QLineEdit *titreEdit = nullptr, *lieuEdit = nullptr, *rechercheEdit = nullptr;
    QComboBox *themeCombo = nullptr, *publicCombo = nullptr, *zoneCombo = nullptr;
    QComboBox *critereCombo = nullptr, *triCombo = nullptr, *ordreCombo = nullptr, *statsCombo = nullptr;
    QDateEdit *dateEdit = nullptr;
    QSpinBox *nbSpin = nullptr;
    QListWidget *animateursList = nullptr;
    QTableView *table = nullptr;
    QChartView *chartView = nullptr;
    QTabWidget *tabs = nullptr;
    QLabel *compteLabel = nullptr;
    MenuLateral *m_menu = nullptr;

    // Ciblage
    QTableWidget *ciblageTable = nullptr;
    QLabel *zonePrioritaireLabel = nullptr;
    QPushButton *btnPlanifier = nullptr;
    // Impact
    QComboBox *impactCombo = nullptr;
    QSpinBox *fenetreSpin = nullptr;
    QLabel *impactAvantLabel = nullptr, *impactApresLabel = nullptr;
    QLabel *impactVariationLabel = nullptr, *impactVerdictLabel = nullptr;
    QChartView *impactChart = nullptr;
    // Affiche
    QComboBox *afficheCombo = nullptr;
    QLineEdit *sloganEdit = nullptr;
    QLabel *apercuLabel = nullptr;
    QPushButton *btnEnregistrerPng = nullptr, *btnApercuAnime = nullptr, *btnEnregistrerGif = nullptr;
    QImage m_affiche;
    QList<QImage> m_images;
    int m_imageCourante = 0;
    QTimer *m_timer = nullptr;
};

#endif // CAMPAGNESWINDOW_H
