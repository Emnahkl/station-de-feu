#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "employe.h"

#include <QColor>
#include <QDate>
#include <QDialog>
#include <QImage>
#include <QMap>
#include <QWidget>
#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAbstractVideoSurface>
#include <QVideoFrame>
#endif
#include <QMainWindow>
#include <QPixmap>
#include <QString>
#include <QVector>

class QLabel;
class QTimer;
class QStackedWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QCalendarWidget;
class QCamera;
class QComboBox;
class QListWidget;
class QProgressBar;
class QPushButton;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QMediaCaptureSession;
class QVideoSink;
#endif

// ===========================================================================
//  Pages et fenetres ajoutees (statistiques, calendrier, Face ID)
// ===========================================================================
// Une part d'un graphique : libelle, valeur, couleur
struct StatItem
{
    QString label;
    int     value = 0;
    QColor  color;
};

// Carte de statistiques : graphique circulaire (anneau) + barres de pourcentage
class StatCard : public QWidget
{
    Q_OBJECT
public:
    StatCard(const QString &title, const QVector<StatItem> &items, QWidget *parent = nullptr);
    QSize sizeHint() const override { return QSize(380, 200); }
    QSize minimumSizeHint() const override { return QSize(320, 190); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QString m_title;
    QVector<StatItem> m_items;
};

// Page "Statistiques" integree a la fenetre principale (barres + cercles en pourcentages)
class StatsPage : public QWidget
{
    Q_OBJECT
public:
    explicit StatsPage(const QVector<Employe> &employees, QWidget *parent = nullptr);

signals:
    void retour();      // bouton "Retour au tableau"
};


// Page "Calendrier de travail" integree a la fenetre principale (employe selectionne) : on choisit un jour, une garde, et on l'enregistre dans Oracle
class CalendarPage : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarPage(const Employe &employe, QWidget *parent = nullptr);

signals:
    void retour();      // bouton "Retour au tableau"

private slots:
    void onAffecter();
    void onRetirer();
    void onAffecterSemaine();
    void refresh();

private:
    void colorize();
    void fillList();
    QColor colorFor(const QString &garde) const;

    Employe m_emp;
    QMap<QDate, QString> m_plan;
    QCalendarWidget *m_cal = nullptr;
    QComboBox *m_combo = nullptr;
    QListWidget *m_list = nullptr;
    QLabel *m_info = nullptr;
};


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// Qt5 : recupere les images de la camera (surface video personnalisee)
class FrameGrabber : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit FrameGrabber(QObject *parent = nullptr) : QAbstractVideoSurface(parent) {}
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;
signals:
    void frameReady(const QImage &img);
};
#endif


class QCamera;
class QComboBox;
class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QMediaCaptureSession;
class QVideoSink;
#endif

// Face ID : ouvre la camera, scanne le visage (1er passage = entree, 2e passage = sortie)
class FaceIdDialog : public QDialog
{
    Q_OBJECT
public:
    FaceIdDialog(const QVector<Employe> &employees, int preselectId, QWidget *parent = nullptr);
    ~FaceIdDialog() override;
    int employeId() const { return m_chosenId; }

protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    void onFrame(const QImage &raw);
    void startScan();
    void tickScan();

private:
    void startCamera();
    void stopCamera();
    void setStatus(const QString &text, const QString &color = "#37474f");
    static bool looksLikeFace(const QImage &img);

    QLabel *m_view = nullptr;
    QLabel *m_status = nullptr;
    QComboBox *m_combo = nullptr;
    QPushButton *m_btnScan = nullptr;
    QProgressBar *m_bar = nullptr;
    QTimer *m_scanTimer = nullptr;

    QCamera *m_camera = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMediaCaptureSession *m_session = nullptr;
    QVideoSink *m_sink = nullptr;
#else
    FrameGrabber *m_grabber = nullptr;
#endif

    bool m_cameraOk = false;
    bool m_scanning = false;
    int  m_elapsedMs = 0;
    int  m_framesTotal = 0;
    int  m_framesOk = 0;
    bool m_lastOk = false;
    int  m_chosenId = -1;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void marquerMenu();      // remet « Equipe » en surbrillance dans le menu lateral

signals:
    void ouvrirInterventions();   // clic sur « Interventions » du menu lateral (integration)

private slots:
    void onFormValider();
    void closeForm();
    void onChoisirPhoto();
    void onSendChat();
    void onGenererPlanning();
    void onFaceId();
    void onExporterPdf();
    void onStatistiques();
    void onTrier();
    void onCalendrier();
    void showPanel(QWidget *page);   // affiche une page (statistiques, calendrier) a la place du tableau
    void hidePanel();                // retour au tableau

private:
    enum class Mode { None, Add, AddVolunteer, Edit, View, Assign, Delete, Face };
    enum class SortKey { Id, NomAZ, NomZA, GradeAsc, GradeDesc, Specialite, Statut };

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

    // logique (les donnees viennent d'Oracle via la classe Employe)
    void      reloadFromDb();                 // SELECT -> m_employees
    const Employe *findEmployee(int id) const;
    int       selectedEmployeeId() const;
    void      openForm(Mode mode, int id = -1);
    void      setInfo(const QString &html);
    void      refreshTable();
    void      updatePlanning();
    QString   pointage(const Employe &e);
    QString   answerFor(const QString &question) const;
    QPixmap   avatarFor(const Employe &e, int size) const;
    bool      validerSaisie(QString &nom, QString &prenom, QString &tel);

    // chat
    void addBubble(const QString &text, bool fromUser);
    void showThinking();
    void hideThinking();

    Ui::MainWindow *ui;

    QVector<Employe> m_employees;       // copie en memoire de la table EMPLOYE
    QVector<Employe> m_rows;            // lignes affichees (filtrees + triees) : utilisees par l'export PDF
    SortKey m_sortKey = SortKey::Id;
    Employe m_dao;                      // sert a appeler afficher()
    Mode m_mode = Mode::None;
    int  m_currentId = -1;

    // images
    QPixmap m_logo, m_sidebar, m_chatbot, m_avatar;
    QPixmap m_gradeBtn[4];     // index 1..3
    QPixmap m_specPx[3];       // Fire, Medic, Logistics
    QString sortLabel() const;

    // chat "en cours de traitement"
    QWidget *m_thinkRow = nullptr;
    QLabel  *m_thinkLabel = nullptr;
    QTimer  *m_thinkTimer = nullptr;
    int      m_thinkDots = 0;

    QLabel *m_badge = nullptr;
    QStackedWidget *m_panel = nullptr;   // zone des pages integrees
};

#endif // MAINWINDOW_H
