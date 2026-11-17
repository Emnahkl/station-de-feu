#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QFrame>
#include <QString>
#include <functional>

class QVBoxLayout;
class QScrollArea;
class QLineEdit;

// Panneau de droite commun aux pages Employés et Interventions :
// chatbot IA, planning / dispatch IA et communication directe.
class RightPanel : public QFrame
{
    Q_OBJECT
public:
    struct Config {
        QString chatTitle;
        QString greeting;
        QString chatPlaceholder = QStringLiteral("Poser une question...");
        QString planTitle;
        QString btnNotify;
        QString btnAlert;
        QString btnGenerate;
    };

    explicit RightPanel(const Config &config, QWidget *parent = nullptr);

    // Fonction appelée pour répondre aux questions posées au chatbot.
    void setResponder(std::function<QString(const QString &)> responder);

private:
    void addBubble(const QString &text, bool fromBot);
    void sendQuestion();

    Config m_config;
    QScrollArea *m_scroll = nullptr;
    QVBoxLayout *m_chatLayout = nullptr;
    QLineEdit *m_chatInput = nullptr;
    std::function<QString(const QString &)> m_responder;
};

#endif // RIGHTPANEL_H
