#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPixmap>
#include <QString>

class QLabel;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Donnees en memoire (pas de base de donnees)
struct Employee {
    int     id = 0;
    QString nom;
    int     grade = 1;                 // 1..3 etoiles
    QString specialite = "Fire";       // Fire | Medic | Logistics
    QString statut = "En service";     // En service | En pause | En formation
    QString garde = "Aucune";          // emploi du temps assigne
    bool    volontaire = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onFormValider();
    void closeForm();
    void onSendChat();
    void onGenererPlanning();
    void onEnvoyerEmail();
    void onEnvoyerSms();
    void onEnvoyerWhatsApp();

private:
    enum class Mode { None, Add, AddVolunteer, Edit, View, Assign, Delete, Qr, Face };

    // initialisation
    void loadAssets();
    void setupStyle();
    void setupSidebar();
    void setupTopBar();
    void setupFilters();
    void setupActionButtons();
    void setupForm();
    void setupTable();
    void setupChat();
    void setupRightPanels();
    void seedData();

    // logique
    Employee *findEmployee(int id);
    int       selectedEmployeeId() const;
    void      openForm(Mode mode, int id = -1);
    void      setInfo(const QString &html);
    void      refreshTable();
    void      updatePlanning();
    QString   pointage(Employee &e);
    QString   answerFor(const QString &question) const;

    // chat
    void addBubble(const QString &text, bool fromUser);
    void showThinking();
    void hideThinking();

    Ui::MainWindow *ui;

    QVector<Employee> m_employees;
    int  m_nextId = 1;
    Mode m_mode = Mode::None;
    int  m_currentId = -1;

    // images
    QPixmap m_logo, m_sidebar, m_chatbot, m_avatar;
    QPixmap m_gradeBtn[4];     // index 1..3
    QPixmap m_specPx[3];       // Fire, Medic, Logistics

    // chat "en cours de traitement"
    QWidget *m_thinkRow = nullptr;
    QLabel  *m_thinkLabel = nullptr;
    QTimer  *m_thinkTimer = nullptr;
    int      m_thinkDots = 0;

    QLabel *m_badge = nullptr;
};

#endif // MAINWINDOW_H
