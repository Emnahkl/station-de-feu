#include "Theme.h"

QString Theme::styleSheet()
{
    return QString::fromUtf8(R"(
QWidget { font-family: "Segoe UI", "Noto Sans", "DejaVu Sans", sans-serif; font-size: 12px; color:#1f2937; }
QMainWindow, #content { background:#f3f4f6; }

#sidebarBg QPushButton { color:#fde8e8; text-align:left; padding:10px 14px; border:none; border-radius:8px; font-size:13px; }
#sidebarBg QPushButton:hover { background: rgba(255,255,255,0.12); }
#sidebarBg QPushButton:checked { background:#d32f2f; color:white; font-weight:700; }
#sidebarSlogan { color:#fde8e8; font-weight:600; }
#sidebarBrand { color:white; font-weight:800; font-size:15px; letter-spacing:1px; }

#header { background:white; border-bottom:1px solid #e5e7eb; }
#pageTitle { font-size:18px; font-weight:800; color:#b71c1c; }
#pageSubtitle { color:#6b7280; }

#card { background:white; border:1px solid #e5e7eb; border-radius:10px; }
#cardTitle { font-size:13px; font-weight:700; color:#111827; }

QPushButton#primary { background:#d32f2f; color:white; border:none; border-radius:6px; padding:7px 14px; font-weight:600; }
QPushButton#primary:hover { background:#b71c1c; }
QPushButton#primary:disabled { background:#e5b5b5; }
QPushButton#tool { background:white; border:1px solid #d1d5db; border-radius:6px; padding:7px 12px; }
QPushButton#tool:hover { background:#fef2f2; border-color:#d32f2f; }
QPushButton#tool:checked { background:#fee2e2; border-color:#d32f2f; color:#b71c1c; font-weight:700; }
QPushButton#tool:disabled { color:#9ca3af; background:#f9fafb; }
QPushButton#danger { background:white; border:1px solid #d1d5db; border-radius:6px; padding:7px 12px; color:#b91c1c; }
QPushButton#danger:hover { background:#fee2e2; border-color:#b91c1c; }
QPushButton#danger:disabled { color:#9ca3af; background:#f9fafb; }

QLineEdit, QComboBox, QDoubleSpinBox, QSpinBox { background:white; border:1px solid #d1d5db; border-radius:6px; padding:5px 8px; }
QLineEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QSpinBox:focus { border-color:#d32f2f; }

QToolButton#mapBtn { background:white; border:1px solid #d1d5db; border-radius:6px; font-size:16px; font-weight:700; }
QToolButton#mapBtn:hover { background:#fee2e2; }

QTableView { background:white; alternate-background-color:#fafafa; border:1px solid #e5e7eb; border-radius:8px;
             gridline-color:#f0f0f0; selection-background-color:#fee2e2; selection-color:#111827; }
QHeaderView::section { background:#c62828; color:white; padding:6px; border:none; font-weight:600; }
QListWidget#alertList { border:none; background:transparent; }
QListWidget#alertList::item { padding:6px 4px; border-bottom:1px solid #f3f4f6; }
QListWidget#alertList::item:hover { background:#fef2f2; }
QTabWidget::pane { border:none; }
QTabBar::tab { padding:7px 16px; background:transparent; color:#6b7280; border-bottom:2px solid transparent; }
QTabBar::tab:selected { color:#b71c1c; border-bottom:2px solid #d32f2f; font-weight:700; }
QSplitter::handle { background:transparent; }
)");
}
