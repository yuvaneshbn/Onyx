#pragma once

#include <QStatusBar>
#include <QMouseEvent>
#include "const.h"

class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);
    virtual ~StatusBar();

protected:
    // Original MFC message handlers mapped to Qt event overrides
    void mouseReleaseEvent(QMouseEvent *event) override;   // ON_WM_LBUTTONUP
    void mouseMoveEvent(QMouseEvent *event) override;      // ON_WM_MOUSEMOVE

public slots:
    // Equivalent of ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
    void OnIdleUpdateCmdUI();
};