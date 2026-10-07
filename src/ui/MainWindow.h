#pragma once
#include <QMainWindow>

class ZoneRepository;
class QStackedWidget;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    QWidget* buildSidebar();
    QWidget* buildHeader();
    QWidget* placeholderPage(const QString& title);

    ZoneRepository* m_repo;
    QStackedWidget* m_stack;
    QLabel* m_title;
};
