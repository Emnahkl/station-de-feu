#pragma once
// ============================================================
//  zone.h : TOUT ce qui concerne les zones de couverture
//  (donnees, carte reelle, CRUD integre, liste prioritaire)
// ============================================================
#include <QFrame>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QPixmap>
#include <QPointF>
#include <QSet>
#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStackedWidget;
class QTabWidget;
class QTableWidget;
class QDoubleSpinBox;
class QVBoxLayout;
class QNetworkAccessManager;

// ---------- Entite ----------
struct Zone {
    QString id;            // Z-001
    QString nom;
    QString region;
    QString risque;        // Eleve / Moyen / Faible
    double  temps = 0;     // temps moyen d'intervention (min)
    int     interventions = 0, agents = 0, equipes = 0, vehicules = 0, population = 0;
    double superficie = 0; // km²

    double  lat = 36.8, lon = 10.18;
    bool sousCouverte(double seuil) const { return temps > seuil; }
};

// ---------- Stockage + CRUD (a brancher sur Oracle plus tard) ----------
class ZoneStore : public QObject {
    Q_OBJECT
public:
    explicit ZoneStore(QObject *parent = nullptr);
    double seuil() const { return m_seuil; }
    void setSeuil(double v);
    const QList<Zone> &all() const { return m_zones; }
    const Zone *find(const QString &id) const;
    QString nextId() const;
    bool ajouter(Zone &z);
    bool modifier(const Zone &z);
    bool supprimer(const QString &id);
    bool connectOracle();
    bool oracleActive() const { return m_oracleActive; }
    QString lastStorageError() const { return m_storageError; }
signals:
    void changed();
    void storageError(const QString &message);
private:
    QList<Zone> m_zones;
    double m_seuil = 12.0;
    void load();
    void persist() const;
    bool loadOracle();
    bool saveOracle(const Zone &z, bool insert);
    bool deleteOracle(const QString &id);
    bool m_oracleActive = false;
    QString m_storageError;
};

// ---------- Carte reelle OpenStreetMap (dessinee en Qt pur, sans WebEngine) ----------
class MapWidget : public QWidget {
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setZones(const QList<Zone> &zones, const QString &sel);
    void setThreshold(double value) { m_threshold=value; update(); }
    void setDraggable(bool b) { m_drag = b; }
    void setCrosshair(bool b) { setCursor(b ? Qt::CrossCursor : Qt::OpenHandCursor); }
    void setTemp(bool on, double lat = 0, double lon = 0) { m_tempOn = on; m_tempLat = lat; m_tempLon = lon; update(); }
    void setFond(int i) { m_fond = i; update(); }
    void flyTo(double lat, double lon, int zoom = 13);
    void locate() { flyTo(36.8000, 10.1800, 13); }
signals:
    void zoneClicked(const QString &id);
    void zoneMoved(const QString &id, double lat, double lon);
    void mapClicked(double lat, double lon);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;
private:
    QPointF toPx(double lat, double lon) const;           // coordonnees -> pixel ecran
    QPointF toLL(const QPointF &px) const;                // pixel ecran -> (lat, lon)
    int hit(const QPoint &p) const;
    void zoomAt(const QPoint &p, int dz);
    QPixmap tile(int z, int x, int y);

    QNetworkAccessManager *m_net;
    QHash<QString, QPixmap> m_cache;
    QSet<QString> m_pending;
    QList<Zone> m_zones;
    double m_threshold=12.0;
    QString m_sel;
    double m_lat = 36.82, m_lon = 10.22;
    int m_zoom = 11, m_fond = 0;
    bool m_drag = false, m_tempOn = false;
    double m_tempLat = 0, m_tempLon = 0;
    QPoint m_press, m_last;
    bool m_panning = false, m_moved = false;
    int m_dragIdx = -1;
};

// ---------- Carte + barre CRUD + formulaire integre ----------
class ZoneCard : public QFrame {
    Q_OBJECT
public:
    explicit ZoneCard(ZoneStore *store, QWidget *parent = nullptr);
    void selectZone(const QString &id, bool fly = true);
    void startAdd();                 // ouvre le formulaire "Nouvelle zone"
    void resetFilters();
    bool searchZone(const QString &text);
signals:
    void zoneSelected(const QString &id);
private:
    enum Mode { Idle, Place, Move };
    enum Form { None, Add, Edit, Delete };

    void buildUi();
    void refreshMap();
    void refreshFilters();
    void setMode(Mode m);
    void openForm(Form f);
    void closeForm();
    void saveForm();
    void fillForm(const Zone &z);
    void hint(const QString &text, bool warn = false);
    bool needSelection();

    ZoneStore *m_store;
    QString m_sel;
    Mode m_mode = Idle;
    Form m_form = None;

    QComboBox *m_region, *m_niveau, *m_fond;
    QPushButton *m_bAdd, *m_bPlace, *m_bMove, *m_bEdit, *m_bDel;
    QLabel *m_hint;
    MapWidget *m_map;
    QFrame *m_panel;
    QStackedWidget *m_pages;
    QLabel *m_formTitle, *m_confirmText;
    QLineEdit *m_nom;
    QComboBox *m_fRegion, *m_fRisque;
    QDoubleSpinBox *m_fTemps, *m_fLat, *m_fLon;
    QSpinBox *m_fInterv, *m_fEquipes, *m_fVeh, *m_fPop;
    QDoubleSpinBox *m_fSuperficie, *m_seuil;
    QTabWidget *m_views;
    QTableWidget *m_table;
    QLineEdit *m_tableSearch;
    void refreshTable();
    void showTable();
    void exportCsv();
    void exportPdf();
    void showStats();
};

// ---------- Carte "Zones a risque prioritaires" ----------
class ZonePrioCard : public QFrame {
    Q_OBJECT
public:
    explicit ZonePrioCard(ZoneStore *store, QWidget *parent = nullptr);
signals:
    void zoneClicked(const QString &id);
    void voirTout();
private:
    void rebuild();
    ZoneStore *m_store;
    QVBoxLayout *m_list;
};
