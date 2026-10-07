#pragma once
#include <QWidget>

// Logo « FIRE STATION ». Si un fichier logo.png est placé à côté de l'exécutable, il est utilisé.
class LogoWidget : public QWidget {
    Q_OBJECT
public:
    explicit LogoWidget(int size = 90, QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap m_pix;
};
