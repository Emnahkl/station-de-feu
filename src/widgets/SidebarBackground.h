#pragma once
#include <QPixmap>
#include <QWidget>

// Conteneur de barre latérale avec image de fond (« cover », ancrée en bas) + voile sombre en haut
class SidebarBackground : public QWidget {
    Q_OBJECT
public:
    explicit SidebarBackground(const QString& resourcePath, QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap m_pix;
};
