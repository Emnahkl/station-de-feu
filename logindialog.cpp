#include "logindialog.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setObjectName("LoginDialog");
    setWindowTitle(tr("FireStation Manager - Connexion"));
    setWindowIcon(QIcon(":/images/logo.png"));
    setFixedSize(860, 520);

    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ---- Partie gauche : visuel (même image que la sidebar) ----
    auto *visuel = new QFrame;
    visuel->setObjectName("loginVisuel");
    visuel->setFixedWidth(330);
    auto *lv = new QVBoxLayout(visuel);
    lv->setContentsMargins(28, 36, 28, 32);
    auto *logo = new QLabel;
    logo->setFixedSize(130, 130);
    logo->setPixmap(QPixmap(":/images/logo.png"));
    logo->setScaledContents(true);
    lv->addWidget(logo, 0, Qt::AlignHCenter);
    auto *nomApp = new QLabel(tr("FIRE STATION"));
    nomApp->setObjectName("loginNomApp");
    nomApp->setAlignment(Qt::AlignCenter);
    lv->addWidget(nomApp);
    auto *slogan = new QLabel(QString::fromUtf8("PRÊTS · PROTÉGER · SAUVER"));
    slogan->setObjectName("loginSlogan");
    slogan->setAlignment(Qt::AlignCenter);
    lv->addWidget(slogan);
    lv->addStretch(1);
    root->addWidget(visuel);

    // ---- Partie droite : formulaire ----
    auto *formulaire = new QFrame;
    formulaire->setObjectName("loginForm");
    auto *lf = new QVBoxLayout(formulaire);
    lf->setContentsMargins(56, 60, 56, 48);
    lf->setSpacing(10);

    auto *titre = new QLabel(tr("Connexion"));
    titre->setObjectName("loginTitre");
    lf->addWidget(titre);
    auto *sousTitre = new QLabel(tr("Accédez à l'espace de gestion de la caserne."));
    sousTitre->setObjectName("loginSousTitre");
    lf->addWidget(sousTitre);
    lf->addSpacing(22);

    auto *lblNom = new QLabel(tr("Nom d'utilisateur"));
    lblNom->setObjectName("loginLabel");
    lf->addWidget(lblNom);
    m_nom = new QLineEdit;
    m_nom->setObjectName("loginNom");
    m_nom->setPlaceholderText(tr("Ex. : rayen.guesmi"));
    m_nom->setClearButtonEnabled(true);
    lf->addWidget(m_nom);
    lf->addSpacing(8);

    auto *lblMdp = new QLabel(tr("Mot de passe"));
    lblMdp->setObjectName("loginLabel");
    lf->addWidget(lblMdp);
    m_motDePasse = new QLineEdit;
    m_motDePasse->setObjectName("loginMotDePasse");
    m_motDePasse->setEchoMode(QLineEdit::Password);
    m_motDePasse->setPlaceholderText(QString::fromUtf8("••••••••"));
    lf->addWidget(m_motDePasse);

    m_erreur = new QLabel;
    m_erreur->setObjectName("loginErreur");
    m_erreur->hide();
    lf->addWidget(m_erreur);
    lf->addSpacing(18);

    auto *btn = new QPushButton(tr("Se connecter"));
    btn->setObjectName("btnSeConnecter");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setDefault(true);
    lf->addWidget(btn);
    lf->addStretch(1);

    auto *pied = new QLabel(QString::fromUtf8("Fire Station · Caserne Centrale · Tunis"));
    pied->setObjectName("loginPied");
    pied->setAlignment(Qt::AlignCenter);
    lf->addWidget(pied);
    root->addWidget(formulaire, 1);

    connect(btn, &QPushButton::clicked, this, &LoginDialog::seConnecter);
    connect(m_nom, &QLineEdit::returnPressed, m_motDePasse, qOverload<>(&QWidget::setFocus));
    connect(m_motDePasse, &QLineEdit::returnPressed, this, &LoginDialog::seConnecter);

    setStyleSheet(R"(
        QDialog#LoginDialog { background: #ffffff; }
        QFrame#loginVisuel { border-image: url(:/images/sidebar_bg.png) 0 0 0 0 stretch stretch; }
        QFrame#loginVisuel QLabel { background: transparent; color: #ffffff; }
        QLabel#loginNomApp { font-size: 24px; font-weight: bold; letter-spacing: 1px; margin-top: 12px; }
        QLabel#loginSlogan { font-size: 11px; font-weight: bold; letter-spacing: 1px; color: #f6dada; }
        QFrame#loginForm { background: #ffffff; }
        QFrame#loginForm QLabel { background: transparent; }
        QLabel#loginTitre { font-size: 28px; font-weight: bold; color: #1f2937; }
        QLabel#loginSousTitre { font-size: 13px; color: #6b7280; }
        QLabel#loginLabel { font-size: 13px; font-weight: 600; color: #374151; }
        QLineEdit#loginNom, QLineEdit#loginMotDePasse {
            background: #f4f5f7; border: 1px solid #d5d9df; border-radius: 8px;
            padding: 11px 12px; font-size: 14px; color: #1f2937; }
        QLineEdit#loginNom:focus, QLineEdit#loginMotDePasse:focus { border: 1px solid #d71920; background: #ffffff; }
        QLabel#loginErreur { color: #c62828; font-size: 12px; }
        QPushButton#btnSeConnecter {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d3122a, stop:1 #a50e1f);
            color: #ffffff; border: none; border-radius: 8px; padding: 13px; font-size: 15px; font-weight: bold; }
        QPushButton#btnSeConnecter:hover { background: #b71c1c; }
        QPushButton#btnSeConnecter:pressed { background: #8e0000; }
        QLabel#loginPied { color: #9ca3af; font-size: 11px; }
    )");

    m_nom->setFocus();
}

QString LoginDialog::nomUtilisateur() const
{
    return m_nom->text().trimmed();
}

void LoginDialog::seConnecter()
{
    if (verifierIdentifiants(m_nom->text().trimmed(), m_motDePasse->text())) {
        accept();
    } else {
        m_erreur->setText(tr("Nom d'utilisateur ou mot de passe incorrect."));
        m_erreur->show();
    }
}

// À compléter plus tard : rechercher l'employé dans la base, comparer le hash du
// mot de passe et récupérer son rôle. Pour l'instant, l'accès est toujours autorisé.
bool LoginDialog::verifierIdentifiants(const QString &nom, const QString &motDePasse) const
{
    Q_UNUSED(nom);
    Q_UNUSED(motDePasse);
    return true;
}
