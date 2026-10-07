#include "StatsChartWidget.h"
#include "core/CoverageAnalyzer.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

struct Bar { QString label; double value; QColor color; };

class ChartCanvas : public QWidget {
public:
    explicit ChartCanvas(QWidget* parent = nullptr) : QWidget(parent) { setMinimumHeight(150); }
    void setBars(const QVector<Bar>& b) { m_bars = b; update(); }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), Qt::white);
        const int left = 40, right = 12, top = 16, bottom = 38;
        QRectF plot(left, top, width() - left - right, height() - top - bottom);
        if (m_bars.isEmpty() || plot.width() < 20 || plot.height() < 20) {
            p.setPen(QColor("#9ca3af"));
            p.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("Aucune donnée"));
            return;
        }
        double maxV = 1;
        for (const Bar& b : m_bars) maxV = std::max(maxV, b.value);
        double step = std::pow(10.0, std::floor(std::log10(maxV)));
        if (maxV / step < 2) step /= 5; else if (maxV / step < 5) step /= 2;
        double top_v = std::ceil(maxV / step) * step;

        QFont small("Sans", 8);
        p.setFont(small);
        for (double v = 0; v <= top_v + 1e-9; v += step) {
            double y = plot.bottom() - plot.height() * v / top_v;
            p.setPen(QPen(QColor("#eceff3"), 1));
            p.drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
            p.setPen(QColor("#6b7280"));
            p.drawText(QRectF(0, y - 8, left - 6, 16), Qt::AlignRight | Qt::AlignVCenter, QString::number(v, 'f', 0));
        }
        double slot = plot.width() / m_bars.size();
        double bw = std::min(48.0, slot * 0.6);
        for (int i = 0; i < m_bars.size(); ++i) {
            const Bar& b = m_bars[i];
            double h = plot.height() * b.value / top_v;
            QRectF r(plot.left() + slot * i + (slot - bw) / 2, plot.bottom() - h, bw, h);
            p.setPen(Qt::NoPen);
            p.setBrush(b.color);
            p.drawRoundedRect(r, 3, 3);
            p.setPen(QColor("#111827"));
            p.drawText(QRectF(r.left() - 10, r.top() - 15, bw + 20, 14), Qt::AlignCenter, QString::number(b.value, 'f', 0));
            p.setPen(QColor("#4b5563"));
            p.drawText(QRectF(plot.left() + slot * i, plot.bottom() + 3, slot, bottom - 3),
                       Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, b.label);
        }
    }

private:
    QVector<Bar> m_bars;
};

StatsChartWidget::StatsChartWidget(QWidget* parent) : QFrame(parent)
{
    setObjectName("card");
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(12, 10, 12, 10);
    auto* head = new QHBoxLayout;
    auto* t = new QLabel(QString::fromUtf8("📊  Statistiques des interventions"));
    t->setObjectName("cardTitle");
    head->addWidget(t, 1);
    m_mode = new QComboBox;
    m_mode->addItems({QString::fromUtf8("Par zone"), QString::fromUtf8("Par niveau de risque")});
    head->addWidget(m_mode);
    lay->addLayout(head);
    m_canvas = new ChartCanvas;
    lay->addWidget(m_canvas, 1);
    connect(m_mode, qOverload<int>(&QComboBox::currentIndexChanged), this, [this] { setZones(m_zonesCache); });
}

void StatsChartWidget::setZones(const QVector<Zone>& zones)
{
    m_zonesCache = zones;
    QVector<Bar> bars;
    if (m_mode->currentIndex() == 0) {
        QVector<Zone> sorted = zones;
        std::sort(sorted.begin(), sorted.end(), [](const Zone& a, const Zone& b) { return a.nbInterventions > b.nbInterventions; });
        for (const Zone& z : sorted) bars.append({z.nom, double(z.nbInterventions), RiskUtil::color(z.risque)});
    } else {
        for (RiskLevel r : {RiskLevel::Eleve, RiskLevel::Moyen, RiskLevel::Faible}) {
            auto s = CoverageAnalyzer::statsForRisk(zones, r);
            bars.append({QString::fromUtf8("%1 (%2 zones)").arg(RiskUtil::label(r)).arg(s.zones),
                         double(s.interventions), RiskUtil::color(r)});
        }
    }
    m_canvas->setBars(bars);
}
