#ifndef MODELESSMESSAGEBOX_H
#define MODELESSMESSAGEBOX_H

#include <QWidget>
#include <QMessageBox>
#include <QString>

/**
 * Class for showing a modeless (non-blocking) message box.
 * The dialog is created on the heap and automatically deleted when closed.
 * Usage: ModelessMessageBox::show(parent, "Text", "Caption", QMessageBox::Information);
 */
class ModelessMessageBox
{
public:
    /**
     * Shows a non-modal message box.
     * @param parent Parent widget (can be nullptr).
     * @param text   The message text.
     * @param caption The window title (optional).
     * @param type   Standard button set and icon (default QMessageBox::Information).
     *               This corresponds to the original uType (MB_OK, etc.).
     */
    static void show(QWidget* parent = nullptr,
                     const QString& text = QString(),
                     const QString& caption = QString(),
                     int type = QMessageBox::Information);

    // Convenience overload with standard buttons
    static void show(QWidget* parent,
                     const QString& text,
                     const QString& caption,
                     QMessageBox::StandardButtons buttons,
                     QMessageBox::Icon icon = QMessageBox::Information);
};

/**
 * Global convenience function (like the original ModelessMessageBox).
 * Shows a non-modal message box.
 */
void ModelessMessageBox(QWidget* parent = nullptr,
                        const QString& text = QString(),
                        const QString& caption = QString(),
                        int type = QMessageBox::Information);

#endif // MODELESSMESSAGEBOX_H
