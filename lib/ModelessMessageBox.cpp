#include "ModelessMessageBox.h"
#include <QMessageBox>
#include <QTimer>
#include <QDebug>

// ----------------------------------------------------------------------
// ModelessMessageBox implementation
// ----------------------------------------------------------------------

void ModelessMessageBox::show(QWidget* parent,
                              const QString& text,
                              const QString& caption,
                              int type)
{
    // Create the message box on the heap
    QMessageBox* msgBox = new QMessageBox(parent);

    // Set properties
    msgBox->setText(text);
    if (!caption.isEmpty())
        msgBox->setWindowTitle(caption);

    // Map the original uType to Qt's icons and buttons
    // The original uType is a combination of button set and icon.
    // We need to extract them. Since the original used MB_OK, MB_YESNO, etc.,
    // we can map common ones. For simplicity, we'll treat 'type' as
    // a combination of standard buttons and icon (like Qt's QMessageBox::StandardButtons and Icon).
    // But to keep compatibility, we'll parse the bit flags.
    // Since the original passed MB_OK, MB_YESNO, etc. which are ints,
    // we'll map them to QMessageBox equivalents.

    // Extract icon part
    int iconPart = type & 0x000000F0; // MB_ICONMASK
    QMessageBox::Icon icon = QMessageBox::Information;
    switch (iconPart)
    {
    case 0x00000010: // MB_ICONHAND
        icon = QMessageBox::Critical;
        break;
    case 0x00000020: // MB_ICONQUESTION
        icon = QMessageBox::Question;
        break;
    case 0x00000030: // MB_ICONEXCLAMATION
        icon = QMessageBox::Warning;
        break;
    case 0x00000040: // MB_ICONASTERISK
        icon = QMessageBox::Information;
        break;
    default:
        icon = QMessageBox::Information;
        break;
    }
    msgBox->setIcon(icon);

    // Extract button part
    int buttonPart = type & 0x0000000F; // MB_TYPEMASK
    QMessageBox::StandardButtons buttons = QMessageBox::NoButton;
    switch (buttonPart)
    {
    case 0x00000000: // MB_OK
        buttons = QMessageBox::Ok;
        break;
    case 0x00000001: // MB_OKCANCEL
        buttons = QMessageBox::Ok | QMessageBox::Cancel;
        break;
    case 0x00000002: // MB_ABORTRETRYIGNORE
        buttons = QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore;
        break;
    case 0x00000003: // MB_YESNOCANCEL
        buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
        break;
    case 0x00000004: // MB_YESNO
        buttons = QMessageBox::Yes | QMessageBox::No;
        break;
    case 0x00000005: // MB_RETRYCANCEL
        buttons = QMessageBox::Retry | QMessageBox::Cancel;
        break;
    case 0x00000006: // MB_CANCELTRYCONTINUE
        buttons = QMessageBox::Cancel | QMessageBox::Retry | QMessageBox::Ignore;
        break;
    default:
        buttons = QMessageBox::Ok;
        break;
    }
    msgBox->setStandardButtons(buttons);

    // Set default button to the first one (like original)
    msgBox->setDefaultButton(QMessageBox::Ok);

    // Make the dialog modeless (non-modal) and show it
    // By default, QMessageBox is modal; we set it to non-modal.
    msgBox->setModal(false);

    // Ensure the dialog is deleted when it is closed
    // We connect the finished() signal (emitted when a button is clicked or dialog closed)
    // to deleteLater().
    QObject::connect(msgBox, &QMessageBox::finished, msgBox, &QMessageBox::deleteLater);

    // Show the dialog
    msgBox->show();
}

void ModelessMessageBox::show(QWidget* parent,
                              const QString& text,
                              const QString& caption,
                              QMessageBox::StandardButtons buttons,
                              QMessageBox::Icon icon)
{
    // Create the message box on the heap
    QMessageBox* msgBox = new QMessageBox(parent);

    msgBox->setText(text);
    if (!caption.isEmpty())
        msgBox->setWindowTitle(caption);
    msgBox->setIcon(icon);
    msgBox->setStandardButtons(buttons);
    msgBox->setDefaultButton(buttons & QMessageBox::Ok ? QMessageBox::Ok : QMessageBox::Yes);

    // Make it non-modal
    msgBox->setModal(false);

    // Auto-delete on close
    QObject::connect(msgBox, &QMessageBox::finished, msgBox, &QMessageBox::deleteLater);

    msgBox->show();
}

// ----------------------------------------------------------------------
// Global convenience function
// ----------------------------------------------------------------------
void ModelessMessageBox(QWidget* parent,
                        const QString& text,
                        const QString& caption,
                        int type)
{
    ModelessMessageBox::show(parent, text, caption, type);
}