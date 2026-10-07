#include "campagnewindow.h"
#include "ui_campagnewindow.h"

#include "barchartwidget.h"
#include "campagne.h"

#include <QAbstractTextDocumentLayout>
#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QIntValidator>
#include <QLabel>
#include <QLinearGradient>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPainter>
#include <QPainterPath>
#include <QPdfWriter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardPaths>
#include <QTableWidgetItem>
#include <QTextDocument>
#include <QTextStream>

// Index des pages du QStackedWidget (même ordre que dans mainwindow.ui)
enum Page { PageDashboard = 0, PageEmployes, PageVehicules, PageEquipements,
            PageInterventions, PageCampagnes, PageZones };

CampagneWindow::CampagneWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Identité visuelle : icône de fenêtre et pied de page
    setWindowIcon(QIcon(":/images/logo.png"));
    ui->statusbar->addWidget(new QLabel(QString::fromUtf8(
        "Fire Station  ·  v1.5.0   |   Caserne Centrale  ·  Tunis"), this));
    ui->statusbar->addPermanentWidget(new QLabel(QString::fromUtf8(
        "Prévenir · Protéger · Sauver"), this));

    // Contrôles de saisie
    ui->lineEdit_id->setValidator(new QIntValidator(1, 999999, this));
    ui->dateEdit_date->setDate(QDate::currentDate());
    ui->dateEdit_date->setMinimumDate(QDate(2000, 1, 1));

    // Graphique des statistiques (dessiné avec QPainter)
    m_chart = new BarChartWidget(ui->groupBox_graphique);
    ui->verticalLayout_chart->addWidget(m_chart);

    ui->tableWidget_ciblage->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_ciblage->horizontalHeader()->setStretchLastSection(true);

    chargerZonesEtEmployes();
    rafraichirListe();
    on_pushButton_vider_clicked();
    on_pushButton_actualiserStats_clicked();

    naviguer(PageCampagnes, QString::fromUtf8("Gestion des campagnes de sensibilisation"));
}

CampagneWindow::~CampagneWindow()
{
    delete m_model;
    delete ui;
}

// ---------------------------------------------------------------- Navigation

void CampagneWindow::naviguer(int index, const QString &titre)
{
    ui->stackedWidget->setCurrentIndex(index);
    ui->label_pageTitle->setText(titre);
}

void CampagneWindow::on_pushButton_navDashboard_clicked() { naviguer(PageDashboard, "Tableau de bord"); }
void CampagneWindow::on_pushButton_navEmployes_clicked() { naviguer(PageEmployes, QString::fromUtf8("Gestion des employés")); }
void CampagneWindow::on_pushButton_navVehicules_clicked() { naviguer(PageVehicules, QString::fromUtf8("Gestion des véhicules")); }
void CampagneWindow::on_pushButton_navEquipements_clicked() { naviguer(PageEquipements, QString::fromUtf8("Gestion des équipements")); }
void CampagneWindow::on_pushButton_navInterventions_clicked() { naviguer(PageInterventions, "Gestion des interventions"); }
void CampagneWindow::on_pushButton_navCampagnes_clicked() { naviguer(PageCampagnes, QString::fromUtf8("Gestion des campagnes de sensibilisation")); }
void CampagneWindow::on_pushButton_navZones_clicked() { naviguer(PageZones, "Gestion des zones de couverture"); }

// ---------------------------------------------------------------- Chargements

void CampagneWindow::chargerZonesEtEmployes()
{
    ui->comboBox_zone->clear();
    QSqlQuery qz("SELECT ID_ZONE, NOM, NIVEAU_RISQUE FROM ZONE_COUVERTURE ORDER BY NOM");
    while (qz.next())
        ui->comboBox_zone->addItem(QString("%1 (%2)").arg(qz.value(1).toString(), qz.value(2).toString()),
                                   qz.value(0).toInt());

    ui->comboBox_animateur->clear();
    QSqlQuery qe("SELECT ID_EMPLOYE, PRENOM, NOM, GRADE FROM EMPLOYE ORDER BY NOM");
    while (qe.next())
        ui->comboBox_animateur->addItem(QString("%1 %2 — %3").arg(qe.value(1).toString(),
                                                                  qe.value(2).toString(),
                                                                  qe.value(3).toString()),
                                        qe.value(0).toInt());
}

void CampagneWindow::chargerListesCampagnes()
{
    const int idImpact = ui->comboBox_campagneImpact->currentData().toInt();
    const int idAffiche = ui->comboBox_afficheCampagne->currentData().toInt();
    ui->comboBox_campagneImpact->clear();
    ui->comboBox_afficheCampagne->clear();

    QSqlQuery q("SELECT c.ID_CAMPAGNE, c.TITRE, c.DATE_CAMPAGNE, z.NOM FROM CAMPAGNE c"
                " LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.ID_ZONE ORDER BY c.DATE_CAMPAGNE DESC");
    while (q.next()) {
        const QDate d = QDate::fromString(q.value(2).toString(), Qt::ISODate);
        const QString libelle = QString("#%1 %2 — %3 (%4)").arg(q.value(0).toString(), q.value(1).toString(),
                                                                q.value(3).toString(), d.toString("dd/MM/yyyy"));
        ui->comboBox_campagneImpact->addItem(libelle, q.value(0).toInt());
        ui->comboBox_afficheCampagne->addItem(libelle, q.value(0).toInt());
    }
    const int i1 = ui->comboBox_campagneImpact->findData(idImpact);
    if (i1 >= 0) ui->comboBox_campagneImpact->setCurrentIndex(i1);
    const int i2 = ui->comboBox_afficheCampagne->findData(idAffiche);
    if (i2 >= 0) ui->comboBox_afficheCampagne->setCurrentIndex(i2);
}

void CampagneWindow::rafraichirListe()
{
    static const char *criteres[] = {"THEME", "PUBLIC_CIBLE", "TITRE", "LIEU"};
    static const char *tris[] = {"DATE_CAMPAGNE DESC", "DATE_CAMPAGNE ASC",
                                 "NB_PARTICIPANTS DESC", "NB_PARTICIPANTS ASC"};

    const int ic = qBound(0, ui->comboBox_critere->currentIndex(), 3);
    const int it = qBound(0, ui->comboBox_tri->currentIndex(), 3);

    QSqlQueryModel *ancien = m_model;
    m_model = Campagne::afficher(criteres[ic], ui->lineEdit_recherche->text(), tris[it]);
    ui->tableView_campagnes->setModel(m_model);
    delete ancien;

    ui->tableView_campagnes->resizeColumnsToContents();
    ui->label_compteur->setText(QString("%1 campagne(s)").arg(m_model->rowCount()));
    chargerListesCampagnes();
}

// ---------------------------------------------------------------- CRUD

bool CampagneWindow::lireFormulaire(Campagne &c)
{
    c.setId(ui->lineEdit_id->text().toInt());
    c.setTitre(ui->lineEdit_titre->text());
    c.setTheme(ui->comboBox_theme->currentText());
    c.setDate(ui->dateEdit_date->date());
    c.setLieu(ui->lineEdit_lieu->text());
    c.setPublicCible(ui->comboBox_public->currentText());
    c.setNbParticipants(ui->spinBox_participants->value());
    c.setIdZone(ui->comboBox_zone->currentData().toInt());
    c.setIdEmploye(ui->comboBox_animateur->currentData().toInt());

    const QString err = c.valider();
    if (!err.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Saisie invalide"), err);
        return false;
    }
    return true;
}

void CampagneWindow::on_pushButton_ajouter_clicked()
{
    Campagne c;
    if (!lireFormulaire(c)) return;
    QString err;
    if (c.ajouter(&err)) {
        QMessageBox::information(this, "Ajout", QString::fromUtf8("Campagne ajoutée avec succès."));
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
        on_pushButton_vider_clicked();
    } else {
        QMessageBox::critical(this, "Ajout", QString::fromUtf8("Échec de l'ajout :\n") + err);
    }
}

void CampagneWindow::on_pushButton_modifier_clicked()
{
    Campagne c;
    if (!lireFormulaire(c)) return;
    QString err;
    if (c.modifier(&err)) {
        QMessageBox::information(this, "Modification", QString::fromUtf8("Campagne modifiée avec succès."));
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
    } else {
        QMessageBox::critical(this, "Modification", QString::fromUtf8("Échec de la modification :\n") + err);
    }
}

void CampagneWindow::on_pushButton_supprimer_clicked()
{
    const int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Suppression", QString::fromUtf8("Sélectionnez une campagne dans la liste."));
        return;
    }
    if (QMessageBox::question(this, "Suppression",
                              QString::fromUtf8("Supprimer définitivement la campagne #%1 ?").arg(id))
        != QMessageBox::Yes)
        return;

    QString err;
    if (Campagne::supprimer(id, &err)) {
        rafraichirListe();
        on_pushButton_actualiserStats_clicked();
        on_pushButton_vider_clicked();
    } else {
        QMessageBox::critical(this, "Suppression", QString::fromUtf8("Échec de la suppression :\n") + err);
    }
}

void CampagneWindow::on_pushButton_vider_clicked()
{
    ui->lineEdit_id->setText(QString::number(Campagne::prochainId()));
    ui->lineEdit_titre->clear();
    ui->comboBox_theme->setCurrentIndex(0);
    ui->dateEdit_date->setDate(QDate::currentDate());
    ui->lineEdit_lieu->clear();
    ui->comboBox_public->setCurrentIndex(0);
    ui->spinBox_participants->setValue(0);
    ui->tableView_campagnes->clearSelection();
    ui->lineEdit_titre->setFocus();
}

void CampagneWindow::on_tableView_campagnes_clicked(const QModelIndex &index)
{
    const int id = m_model->data(m_model->index(index.row(), 0)).toInt();
    Campagne c;
    if (!Campagne::charger(id, c)) return;

    ui->lineEdit_id->setText(QString::number(c.getId()));
    ui->lineEdit_titre->setText(c.getTitre());
    ui->comboBox_theme->setCurrentText(c.getTheme());
    ui->dateEdit_date->setDate(c.getDate());
    ui->lineEdit_lieu->setText(c.getLieu());
    ui->comboBox_public->setCurrentText(c.getPublicCible());
    ui->spinBox_participants->setValue(c.getNbParticipants());
    ui->comboBox_zone->setCurrentIndex(ui->comboBox_zone->findData(c.getIdZone()));
    ui->comboBox_animateur->setCurrentIndex(ui->comboBox_animateur->findData(c.getIdEmploye()));
}

// ---------------------------------------------------------------- Recherche / tri

void CampagneWindow::on_lineEdit_recherche_textChanged(const QString &) { rafraichirListe(); }
void CampagneWindow::on_comboBox_critere_currentIndexChanged(int) { rafraichirListe(); }
void CampagneWindow::on_comboBox_tri_currentIndexChanged(int) { rafraichirListe(); }

// ---------------------------------------------------------------- Export

void CampagneWindow::on_pushButton_exportPdf_clicked()
{
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter en PDF",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/campagnes_sensibilisation.pdf",
        "PDF (*.pdf)");
    if (chemin.isEmpty()) return;

    QString html = "<h2 style='color:#C62828'>FireStation Manager — Campagnes de sensibilisation</h2>"
                   "<p>Exporté le " + QDate::currentDate().toString("dd/MM/yyyy") +
                   " — " + QString::number(m_model->rowCount()) + " campagne(s)</p>"
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
    pdf.setTitle("Campagnes de sensibilisation");

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&pdf);

    QMessageBox::information(this, "Export PDF", QString::fromUtf8("Fichier enregistré :\n") + chemin);
}

void CampagneWindow::on_pushButton_exportExcel_clicked()
{
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Exporter pour Excel",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/campagnes_sensibilisation.csv",
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
    QMessageBox::information(this, "Export Excel", QString::fromUtf8("Fichier enregistré :\n") + chemin);
}

// ---------------------------------------------------------------- Statistiques

void CampagneWindow::on_pushButton_actualiserStats_clicked()
{
    const bool parTheme = ui->comboBox_statCritere->currentIndex() == 0;
    m_chart->setData(parTheme ? Campagne::statistiquesParTheme() : Campagne::statistiquesParZone(),
                     parTheme ? QString::fromUtf8("Nombre de campagnes par thème")
                              : QString::fromUtf8("Nombre de campagnes par zone"));

    QSqlQuery q("SELECT COUNT(*), COALESCE(SUM(NB_PARTICIPANTS), 0) FROM CAMPAGNE");
    if (q.next())
        ui->label_statTotal->setText(QString::fromUtf8("Total : %1 campagne(s) — %2 participant(s)")
                                         .arg(q.value(0).toInt()).arg(q.value(1).toInt()));
}

void CampagneWindow::on_comboBox_statCritere_currentIndexChanged(int)
{
    if (m_chart) on_pushButton_actualiserStats_clicked();
}

// ---------------------------------------------------------------- Métier innovant 1 : ciblage

void CampagneWindow::on_pushButton_lancerCiblage_clicked()
{
    const QList<CiblageZone> res = Campagne::ciblageAutomatique();
    QTableWidget *t = ui->tableWidget_ciblage;
    t->setRowCount(0);

    for (int r = 0; r < res.size(); ++r) {
        const CiblageZone &z = res.at(r);
        t->insertRow(r);
        auto *itemZone = new QTableWidgetItem(z.nom);
        itemZone->setData(Qt::UserRole, z.idZone);
        t->setItem(r, 0, itemZone);
        t->setItem(r, 1, new QTableWidgetItem(z.niveauRisque));
        t->setItem(r, 2, new QTableWidgetItem(QString::number(z.interventions12Mois)));
        t->setItem(r, 3, new QTableWidgetItem(QString::number(z.campagnes12Mois)));
        t->setItem(r, 4, new QTableWidgetItem(QString::number(z.score, 'f', 1)));
        auto *reco = new QTableWidgetItem(z.recommandation);
        if (z.recommandation.startsWith(QString::fromUtf8("Priorité haute")))
            reco->setForeground(QColor("#C62828"));
        else if (z.recommandation.startsWith(QString::fromUtf8("Priorité moyenne")))
            reco->setForeground(QColor("#FF6F00"));
        else
            reco->setForeground(QColor("#2E7D32"));
        t->setItem(r, 5, reco);
    }

    if (!res.isEmpty()) {
        ui->label_zonePrioritaire->setText(QString::fromUtf8("Zone prioritaire : %1 (score %2)")
                                               .arg(res.first().nom).arg(res.first().score, 0, 'f', 1));
        t->selectRow(0);
        ui->pushButton_planifierZone->setEnabled(true);
    }
}

void CampagneWindow::on_pushButton_planifierZone_clicked()
{
    const int ligne = qMax(0, ui->tableWidget_ciblage->currentRow());
    QTableWidgetItem *item = ui->tableWidget_ciblage->item(ligne, 0);
    if (!item) return;

    on_pushButton_vider_clicked();
    ui->comboBox_zone->setCurrentIndex(ui->comboBox_zone->findData(item->data(Qt::UserRole)));
    ui->tabWidget_campagnes->setCurrentWidget(ui->tab_gestion);
    ui->lineEdit_titre->setFocus();
}

// ---------------------------------------------------------------- Métier innovant 2 : impact

void CampagneWindow::on_pushButton_evaluerImpact_clicked()
{
    const int id = ui->comboBox_campagneImpact->currentData().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Impact", "Aucune campagne à évaluer.");
        return;
    }
    const int n = ui->spinBox_fenetre->value();
    const ImpactCampagne r = Campagne::evaluerImpact(id, n);
    if (!r.valide) {
        QMessageBox::warning(this, "Impact", "Impossible d'évaluer cette campagne.");
        return;
    }

    ui->label_impactAvant->setText(QString::fromUtf8("Interventions %1 mois avant : %2").arg(n).arg(r.avant));
    ui->label_impactApres->setText(QString::fromUtf8("Interventions %1 mois après : %2").arg(n).arg(r.apres));
    ui->label_impactVariation->setText(QString::fromUtf8("Variation : %1%2 %")
                                           .arg(r.variation > 0 ? "+" : "")
                                           .arg(r.variation, 0, 'f', 1));
    const QString couleur = r.variation < 0 ? "#2E7D32" : (r.variation > 0 ? "#C62828" : "#FF6F00");
    ui->label_impactVariation->setStyleSheet("font-weight:bold; color:" + couleur + ";");
    ui->label_impactVerdict->setStyleSheet("color:" + couleur + ";");
    ui->label_impactVerdict->setText(r.verdict);
}

// ---------------------------------------------------------------- Service complémentaire : affiche

QString CampagneWindow::sloganPourTheme(const QString &theme) const
{
    if (theme.contains("forêt")) return QString::fromUtf8("Une étincelle suffit. Protégeons nos forêts !");
    if (theme.contains("Premiers")) return QString::fromUtf8("Les bons gestes sauvent des vies.");
    if (theme.contains("domestiques")) return QString::fromUtf8("À la maison, la prudence commence par vous.");
    if (theme.contains("routière")) return QString::fromUtf8("Sur la route, chaque seconde compte.");
    if (theme.contains("naturels")) return QString::fromUtf8("Préparés aujourd'hui, protégés demain.");
    return QString::fromUtf8("Le feu ne prévient pas. Vous, si !");
}

QImage CampagneWindow::genererAffiche(int idCampagne, const QString &sloganSaisi) const
{
    Campagne c;
    if (!Campagne::charger(idCampagne, c)) return QImage();

    QString zone;
    QSqlQuery q;
    q.prepare("SELECT NOM, NIVEAU_RISQUE FROM ZONE_COUVERTURE WHERE ID_ZONE = :z");
    q.bindValue(":z", c.getIdZone());
    if (q.exec() && q.next()) zone = q.value(0).toString();

    const QString slogan = sloganSaisi.trimmed().isEmpty() ? sloganPourTheme(c.getTheme()) : sloganSaisi.trimmed();

    const int W = 900, H = 1200;
    QImage img(W, H, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    // Fond : dégradé rouge pompier → anthracite
    QLinearGradient g(0, 0, 0, H);
    g.setColorAt(0.0, QColor("#C62828"));
    g.setColorAt(0.65, QColor("#8E0000"));
    g.setColorAt(1.0, QColor("#2B2B2B"));
    p.fillRect(img.rect(), g);

    // Flammes stylisées (couleur selon le public)
    QColor accent("#FF6F00");
    if (c.getPublicCible().contains("coles")) accent = QColor("#FFB300");
    else if (c.getPublicCible() == "Entreprises") accent = QColor("#FF6F00");
    else accent = QColor("#FF8F00");
    p.setPen(Qt::NoPen);
    for (int i = 0; i < 9; ++i) {
        QPainterPath flamme;
        const int x = 40 + i * 100, base = H - 220;
        const int h = 120 + ((i * 37) % 90);
        flamme.moveTo(x, base);
        flamme.cubicTo(x - 30, base - h / 2, x + 20, base - h * 0.8, x + 10, base - h);
        flamme.cubicTo(x + 45, base - h * 0.7, x + 70, base - h / 3, x + 60, base);
        flamme.closeSubpath();
        QColor col = accent;
        col.setAlpha(110 + (i % 3) * 40);
        p.setBrush(col);
        p.drawPath(flamme);
    }

    // Logo circulaire
    p.setBrush(QColor("#FFFFFF"));
    p.drawEllipse(QPoint(W / 2, 150), 90, 90);
    p.setPen(QPen(QColor("#C62828"), 8));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPoint(W / 2, 150), 80, 80);
    QFont f("Helvetica", 22, QFont::Bold);
    p.setFont(f);
    p.setPen(QColor("#C62828"));
    p.drawText(QRect(W / 2 - 80, 110, 160, 80), Qt::AlignCenter, "FIRE\nSTATION");

    // Thème
    p.setPen(QColor("#FFFFFF"));
    f.setPointSize(26);
    p.setFont(f);
    p.drawText(QRect(40, 270, W - 80, 50), Qt::AlignCenter, c.getTheme().toUpper());

    // Titre
    f.setPointSize(54);
    p.setFont(f);
    p.drawText(QRect(40, 330, W - 80, 220), Qt::AlignCenter | Qt::TextWordWrap, c.getTitre());

    // Slogan
    f.setPointSize(30);
    f.setItalic(true);
    p.setFont(f);
    p.setPen(QColor("#FFE0B2"));
    p.drawText(QRect(60, 560, W - 120, 140), Qt::AlignCenter | Qt::TextWordWrap, "« " + slogan + " »");

    // Bandeau d'informations
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 235));
    p.drawRoundedRect(QRect(70, 730, W - 140, 200), 18, 18);
    f.setItalic(false);
    f.setPointSize(24);
    p.setFont(f);
    p.setPen(QColor("#2B2B2B"));
    const QString infos = QString::fromUtf8("Date : %1\nLieu : %2\nPublic : %3%4")
                              .arg(c.getDate().toString("dd/MM/yyyy"), c.getLieu(), c.getPublicCible(),
                                   zone.isEmpty() ? QString() : QString::fromUtf8(" — Zone : ") + zone);
    p.drawText(QRect(100, 745, W - 200, 170), Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, infos);

    // Pied
    f.setPointSize(20);
    p.setFont(f);
    p.setPen(QColor("#FFFFFF"));
    p.drawText(QRect(0, H - 90, W, 40), Qt::AlignCenter,
               QString::fromUtf8("Urgence : 198 · Protection civile — Ensemble pour sauver des vies"));
    p.end();
    return img;
}

void CampagneWindow::on_pushButton_genererAffiche_clicked()
{
    const int id = ui->comboBox_afficheCampagne->currentData().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Affiche", QString::fromUtf8("Choisissez une campagne."));
        return;
    }
    m_affiche = genererAffiche(id, ui->lineEdit_slogan->text());
    if (m_affiche.isNull()) return;

    ui->label_affichePreview->setPixmap(QPixmap::fromImage(m_affiche).scaled(
        ui->label_affichePreview->size() - QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->pushButton_enregistrerAffiche->setEnabled(true);
}

void CampagneWindow::on_pushButton_enregistrerAffiche_clicked()
{
    if (m_affiche.isNull()) return;
    const QString chemin = QFileDialog::getSaveFileName(
        this, "Enregistrer l'affiche",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/affiche_campagne.png",
        "Image PNG (*.png)");
    if (chemin.isEmpty()) return;
    if (m_affiche.save(chemin, "PNG"))
        QMessageBox::information(this, "Affiche", QString::fromUtf8("Affiche enregistrée :\n") + chemin);
    else
        QMessageBox::critical(this, "Affiche", "Impossible d'enregistrer l'image.");
}
