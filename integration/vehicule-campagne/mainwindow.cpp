#include "mainwindow.h"
#include "vehiclewindow.h"
#include "campaignwindow.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *central = new QWidget(this);
    auto *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto *sidebar = new QFrame(central);
    sidebar->setObjectName("integrationSidebar");
    sidebar->setFixedWidth(230);
    sidebar->setStyleSheet(R"(
        #integrationSidebar { border-image: url(:/images/sidebar_bg.png) 0 0 0 0 stretch stretch; }
        #integrationSidebar QLabel { background: transparent; color: white; }
        #integrationSidebar QPushButton {
            color: white; text-align: left; border: 0; border-radius: 7px;
            background: rgba(39, 5, 9, 190); padding: 12px 11px; min-height: 25px;
        }
        #integrationSidebar QPushButton:hover,
        #integrationSidebar QPushButton:checked { background: #d71920; }
    )");
    auto *nav = new QVBoxLayout(sidebar);
    nav->setContentsMargins(15, 18, 15, 18);
    nav->setSpacing(8);

    auto *logo = new QLabel(sidebar);
    logo->setFixedSize(132, 132);
    logo->setAlignment(Qt::AlignCenter);
    logo->setPixmap(QPixmap(":/images/logo.png").scaled(132, 132, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    nav->addWidget(logo, 0, Qt::AlignHCenter);

    auto *brand = new QLabel("FIRE STATION", sidebar);
    brand->setStyleSheet("font-size:20px;font-weight:800;letter-spacing:1px;");
    nav->addWidget(brand, 0, Qt::AlignHCenter);
    nav->addSpacing(170);

    auto *pages = new QStackedWidget(central);
    auto *vehicles = new VehicleWindow(pages);
    vehicles->setWindowFlags(Qt::Widget);
    auto *campaigns = new CampaignWindow(pages);
    campaigns->setWindowFlags(Qt::Widget);
    pages->addWidget(vehicles);
    pages->addWidget(campaigns);

    const QStringList labels = {"⌂   Tableau de bord", "🚒   Véhicules", "📣   Campagnes"};
    for (int index = 0; index < labels.size(); ++index) {
        auto *button = new QPushButton(labels[index], sidebar);
        button->setCheckable(true);
        nav->addWidget(button);
        connect(button, &QPushButton::clicked, this, [pages, vehicles, sidebar, button, index]() {
            if (index < 2) {
                pages->setCurrentIndex(0);
                if (index == 0) vehicles->showDashboard();
                else vehicles->showVehicles();
            } else {
                pages->setCurrentIndex(1);
            }
            for (QPushButton *navButton : sidebar->findChildren<QPushButton *>())
                navButton->setChecked(navButton == button);
        });
    }
    nav->addStretch(1);

    layout->addWidget(sidebar);
    layout->addWidget(pages, 1);
    setCentralWidget(central);
    setWindowTitle("Fire Station — Gestion intégrée");
    setWindowIcon(QIcon(":/images/logo.png"));
    sidebar->findChildren<QPushButton *>().at(1)->click();
}

MainWindow::~MainWindow() = default;

