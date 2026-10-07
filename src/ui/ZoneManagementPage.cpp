#include "ZoneManagementPage.h"
#include "core/CoverageAnalyzer.h"
#include "core/Exporter.h"
#include "core/ZoneFilterProxy.h"
#include "core/ZoneRepository.h"
#include "core/ZoneTableModel.h"
#include "ui/ZoneDialog.h"
#include "widgets/AlertPanel.h"
#include "widgets/KpiCard.h"
#include "widgets/StatsChartWidget.h"
#include "widgets/ZoneDetailPanel.h"

#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QSettings>
#include <QSplitter>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

ZoneManagementPage::ZoneManagementPage(ZoneRepository* repo, QWidget* parent)
    : QWidget(parent), m_repo(repo)
{
    m_model = new ZoneTableModel(repo, this);   // créé AVANT la connexion à refresh()
    m_proxy = new ZoneFilterProxy(this);
    m_proxy->setSourceModel(m_model);

    buildUi();

    QSettings s;
    double th = s.value("alert/threshold", 12.0).toDouble();
    m_alerts->setThreshold(th);
    m_model->setThreshold(th);

    connect(repo, &ZoneRepository::changed, this, &ZoneManagementPage::refresh);

    QString bg = s.value("map/background").toString();
    if (!bg.isEmpty()) {
        QImage img(bg);
        if (!img.isNull()) m_map->setBackgroundImage(img);
    }
    refresh();
}

void ZoneManagementPage::buildUi()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 12, 16, 12);
    root->setSpacing(10);

    // ---- Ligne 1 : recherche + filtres + tri ----
    auto* r1 = new QHBoxLayout;
    m_search = new QLineEdit;
    m_search->setPlaceholderText(QString::fromUtf8("🔍  Rechercher par nom ou région…"));
    m_search->setClearButtonEnabled(true);
    m_search->setMinimumWidth(260);
    r1->addWidget(m_search, 2);

    r1->addWidget(new QLabel("Niveau de risque :"));
    m_riskFilter = new QComboBox;
    m_riskFilter->addItem("Tous", -1);
    m_riskFilter->addItem(RiskUtil::label(RiskLevel::Eleve), int(RiskLevel::Eleve));
    m_riskFilter->addItem(RiskUtil::label(RiskLevel::Moyen), int(RiskLevel::Moyen));
    m_riskFilter->addItem(RiskUtil::label(RiskLevel::Faible), int(RiskLevel::Faible));
    r1->addWidget(m_riskFilter);

    r1->addWidget(new QLabel("Trier par :"));
    m_sortCombo = new QComboBox;
    m_sortCombo->addItem("ID zone", int(ZoneTableModel::ColId));
    m_sortCombo->addItem("Nom", int(ZoneTableModel::ColNom));
    m_sortCombo->addItem(QString::fromUtf8("Niveau de risque (↓)"), int(ZoneTableModel::ColRisque));
    m_sortCombo->addItem(QString::fromUtf8("Temps d'intervention (↓)"), int(ZoneTableModel::ColTemps));
    m_sortCombo->addItem(QString::fromUtf8("Population (↓)"), int(ZoneTableModel::ColPopulation));
    r1->addWidget(m_sortCombo);
    root->addLayout(r1);

    // ---- Ligne 2 : actions ----
    auto* r2 = new QHBoxLayout;
    auto mk = [&](const QString& txt, const char* name) {
        auto* b = new QPushButton(txt);
        b->setObjectName(name);
        b->setCursor(Qt::PointingHandCursor);
        r2->addWidget(b);
        return b;
    };
    m_btnAdd = mk(QString::fromUtf8("＋ Nouvelle zone"), "primary");
    m_btnPlace = mk(QString::fromUtf8("📍 Placer sur la carte"), "tool");
    m_btnPlace->setCheckable(true);
    m_btnMove = mk(QString::fromUtf8("✥ Déplacer"), "tool");
    m_btnMove->setCheckable(true);
    m_btnEdit = mk(QString::fromUtf8("✎ Modifier"), "tool");
    m_btnDelete = mk(QString::fromUtf8("🗑 Supprimer"), "danger");
    m_btnInterv = mk(QString::fromUtf8("⏱ Enregistrer une intervention"), "tool");
    r2->addStretch();
    auto* bPdf = mk(QString::fromUtf8("📄 PDF"), "tool");
    auto* bXls = mk(QString::fromUtf8("📊 Excel"), "tool");
    auto* bBg = mk(QString::fromUtf8("🗺 Fond de carte"), "tool");
    auto* menu = new QMenu(bBg);
    menu->addAction(QString::fromUtf8("Charger une image…"), this, &ZoneManagementPage::loadBackground);
    menu->addAction(QString::fromUtf8("Revenir à la carte schématique"), this, &ZoneManagementPage::clearBackground);
    bBg->setMenu(menu);
    root->addLayout(r2);

    // ---- KPI ----
    auto* kp = new QHBoxLayout;
    m_kZones = new KpiCard("ZONES", QColor("#6366f1"));
    m_kRisk = new KpiCard(QString::fromUtf8("RISQUE ÉLEVÉ"), QColor("#D32F2F"));
    m_kPop = new KpiCard("POPULATION COUVERTE", QColor("#0ea5e9"));
    m_kTime = new KpiCard("TEMPS MOYEN GLOBAL", QColor("#F59E0B"));
    m_kAlert = new KpiCard("ZONES SOUS-COUVERTES", QColor("#B91C1C"));
    for (auto* k : {m_kZones, m_kRisk, m_kPop, m_kTime, m_kAlert}) kp->addWidget(k, 1);
    root->addLayout(kp);

    // ---- Corps : (carte / tableau+stats) | (alertes / détails) ----
    m_map = new ZoneMapWidget;
    m_table = new QTableView;
    m_table->setModel(m_proxy);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->setSortingEnabled(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(30);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(ZoneTableModel::ColNom, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(ZoneTableModel::ColRegion, QHeaderView::Stretch);
    for (int c : {int(ZoneTableModel::ColId), int(ZoneTableModel::ColSuperficie), int(ZoneTableModel::ColPopulation),
                  int(ZoneTableModel::ColRisque), int(ZoneTableModel::ColTemps), int(ZoneTableModel::ColInterventions)})
        m_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    m_table->sortByColumn(ZoneTableModel::ColId, Qt::AscendingOrder);

    m_chart = new StatsChartWidget;
    auto* tabs = new QTabWidget;
    tabs->addTab(m_table, QString::fromUtf8("Liste des zones"));
    tabs->addTab(m_chart, QString::fromUtf8("Statistiques"));

    auto* left = new QSplitter(Qt::Vertical);
    left->addWidget(m_map);
    left->addWidget(tabs);
    left->setStretchFactor(0, 3);
    left->setStretchFactor(1, 2);

    m_alerts = new AlertPanel;
    m_detail = new ZoneDetailPanel;
    auto* right = new QSplitter(Qt::Vertical);
    right->addWidget(m_alerts);
    right->addWidget(m_detail);
    right->setStretchFactor(0, 1);
    right->setStretchFactor(1, 1);
    right->setMinimumWidth(340);
    right->setMaximumWidth(400);

    auto* body = new QHBoxLayout;
    body->addWidget(left, 1);
    body->addWidget(right);
    root->addLayout(body, 1);

    m_status = new QLabel;
    m_status->setStyleSheet("color:#6b7280;");
    root->addWidget(m_status);

    // ---- Connexions ----
    connect(m_search, &QLineEdit::textChanged, this, &ZoneManagementPage::onFiltersChanged);
    connect(m_riskFilter, qOverload<int>(&QComboBox::currentIndexChanged), this, &ZoneManagementPage::onFiltersChanged);
    connect(m_sortCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) {
        int col = m_sortCombo->currentData().toInt();
        bool desc = (col == ZoneTableModel::ColRisque || col == ZoneTableModel::ColTemps
                     || col == ZoneTableModel::ColPopulation);
        m_table->sortByColumn(col, desc ? Qt::DescendingOrder : Qt::AscendingOrder);
    });

    connect(m_btnAdd, &QPushButton::clicked, this, &ZoneManagementPage::addZone);
    connect(m_btnEdit, &QPushButton::clicked, this, &ZoneManagementPage::editSelected);
    connect(m_btnDelete, &QPushButton::clicked, this, &ZoneManagementPage::deleteSelected);
    connect(m_btnInterv, &QPushButton::clicked, this, &ZoneManagementPage::recordIntervention);
    connect(bPdf, &QPushButton::clicked, this, &ZoneManagementPage::exportPdf);
    connect(bXls, &QPushButton::clicked, this, &ZoneManagementPage::exportExcel);

    connect(m_btnPlace, &QPushButton::toggled, this, [this](bool on) {
        if (on) m_btnMove->setChecked(false);
        m_map->setMode(on ? ZoneMapWidget::Mode::AddZone
                          : (m_btnMove->isChecked() ? ZoneMapWidget::Mode::MoveZone : ZoneMapWidget::Mode::Navigate));
    });
    connect(m_btnMove, &QPushButton::toggled, this, [this](bool on) {
        if (on) m_btnPlace->setChecked(false);
        m_map->setMode(on ? ZoneMapWidget::Mode::MoveZone
                          : (m_btnPlace->isChecked() ? ZoneMapWidget::Mode::AddZone : ZoneMapWidget::Mode::Navigate));
    });
    connect(m_map, &ZoneMapWidget::modeChanged, this, &ZoneManagementPage::onMapModeChanged);

    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZoneManagementPage::onTableSelectionChanged);
    connect(m_table, &QTableView::doubleClicked, this, [this] { editSelected(); });

    connect(m_map, &ZoneMapWidget::zoneSelected, this, &ZoneManagementPage::selectZone);
    connect(m_map, &ZoneMapWidget::zoneActivated, this, [this](int id) { selectZone(id); editSelected(); });
    connect(m_map, &ZoneMapWidget::locationPicked, this, &ZoneManagementPage::addZoneAt);
    connect(m_map, &ZoneMapWidget::zoneMoved, this, [this](int id, double lat, double lon) {
        if (const Zone* z = m_repo->find(id)) {
            Zone c = *z;
            c.latitude = lat;
            c.longitude = lon;
            m_repo->update(c);
        }
    });

    connect(m_alerts, &AlertPanel::zoneClicked, this, &ZoneManagementPage::selectZone);
    connect(m_alerts, &AlertPanel::thresholdChanged, this, [this](double v) {
        QSettings().setValue("alert/threshold", v);
        m_model->setThreshold(v);
        refresh();
    });
}

// ---------- rafraîchissement global ----------
QVector<Zone> ZoneManagementPage::visibleZones() const
{
    QVector<Zone> v;
    for (int r = 0; r < m_proxy->rowCount(); ++r) {
        QModelIndex src = m_proxy->mapToSource(m_proxy->index(r, 0));
        v.append(m_model->zoneAt(src.row()));
    }
    return v;
}

void ZoneManagementPage::refresh()
{
    m_syncing = true;
    const QVector<Zone>& all = m_repo->zones();
    const QVector<Zone> shown = visibleZones();
    const double th = m_alerts->threshold();
    QLocale fr(QLocale::French);

    // carte
    QSet<int> vis;
    for (const Zone& z : shown) vis.insert(z.id);
    QSet<int> alerts;
    for (int id : CoverageAnalyzer::underCoveredIds(shown, th)) alerts.insert(id);
    m_map->setZones(all);
    m_map->setVisibleIds(vis);
    m_map->setAlertIds(alerts);

    // alertes
    QVector<Zone> under;
    for (const Zone& z : shown)
        if (CoverageAnalyzer::isUnderCovered(z, th)) under.append(z);
    std::sort(under.begin(), under.end(), [](const Zone& a, const Zone& b) { return a.tempsMoyen > b.tempsMoyen; });
    m_alerts->setAlerts(under, th);

    // KPI + stats (sur les zones affichées)
    m_kZones->setValue(QString::number(shown.size()), QString("sur %1").arg(all.size()));
    int high = 0;
    for (const Zone& z : shown) if (z.risque == RiskLevel::Eleve) high++;
    m_kRisk->setValue(QString::number(high));
    m_kPop->setValue(fr.toString(CoverageAnalyzer::totalPopulation(shown)));
    m_kTime->setValue(QString("%1 min").arg(fr.toString(CoverageAnalyzer::averageResponse(shown), 'f', 1)),
                      QString::fromUtf8("seuil : %1 min").arg(fr.toString(th, 'f', 1)));
    m_kAlert->setValue(QString::number(under.size()));
    m_chart->setZones(shown);

    // sélection
    if (!m_repo->find(m_selectedId)) m_selectedId = -1;
    m_map->setSelectedId(m_selectedId);
    m_table->clearSelection();
    if (m_selectedId >= 0) {
        for (int r = 0; r < m_proxy->rowCount(); ++r) {
            QModelIndex src = m_proxy->mapToSource(m_proxy->index(r, 0));
            if (m_model->zoneAt(src.row()).id == m_selectedId) { m_table->selectRow(r); break; }
        }
    }
    m_detail->setZone(m_repo->find(m_selectedId), th);
    m_status->setText(QString::fromUtf8("%1 zone(s) affichée(s) sur %2  •  Molette : zoom  •  Glisser : déplacer la carte  •  Double-clic : modifier")
                          .arg(shown.size()).arg(all.size()));
    m_syncing = false;
    updateActions();
}

void ZoneManagementPage::onFiltersChanged()
{
    m_proxy->setSearchText(m_search->text());
    m_proxy->setRiskFilter(m_riskFilter->currentData().toInt());
    refresh();
}

// ---------- sélection ----------
void ZoneManagementPage::selectZone(int id)
{
    if (m_syncing) return;
    m_selectedId = id;
    refresh();
    // faire défiler le tableau sur la ligne sélectionnée
    QModelIndexList sel = m_table->selectionModel()->selectedRows();
    if (!sel.isEmpty()) m_table->scrollTo(sel.first());
}

void ZoneManagementPage::onTableSelectionChanged()
{
    if (m_syncing) return;
    QModelIndexList sel = m_table->selectionModel()->selectedRows();
    if (sel.isEmpty()) return;
    QModelIndex src = m_proxy->mapToSource(sel.first());
    int id = m_model->zoneAt(src.row()).id;
    if (id == m_selectedId) return;
    m_selectedId = id;
    m_syncing = true;
    m_map->setSelectedId(id);
    m_detail->setZone(m_repo->find(id), m_alerts->threshold());
    m_syncing = false;
    updateActions();
}

void ZoneManagementPage::updateActions()
{
    bool has = m_selectedId >= 0;
    m_btnEdit->setEnabled(has);
    m_btnDelete->setEnabled(has);
    m_btnInterv->setEnabled(has);
}

void ZoneManagementPage::onMapModeChanged(ZoneMapWidget::Mode mode)
{
    QSignalBlocker b1(m_btnPlace), b2(m_btnMove);
    m_btnPlace->setChecked(mode == ZoneMapWidget::Mode::AddZone);
    m_btnMove->setChecked(mode == ZoneMapWidget::Mode::MoveZone);
}

// ---------- CRUD ----------
void ZoneManagementPage::openDialog(Zone z, bool isNew)
{
    ZoneDialog dlg(this);
    dlg.setZone(z, isNew);
    if (dlg.exec() != QDialog::Accepted) return;
    Zone out = dlg.zone();
    if (isNew) m_selectedId = m_repo->add(out);
    else m_repo->update(out);
}

void ZoneManagementPage::addZone()
{
    Zone z;
    m_map->centerLatLon(z.latitude, z.longitude);
    z.nom.clear();
    openDialog(z, true);
}

void ZoneManagementPage::addZoneAt(double lat, double lon)
{
    m_map->setMode(ZoneMapWidget::Mode::Navigate);
    Zone z;
    z.latitude = lat;
    z.longitude = lon;
    openDialog(z, true);
}

void ZoneManagementPage::editSelected()
{
    if (const Zone* z = m_repo->find(m_selectedId)) openDialog(*z, false);
}

void ZoneManagementPage::deleteSelected()
{
    const Zone* z = m_repo->find(m_selectedId);
    if (!z) return;
    auto r = QMessageBox::question(this, "Supprimer la zone",
        QString::fromUtf8("Supprimer définitivement la zone « %1 » (%2) ?").arg(z->nom, z->idLabel()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (r != QMessageBox::Yes) return;
    int id = m_selectedId;
    m_selectedId = -1;
    m_repo->remove(id);
}

void ZoneManagementPage::recordIntervention()
{
    const Zone* z = m_repo->find(m_selectedId);
    if (!z) return;
    bool ok = false;
    double min = QInputDialog::getDouble(this, QString::fromUtf8("Enregistrer une intervention"),
        QString::fromUtf8("Durée de l'intervention dans « %1 » (minutes) :").arg(z->nom),
        10.0, 0.5, 600.0, 1, &ok);
    if (ok) m_repo->recordIntervention(m_selectedId, min);
}

// ---------- exports ----------
void ZoneManagementPage::exportPdf()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter en PDF", "zones_couverture.pdf", "PDF (*.pdf)");
    if (path.isEmpty()) return;
    if (!path.endsWith(".pdf", Qt::CaseInsensitive)) path += ".pdf";
    if (Exporter::toPdf(path, visibleZones(), m_alerts->threshold()))
        QMessageBox::information(this, "Export PDF", QString::fromUtf8("PDF enregistré :\n%1").arg(path));
    else
        QMessageBox::warning(this, "Export PDF", QString::fromUtf8("Impossible d'écrire le fichier."));
}

void ZoneManagementPage::exportExcel()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter pour Excel", "zones_couverture.csv",
                                                "Excel / CSV (*.csv)");
    if (path.isEmpty()) return;
    if (!path.endsWith(".csv", Qt::CaseInsensitive)) path += ".csv";
    if (Exporter::toCsv(path, visibleZones()))
        QMessageBox::information(this, "Export Excel", QString::fromUtf8("Fichier enregistré :\n%1").arg(path));
    else
        QMessageBox::warning(this, "Export Excel", QString::fromUtf8("Impossible d'écrire le fichier."));
}

// ---------- fond de carte ----------
void ZoneManagementPage::loadBackground()
{
    QString path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Choisir une image de fond de carte"),
                                                QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (path.isEmpty()) return;
    QImage img(path);
    if (img.isNull()) {
        QMessageBox::warning(this, "Fond de carte", QString::fromUtf8("Image illisible."));
        return;
    }
    m_map->setBackgroundImage(img);
    QSettings().setValue("map/background", path);
}

void ZoneManagementPage::clearBackground()
{
    m_map->setBackgroundImage(QImage());
    QSettings().remove("map/background");
}
