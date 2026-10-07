#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include <QList>
#include <QPair>
#include <QString>
#include <QWidget>

// Histogramme simple dessiné avec QPainter (aucune dépendance à Qt Charts)
class BarChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    void setData(const QList<QPair<QString, int>> &data, const QString &titre);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPair<QString, int>> m_data;
    QString m_titre;
};

#endif // BARCHARTWIDGET_H
