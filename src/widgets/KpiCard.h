#pragma once
#include <QFrame>
class QLabel;

// Petite carte indicateur (titre + valeur) avec liseré coloré
class KpiCard : public QFrame {
    Q_OBJECT
public:
    KpiCard(const QString& title, const QColor& accent, QWidget* parent = nullptr);
    void setValue(const QString& value, const QString& hint = QString());
private:
    QLabel* m_value;
    QLabel* m_hint;
};
