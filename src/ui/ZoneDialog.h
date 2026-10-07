#pragma once
#include "core/Zone.h"
#include <QDialog>

namespace Ui { class ZoneDialog; }

// Création / modification d'une zone (formulaire conçu dans Qt Designer : ZoneDialog.ui)
class ZoneDialog : public QDialog {
    Q_OBJECT
public:
    explicit ZoneDialog(QWidget* parent = nullptr);
    ~ZoneDialog() override;

    void setZone(const Zone& z, bool isNew);
    Zone zone() const;

public slots:
    void accept() override;

private:
    Ui::ZoneDialog* ui;
    Zone m_zone;
};
