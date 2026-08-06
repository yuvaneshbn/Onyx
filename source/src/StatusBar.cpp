#include "StatusBar.h"
#include <QFrame>

StatusBar::StatusBar(QWidget *parent)
    : QStatusBar(parent)
{
}

StatusBar::~StatusBar()
{
}

void StatusBar::mouseReleaseEvent(QMouseEvent *event)
{
    // Original: OnLButtonUp
    // Empty implementation as per the provided MFC code
    QStatusBar::mouseReleaseEvent(event);
}

void StatusBar::mouseMoveEvent(QMouseEvent *event)
{
    // Original: OnMouseMove
    QStatusBar::mouseMoveEvent(event);
}

void StatusBar::OnIdleUpdateCmdUI()
{
    
    if (isVisible()) {
        QWidget *pParent = parentWidget();
        if (pParent) {
            // pParent->update();   // Qt equivalent of CFrameWnd::OnUpdateCmdUI
        }
    }
}
