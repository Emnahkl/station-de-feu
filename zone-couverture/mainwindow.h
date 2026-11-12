#pragma once
#include <QMainWindow>
class AlerteCard;
class QButtonGroup;
class QFrame;
class QLabel;
class QLineEdit;
class QStackedWidget;
class ZoneCard;
class ZoneStore;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QFrame *buildSidebar();
    QWidget *buildTopbar();
    QWidget *buildDashboard();
    QWidget *buildPlaceholder(const QString &titre);
    QFrame *buildAccesRapide();
    void goTo(int page);
    void updateBell();

    ZoneStore *m_store;
    ZoneCard *m_zones;
    QStackedWidget *m_pages;
    QButtonGroup *m_nav;
    QFrame *m_sidebar;
    QLabel *m_bell;
    QLineEdit *m_search;
};
