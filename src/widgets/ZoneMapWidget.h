#pragma once
#include "core/Zone.h"
#include <QImage>
#include <QSet>
#include <QTimer>
#include <QToolButton>
#include <QVector>
#include <QWidget>

// Carte interactive des zones à risque (rendu QPainter, aucune dépendance réseau).
//  - molette : zoom   - glisser : déplacer la carte   - clic : sélection
//  - double-clic : modifier   - modes « Ajouter » (clic = position) et « Déplacer » (glisser une zone)
class ZoneMapWidget : public QWidget {
    Q_OBJECT
public:
    enum class Mode { Navigate, AddZone, MoveZone };

    explicit ZoneMapWidget(QWidget* parent = nullptr);

    void setZones(const QVector<Zone>& zones);
    void setVisibleIds(const QSet<int>& ids);   // zones affichées (filtre)
    void setAlertIds(const QSet<int>& ids);     // zones sous-couvertes (anneau pulsant)
    void setSelectedId(int id);
    void setMode(Mode m);
    Mode mode() const { return m_mode; }
    void setBackgroundImage(const QImage& img); // image null => fond schématique
    bool hasBackgroundImage() const { return !m_bg.isNull(); }
    void resetView();
    void centerLatLon(double& lat, double& lon) const;
    void zoomBy(double factor);

signals:
    void zoneSelected(int id);
    void zoneActivated(int id);                 // double-clic
    void locationPicked(double lat, double lon);
    void zoneMoved(int id, double lat, double lon);
    void modeChanged(ZoneMapWidget::Mode mode);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void leaveEvent(QEvent*) override;

private:
    QPointF project(double lat, double lon) const;       // lat/lon -> km
    void unproject(const QPointF& w, double& lat, double& lon) const;
    QPointF toScreen(const QPointF& w) const;
    QPointF toWorld(const QPointF& s) const;
    QPointF zoneScreenPos(const Zone& z) const;
    int zoneAt(const QPointF& screen) const;
    int indexOf(int id) const;
    bool isVisibleZone(int id) const;

    void drawBasemap(QPainter& p);
    void drawZones(QPainter& p);
    void drawOverlays(QPainter& p);
    void placeButtons();

    QVector<Zone> m_zones;
    QSet<int> m_visible;
    bool m_filterActive = false;
    QSet<int> m_alerts;
    int m_selected = -1;
    int m_hover = -1;
    Mode m_mode = Mode::Navigate;

    QImage m_bg;
    QPointF m_center;       // km
    double m_scale = 20.0;  // px / km

    bool m_panning = false;
    int m_dragId = -1;
    QPointF m_lastPos;
    double m_phase = 0.0;
    QTimer m_timer;

    QToolButton* m_btnIn;
    QToolButton* m_btnOut;
    QToolButton* m_btnReset;
};
