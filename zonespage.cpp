#include "zonespage.h"
#include "ui_zonespage.h"

#include "barchartwidget.h"
#include "cartezoneswidget.h"
#include "zone.h"

#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QIntValidator>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QSqlQueryModel>
#include <QStandardPaths>
#include <QTableWidgetItem>
#include <QTextDocument>
#include <QTextStream>
#include <algorithm>

ZonesPage::ZonesPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::ZonesPage)
{
    ui->setupUi(this);
    ui->lineEdit_id->setValidator(new QIntValidator(1, 999999, this));

    m_carte = new CarteZonesWidget(ui->groupBox_carte);
    ui->verticalLayout_carte->addWidget(m_carte);
    connect(m_carte, &CarteZonesWidget::zoneCliquee, this, &ZonesPage::afficherZone);

    m_chart = new BarChartWidget(ui->groupBox_graphique);
    ui->verticalLayout_chart->addWidget(m_chart);

    ui->tableWidget_alertes->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_alertes->horizontalHeader()->setStretchLastSection(true);

    rafraichirListe();
    on_pushButton_vider_clicked();
    on_pushButton_actualiserStats_clicked();
}

ZonesPage::~ZonesPage()
{
    delete m_model;
    delete ui;
}

void ZonesPage::setSearch(const QString &texte)
{
    ui->lineEdit_recherche->setText(texte);
}

// ---------------------------------------------------------------- Liste

void ZonesPage::rafraichirListe()
{
    static const char *criteres[] = {"NOM", "REGION"};
    static const char *tris[] = {
        "CASE NIVEAU_RISQUE WHEN 'Élevé' THEN 0 WHEN 'Moyen' THEN 1 ELSE 2 END, NOM",
        "CASE NIVEAU_RISQUE WHEN 'Élevé' THEN 0 WHEN 'Moyen' THEN 1 ELSE 2 END DESC, NOM",
        "TEMPS_INTERVENTION_MOYEN DESC", "TEMPS_INTERVENTION_MOYEN ASC", "NOM ASC"};

    const int ic = qBound(0, ui->comboBox_critere->currentIndex(), 1);
    const int it = qBound(0, ui->comboBox_tri->currentIndex(), 4);

    QSqlQueryModel *ancien = m_model;
    m_model = Zone::afficher(criteres[ic], ui->lineEdit_recherche->text(), QString::fromUtf8(tris[it]));
    ui->tableView_zones->setModel(m_model);
    delete ancien;

    ui->tableView_zones->resizeColumnsToContents();
    ui->label_compteur->setText(QString("%1 zone(s)").arg(m_model->rowCount()));
    rafraichirCarteEtAlertes();
}

void ZonesPage::on_lineEdit_recherche_textChanged(const QString &) { rafraichirListe(); }
void ZonesPage::on_comboBox_critere_currentIndexChanged(int) { rafraichirListe(); }
void ZonesPage::on_comboBox_tri_currentIndexChanged(int) { rafraichirListe(); }

// ---------------------------------------------------------------- CRUD

void ZonesPage::remplirFormulaire(const Zone &z)
{
    ui->lineEdit_id->setText(QString::number(z.getId()));
    ui->lineEdit_nom->setText(z.getNom());
    ui->lineEdit_region->setText(z.getRegion());
    ui->doubleSpinBox_superficie->setValue(z.getSuperficie());
    ui->spinBox_population->setValue(z.getPopulation());
    ui->comboBox_risque->setCurrentText(z.getNiveauRisque());
    ui->spinBox_temps->setValue(z.getTempsIntervention());
    ui->doubleSpinBox_latitude->setValue(z.getLatitude());
    ui->doubleSpinBox_longitude->setValue(z.getLongitude());
}

bool ZonesPage::lireFormulaire(Zone &z)
{
    z.setId(ui->lineEdit_id->text().toInt());
    z.setNom(ui->lineEdit_nom->text());
    z.setRegion(ui->lineEdit_region->text());
    z.setSuperficie(ui->doubleSpinBox_superficie->value());
    z.setPopulation(ui->spinBox_population->value());
    z.setNiveauRisque(ui->comboBox_risque->currentText());
    z.setTempsIntervention(ui->spinBox_temps->value());
    z.setLatitude(ui->doubleSpinBox_latitude->value());
    z.setLongitude(ui->doubleSpinBox_longitude->value());

    const QString err = z.valider();
    if (!err.isEmpty()) {
        QMessageBox::warning(this, "Saisie invalide", err);
        return false;
    }
    return true;
}

void ZonesPage::on_pushButton_ajouter_clicked()
{
    Zone z;
    if (!lireFormulaire(z)) return;
    QString err;
    if (z.ajouter(&err)) {
        QMessageBox::information(this, "Ajout", "Zone ajoutée avec succès.");
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
        on_pushButton_vider_clicked();
    } else {
        QMessageBox::critical(this, "Ajout", "Échec de l'ajout :\n" + err);
    }
}

void ZonesPage::on_pushButton_modifier_clicked()
{
    Zone z;
    if (!lireFormulaire(z)) return;
    QString err;
    if (z.modifier(&err)) {
        QMessageBox::information(this, "Modification", "Zone modifiée avec succès.");
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
    } else {
        QMessageBox::critical(this, "Modification", "Échec de la modification :\n" + err);
    }
}

void ZonesPage::on_pushButton_supprimer_clicked()
{
    const int id = ui->lineEdit_id->text().toInt();
    if (id <= 0 || !Zone::existe(id)) {
        QMessageBox::warning(this, "Suppression", "Sélectionnez une zone dans la liste.");
        return;
    }
    if (QMessageBox::question(this, "Suppression",
                              QString("Supprimer définitivement la zone #%1 ?").arg(id)) != QMessageBox::Yes)
        return;
    QString err;
    if (Zone::supprimer(id, &err)) {
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
        on_pushButton_vider_clicked();
    } else {
        QMessageBox::critical(this, "Suppression", "Suppression impossible :\n" + err);
    }
}

void ZonesPage::on_pushButton_vider_clicked()
{
    ui->lineEdit_id->setText(QString::number(Zone::prochainId()));
    ui->lineEdit_nom->clear();
    ui->lineEdit_region->clear();
    ui->doubleSpinBox_superficie->setValue(1.0);
    ui->spinBox_population->setValue(1000);
    ui->comboBox_risque->setCurrentIndex(1);
    ui->spinBox_temps->setValue(10);
    ui->doubleSpinBox_latitude->setValue(36.80);
    ui->doubleSpinBox_longitude->setValue(10.18);
    ui->tableView_zones->clearSelection();
}

void ZonesPage::on_tableView_zones_clicked(const QModelIndex &index)
{
    Zone z;
    if (Zone::charger(m_model->data(m_model->index(index.row(), 0)).toInt(), z)) {
        remplirFormulaire(z);
        m_carte->setSelection(z.getId());
    }
}

// ---------------------------------------------------------------- Export

void ZonesPage::on_pushButton_exportPdf_clicked()
{
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter en PDF",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/zones_couverture.pdf",
        "PDF (*.pdf)");
    if (chemin.isEmpty()) return;

    QString html = "<h2 style='color:#C62828'>FireStation Manager — Zones de couverture</h2>"
                   "<p>Exporté le " + QDate::currentDate().toString("dd/MM/yyyy") + " — " +
                   QString::number(m_model->rowCount()) + " zone(s)</p>"
                   "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
                   "<tr style='background:#C62828;color:#FFFFFF'>";
    for (int c = 0; c < m_model->columnCount(); ++c)
        html += "<th>" + m_model->headerData(c, Qt::Horizontal).toString().toHtmlEscaped() + "</th>";
    html += "</tr>";
    for (int r = 0; r < m_model->rowCount(); ++r) {
        html += "<tr>";
        for (int c = 0; c < m_model->columnCount(); ++c)
            html += "<td>" + m_model->data(m_model->index(r, c)).toString().toHtmlEscaped() + "</td>";
        html += "</tr>";
    }
    html += "</table>";

    QPdfWriter pdf(chemin);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageOrientation(QPageLayout::Landscape);
    pdf.setResolution(96);
    pdf.setTitle("Zones de couverture");
    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&pdf);
    QMessageBox::information(this, "Export PDF", "Fichier enregistré :\n" + chemin);
}

void ZonesPage::on_pushButton_exportExcel_clicked()
{
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter pour Excel",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/zones_couverture.csv",
        "CSV Excel (*.csv)");
    if (chemin.isEmpty()) return;

    QFile f(chemin);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export Excel", "Impossible d'écrire le fichier.");
        return;
    }
    QTextStream out(&f);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif
    out << QString(QChar(0xFEFF));   // BOM : Excel reconnaît les accents
    auto cellule = [](QString s) { s.replace('"', "\"\""); return "\"" + s + "\""; };
    QStringList ligne;
    for (int c = 0; c < m_model->columnCount(); ++c)
        ligne << cellule(m_model->headerData(c, Qt::Horizontal).toString());
    out << ligne.join(';') << "\n";
    for (int r = 0; r < m_model->rowCount(); ++r) {
        ligne.clear();
        for (int c = 0; c < m_model->columnCount(); ++c)
            ligne << cellule(m_model->data(m_model->index(r, c)).toString());
        out << ligne.join(';') << "\n";
    }
    f.close();
    QMessageBox::information(this, "Export Excel", "Fichier enregistré :\n" + chemin);
}

// ---------------------------------------------------------------- Statistiques

void ZonesPage::on_pushButton_actualiserStats_clicked()
{
    const bool parZone = ui->comboBox_statCritere->currentIndex() == 0;
    const auto data = parZone ? Zone::interventionsParZone() : Zone::interventionsParNiveauRisque();
    m_chart->setData(data, parZone ? "Interventions par zone (12 derniers mois)"
                                   : "Interventions par niveau de risque (12 derniers mois)");
    int total = 0;
    for (const auto &d : data) total += d.second;
    ui->label_statTotal->setText(QString("Total : %1 intervention(s)").arg(total));
}

void ZonesPage::on_comboBox_statCritere_currentIndexChanged(int)
{
    if (m_chart) on_pushButton_actualiserStats_clicked();
}

// ---------------------------------------------------------------- Métiers innovants

// 1) Carte des zones à risque selon l'historique : le risque affiché est calculé
//    à partir du nombre d'interventions des 12 derniers mois pour 10 000 habitants
//    (tiers inférieur = faible, tiers médian = moyen, tiers supérieur = élevé).
// 2) Alerte de zone sous-couverte : temps d'intervention moyen > seuil choisi.
void ZonesPage::rafraichirCarteEtAlertes()
{
    const QList<Zone> zones = Zone::toutes();
    const int seuil = ui->spinBox_seuil->value();

    QList<CarteZonesWidget::Point> points;
    QList<double> taux;
    for (const Zone &z : zones) {
        CarteZonesWidget::Point p;
        p.id = z.getId();
        p.nom = z.getNom();
        p.latitude = z.getLatitude();
        p.longitude = z.getLongitude();
        p.population = z.getPopulation();
        p.interventions = Zone::interventionsRecentes(z.getId());
        p.sousCouverte = z.getTempsIntervention() > seuil;
        taux << (p.population > 0 ? 10000.0 * p.interventions / p.population : 0.0);
        points << p;
    }
    QList<double> tri = taux;
    std::sort(tri.begin(), tri.end());
    const double t1 = tri.isEmpty() ? 0 : tri.at(tri.size() / 3);
    const double t2 = tri.isEmpty() ? 0 : tri.at((2 * tri.size()) / 3);
    for (int i = 0; i < points.size(); ++i)
        points[i].classeRisque = taux.at(i) >= t2 && taux.at(i) > 0 ? 2 : (taux.at(i) >= t1 && taux.at(i) > 0 ? 1 : 0);
    m_carte->setPoints(points);

    // Tableau des alertes
    QTableWidget *t = ui->tableWidget_alertes;
    t->setRowCount(0);
    int n = 0;
    for (int i = 0; i < zones.size(); ++i) {
        const Zone &z = zones.at(i);
        if (z.getTempsIntervention() <= seuil) continue;
        t->insertRow(n);
        auto *nom = new QTableWidgetItem(z.getNom());
        nom->setData(Qt::UserRole, z.getId());
        t->setItem(n, 0, nom);
        t->setItem(n, 1, new QTableWidgetItem(QString("%1 min").arg(z.getTempsIntervention())));
        auto *ecart = new QTableWidgetItem(QString("+%1 min").arg(z.getTempsIntervention() - seuil));
        ecart->setForeground(QColor("#C62828"));
        t->setItem(n, 2, ecart);
        t->setItem(n, 3, new QTableWidgetItem(QString::number(points.at(i).interventions)));
        ++n;
    }

    if (n == 0) {
        ui->label_alerteResume->setText(QString("✔ Aucune zone ne dépasse %1 min.").arg(seuil));
        ui->label_alerteResume->setStyleSheet("background:#E8F5E9; color:#2E7D32;");
    } else {
        ui->label_alerteResume->setText(QString("⚠ %1 zone(s) sous-couverte(s) : temps d'intervention moyen "
                                                "supérieur à %2 min. Renforcer les moyens ou revoir l'affectation "
                                                "des véhicules.").arg(n).arg(seuil));
        ui->label_alerteResume->setStyleSheet("background:#FDECEA; color:#C62828;");
    }
}

void ZonesPage::on_spinBox_seuil_valueChanged(int)
{
    rafraichirCarteEtAlertes();
}

void ZonesPage::on_tableWidget_alertes_cellClicked(int row, int)
{
    if (QTableWidgetItem *it = ui->tableWidget_alertes->item(row, 0))
        afficherZone(it->data(Qt::UserRole).toInt());
}

void ZonesPage::afficherZone(int idZone)
{
    Zone z;
    if (!Zone::charger(idZone, z)) return;
    m_carte->setSelection(idZone);
    remplirFormulaire(z);
    ui->label_zoneSelection->setText(
        QString("<b>%1</b> (%2) — %3 hab., %4 km² — risque déclaré : <b>%5</b> — "
                "temps moyen : <b>%6 min</b> — %7 intervention(s) sur 12 mois.")
            .arg(z.getNom().toHtmlEscaped(), z.getRegion().toHtmlEscaped())
            .arg(z.getPopulation()).arg(z.getSuperficie(), 0, 'f', 1)
            .arg(z.getNiveauRisque()).arg(z.getTempsIntervention())
            .arg(Zone::interventionsRecentes(idZone)));
}
