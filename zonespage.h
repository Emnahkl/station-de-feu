#ifndef ZONESPAGE_H
#define ZONESPAGE_H

#include <QModelIndex>
#include <QWidget>

class QSqlQueryModel;
class BarChartWidget;
class CarteZonesWidget;
class Zone;

QT_BEGIN_NAMESPACE
namespace Ui { class ZonesPage; }
QT_END_NAMESPACE

// Module « Gestion des zones de couverture » (Chef de centre)
class ZonesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ZonesPage(QWidget *parent = nullptr);
    ~ZonesPage();

    void setSearch(const QString &texte);   // intégration : recherche de la barre du haut

private slots:
    // CRUD
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_vider_clicked();
    void on_tableView_zones_clicked(const QModelIndex &index);

    // Recherche, tri, export
    void on_lineEdit_recherche_textChanged(const QString &);
    void on_comboBox_critere_currentIndexChanged(int);
    void on_comboBox_tri_currentIndexChanged(int);
    void on_pushButton_exportPdf_clicked();
    void on_pushButton_exportExcel_clicked();

    // Statistiques
    void on_pushButton_actualiserStats_clicked();
    void on_comboBox_statCritere_currentIndexChanged(int);

    // Métiers innovants : carte des zones à risque + alerte de zone sous-couverte
    void on_spinBox_seuil_valueChanged(int);
    void on_tableWidget_alertes_cellClicked(int row, int);
    void afficherZone(int idZone);

private:
    void rafraichirListe();
    void rafraichirCarteEtAlertes();
    void remplirFormulaire(const Zone &z);
    bool lireFormulaire(Zone &z);

    Ui::ZonesPage *ui;
    QSqlQueryModel *m_model = nullptr;
    BarChartWidget *m_chart = nullptr;
    CarteZonesWidget *m_carte = nullptr;
};

#endif // ZONESPAGE_H
