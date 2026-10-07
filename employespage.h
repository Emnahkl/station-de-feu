#ifndef EMPLOYESPAGE_H
#define EMPLOYESPAGE_H

#include <QWidget>
#include <QString>
#include <QVector>

class QComboBox;
class QTableWidget;
class RightPanel;

// Page « Gestion des employés » (affichée quand on clique sur « Équipe »).
class EmployesPage : public QWidget
{
    Q_OBJECT
public:
    explicit EmployesPage(QWidget *parent = nullptr);

public slots:
    void setSearch(const QString &text);

private:
    struct Agent {
        int id;
        QString nom;
        QString grade;
        QString specialite;
        QString statut;
    };

    void buildUi();
    void loadSampleData();
    void rebuildTable();
    int currentAgentId() const;
    int indexOfId(int id) const;
    bool editAgent(Agent &agent, const QString &title);
    void addAgent(const QString &title);
    void removeCurrent();
    void showAgent(int id);
    void assignAgent(int id);
    QString answer(const QString &question) const;

    QVector<Agent> m_agents;
    int m_nextId = 1;
    QString m_search;

    QComboBox *m_comboGrade = nullptr;
    QComboBox *m_comboSpec = nullptr;
    QTableWidget *m_table = nullptr;
    RightPanel *m_panel = nullptr;
};

#endif // EMPLOYESPAGE_H
