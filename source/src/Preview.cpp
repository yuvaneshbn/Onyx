#include "stdafx.h"
#include "Preview.h"

#ifdef _GLOBAL_VIDEO

#include "langpack.h"
#include "mainDlg.h"
#include "settings.h"

#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <windows.h>

Preview::Preview(QWidget *pParent)
    : QWidget(pParent)
    , widx(static_cast<pjmedia_vid_dev_index>(-1))
{
    setWindowFlags(Qt::WindowStaysOnTopHint |
                   Qt::Window |
                   Qt::WindowTitleHint |
                   Qt::WindowSystemMenuHint |
                   Qt::WindowMinimizeButtonHint);

    setWindowTitle(tr("Local Video"));
    resize(640, 480);

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::gray);
    setAutoFillBackground(true);
    setPalette(pal);

    setAttribute(Qt::WA_DeleteOnClose);
}

Preview::~Preview()
{
    if (mainDlg) {
        mainDlg->previewWin = nullptr;
    }
    if (is_pjsua_running()) {
        pjsua_vid_preview_stop(widx);
    }
}

void Preview::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (langPack.rtl) {
        setLayoutDirection(Qt::RightToLeft);
    }
}

void Preview::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void Preview::onWindowDestroyed()
{
}

void Preview::Start(int id)
{
    pjsua_vid_win_info wi;
    pjsua_vid_win_id wid = pjsua_vid_preview_get_win(widx);
    if (wid != PJSUA_INVALID_ID) {
        if (widx != id) {
            pjsua_vid_preview_stop(widx);
        } else {
            return;
        }
    }

    widx = id;
    pjsua_vid_preview_param pre_param;
    pj_status_t status;
    const pjmedia_coord pos = { 0, 0 };
    pjsua_vid_preview_param_default(&pre_param);
    status = pjsua_vid_preview_start(widx, &pre_param);
    if (status != PJ_SUCCESS) {
        close();
        return;
    }

    wid = pjsua_vid_preview_get_win(widx);
    pjsua_vid_win_get_info(wid, &wi);

    QRect rcClient = rect();
    QRect rcWind = frameGeometry();
    QPoint ptDiff;
    ptDiff.setX(rcWind.width() - rcClient.width());
    ptDiff.setY(rcWind.height() - rcClient.height());

    QRect screenRect = QApplication::primaryScreen()->availableGeometry();
    int w = 320 + ptDiff.x();
    int h = 240 + ptDiff.y();
    int x = screenRect.width() - w;
    int y = ptDiff.y() - 6;

    const pjmedia_rect_size size = { 320, 240 };
    const HWND hWndVideo = (HWND)wi.hwnd.info.win.hwnd;

    QMetaObject::invokeMethod(this, [this, x, y, w, h, wid, pos, size, hWndVideo]() {
        setGeometry(x, y, w, h);
        show();
        pjsua_vid_win_set_pos(wid, &pos);
        pjsua_vid_win_set_size(wid, &size);
        if (hWndVideo) {
            ::SetParent(hWndVideo, (HWND)this->winId());
        }
    }, Qt::QueuedConnection);
}

#endif // _GLOBAL_VIDEO
