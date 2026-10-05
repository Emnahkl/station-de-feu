#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrgen.h"

#include <QDesktopServices>
#include <QEvent>
#include <QLinearGradient>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QScrollBar>
#include <QStatusBar>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <algorithm>

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

QString codeFor(int id) { return QString("FS-AGT-%1").arg(id, 3, 10, QChar('0')); }

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

// Vrai QR Code (version 1, niveau L) contenant le code de l'agent
QPixmap makeQrPixmap(const QString &text, int scale)
{
    const auto m = SimpleQr::encode(text.toStdString());
    const int n = int(m.size()), quiet = 2;
    const int size = (n + 2 * quiet) * scale;
    QPixmap pm(size, size);
    pm.fill(Qt::white);
    QPainter p(&pm);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            if (m[r][c])
                p.drawRect((c + quiet) * scale, (r + quiet) * scale, scale, scale);
    return pm;
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
    seedData();

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
#filterFrame, #actionFrame, #formPanel, #chatFrame, #planFrame, #commFrame
    { background:white; border:1px solid #d9dde1; border-radius:8px; }
#labelChatTitle, #labelPlanTitle, #labelCommTitle, #labelFormTitle
    { font-weight:bold; font-size:14px; color:#37474f; }
#labelFormInfo { color:#455a64; font-size:12px; }
QLineEdit, QComboBox { border:1px solid #cfd4d9; border-radius:6px; padding:3px 8px; background:white; color:#263238; }

/* boutons-images */
#btnQr, #btnFaceId, #btnAjouterVolontaire, #btnAjouterEmploye, #btnSupprimerEmploye
    { border:none; background:transparent; }
#btnQr:hover, #btnFaceId:hover, #btnAjouterVolontaire:hover, #btnAjouterEmploye:hover, #btnSupprimerEmploye:hover
    { background:rgba(0,0,0,0.07); border-radius:8px; }

/* boutons du formulaire / panneaux */
#btnSendChat, #btnFormValider, #btnGenererPlanning
    { background:#2f3e4e; color:white; border:none; border-radius:6px; padding:7px 16px; font-weight:bold; }
#btnSendChat:hover, #btnFormValider:hover, #btnGenererPlanning:hover { background:#3d5166; }
#btnFormValider:disabled, #btnSendChat:disabled { background:#9aa5b1; }
#btnFormAnnuler { background:#d9dcdf; color:#37474f; border:none; border-radius:6px; padding:7px 16px; }
#btnFormAnnuler:hover { background:#c8ccd0; }
#btnNotifier  { background:#dbe8f5; color:#1d4f7c; border:none; border-radius:5px; padding:6px 4px; font-size:11px; }
#btnAlerteGrade { background:#fbe0dc; color:#b3261e; border:none; border-radius:5px; padding:6px 4px; font-size:11px; }
#btnEnvoyerEmail { background:#e53935; color:white; border:none; border-radius:6px; padding:7px 12px; font-weight:bold; }
#btnEnvoyerSms   { background:#2f6fd0; color:white; border:none; border-radius:6px; padding:7px 12px; font-weight:bold; }
#btnEnvoyerWhats { background:#2eaf57; color:white; border:none; border-radius:6px; padding:7px 12px; font-weight:bold; }
#labelDispo, #labelProgress, #labelUnite, #labelP1, #labelP2, #labelP3, #labelEmail, #labelSms { font-size:11px; }

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

    // Les 7 premiers boutons ont une pastille symbole ; "Paramètres" utilise parametre.png
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
        connect(b, &QPushButton::clicked, this, [this, b] {
            QString t = b->text();
            t.replace('\n', ' ');
            statusBar()->showMessage(QString("Module « %1 » : hors de cette tâche (gestion des employés uniquement).").arg(t), 4000);
        });
    }
    ui->btnMenuCarte->setChecked(true);   // comme sur la maquette
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
        for (const Employee &e : qAsConst(m_employees)) if (e.statut == "En formation") ++n;
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
    setImg(ui->btnQr,                loadPx("qrcode"),        "Scanner QR Code (Check-in)");
    setImg(ui->btnFaceId,            loadPx("faceid"),        "Connexion Face ID");
    setImg(ui->btnAjouterVolontaire, loadPx("ajoutervolant"), "Ajouter un volontaire");
    setImg(ui->btnAjouterEmploye,    loadPx("ajouter"),       "Ajouter un employé");
    setImg(ui->btnSupprimerEmploye,  loadPx("supprimer"),     "Supprimer un employé");

    connect(ui->btnQr,                &QPushButton::clicked, this, [this] { openForm(Mode::Qr); });
    connect(ui->btnFaceId,            &QPushButton::clicked, this, [this] { openForm(Mode::Face); });
    connect(ui->btnAjouterVolontaire, &QPushButton::clicked, this, [this] { openForm(Mode::AddVolunteer); });
    connect(ui->btnAjouterEmploye,    &QPushButton::clicked, this, [this] { openForm(Mode::Add); });
    connect(ui->btnSupprimerEmploye,  &QPushButton::clicked, this, [this] { openForm(Mode::Delete, selectedEmployeeId()); });
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

    connect(ui->btnFormValider, &QPushButton::clicked, this, &MainWindow::onFormValider);
    connect(ui->btnFormAnnuler, &QPushButton::clicked, this, &MainWindow::closeForm);
    connect(ui->lineEditNom,  &QLineEdit::returnPressed, this, &MainWindow::onFormValider);
    connect(ui->lineEditCode, &QLineEdit::returnPressed, this, &MainWindow::onFormValider);
    ui->formPanel->setVisible(false);
}

void MainWindow::setupTable()
{
    QTableWidget *t = ui->tableEmployees;
    t->setColumnCount(7);
    t->setHorizontalHeaderLabels({"Avatar", "Nom", "Grade", "Spécialité", "Statut de Présence", "Code QR", "Emploi du temps"});
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

    const int widths[7] = {60, 0, 124, 118, 128, 62, 204};
    QHeaderView *h = t->horizontalHeader();
    for (int c = 0; c < 7; ++c) {
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
        for (const Employee &e : qAsConst(m_employees)) if (e.specialite == "Medic" && e.statut != "En formation") ++n;
        statusBar()->showMessage(QString("Notification envoyée au Lt. Médecin : %1 agent(s) Medic disponible(s).").arg(n), 5000);
    });
    connect(ui->btnAlerteGrade, &QPushButton::clicked, this, [this] {
        int n = 0;
        for (const Employee &e : qAsConst(m_employees)) if (e.statut == "En formation") ++n;
        statusBar()->showMessage(QString("Alerte S/L envoyée : %1 agent(s) en formation (fin de formation / aptitude).").arg(n), 5000);
    });
    connect(ui->btnEnvoyerEmail, &QPushButton::clicked, this, &MainWindow::onEnvoyerEmail);
    connect(ui->btnEnvoyerSms,   &QPushButton::clicked, this, &MainWindow::onEnvoyerSms);
    connect(ui->btnEnvoyerWhats, &QPushButton::clicked, this, &MainWindow::onEnvoyerWhatsApp);
    ui->lineEditSms->setPlaceholderText("216XXXXXXXX ; Message");

    for (QProgressBar *b : {ui->barPlan1, ui->barPlan2, ui->barPlan3, ui->barDispo, ui->barProgress}) {
        b->setRange(0, 100);
        b->setTextVisible(false);
    }
}

void MainWindow::seedData()
{
    struct Seed { int grade; const char *spec; const char *statut; const char *garde; };
    const Seed seeds[12] = {
        {1, "Fire",      "En service",   "Garde de jour (08h-20h)"},
        {2, "Medic",     "En pause",     "Garde de jour (08h-20h)"},
        {2, "Medic",     "En pause",     "Garde de nuit (20h-08h)"},
        {2, "Logistics", "En formation", "Aucune"},
        {2, "Medic",     "En pause",     "Garde de nuit (20h-08h)"},
        {2, "Logistics", "En formation", "Aucune"},
        {2, "Logistics", "En formation", "Astreinte week-end"},
        {3, "Fire",      "En service",   "Garde de jour (08h-20h)"},
        {3, "Logistics", "En pause",     "Aucune"},
        {3, "Logistics", "En formation", "Aucune"},
        {3, "Logistics", "En formation", "Aucune"},
        {3, "Medic",     "En service",   "Garde de nuit (20h-08h)"},
    };
    for (const Seed &s : seeds) {
        Employee e;
        e.id = m_nextId++;
        e.nom = QString("Agent %1").arg(e.id);
        e.grade = s.grade;
        e.specialite = s.spec;
        e.statut = s.statut;
        e.garde = s.garde;
        m_employees.append(e);
    }
}

// ---------------------------------------------------------------------------
//  Logique employes
// ---------------------------------------------------------------------------
Employee *MainWindow::findEmployee(int id)
{
    for (Employee &e : m_employees)
        if (e.id == id) return &e;
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

QString MainWindow::pointage(Employee &e)
{
    const QString heure = QTime::currentTime().toString("HH:mm");
    if (e.statut == "En service") {
        e.statut = "En pause";
        return QString("↩ Check-out : %1 (%2) à %3 → statut « En pause ».").arg(e.nom, codeFor(e.id), heure);
    }
    e.statut = "En service";
    return QString("✔ Check-in : %1 (%2) à %3 → statut « En service ».").arg(e.nom, codeFor(e.id), heure);
}

void MainWindow::openForm(Mode mode, int id)
{
    m_mode = mode;
    m_currentId = id;
    Employee *e = (id >= 0) ? findEmployee(id) : nullptr;

    auto vis = [this](bool nom, bool grade, bool spec, bool statut, bool target, bool garde, bool code) {
        ui->labelNom->setVisible(nom);              ui->lineEditNom->setVisible(nom);
        ui->labelFormGrade->setVisible(grade);      ui->comboFormGrade->setVisible(grade);
        ui->labelFormSpec->setVisible(spec);        ui->comboFormSpec->setVisible(spec);
        ui->labelFormStatut->setVisible(statut);    ui->comboFormStatut->setVisible(statut);
        ui->labelFormTarget->setVisible(target);    ui->comboFormTarget->setVisible(target);
        ui->labelFormGarde->setVisible(garde);      ui->comboFormGarde->setVisible(garde);
        ui->labelFormCode->setVisible(code);        ui->lineEditCode->setVisible(code);
    };

    ui->lineEditNom->clear();
    ui->lineEditCode->clear();
    ui->comboFormGrade->setCurrentIndex(0);
    ui->comboFormSpec->setCurrentIndex(0);
    ui->comboFormStatut->setCurrentIndex(0);
    ui->labelFormQr->setVisible(false);
    ui->btnFormValider->setVisible(true);
    ui->btnFormValider->setEnabled(true);
    ui->btnFormValider->setStyleSheet(QString());
    ui->btnFormAnnuler->setText("Annuler");
    setInfo(QString());

    switch (mode) {
    case Mode::Add:
        ui->labelFormTitle->setText("Ajouter un employé");
        vis(true, true, true, true, false, false, false);
        ui->btnFormValider->setText("Ajouter");
        ui->lineEditNom->setFocus();
        break;
    case Mode::AddVolunteer:
        ui->labelFormTitle->setText("Ajouter un volontaire");
        vis(true, true, true, true, false, false, false);
        ui->btnFormValider->setText("Ajouter");
        setInfo("Le volontaire apparaîtra dans le tableau avec la mention « Volontaire ».");
        ui->lineEditNom->setFocus();
        break;
    case Mode::Edit:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Modifier : %1").arg(e->nom));
        vis(true, true, true, true, false, false, false);
        ui->lineEditNom->setText(e->nom);
        ui->comboFormGrade->setCurrentIndex(qBound(0, e->grade - 1, 2));
        ui->comboFormSpec->setCurrentIndex(qMax(0, kSpecs.indexOf(e->specialite)));
        ui->comboFormStatut->setCurrentIndex(qMax(0, kStatuts.indexOf(e->statut)));
        ui->btnFormValider->setText("Enregistrer");
        break;
    case Mode::View:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Fiche : %1").arg(e->nom));
        vis(false, false, false, false, false, false, false);
        ui->labelFormQr->setPixmap(makeQrPixmap(codeFor(e->id), 3));
        ui->labelFormQr->setVisible(true);
        setInfo(QString("<b>%1</b>%2<br>Code QR : %3<br>Grade : %4 étoile(s) · Spécialité : %5<br>Statut : %6<br>Emploi du temps : %7")
                    .arg(e->nom, e->volontaire ? QString(" (Volontaire)") : QString(), codeFor(e->id))
                    .arg(e->grade).arg(e->specialite, e->statut, e->garde));
        ui->btnFormValider->setVisible(false);
        ui->btnFormAnnuler->setText("Fermer");
        break;
    case Mode::Assign:
        if (!e) return;
        ui->labelFormTitle->setText(QString("Assigner un emploi du temps : %1").arg(e->nom));
        vis(false, false, false, false, false, true, false);
        ui->comboFormGarde->setCurrentIndex(qMax(0, kGardes.indexOf(e->garde)));
        if (e->statut == "En formation")
            setInfo("⚠ Cet agent est en formation : l'assignation reste possible mais déconseillée.");
        ui->btnFormValider->setText("Assigner");
        break;
    case Mode::Delete:
        ui->labelFormTitle->setText("Supprimer un employé");
        vis(false, false, false, false, true, false, false);
        ui->comboFormTarget->clear();
        for (const Employee &x : qAsConst(m_employees))
            ui->comboFormTarget->addItem(QString("%1 (%2)").arg(x.nom, codeFor(x.id)), x.id);
        if (id >= 0) ui->comboFormTarget->setCurrentIndex(qMax(0, ui->comboFormTarget->findData(id)));
        setInfo("Choisissez l'employé à retirer de la liste (ou sélectionnez d'abord une ligne). Action définitive.");
        ui->btnFormValider->setText("Supprimer");
        ui->btnFormValider->setStyleSheet("background:#c62828;color:white;border:none;border-radius:6px;padding:7px 16px;font-weight:bold;");
        break;
    case Mode::Qr:
        ui->labelFormTitle->setText("Scanner QR Code (Check-in / Check-out)");
        vis(false, false, false, false, false, false, true);
        setInfo("Saisissez ou scannez (lecteur code-barres) le code de l'agent. 1er passage = check-in (En service), 2e passage = check-out (En pause).");
        ui->btnFormValider->setText("Pointer");
        ui->lineEditCode->setFocus();
        break;
    case Mode::Face:
        ui->labelFormTitle->setText("Connexion Face ID");
        vis(false, false, false, false, true, false, false);
        ui->comboFormTarget->clear();
        for (const Employee &x : qAsConst(m_employees))
            ui->comboFormTarget->addItem(QString("%1 (%2)").arg(x.nom, codeFor(x.id)), x.id);
        setInfo("Choisissez l'agent puis lancez la vérification du visage.");
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
        const QString nom = ui->lineEditNom->text().trimmed();
        if (nom.isEmpty()) { setInfo("⚠ Veuillez saisir le nom de l'employé."); return; }
        Employee e;
        e.id = m_nextId++;
        e.nom = nom;
        e.grade = ui->comboFormGrade->currentData().toInt();
        e.specialite = ui->comboFormSpec->currentData().toString();
        e.statut = ui->comboFormStatut->currentText();
        e.volontaire = (m_mode == Mode::AddVolunteer);
        m_employees.append(e);
        refreshTable();
        statusBar()->showMessage(QString("%1 ajouté (%2).").arg(nom, codeFor(e.id)), 4000);
        closeForm();
        break;
    }
    case Mode::Edit: {
        Employee *e = findEmployee(m_currentId);
        const QString nom = ui->lineEditNom->text().trimmed();
        if (!e) { closeForm(); return; }
        if (nom.isEmpty()) { setInfo("⚠ Le nom ne peut pas être vide."); return; }
        e->nom = nom;
        e->grade = ui->comboFormGrade->currentData().toInt();
        e->specialite = ui->comboFormSpec->currentData().toString();
        e->statut = ui->comboFormStatut->currentText();
        refreshTable();
        statusBar()->showMessage(QString("%1 modifié.").arg(nom), 4000);
        closeForm();
        break;
    }
    case Mode::Assign: {
        Employee *e = findEmployee(m_currentId);
        if (!e) { closeForm(); return; }
        e->garde = ui->comboFormGarde->currentText();
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 → %2.").arg(e->nom, e->garde), 4000);
        closeForm();
        break;
    }
    case Mode::Delete: {
        const int id = ui->comboFormTarget->currentData().toInt();
        Employee *e = findEmployee(id);
        if (!e) { setInfo("⚠ Aucun employé à supprimer."); return; }
        const QString nom = e->nom;
        m_employees.erase(std::remove_if(m_employees.begin(), m_employees.end(),
                                         [id](const Employee &x) { return x.id == id; }),
                          m_employees.end());
        refreshTable();
        updatePlanning();
        statusBar()->showMessage(QString("%1 supprimé.").arg(nom), 4000);
        closeForm();
        break;
    }
    case Mode::Qr: {
        const QString code = ui->lineEditCode->text().trimmed().toUpper();
        Employee *found = nullptr;
        for (Employee &x : m_employees)
            if (codeFor(x.id) == code) { found = &x; break; }
        if (!found) { setInfo(QString("⚠ Code « %1 » inconnu.").arg(code.toHtmlEscaped())); return; }
        const QString msg = pointage(*found);
        refreshTable();
        updatePlanning();
        setInfo(msg);
        statusBar()->showMessage(msg, 5000);
        ui->lineEditCode->clear();
        ui->lineEditCode->setFocus();
        break;
    }
    case Mode::Face: {
        const int id = ui->comboFormTarget->currentData().toInt();
        if (!findEmployee(id)) { setInfo("⚠ Aucun agent sélectionné."); return; }
        ui->btnFormValider->setEnabled(false);
        setInfo("Analyse du visage en cours…");
        QTimer::singleShot(1300, this, [this, id] {
            Employee *e = findEmployee(id);
            ui->btnFormValider->setEnabled(true);
            if (!e || m_mode != Mode::Face) return;
            const QString msg = pointage(*e);
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

    QVector<Employee> rows;
    for (const Employee &e : qAsConst(m_employees)) {
        if (g > 0 && e.grade != g) continue;
        if (!sp.isEmpty() && e.specialite != sp) continue;
        if (!q.isEmpty() && !e.nom.contains(q, Qt::CaseInsensitive)
                         && !e.specialite.contains(q, Qt::CaseInsensitive)
                         && !codeFor(e.id).contains(q, Qt::CaseInsensitive)) continue;
        rows.append(e);
    }

    t->setRowCount(0);
    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        const Employee &e = rows[r];
        t->setRowHeight(r, 60);

        // 0 : avatar
        auto *av = new QLabel;
        av->setPixmap(m_avatar.scaled(44, 44, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        t->setCellWidget(r, 0, centered(av));

        // 1 : nom
        auto *it = new QTableWidgetItem(e.nom + (e.volontaire ? "  (Volontaire)" : ""));
        it->setData(Qt::UserRole, e.id);
        it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        t->setItem(r, 1, it);

        // 2 : grade (image 1etoile / 2etoile / 3etoile)
        auto *gl = new QLabel;
        gl->setPixmap(m_gradeBtn[qBound(1, e.grade, 3)].scaledToHeight(36, Qt::SmoothTransformation));
        t->setCellWidget(r, 2, centered(gl));

        // 3 : specialite (icone + texte)
        auto *spBox = new QWidget;
        auto *spL = new QHBoxLayout(spBox);
        spL->setContentsMargins(8, 0, 4, 0);
        spL->setSpacing(6);
        auto *spIcon = new QLabel;
        const int si = qMax(0, kSpecs.indexOf(e.specialite));
        spIcon->setPixmap(m_specPx[si].scaled(26, 26, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        auto *spText = new QLabel(e.specialite);
        spText->setStyleSheet("font-weight:bold;color:#263238;");
        spL->addWidget(spIcon);
        spL->addWidget(spText);
        spL->addStretch();
        t->setCellWidget(r, 3, spBox);

        // 4 : statut de presence
        auto *st = new QLabel(e.statut);
        st->setAlignment(Qt::AlignCenter);
        st->setStyleSheet(statusStyle(e.statut));
        t->setCellWidget(r, 4, centered(st));

        // 5 : QR Code reel
        auto *qr = new QLabel;
        qr->setPixmap(makeQrPixmap(codeFor(e.id), 2));
        qr->setToolTip(codeFor(e.id));
        t->setCellWidget(r, 5, centered(qr));

        // 6 : emploi du temps (Visualiser / Modifier / Assigner)
        auto *act = new QWidget;
        auto *al = new QHBoxLayout(act);
        al->setContentsMargins(4, 0, 4, 0);
        al->setSpacing(5);
        auto mk = [this, act, al, id = e.id](const QString &txt, Mode m) {
            auto *b = new QPushButton(txt, act);
            b->setObjectName("rowBtn");
            b->setCursor(Qt::PointingHandCursor);
            connect(b, &QPushButton::clicked, this, [this, m, id] { openForm(m, id); });
            al->addWidget(b);
        };
        mk("Visualiser", Mode::View);
        mk("Modifier", Mode::Edit);
        mk("Assigner", Mode::Assign);
        t->setCellWidget(r, 6, act);
    }

    // badge de la cloche = agents en formation
    int n = 0;
    for (const Employee &e : qAsConst(m_employees)) if (e.statut == "En formation") ++n;
    if (m_badge) { m_badge->setText(QString::number(n)); m_badge->setVisible(n > 0); }
}

// ---------------------------------------------------------------------------
//  Smart Planning IA (regles simples, sans base de donnees)
// ---------------------------------------------------------------------------
void MainWindow::updatePlanning()
{
    const int total = m_employees.size();
    int dispo = 0, assigned = 0;
    int perShift[3] = {0, 0, 0};
    for (const Employee &e : qAsConst(m_employees)) {
        if (e.statut != "En formation") ++dispo;
        const int gi = kGardes.indexOf(e.garde);
        if (gi >= 0 && gi < 3) { ++assigned; ++perShift[gi]; }
    }
    const int cap = 4;
    QProgressBar *bars[3] = {ui->barPlan1, ui->barPlan2, ui->barPlan3};
    for (int i = 0; i < 3; ++i) {
        const int v = qMin(100, perShift[i] * 100 / cap);
        bars[i]->setValue(v);
        const QString color = v >= 75 ? "#2e9e4f" : "#c62828";
        bars[i]->setStyleSheet(QString("QProgressBar{background:#e3e6e9;border:none;border-radius:4px;}"
                                       "QProgressBar::chunk{background:%1;border-radius:4px;}").arg(color));
    }
    ui->barDispo->setValue(total ? dispo * 100 / total : 0);
    ui->barProgress->setValue(total ? assigned * 100 / total : 0);
    ui->barDispo->setStyleSheet("QProgressBar{background:#e3e6e9;border:none;border-radius:3px;}QProgressBar::chunk{background:#2e9e4f;border-radius:3px;}");
    ui->barProgress->setStyleSheet("QProgressBar{background:#e3e6e9;border:none;border-radius:3px;}QProgressBar::chunk{background:#c62828;border-radius:3px;}");
}

void MainWindow::onGenererPlanning()
{
    for (Employee &e : m_employees) e.garde = "Aucune";

    QVector<int> pool;                      // agents disponibles, meilleurs grades d'abord
    for (int i = 0; i < m_employees.size(); ++i)
        if (m_employees[i].statut != "En formation") pool.append(i);
    std::stable_sort(pool.begin(), pool.end(), [this](int a, int b) {
        return m_employees[a].grade > m_employees[b].grade;
    });

    const int cap = 4;
    int counts[3] = {0, 0, 0};
    for (int s = 0; s < 3; ++s) {
        bool any = true;
        while (counts[s] < cap && any) {
            any = false;
            for (const QString &spec : kSpecs) {
                if (counts[s] >= cap) break;
                for (int idx : qAsConst(pool)) {
                    Employee &e = m_employees[idx];
                    if (e.garde == "Aucune" && e.specialite == spec) {
                        e.garde = kGardes[s];
                        ++counts[s];
                        any = true;
                        break;
                    }
                }
            }
        }
    }
    refreshTable();
    updatePlanning();
    const QString msg = QString("Planning généré : jour %1/%4, nuit %2/%4, week-end %3/%4. "
                                "Les agents en formation sont exclus ; spécialités équilibrées, grades élevés prioritaires.")
                            .arg(counts[0]).arg(counts[1]).arg(counts[2]).arg(cap);
    addBubble(msg, false);
    statusBar()->showMessage("Planning de garde généré.", 4000);
}

// ---------------------------------------------------------------------------
//  Chatbot RH (reponses par regles, sans base de donnees)
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
    bubble->setMaximumWidth(210);
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
        for (const Employee &e : m_employees) if (e.statut == statut) l << e.nom;
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
        for (const Employee &e : m_employees) if (e.volontaire) ++n;
        return QString("%1 volontaire(s) enregistré(s).").arg(n);
    }
    for (const QString &spec : kSpecs) {
        if (q.contains(spec.toLower().left(5))) {
            int n = 0;
            for (const Employee &e : m_employees) if (e.specialite == spec) ++n;
            return QString("%1 agent(s) de spécialité %2.").arg(n).arg(spec);
        }
    }
    if (has({"grade", "étoile", "etoile"})) {
        int c[4] = {0, 0, 0, 0};
        for (const Employee &e : m_employees) ++c[qBound(1, e.grade, 3)];
        return QString("Répartition par grade : 1 étoile = %1, 2 étoiles = %2, 3 étoiles = %3.").arg(c[1]).arg(c[2]).arg(c[3]);
    }
    if (has({"planning", "garde"}))
        return "Cliquez sur « Générer Planning » : je répartis les agents disponibles par grade et spécialité (jour / nuit / week-end).";
    if (has({"qr", "pointage", "check"}))
        return "Cliquez sur « Scanner QRCode (Check-in) » puis saisissez le code (ex. FS-AGT-001). Un second passage fait le check-out.";
    if (has({"combien", "nombre", "effectif", "total"})) {
        int s = 0, p = 0, f = 0;
        for (const Employee &e : m_employees) {
            if (e.statut == "En service") ++s;
            else if (e.statut == "En pause") ++p;
            else ++f;
        }
        return QString("Effectif total : %1 (en service %2, en pause %3, en formation %4).").arg(m_employees.size()).arg(s).arg(p).arg(f);
    }
    return "Je n'ai pas compris. Essayez : « combien d'agents », « qui est en pause », « agents Medic », « répartition par grade », « planning ».";
}

// ---------------------------------------------------------------------------
//  Communication directe (ouvre l'application mail / SMS / WhatsApp du systeme)
// ---------------------------------------------------------------------------
void MainWindow::onEnvoyerEmail()
{
    const QString to = ui->lineEditEmail->text().trimmed();
    if (to.isEmpty()) { statusBar()->showMessage("Saisissez au moins une adresse email.", 3000); return; }
    QUrl url;
    url.setScheme("mailto");
    url.setPath(to);
    QUrlQuery q;
    q.addQueryItem("subject", "Fire Station – Communication");
    url.setQuery(q);
    QDesktopServices::openUrl(url);
    statusBar()->showMessage("Ouverture de l'application mail…", 3000);
}

static void splitNumMsg(const QString &raw, QString &num, QString &msg)
{
    const int i = raw.indexOf(';');
    num = (i < 0 ? raw : raw.left(i)).trimmed();
    msg = (i < 0 ? QString() : raw.mid(i + 1)).trimmed();
}

void MainWindow::onEnvoyerSms()
{
    QString num, msg;
    splitNumMsg(ui->lineEditSms->text(), num, msg);
    if (num.isEmpty()) { statusBar()->showMessage("Format : numéro ; message", 3000); return; }
    QDesktopServices::openUrl(QUrl("sms:" + num + "?body=" + QString::fromUtf8(QUrl::toPercentEncoding(msg))));
    statusBar()->showMessage("Ouverture de l'application SMS…", 3000);
}

void MainWindow::onEnvoyerWhatsApp()
{
    QString num, msg;
    splitNumMsg(ui->lineEditSms->text(), num, msg);
    QString digits;
    for (const QChar &c : num) if (c.isDigit()) digits += c;
    if (digits.isEmpty()) { statusBar()->showMessage("Format : 216XXXXXXXX ; message (avec indicatif pays)", 4000); return; }
    QDesktopServices::openUrl(QUrl("https://wa.me/" + digits + "?text=" + QString::fromUtf8(QUrl::toPercentEncoding(msg))));
    statusBar()->showMessage("Ouverture de WhatsApp…", 3000);
}
