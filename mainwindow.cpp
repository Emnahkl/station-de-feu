#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exports.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QtCharts>


// Noms réels des colonnes SQL, dans le même ordre que les listes déroulantes du .ui
static const QStringList CHAMPS_RECHERCHE = { "NOM", "TYPE_EQUIP", "ETAT", "AFFECTATION" };
static const QStringList CHAMPS_TRI = { "ID", "NOM", "TYPE_EQUIP", "QUANTITE",
                                        "DATE_ACQUISITION", "ETAT", "DATE_CONTROLE" };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), model(new EquipementModel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    viderFormulaire();
    rafraichir();
}

MainWindow::~MainWindow() { delete ui; }

// ============================================================ Utilitaires
void MainWindow::rafraichir()
{
    model->setQuery(Equipement::lister(
        CHAMPS_RECHERCHE.value(ui->cbChampRecherche->currentIndex()),
        ui->leRecherche->text(),
        CHAMPS_TRI.value(ui->cbTri->currentIndex()),
        ui->cbOrdre->currentIndex() == 0));
    ui->tableView->resizeColumnsToContents();
    verifierAlertes(false);
}

void MainWindow::viderFormulaire()
{
    ui->leId->clear();
    ui->leNom->clear();
    ui->cbType->setCurrentIndex(0);
    ui->sbQuantite->setValue(0);
    ui->sbSeuil->setValue(5);
    ui->deAcquisition->setDate(QDate::currentDate());
    ui->cbEtat->setCurrentIndex(0);
    ui->deControle->setDate(QDate::currentDate().addMonths(6));
    ui->leAffectation->clear();
    ui->tableView->clearSelection();
}

// Contrôle de saisie + création de l'objet Equipement
bool MainWindow::lireFormulaire(Equipement &e)
{
    const QString nom = ui->leNom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Saisie invalide", "Le nom est obligatoire.");
        return false;
    }
    if (ui->deControle->date() < ui->deAcquisition->date()) {
        QMessageBox::warning(this, "Saisie invalide",
            "La date du prochain contrôle doit être après la date d'acquisition.");
        return false;
    }
    e = Equipement(nom, ui->cbType->currentText(), ui->sbQuantite->value(),
                   ui->deAcquisition->date(), ui->cbEtat->currentText(),
                   ui->deControle->date(), ui->leAffectation->text().trimmed(),
                   ui->sbSeuil->value());
    return true;
}

// ================================================================== CRUD
void MainWindow::on_btnAjouter_clicked()
{
    Equipement e;
    if (!lireFormulaire(e)) return;
    if (e.ajouter()) {
        rafraichir();
        viderFormulaire();
        QMessageBox::information(this, "Ajout", "Équipement ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Ajout", "Échec de l'ajout.");
    }
}

void MainWindow::on_btnModifier_clicked()
{
    if (ui->leId->text().isEmpty()) {
        QMessageBox::warning(this, "Modifier", "Sélectionnez d'abord un équipement dans le tableau.");
        return;
    }
    Equipement e;
    if (!lireFormulaire(e)) return;
    e.setId(ui->leId->text().toInt());
    if (e.modifier()) {
        rafraichir();
        QMessageBox::information(this, "Modification", "Équipement modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Modification", "Échec de la modification.");
    }
}

void MainWindow::on_btnSupprimer_clicked()
{
    if (ui->leId->text().isEmpty()) {
        QMessageBox::warning(this, "Supprimer", "Sélectionnez d'abord un équipement dans le tableau.");
        return;
    }
    if (QMessageBox::question(this, "Suppression",
            "Voulez-vous vraiment supprimer cet équipement ?") != QMessageBox::Yes)
        return;
    Equipement e;
    if (e.supprimer(ui->leId->text().toInt())) {
        rafraichir();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Suppression", "Échec de la suppression.");
    }
}

void MainWindow::on_btnVider_clicked() { viderFormulaire(); }

// Bouton "+ Ajouter un équipement" (en haut de la liste) : prépare un formulaire vide
void MainWindow::on_btnNouveau_clicked()
{
    viderFormulaire();
    ui->leNom->setFocus();
}

// Clic sur une ligne -> remplit le formulaire
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    const int r = index.row();
    auto val = [&](int col) { return model->data(model->index(r, col)).toString(); };
    ui->leId->setText(val(COL_ID));
    ui->leNom->setText(val(COL_NOM));
    ui->cbType->setCurrentText(val(COL_TYPE));
    ui->sbQuantite->setValue(val(COL_QTE).toInt());
    ui->deAcquisition->setDate(QDate::fromString(val(COL_DATE_ACQ), Qt::ISODate));
    ui->cbEtat->setCurrentText(val(COL_ETAT));
    ui->deControle->setDate(QDate::fromString(val(COL_DATE_CTRL), Qt::ISODate));
    ui->leAffectation->setText(val(COL_AFFECT));
    ui->sbSeuil->setValue(val(COL_SEUIL).toInt());
}

// ========================================================= Recherche + tri
void MainWindow::on_leRecherche_textChanged(const QString &)      { rafraichir(); }
void MainWindow::on_cbChampRecherche_currentIndexChanged(int)     { rafraichir(); }
void MainWindow::on_cbTri_currentIndexChanged(int)                { rafraichir(); }
void MainWindow::on_cbOrdre_currentIndexChanged(int)              { rafraichir(); }

// ================================================================ Exports
void MainWindow::on_btnExportPdf_clicked()
{
    const QString f = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                   "equipements.pdf", "PDF (*.pdf)");
    if (f.isEmpty()) return;
    const bool ok = Exports::ecrirePdf(f, Exports::htmlDepuisModele(model, "Liste des équipements"));
    QMessageBox::information(this, "Export PDF", ok ? "PDF créé." : "Échec de l'export.");
}

void MainWindow::on_btnExportExcel_clicked()
{
    const QString f = QFileDialog::getSaveFileName(this, "Exporter vers Excel",
                                                   "equipements.csv", "Excel / CSV (*.csv)");
    if (f.isEmpty()) return;
    const bool ok = Exports::ecrireCsv(f, model);
    QMessageBox::information(this, "Export Excel", ok ? "Fichier créé (ouvrable avec Excel)." : "Échec de l'export.");
}

// ============================================================ Statistiques
void MainWindow::on_btnStats_clicked()
{
    // Camembert : équipements par état
    auto *pie = new QPieSeries();
    const auto parEtat = Equipement::statsParEtat();
    for (auto it = parEtat.constBegin(); it != parEtat.constEnd(); ++it) {
        QPieSlice *part = pie->append(QString("%1 (%2)").arg(it.key()).arg(it.value()), it.value());
        // Couleurs de la charte : vert = fonctionnel, rouge = en panne, orange = en réparation
        if (it.key() == "Fonctionnel")        part->setBrush(QColor("#2E7D32"));
        else if (it.key() == "En panne")      part->setBrush(QColor("#C62828"));
        else if (it.key() == "En réparation") part->setBrush(QColor("#FF6F00"));
    }
    pie->setLabelsVisible(true);
    auto *chartEtat = new QChart();
    chartEtat->addSeries(pie);
    chartEtat->setTitle("Répartition par état");

    // Histogramme : équipements par type
    auto *bar = new QBarSeries();
    const auto parType = Equipement::statsParType();
    QStringList categories;
    const QStringList palette = { "#C62828", "#FF6F00", "#2B2B2B", "#2E7D32", "#8E1B1B" };
    int i = 0;
    for (auto it = parType.constBegin(); it != parType.constEnd(); ++it, ++i) {
        auto *set = new QBarSet(it.key());
        *set << it.value();
        set->setColor(QColor(palette.at(i % palette.size())));
        bar->append(set);
    }
    categories << "Types";
    auto *chartType = new QChart();
    chartType->addSeries(bar);
    chartType->setTitle("Nombre d'équipements par type");
    auto *axeX = new QBarCategoryAxis();
    axeX->append(categories);
    chartType->addAxis(axeX, Qt::AlignBottom);
    bar->attachAxis(axeX);
    auto *axeY = new QValueAxis();
    axeY->setLabelFormat("%d");
    axeY->setMin(0);
    chartType->addAxis(axeY, Qt::AlignLeft);
    bar->attachAxis(axeY);

    auto *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowTitle("Statistiques des équipements");
    dlg->resize(700, 480);
    auto *tabs = new QTabWidget(dlg);
    tabs->addTab(new QChartView(chartEtat), "Par état");
    tabs->addTab(new QChartView(chartType), "Par type");
    auto *lay = new QVBoxLayout(dlg);
    lay->addWidget(tabs);
    dlg->show();
}

// ================================== Métier innovant 1 : stock + bon de commande
void MainWindow::on_btnBonCommande_clicked()
{
    QSqlQuery q = Equipement::stockEnAlerte();
    QString lignes;
    int n = 0;
    while (q.next()) {
        const int stock = q.value(COL_QTE).toInt();
        const int seuil = q.value(COL_SEUIL).toInt();
        const int aCommander = qMax(1, 2 * seuil - stock);   // on recommande jusqu'à 2 x le seuil
        lignes += QString("<tr><td>%1</td><td>%2</td><td align='center'>%3</td>"
                          "<td align='center'>%4</td><td align='center'><b>%5</b></td></tr>")
                  .arg(q.value(COL_NOM).toString().toHtmlEscaped(),
                       q.value(COL_TYPE).toString().toHtmlEscaped())
                  .arg(stock).arg(seuil).arg(aCommander);
        ++n;
    }
    if (n == 0) {
        QMessageBox::information(this, "Bon de commande", "Aucun équipement sous le seuil d'alerte.");
        return;
    }
    const QString f = QFileDialog::getSaveFileName(this, "Enregistrer le bon de commande",
                                                   "bon_de_commande.pdf", "PDF (*.pdf)");
    if (f.isEmpty()) return;

    const QString html =
        "<h2 align='center'>BON DE COMMANDE</h2>"
        "<p align='center'>SafeStation - " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>"
        "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
        "<tr bgcolor='#C62828' style='color:white'><th>Équipement</th><th>Type</th>"
        "<th>Stock actuel</th><th>Seuil</th><th>Quantité à commander</th></tr>" + lignes + "</table>";
    const bool ok = Exports::ecrirePdf(f, html);
    QMessageBox::information(this, "Bon de commande",
        ok ? QString("Bon de commande généré (%1 article(s)).").arg(n) : "Échec de la génération.");
}

// ===== Métiers innovants 1 + 2 : alertes de stock et de contrôles périodiques
void MainWindow::verifierAlertes(bool avecPopup)
{
    int nStock = 0, nCtrl = 0;
    QSqlQuery qs = Equipement::stockEnAlerte();
    while (qs.next()) ++nStock;
    QSqlQuery qc = Equipement::controlesProches(30);
    while (qc.next()) ++nCtrl;

    if (nStock == 0 && nCtrl == 0) {
        ui->lblAlertes->setText("✔ Aucune alerte");
        ui->lblAlertes->setStyleSheet("background:#E8F5E9; color:#2E7D32; padding:8px 12px; border-radius:8px; font-weight:bold;");
        return;
    }
    const QString msg = QString("⚠ %1 équipement(s) sous le seuil de stock  |  "
                                "%2 contrôle(s) en retard ou à effectuer sous 30 jours")
                        .arg(nStock).arg(nCtrl);
    ui->lblAlertes->setText(msg);
    ui->lblAlertes->setStyleSheet("background:#FFF3E0; color:#E65100; padding:8px 12px; border-radius:8px; font-weight:bold;");
    if (avecPopup)
        QMessageBox::warning(this, "Alertes équipements", msg);
}
