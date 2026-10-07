#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "equipement.h"
#include "equipementmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void verifierAlertes(bool avecPopup);   // métier innovant 1 + 2

private slots:
    // CRUD
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btnVider_clicked();
    void on_btnNouveau_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    // Recherche + tri
    void on_leRecherche_textChanged(const QString &);
    void on_cbChampRecherche_currentIndexChanged(int);
    void on_cbTri_currentIndexChanged(int);
    void on_cbOrdre_currentIndexChanged(int);
    // Exports, stats, bon de commande
    void on_btnExportPdf_clicked();
    void on_btnExportExcel_clicked();
    void on_btnStats_clicked();
    void on_btnBonCommande_clicked();

private:
    void rafraichir();                       // recharge le tableau (recherche + tri)
    bool lireFormulaire(Equipement &e);      // formulaire -> objet (avec contrôle de saisie)
    void viderFormulaire();

    Ui::MainWindow  *ui;
    EquipementModel *model;
};

#endif // MAINWINDOW_H
