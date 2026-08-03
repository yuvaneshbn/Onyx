// BaseDialog.cpp
#include "BaseDialog.h"
#include "resource.h"        // for command IDs (ID_ACCOUNT_EDIT_RANGE, etc.)
#include "global.h"          // for _GLOBAL_MENU_HELP
#include "settings.h"        // (if needed)

#include <QShowEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QAbstractEventDispatcher>

BaseDialog *BaseDialog::s_mainWindow = nullptr;

BaseDialog::BaseDialog(QWidget *parent)
    : QDialog(parent)
{
    // In original, mainWnd was set to NULL; here static pointer used via SetMainWindow
    m_minimumSize = QSize(0, 0);   // default minimum
}

BaseDialog::~BaseDialog() = default;

// ---------- Original WM_INITDIALOG equivalent ----------
void BaseDialog::showEvent(QShowEvent *event)
{
    if (!m_initialized) {
        // Use initial dialog size as default minimum (like original)
        if (m_minimumSize.isNull()) {
            m_minimumSize = size();
        }
        setMinimumSize(m_minimumSize);

        // Store the initial client area size as baseline for auto‑move calculations
        m_initialSize = size();  // client area = dialog size in Qt (no separate client rect)

        // For each auto‑move child, store its initial geometry
        for (auto &child : m_autoMoveChildren) {
            if (child.widget)
                child.rcInitial = child.widget->geometry();
        }

        m_initialized = true;
    }
    QDialog::showEvent(event);
}

// ---------- Original WM_SIZE ----------
void BaseDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    if (!m_initialized)
        return;

    int iXDelta = width()  - m_initialSize.width();
    int iYDelta = height() - m_initialSize.height();

    for (auto &child : m_autoMoveChildren) {
        if (child.widget) {
            QRect rcNew = child.rcInitial;
            rcNew.moveLeft(rcNew.left() + int(iXDelta * child.dXMoveFrac));
            rcNew.moveTop(rcNew.top()   + int(iYDelta * child.dYMoveFrac));
            rcNew.setWidth(rcNew.width()   + int(iXDelta * child.dXSizeFrac));
            rcNew.setHeight(rcNew.height() + int(iYDelta * child.dYSizeFrac));
            child.widget->setGeometry(rcNew);
        }
    }
}

// ---------- Original PreTranslateMessage (key handling) ----------
void BaseDialog::keyPressEvent(QKeyEvent *event)
{
    // Ensure mainWnd pointer is resolved (once per process)
    if (!s_mainWindow)
        s_mainWindow = qobject_cast<BaseDialog*>(QApplication::activeWindow());

    BaseDialog *mainWnd = s_mainWindow;
    bool catched = false;
    const int key = event->key();
    const Qt::KeyboardModifiers mods = event->modifiers();
    const bool control = mods & Qt::ControlModifier;
    const bool alt = mods & Qt::AltModifier;
    const bool shift = mods & Qt::ShiftModifier;

    int postCommand = 0;

    if (!catched) {
        if (control && !alt) {
            // Ctrl+Tab
            if (key == Qt::Key_Tab) {
                if (mainWnd && (this == mainWnd || parentWidget() == mainWnd)) {
                    if (shift)
                        mainWnd->GotoTab(-2);
                    else
                        mainWnd->GotoTab(-1);
                    catched = true;
                }
            }
            // Ctrl+letter shortcuts
            switch (key) {
            case Qt::Key_M: postCommand = ID_ACCOUNT_EDIT_RANGE; break;
            case Qt::Key_L: postCommand = ID_ACCOUNT_EDIT_LOCAL; break;
            case Qt::Key_P: postCommand = ID_SETTINGS;            break;
            case Qt::Key_S: postCommand = ID_SHORTCUTS;           break;
            case Qt::Key_Q: postCommand = ID_EXIT;                break;
            case Qt::Key_AsciiTilde: // VK_OEM_3 (tilde)
            case Qt::Key_I: postCommand = ID_MUTE_INPUT;          break;
            case Qt::Key_O: postCommand = ID_MUTE_OUTPUT;         break;
            case Qt::Key_W: postCommand = ID_MENU_WEBSITE;        break;
            default: break;
            }
        }
        else {
            // F2 – answer call
            if (key == Qt::Key_F2) {
                onyx_call_answer();
                catched = true;
            }
            // F4 – hangup all
            else if (key == Qt::Key_F4) {
                pjsua_call_hangup_all();
                catched = true;
            }
            // Escape – if menu button has focus, return focus to main area
            else if (key == Qt::Key_Escape) {
                if (mainWnd && (this == mainWnd || parentWidget() == mainWnd)) {
                    QWidget *menuButton = mainWnd->findChildByControlId(IDC_MAIN_MENU);
                    if (menuButton && menuButton->hasFocus()) {
                        mainWnd->TabFocusSet();
                        catched = true;
                    }
                }
            }
        }

        // Post command to main window if any
        if (mainWnd && postCommand) {
            emit mainWnd->postCommand(postCommand);
            catched = true;
        }
    }

    // Original WM_SYSKEYDOWN / WM_SYSKEYUP handling (Alt, F10 menu focus)
    if (!catched) {
        if ((key == Qt::Key_F10 && !control) || (key == Qt::Key_Alt && event->type() == QEvent::KeyPress)) {
            if (mainWnd && (this == mainWnd || parentWidget() == mainWnd)) {
                QWidget *menuButton = mainWnd->findChildByControlId(IDC_MAIN_MENU);
                if (menuButton) {
                    if (menuButton->hasFocus()) {
                        if (key == Qt::Key_F10)
                            mainWnd->TabFocusSet();
                    } else {
                        menuButton->setFocus();
                    }
                    catched = true;
                }
            }
        }
    }

    if (catched)
        event->accept();
    else
        QDialog::keyPressEvent(event);
}

// ---------- AutoMove / AutoUnmove ----------
void BaseDialog::AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
{
    QWidget *child = findChildByControlId(iID);
    if (child)
        AutoMove(child, dXMovePct, dYMovePct, dXSizePct, dYSizePct);
}

void BaseDialog::AutoMove(QWidget *child, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct)
{
    if (!child)
        return;

    Q_ASSERT((dXMovePct + dXSizePct) <= 100.0);
    Q_ASSERT((dYMovePct + dYSizePct) <= 100.0);

    AutoMoveChild s;
    s.widget = child;
    s.dXMoveFrac = dXMovePct / 100.0;
    s.dYMoveFrac = dYMovePct / 100.0;
    s.dXSizeFrac = dXSizePct / 100.0;
    s.dYSizeFrac = dYSizePct / 100.0;

    // If already initialized, store current geometry; otherwise it will be captured in showEvent()
    if (m_initialized)
        s.rcInitial = child->geometry();

    m_autoMoveChildren.push_back(s);
}

void BaseDialog::AutoUnmove(QWidget *child)
{
    for (auto it = m_autoMoveChildren.begin(); it != m_autoMoveChildren.end(); ++it) {
        if (it->widget == child) {
            m_autoMoveChildren.erase(it);
            break;
        }
    }
}

// ---------- Help functions ----------
void BaseDialog::WinHelp(DWORD dwData, UINT nCmd)
{
    OpenHelp();
}

void BaseDialog::OpenHelp()
{
    QDesktopServices::openUrl(QUrl(QString::fromLatin1(_GLOBAL_MENU_HELP)));
}

// ---------- Static main window access ----------
void BaseDialog::SetMainWindow(BaseDialog *mainWnd)
{
    s_mainWindow = mainWnd;
}

BaseDialog *BaseDialog::MainWindow()
{
    return s_mainWindow;
}

// ---------- Helper to map control ID to QWidget (using object name) ----------
QWidget *BaseDialog::findChildByControlId(int id) const
{
    // In Qt, we can use the resource ID's string representation as the object name.
    // For example, a button with ID IDC_MAIN_MENU might be named "IDC_MAIN_MENU".
    // This requires the .ui file or code to set that name.
    // Here we simply search by a common naming pattern: "ID_<number>" or numeric string.
    const QString name = QString::number(id);  // or better: "ID_%1".arg(id)
    QWidget *w = findChild<QWidget*>(name);
    return w;
}
