#include "DarkTheme.h"
#include <QPalette>
#include <QColor>
#include <QFont>

namespace Onyx {

void DarkTheme::applyTheme(QApplication& app) {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(15, 23, 42));       // Slate 900
    darkPalette.setColor(QPalette::WindowText, QColor(248, 250, 252)); // Slate 50
    darkPalette.setColor(QPalette::Base, QColor(30, 41, 59));         // Slate 800
    darkPalette.setColor(QPalette::AlternateBase, QColor(15, 23, 42));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(248, 250, 252));
    darkPalette.setColor(QPalette::ToolTipText, QColor(15, 23, 42));
    darkPalette.setColor(QPalette::Text, QColor(248, 250, 252));
    darkPalette.setColor(QPalette::Button, QColor(30, 41, 59));
    darkPalette.setColor(QPalette::ButtonText, QColor(248, 250, 252));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(0, 120, 212));        // Onyx Blue
    darkPalette.setColor(QPalette::Highlight, QColor(0, 120, 212));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    app.setPalette(darkPalette);
    app.setStyleSheet(getStyleSheet());

    QFont font("Segoe UI", 9);
    app.setFont(font);
}

QString DarkTheme::getStyleSheet() {
    return R"(
        QMainWindow {
            background-color: #0d111a;
        }

        QWidget {
            color: #e2e8f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 10pt;
        }

        /* Tabs */
        QTabWidget::pane {
            border: 1px solid #1e293b;
            background-color: #0f172a;
            border-radius: 8px;
        }

        QTabBar::tab {
            background-color: #1e293b;
            color: #94a3b8;
            padding: 10px 20px;
            margin-right: 4px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            font-weight: 600;
        }

        QTabBar::tab:selected {
            background-color: #0284c7;
            color: #ffffff;
        }

        QTabBar::tab:hover:!selected {
            background-color: #334155;
            color: #f1f5f9;
        }

        /* Buttons */
        QPushButton {
            background-color: #1e293b;
            color: #f8fafc;
            border: 1px solid #334155;
            border-radius: 6px;
            font-weight: 600;
        }

        QPushButton:hover {
            background-color: #334155;
            border-color: #475569;
        }

        QPushButton:pressed {
            background-color: #0f172a;
        }

        QPushButton#primaryBtn {
            background-color: #0284c7;
            border: 1px solid #0369a1;
        }

        QPushButton#primaryBtn:hover {
            background-color: #0369a1;
        }

        QPushButton#dangerBtn {
            background-color: #dc2626;
            border: 1px solid #b91c1c;
        }

        QPushButton#dangerBtn:hover {
            background-color: #b91c1c;
        }

        QPushButton#successBtn {
            background-color: #16a34a;
            border: 1px solid #15803d;
        }

        QPushButton#successBtn:hover {
            background-color: #15803d;
        }

        /* Line Edits & Text Areas */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: #1e293b;
            color: #f8fafc;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 8px;
            selection-background-color: #0284c7;
        }

        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border: 1px solid #0284c7;
        }

        /* List & Tree & Table Views */
        QListView, QTableView, QTreeWidget {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 6px;
            gridline-color: #334155;
            color: #f8fafc;
            selection-background-color: #0284c7;
            selection-color: #ffffff;
        }

        QHeaderView::section {
            background-color: #0f172a;
            color: #94a3b8;
            padding: 6px;
            border: 1px solid #334155;
            font-weight: bold;
        }

        /* Progress Bar */
        QProgressBar {
            border: 1px solid #334155;
            border-radius: 6px;
            text-align: center;
            background-color: #1e293b;
            color: #ffffff;
            font-weight: bold;
        }

        QProgressBar::chunk {
            background-color: #0284c7;
            border-radius: 5px;
        }

        /* Combo Box */
        QComboBox {
            background-color: #1e293b;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 6px 12px;
            color: #f8fafc;
        }

        QComboBox::drop-down {
            border: none;
        }

        QComboBox QAbstractItemView {
            background-color: #1e293b;
            border: 1px solid #334155;
            selection-background-color: #0284c7;
            color: #f8fafc;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            border: none;
            background-color: #0f172a;
            width: 10px;
            margin: 0;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background-color: #334155;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #475569;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
}

} // namespace Onyx
