#include "rightpanel.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>

namespace {

QProgressBar *makeBar(const QString &name, int value)
{
    auto *bar = new QProgressBar;
    bar->setObjectName(name);
    bar->setRange(0, 100);
    bar->setValue(value);
    bar->setTextVisible(false);
    return bar;
}

QLabel *makeLabel(const QString &text, const QString &name = QString())
{
    auto *l = new QLabel(text);
    if (!name.isEmpty())
        l->setObjectName(name);
    return l;
}

} // namespace

RightPanel::RightPanel(const Config &config, QWidget *parent)
    : QFrame(parent), m_config(config)
{
    setObjectName("rightPanel");
    setFixedWidth(310);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(12);

    // ------------------------------------------------------------ Chatbot
    auto *chat = new QFrame;
    chat->setObjectName("panelChat");
    auto *chatLay = new QVBoxLayout(chat);
    chatLay->setContentsMargins(14, 12, 14, 14);
    chatLay->setSpacing(10);
    chatLay->addWidget(makeLabel(config.chatTitle, "panelChatTitle"));

    m_scroll = new QScrollArea;
    m_scroll->setObjectName("chatScroll");
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setMinimumHeight(60);
    auto *host = new QWidget;
    host->setObjectName("chatHost");
    m_chatLayout = new QVBoxLayout(host);
    m_chatLayout->setContentsMargins(0, 0, 0, 0);
    m_chatLayout->setSpacing(8);
    m_chatLayout->addStretch(1);
    m_scroll->setWidget(host);
    chatLay->addWidget(m_scroll, 1);

    auto *inputRow = new QHBoxLayout;
    inputRow->setSpacing(8);
    m_chatInput = new QLineEdit;
    m_chatInput->setPlaceholderText(config.chatPlaceholder);
    auto *btnSend = new QPushButton(tr("Envoyer"));
    btnSend->setObjectName("btnChatSend");
    btnSend->setCursor(Qt::PointingHandCursor);
    inputRow->addWidget(m_chatInput, 1);
    inputRow->addWidget(btnSend);
    chatLay->addLayout(inputRow);
    root->addWidget(chat, 1);

    connect(btnSend, &QPushButton::clicked, this, &RightPanel::sendQuestion);
    connect(m_chatInput, &QLineEdit::returnPressed, this, &RightPanel::sendQuestion);
    addBubble(config.greeting, true);

    // ------------------------------------------------------------ Planning
    auto *plan = new QFrame;
    plan->setObjectName("panelPlanning");
    auto *planLay = new QVBoxLayout(plan);
    planLay->setContentsMargins(14, 12, 14, 14);
    planLay->setSpacing(7);
    planLay->addWidget(makeLabel(config.planTitle, "panelPlanningTitle"));

    auto *head = new QHBoxLayout;
    head->setSpacing(12);
    head->addWidget(makeLabel(tr("Unité Chef"), "lblUniteChef"), 1);
    head->addWidget(makeLabel(tr("Jour"), "lblJour"));
    head->addWidget(makeLabel(tr("Nuit"), "lblNuit"));
    head->addWidget(makeLabel(tr("Week-end"), "lblWeekend"));
    planLay->addLayout(head);
    planLay->addWidget(makeBar("barJour", 78));
    planLay->addWidget(makeBar("barNuit", 74));
    planLay->addWidget(makeBar("barWeekend", 24));

    auto *dispo = new QHBoxLayout;
    dispo->setSpacing(10);
    dispo->addWidget(makeLabel(tr("Disponibilité"), "lblDispo"));
    dispo->addWidget(makeBar("barDispo", 62), 1);
    planLay->addLayout(dispo);
    auto *prog = new QHBoxLayout;
    prog->setSpacing(10);
    prog->addWidget(makeLabel(tr("Progress"), "lblProgress"));
    prog->addWidget(makeBar("barProgress", 58), 1);
    planLay->addLayout(prog);

    auto *alerts = new QHBoxLayout;
    alerts->setSpacing(8);
    auto *btnNotify = new QPushButton(config.btnNotify);
    btnNotify->setObjectName("btnNotifier");
    auto *btnAlert = new QPushButton(config.btnAlert);
    btnAlert->setObjectName("btnAlerte");
    alerts->addWidget(btnNotify);
    alerts->addWidget(btnAlert);
    planLay->addLayout(alerts);
    auto *btnGen = new QPushButton(config.btnGenerate);
    btnGen->setObjectName("btnGenerer");
    planLay->addWidget(btnGen);
    for (auto *b : {btnNotify, btnAlert, btnGen})
        b->setCursor(Qt::PointingHandCursor);
    plan->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    root->addWidget(plan);

    connect(btnNotify, &QPushButton::clicked, this, [this, btnNotify] {
        QMessageBox::information(this, btnNotify->text(), tr("Notification envoyée (simulation)."));
    });
    connect(btnAlert, &QPushButton::clicked, this, [this, btnAlert] {
        QMessageBox::warning(this, btnAlert->text(), tr("Alerte déclenchée (simulation)."));
    });
    connect(btnGen, &QPushButton::clicked, this, [this, btnGen] {
        QMessageBox::information(this, btnGen->text(), tr("Génération terminée (simulation)."));
    });

    // ------------------------------------------------------------ Communication
    auto *comm = new QFrame;
    comm->setObjectName("panelComm");
    auto *commLay = new QVBoxLayout(comm);
    commLay->setContentsMargins(14, 12, 14, 14);
    commLay->setSpacing(7);
    commLay->addWidget(makeLabel(tr("Communication Directe"), "panelCommTitle"));
    commLay->addWidget(makeLabel(tr("Email (Sujet, Corps, Liste)"), "lblEmail"));

    auto *emailRow = new QHBoxLayout;
    emailRow->setSpacing(8);
    auto *email = new QLineEdit;
    email->setPlaceholderText(tr("Email"));
    auto *btnEmail = new QPushButton(tr("Envoyer"));
    btnEmail->setObjectName("btnEnvoyerEmail");
    emailRow->addWidget(email, 1);
    emailRow->addWidget(btnEmail);
    commLay->addLayout(emailRow);

    commLay->addWidget(makeLabel(tr("SMS/WhatsApp (Message, Numéros)"), "lblSms"));
    auto *sms = new QLineEdit;
    sms->setPlaceholderText(tr("216XXXXXXXX ; Message"));
    commLay->addWidget(sms);
    auto *smsRow = new QHBoxLayout;
    smsRow->setSpacing(8);
    auto *btnSms = new QPushButton(tr("Envoyer"));
    btnSms->setObjectName("btnEnvoyerSms");
    auto *btnWa = new QPushButton(tr("Envoyer"));
    btnWa->setObjectName("btnEnvoyerWhatsapp");
    smsRow->addWidget(btnSms);
    smsRow->addWidget(btnWa);
    commLay->addLayout(smsRow);
    for (auto *b : {btnEmail, btnSms, btnWa, btnSend})
        b->setCursor(Qt::PointingHandCursor);
    comm->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    root->addWidget(comm);

    connect(btnEmail, &QPushButton::clicked, this, [this, email] {
        if (email->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Email"), tr("Saisissez une adresse email."));
            return;
        }
        QMessageBox::information(this, tr("Email"), tr("Email envoyé à %1 (simulation).").arg(email->text().trimmed()));
        email->clear();
    });
    auto sendSms = [this, sms](const QString &canal) {
        if (sms->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, canal, tr("Saisissez « numéro ; message »."));
            return;
        }
        QMessageBox::information(this, canal, tr("Message %1 envoyé (simulation).").arg(canal));
        sms->clear();
    };
    connect(btnSms, &QPushButton::clicked, this, [sendSms] { sendSms(QStringLiteral("SMS")); });
    connect(btnWa, &QPushButton::clicked, this, [sendSms] { sendSms(QStringLiteral("WhatsApp")); });
}

void RightPanel::setResponder(std::function<QString(const QString &)> responder)
{
    m_responder = std::move(responder);
}

void RightPanel::addBubble(const QString &text, bool fromBot)
{
    auto *row = new QHBoxLayout;
    row->setSpacing(8);

    auto *bubble = new QLabel(text);
    bubble->setObjectName(fromBot ? "bubbleBot" : "bubbleUser");
    bubble->setWordWrap(true);
    bubble->setTextInteractionFlags(Qt::TextSelectableByMouse);
    bubble->setMaximumWidth(230);

    if (fromBot) {
        auto *icon = new QLabel;
        icon->setPixmap(QPixmap(":/icons/robot.png").scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setFixedSize(30, 30);
        row->addWidget(icon, 0, Qt::AlignTop);
        row->addWidget(bubble, 0, Qt::AlignTop);
        row->addStretch(1);
    } else {
        row->addStretch(1);
        row->addWidget(bubble, 0, Qt::AlignTop);
    }
    m_chatLayout->insertLayout(m_chatLayout->count() - 1, row);

    QTimer::singleShot(30, this, [this] {
        m_scroll->verticalScrollBar()->setValue(m_scroll->verticalScrollBar()->maximum());
    });
}

void RightPanel::sendQuestion()
{
    const QString q = m_chatInput->text().trimmed();
    if (q.isEmpty())
        return;
    m_chatInput->clear();
    addBubble(q, false);
    QTimer::singleShot(350, this, [this, q] {
        addBubble(m_responder ? m_responder(q) : tr("Je n'ai pas encore de réponse à cela."), true);
    });
}
