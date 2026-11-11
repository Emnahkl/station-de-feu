#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QActionGroup>
#include <QDate>
#include <QDesktopServices>
#include <QEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLinearGradient>
#include <QMenu>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QScrollBar>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStyle>
#include <QTextDocument>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <algorithm>
#include <utility>
#include <QCalendarWidget>
#include <QCamera>
#include <QCloseEvent>
#include <QComboBox>
#include <QGridLayout>
#include <QListWidget>
#include <QProgressBar>
#include <QScrollArea>
#include <QTextCharFormat>
#include <QVBoxLayout>
#include <cmath>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QCameraDevice>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QVideoFrame>
#include <QVideoSink>
#else
#include <QCameraInfo>
#endif

// ---------------------------------------------------------------------------
//  Fonctions utilitaires
// ---------------------------------------------------------------------------
namespace {

// Dessine sidebar.png en fond du sidebar (ancre en bas, recadre) + voile rouge en haut
class SidebarPainter : public QObject
{
public:
    SidebarPainter(const QPixmap &px, QObject *parent) : QObject(parent), m_px(px) {}
    bool eventFilter(QObject *obj, QEvent *ev) override
    {
        if (ev->type() == QEvent::Paint) {
            QWidget *w = static_cast<QWidget *>(obj);
            QPainter p(w);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            p.fillRect(w->rect(), QColor("#7a0f0f"));
            if (!m_px.isNull()) {
                QPixmap s = m_px.scaled(w->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                p.drawPixmap((w->width() - s.width()) / 2, w->height() - s.height(), s);
            }
            QLinearGradient g(0, 0, 0, w->height());
            g.setColorAt(0.0, QColor(176, 28, 28, 245));
            g.setColorAt(0.55, QColor(176, 28, 28, 120));
            g.setColorAt(1.0, QColor(0, 0, 0, 0));
            p.fillRect(w->rect(), g);
        }
        return false;
    }
private:
    QPixmap m_px;
};

const QStringList kSpecs   = {"Fire", "Medic", "Logistics"};
const QStringList kStatuts = {"En service", "En pause", "En formation"};
const QStringList kGardes  = {"Garde de jour (08h-20h)", "Garde de nuit (20h-08h)",
                              "Astreinte week-end", "Aucune"};

QPixmap loadPx(const QString &name) { return QPixmap(QString(":/images/%1.png").arg(name)); }

QPixmap tinted(const QPixmap &src, const QColor &c)
{
    QPixmap out(src.size());
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.drawPixmap(0, 0, src);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(out.rect(), c);
    return out;
}

QIcon glyphIcon(const QString &glyph, const QColor &c, int size = 22)
{
    QPixmap pm(size, size);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setPen(c);
    QFont f = p.font();
    f.setPixelSize(int(size * 0.8));
    p.setFont(f);
    p.drawText(pm.rect(), Qt::AlignCenter, glyph);
    return QIcon(pm);
}

// Avatar rond (pompier stylise) dessine par code : pas d'image externe necessaire
QPixmap makeAvatar(int s)
{
    QPixmap pm(s, s);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath clip;
    clip.addEllipse(0, 0, s, s);
    p.setClipPath(clip);
    p.fillRect(0, 0, s, s, QColor("#cfd8e0"));
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#2f3b4a"));
    p.drawEllipse(QRectF(s * 0.12, s * 0.64, s * 0.76, s * 0.7));
    p.setBrush(QColor("#e3a982"));
    p.drawEllipse(QRectF(s * 0.3, s * 0.3, s * 0.4, s * 0.44));
    p.setBrush(QColor("#b3261e"));
    QPainterPath h;
    h.moveTo(s * 0.24, s * 0.4);
    h.arcTo(QRectF(s * 0.24, s * 0.1, s * 0.52, s * 0.6), 180, -180);
    h.closeSubpath();
    p.drawPath(h);
    p.drawRect(QRectF(s * 0.2, s * 0.38, s * 0.6, s * 0.07));
    return pm;
}

// Photo recadree en cercle
QPixmap roundPhoto(const QPixmap &src, int s)
{
    QPixmap scaled = src.scaled(s, s, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap out(s, s);
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath clip;
    clip.addEllipse(0, 0, s, s);
    p.setClipPath(clip);
    p.drawPixmap((s - scaled.width()) / 2, (s - scaled.height()) / 2, scaled);
    return out;
}

QWidget *centered(QWidget *w)
{
    auto *box = new QWidget;
    auto *l = new QHBoxLayout(box);
    l->setContentsMargins(2, 2, 2, 2);
    l->addWidget(w, 0, Qt::AlignCenter);
    return box;
}

QString statusStyle(const QString &statut)
{
    QString bg = "#b9e8c0", fg = "#1b6b2b";
    if (statut == "En pause")      { bg = "#f7dc7f"; fg = "#7a5b00"; }
    if (statut == "En formation")  { bg = "#cfe3f5"; fg = "#1d4f7c"; }
    return QString("background:%1;color:%2;border-radius:12px;padding:3px 12px;font-size:12px;").arg(bg, fg);
}

} // namespace

// ---------------------------------------------------------------------------
//  Construction
// ---------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_panel = new QStackedWidget;               // pages integrees : statistiques, calendrier
    m_panel->setVisible(false);
    ui->leftLayout->addWidget(m_panel, 1);
    setWindowTitle("Fire Station – Gestion des employés");
    setMinimumSize(1280, 720);
    resize(1366, 768);

    loadAssets();
    setupStyle();
    setupSidebar();
    setupTopBar();
    setupFilters();
    setupActionButtons();
    setupForm();
    setupTable();
    setupChat();
    setupRightPanels();

    reloadFromDb();          // lit la table EMPLOYE d'Oracle
    refreshTable();
    updatePlanning();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadAssets()
{
    m_logo    = loadPx("logo");
    m_sidebar = loadPx("sidebar");
    m_chatbot = loadPx("chatboot");
    m_avatar  = makeAvatar(80);
    for (int g = 1; g <= 3; ++g)
        m_gradeBtn[g] = loadPx(QString("%1etoile").arg(g));
    m_specPx[0] = loadPx("fire");
    m_specPx[1] = loadPx("medic");
    m_specPx[2] = loadPx("logistc");
}

void MainWindow::setupStyle()
{
    setStyleSheet(R"(
QMainWindow, #contentWidget, #bodyWidget { background:#eef0f2; }
QLabel { background:transparent; color:#37474f; }

/* barre du haut */
#topBar { background:#ffffff; border-bottom:1px solid #d5d8dc; }
#lineEditSearch { background:#eceff1; border:1px solid #d5d8dc; border-radius:6px; padding:4px 10px; }
#labelUserTop { font-size:11px; font-weight:bold; color:#37474f; }
#btnHamburger, #btnPower { border:none; font-size:20px; color:#455a64; background:transparent; padding:4px 8px; }
#btnAlerte, #btnTopParam { border:none; background:transparent; }

/* sidebar */
#sidebar QPushButton { color:white; text-align:left; padding:10px 14px; border:none;
                       border-left:4px solid transparent; font-size:13px; background:transparent; }
#sidebar QPushButton:hover { background:rgba(255,255,255,0.14); }
#sidebar QPushButton:checked { background:rgba(0,0,0,0.38); border-left:4px solid white; font-weight:bold; }

/* panneaux */
#filterFrame, #actionFrame, #actionFrame2, #formPanel, #chatFrame, #planFrame
    { background:white; border:1px solid #d9dde1; border-radius:8px; }
#labelChatTitle, #labelPlanTitle, #labelFormTitle
    { font-weight:bold; font-size:14px; color:#37474f; }
#labelFormInfo { color:#455a64; font-size:12px; }
#labelCount { color:#607d8b; font-size:12px; font-weight:bold; padding-right:8px; }
QLineEdit, QComboBox { border:1px solid #cfd4d9; border-radius:6px; padding:3px 8px; background:white; color:#263238; }
QLineEdit[invalide="true"] { border:1px solid #c62828; background:#fff5f5; }

/* boutons-images */
#btnFaceId, #btnExporterPdf, #btnAjouterEmploye, #btnSupprimerEmploye, #btnStatistiques, #btnTrier
    { border:none; background:transparent; }
#btnFaceId:hover, #btnExporterPdf:hover, #btnAjouterEmploye:hover, #btnSupprimerEmploye:hover,
#btnStatistiques:hover, #btnTrier:hover
    { background:rgba(0,0,0,0.07); border-radius:8px; }

/* boutons du formulaire / panneaux */
#btnSendChat, #btnFormValider, #btnGenererPlanning
    { background:#2f3e4e; color:white; border:none; border-radius:6px; padding:7px 16px; font-weight:bold; }
#btnSendChat:hover, #btnFormValider:hover, #btnGenererPlanning:hover { background:#3d5166; }
#btnCalendrier { background:#2f3e4e; color:white; border:none; border-radius:6px; padding:6px 12px; font-weight:bold; }
#btnCalendrier:hover { background:#3d5166; }
#btnFormValider:disabled, #btnSendChat:disabled { background:#9aa5b1; }
#btnVueSql { background:#e9ecef; color:#37474f; border:1px solid #cfd4d9; border-radius:6px; padding:4px 10px; font-weight:bold; }
#btnVueSql:checked { background:#2f3e4e; color:white; }
#btnFormAnnuler, #btnFormPhoto { background:#d9dcdf; color:#37474f; border:none; border-radius:6px; padding:7px 16px; }
#btnFormAnnuler:hover, #btnFormPhoto:hover { background:#c8ccd0; }
#btnNotifier  { background:#dbe8f5; color:#1d4f7c; border:none; border-radius:5px; padding:8px 4px; font-size:11px; }
#btnAlerteGrade { background:#fbe0dc; color:#b3261e; border:none; border-radius:5px; padding:8px 4px; font-size:11px; }
#labelDispo, #labelProgress, #labelUnite, #labelP1, #labelP2, #labelP3 { font-size:11px; }

/* tableau */
QTableWidget { background:white; alternate-background-color:#f6f7f8; border:1px solid #d9dde1; border-radius:8px;
               selection-background-color:#fde8e8; selection-color:#263238; outline:0; }
QTableWidget::item { padding-left:8px; color:#263238; font-weight:bold; }
QHeaderView::section { background:#2f3e4e; color:white; font-weight:bold; padding:9px 4px; border:none;
                       border-right:1px solid #44566b; }
#rowBtn { background:#d9dcdf; border:none; border-radius:4px; padding:4px 7px; font-size:11px; color:#37474f; }
#rowBtn:hover { background:#c3c8cc; }

/* chat */
#chatScroll, #chatContent { background:transparent; }
#bubbleBot  { background:#eceff1; border-radius:12px; padding:8px 10px; color:#263238; }
#bubbleUser { background:#3b6fb6; border-radius:12px; padding:8px 10px; color:white; }

QStatusBar { background:white; color:#455a64; }
)");
}

void MainWindow::setupSidebar()
{
    ui->sidebar->installEventFilter(new SidebarPainter(m_sidebar, ui->sidebar));
    ui->labelLogo->setPixmap(m_logo.scaledToWidth(120, Qt::SmoothTransformation));

    struct Item { QPushButton *b; QString text; QString glyph; };
    const QVector<Item> items = {
        {ui->btnMenuDashboard,     "Tableau de bord",               "▦"},
        {ui->btnMenuInterventions, "Interventions",                 "⚠"},
        {ui->btnMenuEquipe,        "Équipe",                        "☻"},
        {ui->btnMenuVehicules,     "Véhicules",                     "▣"},
        {ui->btnMenuCarte,         "Carte",                         "◈"},
        {ui->btnMenuCampagne,      "Campagne de\nSensibilisation",  "✉"},
        {ui->btnMenuVolontaires,   "Volontaires",                   "★"},
    };
    for (const Item &it : items) {
        it.b->setText(it.text);
        it.b->setIcon(glyphIcon(it.glyph, Qt::white));
        it.b->setIconSize(QSize(22, 22));
    }
    ui->btnMenuParametres->setText("Paramètres");
    ui->btnMenuParametres->setIcon(QIcon(tinted(loadPx("parametre").scaled(44, 44, Qt::KeepAspectRatio, Qt::SmoothTransformation), Qt::white)));
    ui->btnMenuParametres->setIconSize(QSize(22, 22));

    const QList<QPushButton *> all = {ui->btnMenuDashboard, ui->btnMenuInterventions, ui->btnMenuEquipe,
                                      ui->btnMenuVehicules, ui->btnMenuCarte, ui->btnMenuCampagne,
                                      ui->btnMenuVolontaires, ui->btnMenuParametres};
    for (QPushButton *b : all) {
        b->setCursor(Qt::PointingHandCursor);
        if (b == ui->btnMenuInterventions) {          // integration : ouvre le module Interventions
            connect(b, &QPushButton::clicked, this, &MainWindow::ouvrirInterventions);
            continue;
        }
        connect(b, &QPushButton::clicked, this, [this, b] {
            QString t = b->text();
            t.replace('\n', ' ');
            statusBar()->showMessage(QString("Module « %1 » : hors de cette tâche (gestion des employés uniquement).").arg(t), 4000);
        });
    }
    ui->btnMenuEquipe->setChecked(true);   // le module gere ici = Equipe (employes)
}

void MainWindow::marquerMenu()
{
    ui->btnMenuEquipe->setChecked(true);
}

void MainWindow::setupTopBar()
{
    ui->lineEditSearch->setClearButtonEnabled(true);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, [this] { refreshTable(); });

    ui->btnAlerte->setIcon(QIcon(loadPx("alerte").scaledToHeight(44, Qt::SmoothTransformation)));
    ui->btnAlerte->setIconSize(QSize(22, 26));
    ui->btnAlerte->setFixedSize(38, 38);
    ui->btnAlerte->setCursor(Qt::PointingHandCursor);

    m_badge = new QLabel("0", ui->btnAlerte);
    m_badge->setFixedSize(17, 17);
    m_badge->setAlignment(Qt::AlignCenter);
    m_badge->setStyleSheet("background:#e53935;color:white;border-radius:8px;font-size:10px;font-weight:bold;");
    m_badge->move(21, 1);
    m_badge->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->btnTopParam->setIcon(QIcon(tinted(loadPx("parametre").scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation), QColor("#546e7a"))));
    ui->btnTopParam->setIconSize(QSize(24, 24));
    ui->btnTopParam->setFixedSize(38, 38);
    ui->btnTopParam->setCursor(Qt::PointingHandCursor);

    ui->labelAvatarTop->setPixmap(makeAvatar(72).scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->labelAvatarTop->setFixedSize(36, 36);

    connect(ui->btnHamburger, &QPushButton::clicked, this, [this] { ui->sidebar->setVisible(!ui->sidebar->isVisible()); });
    connect(ui->btnPower, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btnAlerte, &QPushButton::clicked, this, [this] {
        int n = 0;
        for (const Employe &e : std::as_const(m_employees)) if (e.getStatut() == "En formation") ++n;
        statusBar()->showMessage(QString("%1 alerte(s) : agents en formation (fin de formation / aptitude à surveiller).").arg(n), 5000);
    });
    connect(ui->btnTopParam, &QPushButton::clicked, this, [this] {
        statusBar()->showMessage("Paramètres : hors de cette tâche.", 3000);
    });
}

void MainWindow::setupFilters()
{
    ui->comboGrade->setIconSize(QSize(66, 22));
    ui->comboGrade->addItem("Tous les grades", 0);
    for (int g = 1; g <= 3; ++g)
        ui->comboGrade->addItem(QIcon(m_gradeBtn[g].scaledToHeight(44, Qt::SmoothTransformation)),
                                QString("%1 étoile%2").arg(g).arg(g > 1 ? "s" : ""), g);

    ui->comboSpec->setIconSize(QSize(20, 20));
    ui->comboSpec->addItem("Toutes les spécialités", QString());
    for (int i = 0; i < 3; ++i)
        ui->comboSpec->addItem(QIcon(m_specPx[i].scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)),
                               kSpecs[i], kSpecs[i]);

    connect(ui->comboGrade, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { refreshTable(); });
    connect(ui->comboSpec,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { refreshTable(); });
}

void MainWindow::setupActionButtons()
{
    auto setImg = [](QPushButton *b, const QPixmap &p, const QString &tip) {
        const int h = 52;
        const QSize sz(qRound(double(h) * p.width() / p.height()), h);
        b->setIcon(QIcon(p.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        b->setIconSize(sz);
        b->setFixedSize(sz.width() + 8, sz.height() + 8);
        b->setCursor(Qt::PointingHandCursor);
        b->setToolTip(tip);
    };
    setImg(ui->btnFaceId,            loadPx("faceid"),        "Connexion Face ID");
    setImg(ui->btnExporterPdf,       loadPx("exporterpdf"),   "Exporter la liste des employés en PDF");
    setImg(ui->btnAjouterEmploye,    loadPx("ajouter"),       "Ajouter un employé");
    setImg(ui->btnSupprimerEmploye,  loadPx("supprimer"),     "Supprimer un employé");
    setImg(ui->btnStatistiques,      loadPx("statistiques"),  "Statistiques (pourcentages, barres et cercles)");
    setImg(ui->btnTrier,             loadPx("trier"),         "Trier les employés");

    connect(ui->btnFaceId,            &QPushButton::clicked, this, &MainWindow::onFaceId);
    connect(ui->btnExporterPdf,       &QPushButton::clicked, this, &MainWindow::onExporterPdf);
    connect(ui->btnStatistiques,      &QPushButton::clicked, this, &MainWindow::onStatistiques);
    connect(ui->btnTrier,             &QPushButton::clicked, this, &MainWindow::onTrier);
    connect(ui->btnCalendrier,        &QPushButton::clicked, this, &MainWindow::onCalendrier);
    connect(ui->btnAjouterEmploye,    &QPushButton::clicked, this, [this] { openForm(Mode::Add); });
    connect(ui->btnSupprimerEmploye,  &QPushButton::clicked, this, [this] { openForm(Mode::Delete, selectedEmployeeId()); });

    // bascule entre le tableau personnalise et la vue SQL (QTableView + QSqlQueryModel)
    connect(ui->btnVueSql, &QPushButton::toggled, this, [this](bool on) {
        m_panel->setVisible(false);
        ui->tableViewSql->setVisible(on);
        ui->tableEmployees->setVisible(!on);
        ui->btnVueSql->setText(on ? "Vue design" : "Vue SQL");
    });
}

void MainWindow::setupForm()
{
    ui->comboFormGrade->setIconSize(QSize(60, 20));
    for (int g = 1; g <= 3; ++g)
        ui->comboFormGrade->addItem(QIcon(m_gradeBtn[g].scaledToHeight(40, Qt::SmoothTransformation)),
                                    QString("%1 étoile%2").arg(g).arg(g > 1 ? "s" : ""), g);
    for (int i = 0; i < 3; ++i)
        ui->comboFormSpec->addItem(QIcon(m_specPx[i].scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation)),
                                   kSpecs[i], kSpecs[i]);
    ui->comboFormStatut->addItems(kStatuts);
    ui->comboFormGarde->addItems(kGardes);

    // controles de saisie
    ui->lineEditNom->setMaxLength(50);
    ui->lineEditPrenom->setMaxLength(50);
    ui->lineEditTel->setMaxLength(15);
    const QRegularExpression reNom(QString::fromUtf8("[A-Za-zÀ-ÿ' \\-]*"));
    ui->lineEditNom->setValidator(new QRegularExpressionValidator(reNom, this));
    ui->lineEditPrenom->setValidator(new QRegularExpressionValidator(reNom, this));
    ui->lineEditTel->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]*"), this));

    connect(ui->btnFormValider, &QPushButton::clicked, this, &MainWindow::onFormValider);
    connect(ui->btnFormAnnuler, &QPushButton::clicked, this, &MainWindow::closeForm);
    connect(ui->btnFormPhoto,   &QPushButton::clicked, this, &MainWindow::onChoisirPhoto);
    connect(ui->lineEditNom,    &QLineEdit::returnPressed, this, &MainWindow::onFormValider);
    connect(ui->lineEditPrenom, &QLineEdit::returnPressed, this, &MainWindow::onFormValider);
    connect(ui->lineEditTel,    &QLineEdit::returnPressed, this, &MainWindow::onFormValider);
    ui->formPanel->setVisible(false);
}

void MainWindow::setupTable()
{
    QTableWidget *t = ui->tableEmployees;
    t->setColumnCount(6);
    t->setHorizontalHeaderLabels({"Avatar", "Nom", "Grade", "Spécialité", "Statut de Présence", "Emploi du temps"});
    t->verticalHeader()->setVisible(false);
    t->horizontalHeader()->setHighlightSections(false);
    t->horizontalHeader()->setFixedHeight(42);
    t->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setShowGrid(false);
    t->setAlternatingRowColors(true);
    t->setFocusPolicy(Qt::NoFocus);
    t->verticalHeader()->setDefaultSectionSize(60);

    const int widths[6] = {60, 0, 112, 118, 134, 204};
    QHeaderView *h = t->horizontalHeader();
    for (int c = 0; c < 6; ++c) {
        if (c == 1) { h->setSectionResizeMode(c, QHeaderView::Stretch); continue; }
        h->setSectionResizeMode(c, QHeaderView::Fixed);
        t->setColumnWidth(c, widths[c]);
    }
}

void MainWindow::setupChat()
{
    m_thinkTimer = new QTimer(this);
    m_thinkTimer->setInterval(350);
    connect(m_thinkTimer, &QTimer::timeout, this, [this] {
        if (!m_thinkLabel) return;
        m_thinkDots = (m_thinkDots + 1) % 4;
        m_thinkLabel->setText("Traitement en cours" + QString(m_thinkDots, '.'));
    });
    ui->chatScroll->viewport()->setAutoFillBackground(false);
    ui->chatContent->setAutoFillBackground(false);
    connect(ui->btnSendChat, &QPushButton::clicked, this, &MainWindow::onSendChat);
    connect(ui->lineEditChat, &QLineEdit::returnPressed, this, &MainWindow::onSendChat);
    addBubble("Bonjour ! Je suis l'assistant RH. Demandez-moi par exemple : « combien d'agents en service ? »", false);
}

void MainWindow::setupRightPanels()
{
    connect(ui->btnGenererPlanning, &QPushButton::clicked, this, &MainWindow::onGenererPlanning);
    connect(ui->btnNotifier, &QPushButton::clicked, this, [this] {
        int n = 0;
        for (const Employe &e : std::as_const(m_employees))
            if (e.getSpecialite() == "Medic" && e.getStatut() != "En formation") ++n;
        statusBar()->showMessage(QString("Notification envoyée au Lt. Médecin : %1 agent(s) Medic disponible(s).").arg(n), 5000);
    });
    connect(ui->btnAlerteGrade, &QPushButton::clicked, this, [this] {
        int n = 0;
        for (const Employe &e : std::as_const(m_employees)) if (e.getStatut() == "En formation") ++n;
        statusBar()->showMessage(QString("Alerte S/L envoyée : %1 agent(s) en formation (fin de formation / aptitude).").arg(n), 5000);
    });

    for (QProgressBar *b : {ui->barPlan1, ui->barPlan2, ui->barPlan3, ui->barDispo, ui->barProgress}) {
        b->setRange(0, 100);
        b->setTextVisible(false);
    }
}

// ---------------------------------------------------------------------------
//  Acces aux donnees (Oracle)
// ---------------------------------------------------------------------------
void MainWindow::reloadFromDb()
{
    m_employees = Employe::chargerTous();
    if (!Employe::derniereErreur().isEmpty() && m_employees.isEmpty())
        statusBar()->showMessage("Lecture Oracle : " + Employe::derniereErreur(), 8000);
}

const Employe *MainWindow::findEmployee(int id) const
{
    for (const Employe &e : m_employees)
        if (e.getId() == id) return &e;
    return nullptr;
}

int MainWindow::selectedEmployeeId() const
{
    const int r = ui->tableEmployees->currentRow();
    if (r < 0) return -1;
    QTableWidgetItem *it = ui->tableEmployees->item(r, 1);
    return it ? it->data(Qt::UserRole).toInt() : -1;
}

void MainWindow::setInfo(const QString &html)
{
    ui->labelFormInfo->setText(html);
    ui->labelFormInfo->setVisible(!html.isEmpty());
}

QPixmap MainWindow::avatarFor(const Employe &e, int size) const
{
    if (!e.getPhoto().isEmpty()) {
        QPixmap px(e.getPhoto());
        if (!px.isNull()) return roundPhoto(px, size);
    }
    return m_avatar.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QString MainWindow::pointage(const Employe &e)
{
    const QString heure = QTime::currentTime().toString("HH:mm");
    const bool enService = (e.getStatut() == "En service");
    const QString nouveau = enService ? "En pause" : "En service";
    if (!Employe::changerStatut(e.getId(), nouveau))
        return "⚠ Échec Oracle : " + Employe::derniereErreur();
    return enService
        ? QString("↩ Sortie : %1 à %2 → statut « En pause ».").arg(e.nomComplet(), heure)
        : QString("✔ Entrée : %1 à %2 → statut « En service ».").arg(e.nomComplet(), heure);
}

// Verifie nom / prenom / telephone. Retourne false et affiche le probleme sinon.
bool MainWindow::validerSaisie(QString &nom, QString &prenom, QString &tel)
{
    nom = ui->lineEditNom->text().trimmed();
    prenom = ui->lineEditPrenom->text().trimmed();
    tel = ui->lineEditTel->text().trimmed();

    auto flag = [](QLineEdit *le, bool bad) {
        le->setProperty("invalide", bad);
        le->style()->unpolish(le);
        le->style()->polish(le);
    };
    const bool badNom = nom.size() < 2;
    const bool badPre = prenom.size() < 2;
    const bool badTel = tel.size() < 8 || tel.size() > 15;
    flag(ui->lineEditNom, badNom);
    flag(ui->lineEditPrenom, badPre);
    flag(ui->lineEditTel, badTel);

    if (badNom)  { setInfo("⚠ Le nom est obligatoire (2 lettres minimum, sans chiffres)."); return false; }
    if (badPre)  { setInfo("⚠ Le prénom est obligatoire (2 lettres minimum, sans chiffres)."); return false; }
    if (badTel)  { setInfo("⚠ Le téléphone doit contenir 8 à 15 chiffres."); return false; }
    return true;
}

void MainWindow::onChoisirPhoto()
{
    const QString f = QFileDialog::getOpenFileName(this, "Choisir une photo", QString(),
                                                   "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!f.isEmpty()) ui->lineEditPhoto->setText(f);
}

void MainWindow::openForm(Mode mode, int id)
{
    hidePanel();
    m_mode = mode;
    m_currentId = id;
    const Employe *e = (id >= 0) ? findEmployee(id) : nullptr;

    // r1 : nom/prenom/tel   r2 : grade/spec/statut   r3 : photo   r4 : employe cible / garde
    auto vis = [this](bool r1, bool r2, bool r3, bool target, bool garde) {
        ui->formRow1->setVisible(r1);
        ui->formRow2->setVisible(r2);
        ui->formRow3->setVisible(r3);
        ui->formRow4->setVisible(target || garde);
        ui->labelFormTarget->setVisible(target);  ui->comboFormTarget->setVisible(target);
        ui->labelFormGarde->setVisible(garde);    ui->comboFormGarde->setVisible(garde);
    };

    ui->lineEditNom->clear();
    ui->lineEditPrenom->clear();
    ui->lineEditTel->clear();
    ui->lineEditPhoto->clear();
    for (QLineEdit *le : {ui->lineEditNom, ui->lineEditPrenom, ui->lineEditTel}) {
        le->setProperty("invalide", false);
        le->style()->unpolish(le);
        le->style()->polish(le);
    }
    ui->comboFormGrade->setCurrentIndex(0);
    ui->comboFormSpec->setCurrentIndex(0);
    ui->comboFormStatut->setCurrentIndex(0);
    ui->btnFormValider->setVisible(true);
    ui->btnFormValider->setEnabled(true);
    ui->btnFormValider->setStyleSheet(QString());
    ui->btnFormAnnuler->setText("Annuler");
    setInfo(QString());

    auto remplirCibles = [this] {
        ui->comboFormTarget->clear();
        for (const Employe &x : std::as_const(m_employees))
            ui->comboFormTarget->addItem(QString("%1 (n° %2)").arg(x.nomComplet()).arg(x.getId()), x.getId());
    };

    switch (mode) {
    case Mode::Add:
        ui->labelFormTitle->setText("Ajouter un employé");
        vis(true, true, true, false, false);
        ui->btnFormValider->setText("Ajouter");
        ui->lineEditNom->setFocus();
        break;
    case Mode::AddVolunteer:
        ui->labelFormTitle->setText("Ajouter un volontaire");
        vis(true, true, true, false, false);
        ui->btnFormValider->setText("Ajouter");
        setInfo("Le volontaire apparaîtra dans le tableau avec la mention « Volontaire ».");
        ui->lineEditNom->setFocus();
        break;
    case Mode::Edit:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Modifier : %1").arg(e->nomComplet()));
        vis(true, true, true, false, false);
        ui->lineEditNom->setText(e->getNom());
        ui->lineEditPrenom->setText(e->getPrenom());
        ui->lineEditTel->setText(e->getTelephone());
        ui->lineEditPhoto->setText(e->getPhoto());
        ui->comboFormGrade->setCurrentIndex(qBound(0, e->getGrade() - 1, 2));
        ui->comboFormSpec->setCurrentIndex(qMax(0, kSpecs.indexOf(e->getSpecialite())));
        ui->comboFormStatut->setCurrentIndex(qMax(0, kStatuts.indexOf(e->getStatut())));
        ui->btnFormValider->setText("Enregistrer");
        break;
    case Mode::View:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Fiche : %1").arg(e->nomComplet()));
        vis(false, false, false, false, false);
        setInfo(QString("<b>%1</b>%2<br>Téléphone : %3<br>Grade : %4 étoile(s) · Spécialité : %5<br>Statut : %6<br>Emploi du temps : %7")
                    .arg(e->nomComplet(), e->isVolontaire() ? QString(" (Volontaire)") : QString(), e->getTelephone())
                    .arg(e->getGrade()).arg(e->getSpecialite(), e->getStatut(), e->getGarde()));
        ui->btnFormValider->setVisible(false);
        ui->btnFormAnnuler->setText("Fermer");
        break;
    case Mode::Assign:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Assigner un emploi du temps : %1").arg(e->nomComplet()));
        vis(false, false, false, false, true);
        ui->comboFormGarde->setCurrentIndex(qMax(0, kGardes.indexOf(e->getGarde())));
        if (e->getStatut() == "En formation")
            setInfo("⚠ Cet agent est en formation : l'assignation reste possible mais déconseillée.");
        ui->btnFormValider->setText("Assigner");
        break;
    case Mode::Delete:
        ui->labelFormTitle->setText("Supprimer un employé");
        vis(false, false, false, true, false);
        remplirCibles();
        if (id >= 0) ui->comboFormTarget->setCurrentIndex(qMax(0, ui->comboFormTarget->findData(id)));
        setInfo("Choisissez l'employé à retirer de la base (ou sélectionnez d'abord une ligne). Action définitive.");
        ui->btnFormValider->setText("Supprimer");
        ui->btnFormValider->setStyleSheet("background:#c62828;color:white;border:none;border-radius:6px;padding:7px 16px;font-weight:bold;");
        break;
    case Mode::Face:
        ui->labelFormTitle->setText("Connexion Face ID");
        vis(false, false, false, true, false);
        remplirCibles();
        setInfo("Choisissez l'agent puis lancez la vérification du visage (1er passage = entrée, 2e passage = sortie).");
        ui->btnFormValider->setText("Vérifier le visage");
        break;
    default:
        return;
    }
    ui->formPanel->setVisible(true);
}

void MainWindow::closeForm()
{
    m_mode = Mode::None;
    m_currentId = -1;
    ui->formPanel->setVisible(false);
}

void MainWindow::onFormValider()
{
    switch (m_mode) {
    case Mode::Add:
    case Mode::AddVolunteer: {
        QString nom, prenom, tel;
        if (!validerSaisie(nom, prenom, tel)) return;
        Employe e(0, nom, prenom, ui->lineEditPhoto->text().trimmed(),
                  ui->comboFormGrade->currentData().toInt(),
                  ui->comboFormSpec->currentData().toString(), tel,
                  ui->comboFormStatut->currentText(), "Aucune",
                  m_mode == Mode::AddVolunteer);
        if (!e.ajouter()) {
            QMessageBox::critical(this, "Not OK", "Ajout non effectué.\n" + Employe::derniereErreur());
            return;
        }
        reloadFromDb();
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 ajouté.").arg(e.nomComplet()), 4000);
        QMessageBox::information(this, "OK", "Ajout effectué.");
        closeForm();
        break;
    }
    case Mode::Edit: {
        const Employe *old = findEmployee(m_currentId);
        if (!old) { closeForm(); return; }
        QString nom, prenom, tel;
        if (!validerSaisie(nom, prenom, tel)) return;
        Employe e(m_currentId, nom, prenom, ui->lineEditPhoto->text().trimmed(),
                  ui->comboFormGrade->currentData().toInt(),
                  ui->comboFormSpec->currentData().toString(), tel,
                  ui->comboFormStatut->currentText(), old->getGarde(), old->isVolontaire());
        if (!e.modifier()) {
            QMessageBox::critical(this, "Not OK", "Modification non effectuée.\n" + Employe::derniereErreur());
            return;
        }
        reloadFromDb();
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 modifié.").arg(e.nomComplet()), 4000);
        QMessageBox::information(this, "OK", "Modification effectuée.");
        closeForm();
        break;
    }
    case Mode::Assign: {
        const Employe *e = findEmployee(m_currentId);
        if (!e) { closeForm(); return; }
        const QString garde = ui->comboFormGarde->currentText();
        const QString nom = e->nomComplet();
        if (!Employe::changerGarde(m_currentId, garde)) {
            setInfo("⚠ Échec Oracle : " + Employe::derniereErreur().toHtmlEscaped());
            return;
        }
        reloadFromDb();
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 → %2.").arg(nom, garde), 4000);
        closeForm();
        break;
    }
    case Mode::Delete: {
        const int id = ui->comboFormTarget->currentData().toInt();
        const Employe *e = findEmployee(id);
        if (!e) { setInfo("⚠ Aucun employé à supprimer."); return; }
        const QString nom = e->nomComplet();
        if (QMessageBox::question(this, "Suppression",
                                  QString("Supprimer définitivement %1 ?").arg(nom)) != QMessageBox::Yes)
            return;
        if (!m_dao.supprimer(id)) {
            QMessageBox::critical(this, "Not OK", "Suppression non effectuée.\n" + Employe::derniereErreur());
            return;
        }
        reloadFromDb();
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 supprimé.").arg(nom), 4000);
        QMessageBox::information(this, "OK", "Suppression effectuée.");
        closeForm();
        break;
    }
    case Mode::Face: {
        const int id = ui->comboFormTarget->currentData().toInt();
        if (!findEmployee(id)) { setInfo("⚠ Aucun agent sélectionné."); return; }
        ui->btnFormValider->setEnabled(false);
        setInfo("Analyse du visage en cours…");
        QTimer::singleShot(1300, this, [this, id] {
            const Employe *e = findEmployee(id);
            ui->btnFormValider->setEnabled(true);
            if (!e || m_mode != Mode::Face) return;
            const QString msg = pointage(*e);
            reloadFromDb();
            refreshTable();
            updatePlanning();
            setInfo("Visage reconnu. " + msg);
            statusBar()->showMessage(msg, 5000);
        });
        break;
    }
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
//  Tableau
// ---------------------------------------------------------------------------
void MainWindow::refreshTable()
{
    QTableWidget *t = ui->tableEmployees;
    const int g = ui->comboGrade->currentData().toInt();
    const QString sp = ui->comboSpec->currentData().toString();
    const QString q = ui->lineEditSearch->text().trimmed();

    QVector<Employe> rows;
    for (const Employe &e : std::as_const(m_employees)) {
        if (g > 0 && e.getGrade() != g) continue;
        if (!sp.isEmpty() && e.getSpecialite() != sp) continue;
        if (!q.isEmpty() && !e.getNom().contains(q, Qt::CaseInsensitive)
                         && !e.getPrenom().contains(q, Qt::CaseInsensitive)
                         && !e.getSpecialite().contains(q, Qt::CaseInsensitive)
                         && !e.getTelephone().contains(q, Qt::CaseInsensitive)) continue;
        rows.append(e);
    }

    // tri choisi avec le bouton "Trier"
    std::stable_sort(rows.begin(), rows.end(), [this](const Employe &a, const Employe &b) {
        switch (m_sortKey) {
        case SortKey::NomAZ:      return QString::localeAwareCompare(a.nomComplet(), b.nomComplet()) < 0;
        case SortKey::NomZA:      return QString::localeAwareCompare(a.nomComplet(), b.nomComplet()) > 0;
        case SortKey::GradeAsc:   return a.getGrade() < b.getGrade();
        case SortKey::GradeDesc:  return a.getGrade() > b.getGrade();
        case SortKey::Specialite: return QString::localeAwareCompare(a.getSpecialite(), b.getSpecialite()) < 0;
        case SortKey::Statut:     return QString::localeAwareCompare(a.getStatut(), b.getStatut()) < 0;
        default:                  return a.getId() < b.getId();
        }
    });
    m_rows = rows;

    const int selected = selectedEmployeeId();
    t->setRowCount(0);
    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        const Employe &e = rows[r];
        t->setRowHeight(r, 60);

        // 0 : avatar (photo si renseignee)
        auto *av = new QLabel;
        av->setPixmap(avatarFor(e, 44));
        t->setCellWidget(r, 0, centered(av));

        // 1 : nom prenom
        auto *it = new QTableWidgetItem(e.nomComplet() + (e.isVolontaire() ? "  (Volontaire)" : ""));
        it->setData(Qt::UserRole, e.getId());
        it->setToolTip(QString("Tél : %1").arg(e.getTelephone()));
        it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        t->setItem(r, 1, it);

        // 2 : grade (image 1etoile / 2etoile / 3etoile)
        auto *gl = new QLabel;
        gl->setPixmap(m_gradeBtn[qBound(1, e.getGrade(), 3)].scaledToHeight(36, Qt::SmoothTransformation));
        t->setCellWidget(r, 2, centered(gl));

        // 3 : specialite (icone + texte)
        auto *spBox = new QWidget;
        auto *spL = new QHBoxLayout(spBox);
        spL->setContentsMargins(8, 0, 4, 0);
        spL->setSpacing(6);
        auto *spIcon = new QLabel;
        const int si = qMax(0, kSpecs.indexOf(e.getSpecialite()));
        spIcon->setPixmap(m_specPx[si].scaled(26, 26, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        auto *spText = new QLabel(e.getSpecialite());
        spText->setStyleSheet("font-weight:bold;color:#263238;");
        spL->addWidget(spIcon);
        spL->addWidget(spText);
        spL->addStretch();
        t->setCellWidget(r, 3, spBox);

        // 4 : statut de presence
        auto *st = new QLabel(e.getStatut());
        st->setAlignment(Qt::AlignCenter);
        st->setStyleSheet(statusStyle(e.getStatut()));
        t->setCellWidget(r, 4, centered(st));

        // 5 : emploi du temps (Visualiser / Modifier / Assigner)
        auto *act = new QWidget;
        auto *al = new QHBoxLayout(act);
        al->setContentsMargins(4, 0, 4, 0);
        al->setSpacing(5);
        auto mk = [this, act, al, id = e.getId()](const QString &txt, Mode m) {
            auto *b = new QPushButton(txt, act);
            b->setObjectName("rowBtn");
            b->setCursor(Qt::PointingHandCursor);
            connect(b, &QPushButton::clicked, this, [this, m, id] { openForm(m, id); });
            al->addWidget(b);
        };
        mk("Visualiser", Mode::View);
        mk("Modifier", Mode::Edit);
        mk("Assigner", Mode::Assign);
        t->setCellWidget(r, 5, act);

        if (e.getId() == selected) t->selectRow(r);
    }

    // Vue SQL : le modele est fourni par Employe::afficher() (architecture Modele-Vue)
    QAbstractItemModel *old = ui->tableViewSql->model();
    ui->tableViewSql->setModel(m_dao.afficher());
    delete old;
    ui->tableViewSql->resizeColumnsToContents();
    ui->tableViewSql->horizontalHeader()->setStretchLastSection(true);

    ui->labelCount->setText(QString("%1 employé(s) affiché(s) sur %2").arg(rows.size()).arg(m_employees.size()));

    // badge de la cloche = agents en formation
    int n = 0;
    for (const Employe &e : std::as_const(m_employees)) if (e.getStatut() == "En formation") ++n;
    if (m_badge) { m_badge->setText(QString::number(n)); m_badge->setVisible(n > 0); }
}

// ---------------------------------------------------------------------------
//  Smart Planning IA (regles simples ; les gardes sont enregistrees dans Oracle)
// ---------------------------------------------------------------------------
void MainWindow::updatePlanning()
{
    const int total = m_employees.size();
    int dispo = 0, assigned = 0;
    int perShift[3] = {0, 0, 0};
    for (const Employe &e : std::as_const(m_employees)) {
        if (e.getStatut() != "En formation") ++dispo;
        const int gi = kGardes.indexOf(e.getGarde());
        if (gi >= 0 && gi < 3) { ++assigned; ++perShift[gi]; }
    }
    const int cap = 4;
    QProgressBar *bars[3] = {ui->barPlan1, ui->barPlan2, ui->barPlan3};
    for (int i = 0; i < 3; ++i) {
        const int v = qMin(100, perShift[i] * 100 / cap);
        bars[i]->setValue(v);
        const QString color = v >= 75 ? "#2e9e4f" : "#c62828";
        bars[i]->setStyleSheet(QString("QProgressBar{background:#e3e6e9;border:none;border-radius:6px;}"
                                       "QProgressBar::chunk{background:%1;border-radius:6px;}").arg(color));
    }
    ui->barDispo->setValue(total ? dispo * 100 / total : 0);
    ui->barProgress->setValue(total ? assigned * 100 / total : 0);
    ui->barDispo->setStyleSheet("QProgressBar{background:#e3e6e9;border:none;border-radius:5px;}QProgressBar::chunk{background:#2e9e4f;border-radius:5px;}");
    ui->barProgress->setStyleSheet("QProgressBar{background:#e3e6e9;border:none;border-radius:5px;}QProgressBar::chunk{background:#c62828;border-radius:5px;}");
}

void MainWindow::onGenererPlanning()
{
    const int n = m_employees.size();
    QVector<QString> garde(n, "Aucune");

    QVector<int> pool;                      // agents disponibles, meilleurs grades d'abord
    for (int i = 0; i < n; ++i)
        if (m_employees[i].getStatut() != "En formation") pool.append(i);
    std::stable_sort(pool.begin(), pool.end(), [this](int a, int b) {
        return m_employees[a].getGrade() > m_employees[b].getGrade();
    });

    const int cap = 4;
    int counts[3] = {0, 0, 0};
    for (int s = 0; s < 3; ++s) {
        bool any = true;
        while (counts[s] < cap && any) {
            any = false;
            for (const QString &spec : kSpecs) {
                if (counts[s] >= cap) break;
                for (int idx : std::as_const(pool)) {
                    if (garde[idx] == "Aucune" && m_employees[idx].getSpecialite() == spec) {
                        garde[idx] = kGardes[s];
                        ++counts[s];
                        any = true;
                        break;
                    }
                }
            }
        }
    }

    QVector<QPair<int, QString>> maj;
    for (int i = 0; i < n; ++i)
        maj.append({m_employees[i].getId(), garde[i]});
    if (!Employe::changerGardes(maj)) {
        addBubble("⚠ Impossible d'enregistrer le planning dans Oracle : " + Employe::derniereErreur(), false);
        return;
    }

    reloadFromDb();
    refreshTable();
    updatePlanning();
    const QString msg = QString("Planning généré : jour %1/%4, nuit %2/%4, week-end %3/%4. "
                                "Les agents en formation sont exclus ; spécialités équilibrées, grades élevés prioritaires.")
                            .arg(counts[0]).arg(counts[1]).arg(counts[2]).arg(cap);
    addBubble(msg, false);
    statusBar()->showMessage("Planning de garde généré et enregistré.", 4000);
}

// ---------------------------------------------------------------------------
//  Chatbot RH (reponses par regles a partir des donnees Oracle)
// ---------------------------------------------------------------------------
void MainWindow::addBubble(const QString &text, bool fromUser)
{
    auto *row = new QWidget;
    auto *h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(6);

    auto *bubble = new QLabel(text);
    bubble->setWordWrap(true);
    bubble->setObjectName(fromUser ? "bubbleUser" : "bubbleBot");
    bubble->setMaximumWidth(270);
    bubble->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto *av = new QLabel;
    av->setFixedSize(30, 30);
    av->setPixmap((fromUser ? m_avatar : m_chatbot).scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if (fromUser) {
        h->addStretch();
        h->addWidget(bubble);
        h->addWidget(av, 0, Qt::AlignTop);
    } else {
        h->addWidget(av, 0, Qt::AlignTop);
        h->addWidget(bubble);
        h->addStretch();
    }
    auto *lay = qobject_cast<QVBoxLayout *>(ui->chatContent->layout());
    lay->insertWidget(lay->count() - 1, row);
    QTimer::singleShot(30, this, [this] {
        ui->chatScroll->verticalScrollBar()->setValue(ui->chatScroll->verticalScrollBar()->maximum());
    });
}

void MainWindow::showThinking()
{
    hideThinking();
    m_thinkRow = new QWidget;
    auto *h = new QHBoxLayout(m_thinkRow);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(8);
    auto *img = new QLabel;                   // image chatboot.png : visible pendant le traitement
    img->setPixmap(m_chatbot.scaled(46, 46, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_thinkLabel = new QLabel("Traitement en cours");
    m_thinkLabel->setStyleSheet("color:#607d8b;font-style:italic;");
    h->addWidget(img);
    h->addWidget(m_thinkLabel);
    h->addStretch();
    auto *lay = qobject_cast<QVBoxLayout *>(ui->chatContent->layout());
    lay->insertWidget(lay->count() - 1, m_thinkRow);
    m_thinkDots = 0;
    m_thinkTimer->start();
    QTimer::singleShot(30, this, [this] {
        ui->chatScroll->verticalScrollBar()->setValue(ui->chatScroll->verticalScrollBar()->maximum());
    });
}

void MainWindow::hideThinking()
{
    m_thinkTimer->stop();
    if (m_thinkRow) {
        m_thinkRow->deleteLater();
        m_thinkRow = nullptr;
        m_thinkLabel = nullptr;
    }
}

void MainWindow::onSendChat()
{
    const QString q = ui->lineEditChat->text().trimmed();
    if (q.isEmpty() || !ui->btnSendChat->isEnabled()) return;
    addBubble(q, true);
    ui->lineEditChat->clear();
    ui->btnSendChat->setEnabled(false);
    ui->lineEditChat->setEnabled(false);
    showThinking();
    QTimer::singleShot(1500, this, [this, q] {
        hideThinking();
        addBubble(answerFor(q), false);
        ui->btnSendChat->setEnabled(true);
        ui->lineEditChat->setEnabled(true);
        ui->lineEditChat->setFocus();
    });
}

QString MainWindow::answerFor(const QString &question) const
{
    const QString q = question.toLower();
    auto has = [&q](const QStringList &keys) {
        for (const QString &k : keys) if (q.contains(k)) return true;
        return false;
    };
    auto names = [this](const QString &statut) {
        QStringList l;
        for (const Employe &e : m_employees) if (e.getStatut() == statut) l << e.nomComplet();
        return l;
    };

    if (has({"bonjour", "salut", "hello", "salam", "bonsoir"}))
        return "Bonjour ! Je peux vous renseigner sur l'effectif, les statuts, les grades, les spécialités et le planning.";

    if (has({"service", "présent", "present"})) {
        const QStringList l = names("En service");
        return l.isEmpty() ? "Aucun agent n'est actuellement en service."
                           : QString("%1 agent(s) en service : %2.").arg(l.size()).arg(l.join(", "));
    }
    if (has({"pause"})) {
        const QStringList l = names("En pause");
        return l.isEmpty() ? "Aucun agent en pause." : QString("%1 agent(s) en pause : %2.").arg(l.size()).arg(l.join(", "));
    }
    if (has({"formation", "aptitude"})) {
        const QStringList l = names("En formation");
        return l.isEmpty() ? "Aucun agent en formation."
                           : QString("%1 agent(s) en formation : %2. Pensez aux alertes de fin de formation.").arg(l.size()).arg(l.join(", "));
    }
    if (has({"volontaire"})) {
        int n = 0;
        for (const Employe &e : m_employees) if (e.isVolontaire()) ++n;
        return QString("%1 volontaire(s) enregistré(s).").arg(n);
    }
    for (const QString &spec : kSpecs) {
        if (q.contains(spec.toLower().left(5))) {
            int n = 0;
            for (const Employe &e : m_employees) if (e.getSpecialite() == spec) ++n;
            return QString("%1 agent(s) de spécialité %2.").arg(n).arg(spec);
        }
    }
    if (has({"grade", "étoile", "etoile"})) {
        int c[4] = {0, 0, 0, 0};
        for (const Employe &e : m_employees) ++c[qBound(1, e.getGrade(), 3)];
        return QString("Répartition par grade : 1 étoile = %1, 2 étoiles = %2, 3 étoiles = %3.").arg(c[1]).arg(c[2]).arg(c[3]);
    }
    if (has({"planning", "garde"}))
        return "Cliquez sur « Générer Planning » : je répartis les agents disponibles par grade et spécialité (jour / nuit / week-end) et j'enregistre le résultat dans la base.";
    if (has({"face", "pointage", "check"}))
        return "Cliquez sur « Face ID », choisissez l'agent : un premier passage le met en service, un second en pause.";
    if (has({"combien", "nombre", "effectif", "total"})) {
        int s = 0, p = 0, f = 0;
        for (const Employe &e : m_employees) {
            if (e.getStatut() == "En service") ++s;
            else if (e.getStatut() == "En pause") ++p;
            else ++f;
        }
        return QString("Effectif total : %1 (en service %2, en pause %3, en formation %4).").arg(m_employees.size()).arg(s).arg(p).arg(f);
    }
    return "Je n'ai pas compris. Essayez : « combien d'agents », « qui est en pause », « agents Medic », « répartition par grade », « planning ».";
}


// ---------------------------------------------------------------------------
//  Face ID (camera)
// ---------------------------------------------------------------------------
void MainWindow::onFaceId()
{
    if (m_employees.isEmpty()) {
        statusBar()->showMessage("Aucun employé : ajoutez d'abord un employé.", 4000);
        return;
    }
    FaceIdDialog dlg(m_employees, selectedEmployeeId(), this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const Employe *e = findEmployee(dlg.employeId());
    if (!e) return;
    const QString msg = pointage(*e);          // 1er passage = entree, 2e passage = sortie (Oracle)
    reloadFromDb();
    refreshTable();
    updatePlanning();
    statusBar()->showMessage(msg, 6000);
    QMessageBox::information(this, "Face ID", "Visage reconnu.\n" + msg);
}

// ---------------------------------------------------------------------------
//  Statistiques
// ---------------------------------------------------------------------------
void MainWindow::onStatistiques()
{
    auto *page = new StatsPage(m_employees);
    connect(page, &StatsPage::retour, this, [this] { hidePanel(); });
    showPanel(page);
}

// ---------------------------------------------------------------------------
//  Tri
// ---------------------------------------------------------------------------
QString MainWindow::sortLabel() const
{
    switch (m_sortKey) {
    case SortKey::NomAZ:      return "nom A → Z";
    case SortKey::NomZA:      return "nom Z → A";
    case SortKey::GradeAsc:   return "grade croissant";
    case SortKey::GradeDesc:  return "grade décroissant";
    case SortKey::Specialite: return "spécialité";
    case SortKey::Statut:     return "statut";
    default:                  return "numéro";
    }
}

void MainWindow::onTrier()
{
    QMenu menu(this);
    auto *group = new QActionGroup(&menu);
    const QVector<QPair<QString, SortKey>> items = {
        {"Par numéro (ID)",         SortKey::Id},
        {"Nom : A → Z",             SortKey::NomAZ},
        {"Nom : Z → A",             SortKey::NomZA},
        {"Grade : croissant",       SortKey::GradeAsc},
        {"Grade : décroissant",     SortKey::GradeDesc},
        {"Spécialité",              SortKey::Specialite},
        {"Statut de présence",      SortKey::Statut},
    };
    for (const auto &it : items) {
        QAction *a = menu.addAction(it.first);
        a->setCheckable(true);
        a->setChecked(it.second == m_sortKey);
        a->setActionGroup(group);
        a->setData(int(it.second));
    }
    QAction *chosen = menu.exec(ui->btnTrier->mapToGlobal(QPoint(0, ui->btnTrier->height())));
    if (!chosen) return;
    m_sortKey = static_cast<SortKey>(chosen->data().toInt());
    refreshTable();
    statusBar()->showMessage("Tri appliqué : " + sortLabel(), 3000);
}

// ---------------------------------------------------------------------------
//  Calendrier de l'employe selectionne
// ---------------------------------------------------------------------------
void MainWindow::onCalendrier()
{
    const int id = selectedEmployeeId();
    const Employe *e = (id >= 0) ? findEmployee(id) : nullptr;
    if (!e) {
        const QString msg = "Sélectionnez d'abord un employé dans le tableau (clic sur sa ligne), puis cliquez sur « Calendrier ».";
        addBubble(msg, false);
        statusBar()->showMessage(msg, 5000);
        return;
    }
    auto *page = new CalendarPage(*e);
    connect(page, &CalendarPage::retour, this, [this] { hidePanel(); });
    showPanel(page);
}

// ---------------------------------------------------------------------------
//  Pages integrees (meme fenetre) : le tableau est remplace par la page, puis restaure
// ---------------------------------------------------------------------------
void MainWindow::showPanel(QWidget *page)
{
    while (m_panel->count() > 0) {                // supprime l'ancienne page
        QWidget *old = m_panel->widget(0);
        m_panel->removeWidget(old);
        old->deleteLater();
    }
    m_panel->addWidget(page);
    m_panel->setCurrentWidget(page);
    ui->formPanel->setVisible(false);
    ui->tableEmployees->setVisible(false);
    ui->tableViewSql->setVisible(false);
    m_panel->setVisible(true);
}

void MainWindow::hidePanel()
{
    if (!m_panel->isVisible())
        return;
    m_panel->setVisible(false);
    const bool sql = ui->btnVueSql->isChecked();
    ui->tableViewSql->setVisible(sql);
    ui->tableEmployees->setVisible(!sql);
}

// ---------------------------------------------------------------------------
//  Export PDF (liste affichee : filtres + tri pris en compte)
// ---------------------------------------------------------------------------
void MainWindow::onExporterPdf()
{
    if (m_rows.isEmpty()) {
        QMessageBox::information(this, "Exporter PDF", "Aucun employé à exporter.");
        return;
    }
    const QString def = QString("employes_%1.pdf").arg(QDate::currentDate().toString("yyyyMMdd"));
    QString path = QFileDialog::getSaveFileName(this, "Exporter en PDF", def, "Fichier PDF (*.pdf)");
    if (path.isEmpty()) return;
    if (!path.endsWith(".pdf", Qt::CaseInsensitive)) path += ".pdf";

    int s = 0, p = 0, f = 0, v = 0;
    for (const Employe &e : std::as_const(m_rows)) {
        if (e.getStatut() == "En service") ++s;
        else if (e.getStatut() == "En pause") ++p;
        else ++f;
        if (e.isVolontaire()) ++v;
    }

    auto esc = [](const QString &x) { return x.toHtmlEscaped(); };
    QString html;
    html += "<html><body style='font-family:Arial;'>";
    html += "<table width='100%'><tr>";
    html += "<td width='14%'><img src='logo' width='90'></td>";
    html += "<td><h1 style='color:#7a0f0f;font-size:20pt;'>Fire Station – Liste des employés</h1>"
            "<p style='font-size:10pt;color:#455a64;'>Édité le " + QDate::currentDate().toString("dd/MM/yyyy") +
            " · " + QString::number(m_rows.size()) + " employé(s) · tri : " + esc(sortLabel()) + "</p></td>";
    html += "</tr></table><br>";
    html += "<table width='100%' border='1' cellspacing='0' cellpadding='5' style='border-collapse:collapse;font-size:9pt;'>";
    html += "<tr style='background-color:#2f3e4e;color:white;'>"
            "<th>N°</th><th>Nom</th><th>Prénom</th><th>Téléphone</th><th>Grade</th>"
            "<th>Spécialité</th><th>Statut</th><th>Emploi du temps</th><th>Type</th></tr>";
    bool alt = false;
    for (const Employe &e : std::as_const(m_rows)) {
        const QString bg = alt ? "#f3f5f7" : "#ffffff";
        alt = !alt;
        QString statutBg = "#b9e8c0";
        if (e.getStatut() == "En pause") statutBg = "#f7dc7f";
        if (e.getStatut() == "En formation") statutBg = "#cfe3f5";
        html += QString("<tr style='background-color:%1;'>").arg(bg);
        html += QString("<td align='center'>%1</td><td>%2</td><td>%3</td><td>%4</td><td align='center'>%5 ★</td><td>%6</td>")
                    .arg(e.getId()).arg(esc(e.getNom()), esc(e.getPrenom()), esc(e.getTelephone()))
                    .arg(e.getGrade()).arg(esc(e.getSpecialite()));
        html += QString("<td align='center' style='background-color:%1;'>%2</td><td>%3</td><td align='center'>%4</td></tr>")
                    .arg(statutBg, esc(e.getStatut()), esc(e.getGarde()), e.isVolontaire() ? QString("Volontaire") : QString("Employé"));
    }
    html += "</table><br>";
    html += QString("<p style='font-size:10pt;'><b>Résumé :</b> %1 en service · %2 en pause · %3 en formation · %4 volontaire(s)</p>")
                .arg(s).arg(p).arg(f).arg(v);
    html += "</body></html>";

    QTextDocument doc;
    if (!m_logo.isNull())
        doc.addResource(QTextDocument::ImageResource, QUrl("logo"), m_logo.toImage());
    doc.setHtml(html);

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setPageMargins(QMarginsF(12, 12, 12, 12), QPageLayout::Millimeter);
    writer.setTitle("Liste des employés");
    doc.print(&writer);

    statusBar()->showMessage("PDF exporté : " + path, 6000);
    if (QMessageBox::question(this, "Exporter PDF", "PDF créé avec succès.\nVoulez-vous l'ouvrir maintenant ?") == QMessageBox::Yes)
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


// ===========================================================================
//  Page STATISTIQUES (barres + cercles)
// ===========================================================================


// ---------------------------------------------------------------------------
//  StatCard
// ---------------------------------------------------------------------------
StatCard::StatCard(const QString &title, const QVector<StatItem> &items, QWidget *parent)
    : QWidget(parent), m_title(title), m_items(items)
{
    setMinimumHeight(200);
}

void StatCard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // fond de la carte
    p.setPen(QColor("#d9dde1"));
    p.setBrush(Qt::white);
    p.drawRoundedRect(QRectF(1, 1, width() - 2, height() - 2), 8, 8);

    // titre
    QFont tf = font();
    tf.setBold(true);
    tf.setPixelSize(14);
    p.setFont(tf);
    p.setPen(QColor("#37474f"));
    p.drawText(QRectF(16, 8, width() - 32, 24), Qt::AlignLeft | Qt::AlignVCenter, m_title);

    int total = 0;
    for (const StatItem &it : m_items)
        total += it.value;

    // ---- graphique circulaire (anneau) ----
    const int side = qMax(80, std::min({height() - 54, 150, int(width() * 0.33)}));
    const QRectF ring(18, 40, side, side);
    p.setPen(Qt::NoPen);
    if (total <= 0) {
        p.setBrush(QColor("#e3e6e9"));
        p.drawEllipse(ring);
    } else {
        int start = 90 * 16;                       // on commence en haut
        int used = 0;
        for (int i = 0; i < m_items.size(); ++i) {
            int span = int(-(m_items[i].value * 360.0 / total) * 16);
            if (i == m_items.size() - 1)           // evite le petit vide d'arrondi
                span = -(360 * 16) - used;
            used += span;
            if (m_items[i].value > 0) {
                p.setBrush(m_items[i].color);
                p.drawPie(ring, start, span);
            }
            start += span;
        }
    }
    p.setBrush(Qt::white);                         // trou de l'anneau
    p.drawEllipse(ring.center(), side * 0.30, side * 0.30);

    QFont cf = font();
    cf.setBold(true);
    cf.setPixelSize(20);
    p.setFont(cf);
    p.setPen(QColor("#263238"));
    p.drawText(QRectF(ring.center().x() - 30, ring.center().y() - 16, 60, 22), Qt::AlignCenter, QString::number(total));
    QFont sf = font();
    sf.setPixelSize(10);
    p.setFont(sf);
    p.setPen(QColor("#607d8b"));
    p.drawText(QRectF(ring.center().x() - 30, ring.center().y() + 4, 60, 14), Qt::AlignCenter, "total");

    // ---- barres de pourcentage ----
    const qreal x0 = ring.right() + 20;
    const qreal barW = width() - x0 - 16;
    const int n = qMax(1, m_items.size());
    const qreal rowH = qMin<qreal>(40.0, (height() - 54.0) / n);
    qreal y = 42;
    QFont lf = font();
    lf.setPixelSize(11);
    for (const StatItem &it : m_items) {
        const double pct = total > 0 ? it.value * 100.0 / total : 0.0;

        p.setFont(lf);
        p.setPen(QColor("#37474f"));
        p.drawText(QRectF(x0, y, barW - 58, 16), Qt::AlignLeft | Qt::AlignVCenter,
                   QString("%1 (%2)").arg(it.label).arg(it.value));
        QFont bf = lf;
        bf.setBold(true);
        p.setFont(bf);
        p.drawText(QRectF(x0 + barW - 58, y, 58, 16), Qt::AlignRight | Qt::AlignVCenter,
                   QString("%1 %").arg(pct, 0, 'f', 1));

        const QRectF track(x0, y + 18, barW, 10);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#e3e6e9"));
        p.drawRoundedRect(track, 5, 5);
        if (pct > 0) {
            p.setBrush(it.color);
            p.drawRoundedRect(QRectF(track.x(), track.y(), qMax<qreal>(10.0, barW * pct / 100.0), track.height()), 5, 5);
        }
        y += rowH;
    }
}

// ---------------------------------------------------------------------------
//  StatsDialog
// ---------------------------------------------------------------------------
StatsPage::StatsPage(const QVector<Employe> &employees, QWidget *parent)
    : QWidget(parent)
{

    auto count = [&](auto pred) {
        int n = 0;
        for (const Employe &e : employees)
            if (pred(e)) ++n;
        return n;
    };

    const QColor vert("#2e9e4f"), jaune("#f0b429"), bleu("#3b6fb6"), rouge("#c62828"), gris("#90a4ae");

    // ---- statut de presence
    QVector<StatItem> statut = {
        {"En service",   count([](const Employe &e) { return e.getStatut() == "En service"; }),   vert},
        {"En pause",     count([](const Employe &e) { return e.getStatut() == "En pause"; }),     jaune},
        {"En formation", count([](const Employe &e) { return e.getStatut() == "En formation"; }), bleu},
    };
    // ---- specialite
    QVector<StatItem> spec = {
        {"Fire",      count([](const Employe &e) { return e.getSpecialite() == "Fire"; }),      rouge},
        {"Medic",     count([](const Employe &e) { return e.getSpecialite() == "Medic"; }),     bleu},
        {"Logistics", count([](const Employe &e) { return e.getSpecialite() == "Logistics"; }), jaune},
    };
    // ---- grade
    QVector<StatItem> grade = {
        {"1 étoile",  count([](const Employe &e) { return e.getGrade() == 1; }), bleu},
        {"2 étoiles", count([](const Employe &e) { return e.getGrade() == 2; }), jaune},
        {"3 étoiles", count([](const Employe &e) { return e.getGrade() == 3; }), rouge},
    };
    // ---- emploi du temps
    QVector<StatItem> garde = {
        {"Garde de jour",  count([](const Employe &e) { return e.getGarde().startsWith("Garde de jour"); }),  jaune},
        {"Garde de nuit",  count([](const Employe &e) { return e.getGarde().startsWith("Garde de nuit"); }),  bleu},
        {"Astreinte week-end", count([](const Employe &e) { return e.getGarde().startsWith("Astreinte"); }), vert},
        {"Aucune",         count([](const Employe &e) { return e.getGarde() == "Aucune"; }),                  gris},
    };
    // ---- employes / volontaires
    const int nbVol = count([](const Employe &e) { return e.isVolontaire(); });
    QVector<StatItem> type = {
        {"Employés",    int(employees.size()) - nbVol, bleu},
        {"Volontaires", nbVol,                          rouge},
    };

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(10);

    auto *head = new QLabel(QString("<b style='font-size:16px'>Statistiques</b> &nbsp; — &nbsp; %1 employé(s) enregistré(s)")
                                .arg(employees.size()));
    root->addWidget(head);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto *content = new QWidget;
    content->setStyleSheet("background:transparent;");
    auto *grid = new QGridLayout(content);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(12);
    grid->addWidget(new StatCard("Statut de présence", statut), 0, 0);
    grid->addWidget(new StatCard("Spécialités", spec), 0, 1);
    grid->addWidget(new StatCard("Grades", grade), 1, 0);
    grid->addWidget(new StatCard("Emploi du temps", garde), 1, 1);
    grid->addWidget(new StatCard("Employés / Volontaires", type), 2, 0);
    scroll->setWidget(content);
    root->addWidget(scroll, 1);

    auto *close = new QPushButton("← Retour au tableau");
    close->setCursor(Qt::PointingHandCursor);
    close->setStyleSheet("QPushButton { background:#2f3e4e; color:white; border:none; border-radius:6px;"
                         " padding:8px 22px; font-weight:bold; } QPushButton:hover { background:#3d5166; }");
    connect(close, &QPushButton::clicked, this, &StatsPage::retour);
    auto *row = new QHBoxLayout;
    row->addStretch();
    row->addWidget(close);
    root->addLayout(row);
}


// ===========================================================================
//  Page CALENDRIER de travail
// ===========================================================================


namespace {
const QStringList kGardesJour = {"Garde de jour (08h-20h)", "Garde de nuit (20h-08h)", "Astreinte week-end"};

QString styleBtn(bool dark)
{
    return dark ? "QPushButton { background:#2f3e4e; color:white; border:none; border-radius:6px; padding:8px 14px; font-weight:bold; }"
                  "QPushButton:hover { background:#3d5166; }"
                : "QPushButton { background:#d9dcdf; color:#37474f; border:none; border-radius:6px; padding:8px 14px; }"
                  "QPushButton:hover { background:#c8ccd0; }";
}
} // namespace

CalendarPage::CalendarPage(const Employe &employe, QWidget *parent)
    : QWidget(parent), m_emp(employe)
{
    setStyleSheet("QLabel { color:#37474f; }"
                  "QComboBox { border:1px solid #cfd4d9; border-radius:6px; padding:3px 8px; background:white; color:#263238; }"
                  "QListWidget { background:white; border:1px solid #d9dde1; border-radius:6px; color:#263238; }");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(10);

    root->addWidget(new QLabel(QString("<b style='font-size:15px'>%1</b> &nbsp; Spécialité : %2 &nbsp; · &nbsp; Statut : %3")
                                   .arg(m_emp.nomComplet(), m_emp.getSpecialite(), m_emp.getStatut())));

    auto *body = new QHBoxLayout;
    body->setSpacing(12);

    m_cal = new QCalendarWidget;
    m_cal->setGridVisible(true);
    m_cal->setFirstDayOfWeek(Qt::Monday);
    m_cal->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);
    m_cal->setSelectedDate(QDate::currentDate());
    body->addWidget(m_cal, 3);

    auto *side = new QVBoxLayout;
    side->setSpacing(8);
    side->addWidget(new QLabel("<b>Garde pour le jour sélectionné</b>"));
    m_combo = new QComboBox;
    m_combo->addItems(kGardesJour);
    side->addWidget(m_combo);

    auto *bAff = new QPushButton("Affecter à ce jour");
    auto *bSem = new QPushButton("Affecter à toute la semaine");
    auto *bRet = new QPushButton("Retirer ce jour");
    bAff->setStyleSheet(styleBtn(true));
    bSem->setStyleSheet(styleBtn(true));
    bRet->setStyleSheet(styleBtn(false));
    for (QPushButton *b : {bAff, bSem, bRet})
        b->setCursor(Qt::PointingHandCursor);
    side->addWidget(bAff);
    side->addWidget(bSem);
    side->addWidget(bRet);

    side->addSpacing(6);
    side->addWidget(new QLabel("<b>Jours planifiés ce mois-ci</b>"));
    m_list = new QListWidget;
    side->addWidget(m_list, 1);

    // legende
    auto *leg = new QLabel("<span style='background:#f7dc7f'>&nbsp;&nbsp;&nbsp;</span> jour &nbsp; "
                           "<span style='background:#9fb4d6'>&nbsp;&nbsp;&nbsp;</span> nuit &nbsp; "
                           "<span style='background:#b9e8c0'>&nbsp;&nbsp;&nbsp;</span> week-end");
    side->addWidget(leg);
    body->addLayout(side, 2);
    root->addLayout(body, 1);

    m_info = new QLabel;
    m_info->setWordWrap(true);
    root->addWidget(m_info);

    auto *close = new QPushButton("← Retour au tableau");
    close->setStyleSheet(styleBtn(false));
    close->setCursor(Qt::PointingHandCursor);
    auto *row = new QHBoxLayout;
    row->addStretch();
    row->addWidget(close);
    root->addLayout(row);

    connect(bAff, &QPushButton::clicked, this, &CalendarPage::onAffecter);
    connect(bSem, &QPushButton::clicked, this, &CalendarPage::onAffecterSemaine);
    connect(bRet, &QPushButton::clicked, this, &CalendarPage::onRetirer);
    connect(close, &QPushButton::clicked, this, &CalendarPage::retour);
    connect(m_cal, &QCalendarWidget::currentPageChanged, this, [this](int, int) { fillList(); });
    connect(m_cal, &QCalendarWidget::clicked, this, [this](const QDate &d) {
        const QString g = m_plan.value(d);
        const int i = kGardesJour.indexOf(g);
        if (i >= 0) m_combo->setCurrentIndex(i);
        m_info->setText(g.isEmpty() ? QString("%1 : aucune garde.").arg(d.toString("dddd d MMMM yyyy"))
                                    : QString("%1 : %2.").arg(d.toString("dddd d MMMM yyyy"), g));
    });

    Employe::assurerTablePlanning();
    refresh();
}

QColor CalendarPage::colorFor(const QString &garde) const
{
    if (garde.startsWith("Garde de jour")) return QColor("#f7dc7f");
    if (garde.startsWith("Garde de nuit")) return QColor("#9fb4d6");
    return QColor("#b9e8c0");
}

void CalendarPage::refresh()
{
    m_plan = Employe::chargerPlanning(m_emp.getId());
    colorize();
    fillList();
}

void CalendarPage::colorize()
{
    m_cal->setDateTextFormat(QDate(), QTextCharFormat());       // efface les anciennes couleurs
    for (auto it = m_plan.cbegin(); it != m_plan.cend(); ++it) {
        QTextCharFormat f;
        f.setBackground(colorFor(it.value()));
        f.setFontWeight(QFont::Bold);
        f.setForeground(QColor("#263238"));
        m_cal->setDateTextFormat(it.key(), f);
    }
}

void CalendarPage::fillList()
{
    m_list->clear();
    const int y = m_cal->yearShown(), m = m_cal->monthShown();
    for (auto it = m_plan.cbegin(); it != m_plan.cend(); ++it) {
        if (it.key().year() != y || it.key().month() != m) continue;
        auto *item = new QListWidgetItem(QString("%1 — %2").arg(it.key().toString("ddd dd/MM"), it.value()));
        item->setBackground(colorFor(it.value()));
        m_list->addItem(item);
    }
    if (m_list->count() == 0)
        m_list->addItem("Aucun jour planifié.");
}

void CalendarPage::onAffecter()
{
    const QDate d = m_cal->selectedDate();
    const QString g = m_combo->currentText();
    if (!Employe::affecterJour(m_emp.getId(), d, g)) {
        m_info->setText("⚠ Échec Oracle : " + Employe::derniereErreur().toHtmlEscaped());
        return;
    }
    refresh();
    m_info->setText(QString("✔ %1 → %2 (enregistré dans Oracle).").arg(d.toString("dddd d MMMM"), g));
}

void CalendarPage::onAffecterSemaine()
{
    const QDate d = m_cal->selectedDate();
    const QDate lundi = d.addDays(-(d.dayOfWeek() - 1));
    const QString g = m_combo->currentText();
    bool ok = true;
    for (int i = 0; i < 7 && ok; ++i) {
        const QDate j = lundi.addDays(i);
        const bool we = j.dayOfWeek() >= 6;
        if (g.startsWith("Astreinte") != we) continue;   // astreinte = samedi/dimanche, gardes = lundi-vendredi
        ok = Employe::affecterJour(m_emp.getId(), j, g);
    }
    if (!ok) {
        m_info->setText("⚠ Échec Oracle : " + Employe::derniereErreur().toHtmlEscaped());
        return;
    }
    refresh();
    m_info->setText(QString("✔ Semaine du %1 planifiée : %2.").arg(lundi.toString("dd/MM/yyyy"), g));
}

void CalendarPage::onRetirer()
{
    const QDate d = m_cal->selectedDate();
    if (!m_plan.contains(d)) {
        m_info->setText("Aucune garde à retirer pour ce jour.");
        return;
    }
    if (!Employe::retirerJour(m_emp.getId(), d)) {
        m_info->setText("⚠ Échec Oracle : " + Employe::derniereErreur().toHtmlEscaped());
        return;
    }
    refresh();
    m_info->setText(QString("Garde retirée pour le %1.").arg(d.toString("dddd d MMMM")));
}


// ===========================================================================
//  FACE ID (camera)
// ===========================================================================



namespace {
constexpr int kScanDurationMs = 3000;
constexpr int kScanStepMs = 50;
constexpr double kOvalRx = 0.19;   // demi-largeur du guide (fraction de la largeur)
constexpr double kOvalRy = 0.38;   // demi-hauteur du guide (fraction de la hauteur)
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QList<QVideoFrame::PixelFormat> FrameGrabber::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    Q_UNUSED(type);
    return {QVideoFrame::Format_RGB32, QVideoFrame::Format_ARGB32, QVideoFrame::Format_RGB24,
            QVideoFrame::Format_BGR24, QVideoFrame::Format_BGR32, QVideoFrame::Format_ARGB32_Premultiplied};
}

bool FrameGrabber::present(const QVideoFrame &frame)
{
    if (!frame.isValid())
        return false;
    QVideoFrame f(frame);
    if (!f.map(QAbstractVideoBuffer::ReadOnly))
        return false;
    const QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(f.pixelFormat());
    QImage img;
    if (fmt != QImage::Format_Invalid)
        img = QImage(f.bits(), f.width(), f.height(), f.bytesPerLine(), fmt).copy();
    f.unmap();
    if (!img.isNull())
        emit frameReady(img);
    return true;
}
#endif

FaceIdDialog::FaceIdDialog(const QVector<Employe> &employees, int preselectId, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Connexion Face ID");
    setModal(true);
    setStyleSheet("QDialog { background:#eef0f2; } QLabel { color:#37474f; }"
                  "QComboBox { border:1px solid #cfd4d9; border-radius:6px; padding:3px 8px; background:white; color:#263238; }");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(14, 12, 14, 12);
    root->setSpacing(8);

    auto *top = new QHBoxLayout;
    top->addWidget(new QLabel("<b>Agent :</b>"));
    m_combo = new QComboBox;
    m_combo->setMinimumWidth(300);
    m_combo->setMinimumHeight(30);
    for (const Employe &e : employees)
        m_combo->addItem(QString("%1 (n° %2) — %3").arg(e.nomComplet()).arg(e.getId()).arg(e.getStatut()), e.getId());
    const int idx = m_combo->findData(preselectId);
    if (idx >= 0) m_combo->setCurrentIndex(idx);
    top->addWidget(m_combo, 1);
    root->addLayout(top);

    m_view = new QLabel;
    m_view->setFixedSize(640, 480);
    m_view->setAlignment(Qt::AlignCenter);
    m_view->setStyleSheet("background:#1b2631; color:#cfd8dc; border-radius:8px;");
    m_view->setText("Ouverture de la caméra…");
    root->addWidget(m_view, 0, Qt::AlignCenter);

    m_bar = new QProgressBar;
    m_bar->setRange(0, 100);
    m_bar->setTextVisible(false);
    m_bar->setFixedHeight(10);
    m_bar->setStyleSheet("QProgressBar{background:#e3e6e9;border:none;border-radius:5px;}"
                         "QProgressBar::chunk{background:#2e9e4f;border-radius:5px;}");
    root->addWidget(m_bar);

    m_status = new QLabel("Placez votre visage dans l'ovale puis cliquez sur « Scanner le visage ».");
    m_status->setWordWrap(true);
    m_status->setAlignment(Qt::AlignCenter);
    root->addWidget(m_status);

    auto *btns = new QHBoxLayout;
    btns->addStretch();
    auto *cancel = new QPushButton("Annuler");
    cancel->setCursor(Qt::PointingHandCursor);
    cancel->setStyleSheet("QPushButton { background:#d9dcdf; color:#37474f; border:none; border-radius:6px; padding:8px 18px; }"
                          "QPushButton:hover { background:#c8ccd0; }");
    m_btnScan = new QPushButton("Scanner le visage");
    m_btnScan->setCursor(Qt::PointingHandCursor);
    m_btnScan->setEnabled(false);
    m_btnScan->setStyleSheet("QPushButton { background:#2f3e4e; color:white; border:none; border-radius:6px; padding:8px 18px; font-weight:bold; }"
                             "QPushButton:hover { background:#3d5166; } QPushButton:disabled { background:#9aa5b1; }");
    btns->addWidget(cancel);
    btns->addWidget(m_btnScan);
    root->addLayout(btns);

    m_scanTimer = new QTimer(this);
    m_scanTimer->setInterval(kScanStepMs);
    connect(m_scanTimer, &QTimer::timeout, this, &FaceIdDialog::tickScan);
    connect(m_btnScan, &QPushButton::clicked, this, &FaceIdDialog::startScan);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);

    if (employees.isEmpty()) {
        setStatus("⚠ Aucun employé dans la base : ajoutez d'abord un employé.", "#c62828");
        return;
    }
    startCamera();
}

FaceIdDialog::~FaceIdDialog()
{
    stopCamera();
}

void FaceIdDialog::closeEvent(QCloseEvent *e)
{
    stopCamera();
    QDialog::closeEvent(e);
}

void FaceIdDialog::setStatus(const QString &text, const QString &color)
{
    m_status->setText(text);
    m_status->setStyleSheet(QString("color:%1; font-weight:bold;").arg(color));
}

// ---------------------------------------------------------------------------
//  Camera
// ---------------------------------------------------------------------------
void FaceIdDialog::startCamera()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QCameraDevice dev = QMediaDevices::defaultVideoInput();
    if (dev.isNull()) {
        m_view->setText("Aucune caméra détectée.");
        setStatus("⚠ Aucune caméra détectée. Branchez une webcam et vérifiez Paramètres Windows > Confidentialité > Caméra.", "#c62828");
        return;
    }
    m_camera = new QCamera(dev, this);
    m_session = new QMediaCaptureSession(this);
    m_session->setCamera(m_camera);
    m_sink = new QVideoSink(this);
    m_session->setVideoSink(m_sink);
    connect(m_sink, &QVideoSink::videoFrameChanged, this, [this](const QVideoFrame &f) { onFrame(f.toImage()); });
    connect(m_camera, &QCamera::errorOccurred, this, [this](QCamera::Error, const QString &msg) {
        m_cameraOk = false;
        m_btnScan->setEnabled(false);
        setStatus("⚠ Erreur caméra : " + msg, "#c62828");
    });
#else
    if (QCameraInfo::availableCameras().isEmpty()) {
        m_view->setText("Aucune caméra détectée.");
        setStatus("⚠ Aucune caméra détectée. Branchez une webcam et vérifiez Paramètres Windows > Confidentialité > Caméra.", "#c62828");
        return;
    }
    m_camera = new QCamera(QCameraInfo::defaultCamera(), this);
    m_grabber = new FrameGrabber(this);
    m_camera->setViewfinder(m_grabber);
    connect(m_grabber, &FrameGrabber::frameReady, this, &FaceIdDialog::onFrame);
    connect(m_camera, QOverload<QCamera::Error>::of(&QCamera::error), this, [this](QCamera::Error) {
        m_cameraOk = false;
        m_btnScan->setEnabled(false);
        setStatus("⚠ Erreur caméra : " + m_camera->errorString(), "#c62828");
    });
#endif
    m_camera->start();
}

void FaceIdDialog::stopCamera()
{
    m_scanTimer->stop();
    m_scanning = false;
    if (m_camera) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (m_sink) m_sink->disconnect(this);
#else
        if (m_grabber) m_grabber->disconnect(this);
#endif
        m_camera->stop();
    }
}

// ---------------------------------------------------------------------------
//  Detection du visage (zone ovale : couleur de peau + relief des traits)
// ---------------------------------------------------------------------------
bool FaceIdDialog::looksLikeFace(const QImage &img)
{
    const QImage s = img.scaled(160, 120, Qt::IgnoreAspectRatio, Qt::FastTransformation)
                         .convertToFormat(QImage::Format_RGB32);
    const double cx = 80.0, cy = 60.0, rx = kOvalRx * 160.0, ry = kOvalRy * 120.0;
    int inside = 0, skin = 0;
    double sum = 0.0, sum2 = 0.0;
    for (int y = 0; y < s.height(); ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(s.constScanLine(y));
        for (int x = 0; x < s.width(); ++x) {
            const double dx = (x - cx) / rx, dy = (y - cy) / ry;
            if (dx * dx + dy * dy > 1.0) continue;
            const double r = qRed(line[x]), g = qGreen(line[x]), b = qBlue(line[x]);
            const double Y  = 0.299 * r + 0.587 * g + 0.114 * b;
            const double Cb = 128.0 - 0.168736 * r - 0.331264 * g + 0.5 * b;
            const double Cr = 128.0 + 0.5 * r - 0.418688 * g - 0.081312 * b;
            ++inside;
            sum += Y;
            sum2 += Y * Y;
            if (Cb >= 77 && Cb <= 127 && Cr >= 133 && Cr <= 173 && Y > 50) ++skin;
        }
    }
    if (inside == 0) return false;
    const double ratio = double(skin) / inside;
    const double mean = sum / inside;
    const double sd = std::sqrt(qMax(0.0, sum2 / inside - mean * mean));
    return ratio >= 0.45 && sd >= 14.0 && mean > 45.0 && mean < 235.0;
}

void FaceIdDialog::onFrame(const QImage &raw)
{
    if (raw.isNull()) return;
    if (!m_cameraOk) {
        m_cameraOk = true;
        if (!m_scanning) {
            m_btnScan->setEnabled(true);
            setStatus("Caméra prête : placez votre visage dans l'ovale puis cliquez sur « Scanner le visage ».");
        }
    }

    const QImage img = raw.convertToFormat(QImage::Format_RGB32).mirrored(true, false);   // effet miroir
    if (m_scanning) {
        m_lastOk = looksLikeFace(img);
        ++m_framesTotal;
        if (m_lastOk) ++m_framesOk;
    }

    QImage out = img.scaled(m_view->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)
                     .convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing);
    const double W = out.width(), H = out.height();
    const QRectF oval(W * (0.5 - kOvalRx), H * (0.5 - kOvalRy), W * 2 * kOvalRx, H * 2 * kOvalRy);

    QColor c = Qt::white;
    if (m_scanning) c = m_lastOk ? QColor("#43d17a") : QColor("#ffb300");
    p.setPen(QPen(c, 3, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(oval);

    if (m_scanning) {                                     // ligne de balayage
        const double t = double(m_elapsedMs % 1000) / 1000.0;
        const double yy = oval.top() + oval.height() * (t < 0.5 ? t * 2 : (1 - t) * 2);
        p.setClipRegion(QRegion(oval.toAlignedRect(), QRegion::Ellipse));
        p.setPen(QPen(QColor(67, 209, 122, 230), 3));
        p.drawLine(QPointF(oval.left(), yy), QPointF(oval.right(), yy));
    }
    p.end();
    m_view->setPixmap(QPixmap::fromImage(out));
}

// ---------------------------------------------------------------------------
//  Scan
// ---------------------------------------------------------------------------
void FaceIdDialog::startScan()
{
    if (!m_cameraOk || m_scanning) return;
    m_scanning = true;
    m_elapsedMs = 0;
    m_framesTotal = 0;
    m_framesOk = 0;
    m_lastOk = false;
    m_bar->setValue(0);
    m_btnScan->setEnabled(false);
    m_combo->setEnabled(false);
    setStatus("Scan en cours… restez face à la caméra, sans bouger.");
    m_scanTimer->start();
}

void FaceIdDialog::tickScan()
{
    m_elapsedMs += kScanStepMs;
    m_bar->setValue(qMin(100, m_elapsedMs * 100 / kScanDurationMs));
    if (m_elapsedMs < kScanDurationMs) return;

    m_scanTimer->stop();
    m_scanning = false;
    m_combo->setEnabled(true);

    const bool ok = m_framesTotal >= 8 && m_framesOk * 100 >= m_framesTotal * 60;
    if (!ok) {
        m_btnScan->setEnabled(true);
        m_bar->setValue(0);
        setStatus(m_framesTotal < 8
                      ? "⚠ Image de la caméra insuffisante. Réessayez."
                      : "⚠ Visage non reconnu : placez-vous face à la caméra, dans l'ovale, avec une bonne lumière.",
                  "#c62828");
        return;
    }
    m_chosenId = m_combo->currentData().toInt();
    setStatus("✔ Visage reconnu.", "#1b6b2b");
    m_btnScan->setEnabled(false);
    QTimer::singleShot(700, this, [this] { stopCamera(); accept(); });
}
