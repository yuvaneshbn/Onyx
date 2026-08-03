#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QFont>
#include <QIcon>
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "langpack.h"

// Forward declarations of external globals (assumed defined elsewhere)
class CmainDlg;
extern CmainDlg* mainDlg;
struct AccountSettings;   // assume defined
extern AccountSettings accountSettings;
extern LangPackStruct langPack;

class RinginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RinginDlg(QWidget *pParent = nullptr);
    ~RinginDlg();

    pjsua_call_id call_id;
    bool answered;
    bool remoteHasVideo;

    void SetCallId(pjsua_call_id new_call_id);
    void OnAnswer();
    void CallAccept(bool hasVideo = false);
    void OnBnClickedDecline() { onDeclineClicked(); }

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void onAnswerClicked();
    void onDeclineClicked();
    void onVideoClicked();
    void onTransferClicked();

private:
    void Close(bool accept = false);

    // UI elements
    QPushButton *m_pAnswerBtn;
    QPushButton *m_pDeclineBtn;
    QPushButton *m_pIgnoreBtn;
    QPushButton *m_pVideoBtn;
    QPushButton *m_pTransferBtn;
    QLabel *m_pCallerNameLabel;
    QLabel *m_pCallerAddrLabel;
    QLabel *m_pRinginTextLabel;

    QTimer *m_pInitTimer;
    QFont m_font;
    QFont m_font_ignore;
    QIcon m_iconTransfer;

    // Helper to find child widget by ID (since we set objectName to the ID string)
    template <typename T>
    T* findChildById(int id) {
        return findChild<T*>(QString::number(id));
    }
};
