// BaseDialog.h
#pragma once

#include "define.h"
#include <QDialog>
#include <QWidget>
#include <QRect>
#include <QSize>
#include <vector>

class QShowEvent;
class QResizeEvent;
class QKeyEvent;

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor matches original signature but uses QWidget* parent
    explicit BaseDialog(QWidget *parent = nullptr);
    ~BaseDialog() override;

    // --- Public interface mirrored from CBaseDialog ---

    // Original overloads: one by control ID, one by HWND.
    // In Qt, we keep the HWND version as QWidget*, and the ID version uses findChild
    void AutoMove(int iID, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);
    void AutoMove(QWidget *child, double dXMovePct, double dYMovePct, double dXSizePct, double dYSizePct);
    void AutoUnmove(QWidget *child);

    void WinHelp(DWORD dwData, UINT nCmd);
    void OpenHelp();

    // Pure virtual functions (abstract interface)
    virtual void TabFocusSet() = 0;
    virtual bool GotoTab(int i) = 0;                    // CTabCtrl* removed – Qt doesn't need it
    virtual void ProcessCommand(const QString &str) = 0;

    // Static main window access (replaces AfxGetApp()->GetMainWnd())
    static void SetMainWindow(BaseDialog *mainWnd);
    static BaseDialog *MainWindow();

signals:
    // Emitted when a command should be posted to the main window (replaces PostMessage(WM_COMMAND))
    void postCommand(int commandId);

protected:
    // Override Qt events to mimic original MFC messages
    void showEvent(QShowEvent *event) override;       // Replaces OnInitDialog
    void resizeEvent(QResizeEvent *event) override;   // Replaces OnSize
    void keyPressEvent(QKeyEvent *event) override;    // Replaces PreTranslateMessage (key handling part)

private:
    // Helper to find a child widget by its object name (mapping for control IDs)
    QWidget *findChildByControlId(int id) const;

    // Internal structure for auto‑move / auto‑size children
    struct AutoMoveChild
    {
        QWidget *widget = nullptr;
        double dXMoveFrac = 0.0;
        double dYMoveFrac = 0.0;
        double dXSizeFrac = 0.0;
        double dYSizeFrac = 0.0;
        QRect rcInitial;          // stored baseline (parent‑relative)
    };

    std::vector<AutoMoveChild> m_autoMoveChildren;
    QSize m_initialSize;          // client area size at first show
    QSize m_minimumSize;          // minimum dialog size (like m_szMinimum)
    bool m_initialized = false;   // true after first showEvent

    static BaseDialog *s_mainWindow;
};
