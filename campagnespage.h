#ifndef CAMPAGNESPAGE_H
#define CAMPAGNESPAGE_H

#include <QImage>
#include <QDate>
#include <QMainWindow>
#include <QModelIndex>

class QSqlQueryModel;
class BarChartWidget;
class Campagne;
class QPlainTextEdit;

QT_BEGIN_NAMESPACE
namespace Ui { class CampagnesPage; }
QT_END_NAMESPACE

class CampagnesPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit CampagnesPage(QWidget *parent = nullptr);
    ~CampagnesPage();
    void setSearch(const QString &text);

private slots:
    // Navigation (QStackedWidget)
    void on_pushButton_navDashboard_clicked();
    void on_pushButton_navEmployes_clicked();
    void on_pushButton_navVehicules_clicked();
    void on_pushButton_navEquipements_clicked();
    void on_pushButton_navInterventions_clicked();
    void on_pushButton_navCampagnes_clicked();
    void on_pushButton_navZones_clicked();

    // CRUD
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_vider_clicked();
    void on_tableView_campagnes_clicked(const QModelIndex &index);

    // Recherche, tri, export
    void on_lineEdit_recherche_textChanged(const QString &);
    void on_comboBox_critere_currentIndexChanged(int);
    void on_comboBox_tri_currentIndexChanged(int);
    void on_pushButton_exportPdf_clicked();
    void on_pushButton_exportExcel_clicked();

    // Statistiques
    void on_pushButton_actualiserStats_clicked();
    void on_comboBox_statCritere_currentIndexChanged(int);

    // Métiers innovants
    void on_pushButton_lancerCiblage_clicked();
    void on_pushButton_planifierZone_clicked();
    void on_pushButton_evaluerImpact_clicked();
    void on_pushButton_genererAffiche_clicked();
    void on_pushButton_enregistrerAffiche_clicked();
    void ouvrirDetailsAvances();

private:
    void naviguer(int index, const QString &titre);
    void chargerZonesEtEmployes();
    void chargerListesCampagnes();
    void rafraichirListe();
    bool lireFormulaire(Campagne &c);
    QString sloganPourTheme(const QString &theme) const;
    QImage genererAffiche(int idCampagne, const QString &slogan) const;

    Ui::CampagnesPage *ui;
    QSqlQueryModel *m_model = nullptr;
    BarChartWidget *m_chart = nullptr;
    QImage m_affiche;
    QString m_description;
    QDate m_dateDebut;
    QDate m_dateFin;
    double m_budget = 0.0;
    QString m_statut = QString::fromUtf8("Planifiée");
    double m_scoreImpact = 0.0;
};

#endif // CAMPAGNESPAGE_H
