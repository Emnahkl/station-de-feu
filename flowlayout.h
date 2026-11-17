#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QList>
#include <QStyle>

// Layout qui passe à la ligne quand la largeur manque (évite les boutons tronqués).
class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent = nullptr, int margin = 0, int hSpacing = 10, int vSpacing = 10)
        : QLayout(parent), m_h(hSpacing), m_v(vSpacing)
    {
        setContentsMargins(margin, margin, margin, margin);
    }
    ~FlowLayout() override { qDeleteAll(m_items); }

    void addItem(QLayoutItem *item) override { m_items.append(item); }
    int count() const override { return m_items.size(); }
    QLayoutItem *itemAt(int i) const override { return m_items.value(i); }
    QLayoutItem *takeAt(int i) override { return (i >= 0 && i < m_items.size()) ? m_items.takeAt(i) : nullptr; }
    Qt::Orientations expandingDirections() const override { return {}; }
    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int w) const override { return doLayout(QRect(0, 0, w, 0), true); }
    void setGeometry(const QRect &r) override { QLayout::setGeometry(r); doLayout(r, false); }
    QSize sizeHint() const override { return minimumSize(); }
    QSize minimumSize() const override
    {
        QSize s;
        for (const QLayoutItem *it : m_items) s = s.expandedTo(it->minimumSize());
        const QMargins m = contentsMargins();
        return s + QSize(m.left() + m.right(), m.top() + m.bottom());
    }

private:
    int doLayout(const QRect &rect, bool testOnly) const
    {
        const QMargins m = contentsMargins();
        const QRect eff = rect.adjusted(m.left(), m.top(), -m.right(), -m.bottom());
        int x = eff.x(), y = eff.y(), lineH = 0;
        for (QLayoutItem *it : m_items) {
            const QSize hint = it->sizeHint();
            int nextX = x + hint.width() + m_h;
            if (nextX - m_h > eff.right() + 1 && lineH > 0) {
                x = eff.x();
                y += lineH + m_v;
                nextX = x + hint.width() + m_h;
                lineH = 0;
            }
            if (!testOnly) it->setGeometry(QRect(QPoint(x, y), hint));
            x = nextX;
            lineH = qMax(lineH, hint.height());
        }
        return y + lineH - rect.y() + m.top() + m.bottom();
    }

    QList<QLayoutItem *> m_items;
    int m_h, m_v;
};

#endif // FLOWLAYOUT_H
