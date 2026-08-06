// MessagesDlg.h converted to Qt
#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QPushButton>
#include <QTextBrowser>
#include <QFont>
#include <QList>
#include <QIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QMouseEvent>
#include <QAction>
#include <QMap>
#include <QTimer>

#include "resource.h"
#include "global.h"
#include "BaseDialog.h"   // your base class if needed, otherwise QWidget
#include "ButtonEx.h"     // custom button class if used, otherwise QPushButton
#include "ClosableTabCtrl.h" // if needed, otherwise use QTabWidget with closeable tabs
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

enum {
    ONYX_TAB_ICON_CALL_OUT,
    ONYX_TAB_ICON_CALL_IN,
    ONYX_TAB_ICON_CALL_MISS,
    ONYX_TAB_ICON_ACTIVE,
    ONYX_TAB_ICON_ACTIVE_SECURE,
    ONYX_TAB_ICON_CONFERENCE,
    ONYX_TAB_ICON_CONFERENCE_SECURE,
    ONYX_TAB_ICON_MESSAGE_IN,
    ONYX_TAB_ICON_ON_HOLD,
    ONYX_TAB_ICON_ON_REMOTE_HOLD,
    ONYX_TAB_ICON_ON_REMOTE_HOLD_CONFERENCE
};

class MessagesDlg : public QWidget
{
    Q_OBJECT
public:
    explicit MessagesDlg(QWidget *parent = nullptr);
    ~MessagesDlg();

    // Replaces CBaseDialog::PostNcDestroy, etc.
    void TabFocusSet() {} // not needed in Qt, focus policy
    bool GotoTab(int i, QTabWidget *tab = nullptr);
    void ProcessCommand(const QString &str) {}

    MessagesContact* AddTab(const QString &number, bool activate = false,
                            pjsua_call_info *call_info = nullptr,
                            call_user_data *user_data = nullptr,
                            bool notShowWindow = false,
                            bool ifExists = false,
                            const QString &numberOriginal = "",
                            const QString &name = "");
    void OnChangeTab(pjsua_call_info *p_call_info = nullptr, call_user_data *user_data = nullptr);
    void OnEndCall(pjsua_call_info *call_info, call_user_data *user_data);
    bool CallCheck();
    pjsua_call_id CurrentCallId();
    void Call(bool hasVideo = false);
    pjsua_call_id CallMake(const QString &number, bool hasVideo = false,
                           pj_status_t *pStatus = nullptr,
                           call_user_data *user_data = nullptr);
    void CallStart(bool hasVideo = false, call_user_data *user_data = nullptr);
    void AddMessage(MessagesContact* messagesContact, const QString &message,
                    int type = ONYX_MESSAGE_TYPE_SYSTEM, bool blockForeground = false,
                    QDateTime *pTime = nullptr);
    MessagesContact* GetMessageContact(int i = -1);
    MessagesContact* GetMessageContactInCall();
    int GetCallDuration(pjsua_call_id *call_id = nullptr);
    int GetCallsCount(bool withIncoming = false, bool onlyActive = false);
    bool SendInstantMessage(MessagesContact *messagesContact, const QString &message,
                            const QString &number = "");
    void UpdateHoldButton(pjsua_call_info *call_info);
    void UpdateRecButton(call_user_data *user_data = nullptr);
    void UpdateTabIcon(MessagesContact *messagesContact, int tabIndex = -1,
                       pjsua_call_info *call_info = nullptr,
                       call_user_data *user_data = nullptr);
    bool CallAction(int action, const QString &number, pjsua_call_id call_id = PJSUA_INVALID_ID);
    void Merge(pjsua_call_id call_id);
    void Separate(pjsua_call_id call_id);

    QTabWidget* tab;
    MessagesContact* lastCall;
    QPushButton* m_ButtonCall;
    QPushButton* m_ButtonEnd;

    QIcon m_hIconHold;
    QIcon m_hIconResume;

    void UpdateCallButton(bool active = false, pjsua_call_info *call_info = nullptr,
                          call_user_data *user_data = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void OnTcnSelchangeTab(int index);
    void OnTcnSelchangingTab(int index);
    void OnBnClickedCallEnd();
    void OnBnClickedVideoCall();
    void OnBnClickedActions(bool isConference = false);
    void OnBnClickedTransfer();
    void OnBnClickedConference();
    void OnBnClickedHold();
    void OnBnClickedEnd();
    void OnCloseAllTabs();
    void OnGoToLastTab();
    void OnBnClickedCloseAll();
    void OnBnClickedLastCall();
    void OnCopy();
    void OnSelectAll();
    void OnTransfer();
    void OnAttendedTransfer();
    void OnAttendedTransferRange(int index);
    void OnConference();
    void OnMerge(int index);
    void OnMergeAll();
    void OnSeparate();
    void OnSeparateAll();
    void OnDisconnect();
    void OnEnMsgfilterMessage();
    void OnEnLink(const QUrl &url);
    void onTabCloseRequested(int index);
    void onContextMenuRequested(const QPoint &pos);

private:
    bool CloseTab(int i, bool safe = false);

    QFont fontList;
    QFont fontMessage;
    QTextBrowser *richEditList;
    QTextEdit *richEdit;
    QList<QIcon> tabIcons;      // for tab icons
    QMenu *menuTransfer;
    QMenu *menuConference;
    QMenu *menuAttendedTransfer;
    QMenu *menuMerge;

    // Store messages per contact as RTF/HTML strings
    QMap<MessagesContact*, QString> contactMessages;

    void setupUI();
    void retranslateUI();
    void updateButtonVisibility();
};
