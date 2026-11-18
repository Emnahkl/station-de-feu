#ifndef EQUIPEMENTSWINDOW_H
#define EQUIPEMENTSWINDOW_H

#include <QMainWindow>
#include <QDateEdit>
#include "equipement.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EquipementsWindow; }
QT_END_NAMESPACE

class EquipementsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EquipementsWindow(QWidget *parent = nullptr);
    ~EquipementsWindow();
    void verifierAlertes(bool avecPopup);

private slots:
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btnVider_clicked();
    void on_btnNouveau_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_leRecherche_textChanged(const QString &);
    void on_cbChampRecherche_currentIndexChanged(int);
    void on_cbTri_currentIndexChanged(int);
    void on_cbOrdre_currentIndexChanged(int);
    void on_btnExportPdf_clicked();
    void on_btnExportExcel_clicked();
    void on_btnStats_clicked();
    void on_btnBonCommande_clicked();

private:
    void rafraichir();
    bool lireFormulaire(Equipement &e);
    void viderFormulaire();
    void afficherQrSelectionne();
    void lancerInventaireQr();

    Ui::EquipementsWindow *ui;
    EquipementModel *model;
    QDateEdit *dePeremption;
};

#endif // EQUIPEMENTSWINDOW_H
