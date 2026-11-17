#ifndef INTERVENTIONSPAGE_H
#define INTERVENTIONSPAGE_H

#include <QDateTime>
#include <QString>
#include <QVector>
#include <QWidget>

class QComboBox;
class QLabel;
class QTableWidget;
class RightPanel;

// Page « Gestion des interventions » (affichée quand on clique sur « Interventions »).
class InterventionsPage : public QWidget
{
    Q_OBJECT
public:
    explicit InterventionsPage(QWidget *parent = nullptr);

public slots:
    void setSearch(const QString &text);

private:
    struct Intervention {
        int id;
        QString type;
        QString adresse;
        QDateTime date;
        QString zone;
        QString gravite;
        QString statut;
    };

    void buildUi();
    void loadSampleData();
    void rebuildTable();
    void updateCards();
    void sortByDate();
    void sortByGravite();
    int currentId() const;
    int indexOfId(int id) const;
    bool editIntervention(Intervention &it, const QString &title);
    void addIntervention();
    void modifyCurrent();
    void removeCurrent();
    void consultCurrent();
    void exportPdf();
    void showStats();
    void autoAssign();
    void showRoute();
    QString answer(const QString &question) const;

    QVector<Intervention> m_items;
    int m_nextId = 1;
    QString m_search;

    QComboBox *m_comboGravite = nullptr;
    QComboBox *m_comboStatut = nullptr;
    QLabel *m_lblEnCours = nullptr;
    QLabel *m_lblEnAttente = nullptr;
    QLabel *m_lblTerminee = nullptr;
    QTableWidget *m_table = nullptr;
    RightPanel *m_panel = nullptr;
};

#endif // INTERVENTIONSPAGE_H
