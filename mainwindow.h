#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "campagne.h"

class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QDateEdit;
class QTableView;
class QPushButton;
class QChartView;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_supprimer_clicked();
    void on_vider_clicked();
    void on_table_clicked(const QModelIndex &index);
    void on_recherche_changed();
    void on_exportPdf_clicked();
    void on_stats_refresh();

private:
    bool controleSaisie(bool ajout);
    Campagne campagneDuFormulaire() const;
    QString ordreCourant() const;
    void rafraichir();
    void construireInterface();
    void chargerListes();

    Campagne c;

    QSpinBox *idSpin;
    QLineEdit *titreEdit, *rechercheEdit;
    QPlainTextEdit *descriptionEdit;
    QComboBox *employeCombo, *zoneCombo;
    QComboBox *themeCombo, *publicCombo, *statutCombo;
    QComboBox *critereCombo, *triCombo, *ordreCombo, *statsCombo;
    QDateEdit *dateDebutEdit, *dateFinEdit;
    QSpinBox *nbSpin;
    QDoubleSpinBox *budgetSpin, *scoreSpin;
    QTableView *table;
    QChartView *chartView;
};

#endif
