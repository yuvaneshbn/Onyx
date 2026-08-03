#pragma once

#include "BaseDialog.h"
#include "define.h"
#include "resource.h"
#include "json.h"
#include "addons.h"
#include "global.h"
#include "Transfer.h"

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

#include <QCloseEvent>
#include <QDateTime>
#include <QHideEvent>
#include <QIcon>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QMenu>
#include <QPair>
#include <QMoveEvent>
#include <QMutex>
#include <QNetworkReply>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QTimer>
#include <functional>

#ifndef NIIF_INFO
#define NIIF_INFO QSystemTrayIcon::Information
#endif

class RinginDlg;
class AccountDlg;
class SettingsDlg;
class ShortcutsDlg;
class MessagesDlg;
class Dialer;
class Contacts;
class Calls;
class Preview;
class Transfer;
class CWnd;
class CDC;
class CCmdUI;

class CmainDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit CmainDlg(QWidget* parent = nullptr);
    ~CmainDlg() override;

    enum { IDD = IDD_MAIN };

    bool m_startMinimized = false;
    QPushButton* m_ButtonMenu = nullptr;

    SettingsDlg* settingsDlg = nullptr;
    bool shortcutsEnabled = false;
    bool shortcutsBottom = false;
    int shortcutsCount = 0;
    ShortcutsDlg* shortcutsDlg = nullptr;
    MessagesDlg* messagesDlg = nullptr;
    Transfer* transferDlg = nullptr;
    AccountDlg* accountDlg = nullptr;

    Dialer* pageDialer = nullptr;
    Contacts* pageContacts = nullptr;
    bool usersDirectoryLoaded = false;
    bool shortcutsURLLoaded = false;
    Calls* pageCalls = nullptr;

    bool notStopRinging = false;
    QList<RinginDlg*> ringinDlgs;
    QString dialNumberDelayed;
    pjsua_call_id autoAnswerTimerCallId = PJSUA_INVALID_ID;
    pjsua_call_id autoAnswerPlayCallId = PJSUA_INVALID_ID;
    pjsua_call_id forwardingTimerCallId = PJSUA_INVALID_ID;

    player_eof_data* player_eof_data = nullptr;

    int iconStatusbar = 0;
    QList<QIcon> imageListStatus;
    int widthAdd = 0;
    int heightAdd = 0;
    bool missed = false;

    QString callIdIncomingIgnore;
    QList<int> toneCalls;
    QList<int> attendedCalls;
    QMap<QString, QString> audioCodecList;
    QList<int> conferenceCalls;

    void InitUI();
    void ShowTrayIcon();
    void OnCreated();
    void PJCreate();
    void PJCreateRaw();
    void PJDestroy(bool exit = false);
    void PJAccountAdd();
    void PJAccountAddRaw();
    void PJAccountAddLocal();
    void PJAccountDelete(bool deep = false, bool exit = false, const QByteArray& code = QByteArray());
    void PJAccountDeleteLocal();
    void PJAccountConfig(pjsua_acc_config* acc_cfg, Account* account);
    void PJAudioCodecs();
#ifdef _GLOBAL_VIDEO
    void PJVideoCodecs();
#endif

    bool CommandLine(const QString& params);
    void TabFocusSet() override;
    void UpdateWindowText(const QString& text = QString(), int icon = IDI_DEFAULT, bool afterRegister = false);
    void PublishStatus(bool online = true, bool init = false);
    void TrayIconUpdateTip();
    void BaloonPopup(const QString& title, const QString& message, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Warning);
    void SwitchDND(int state = -1, bool update = false);
    bool GotoTabLParam(quintptr lParam);
    bool GotoTab(int i) override;
    bool GotoTab(int i, QTabWidget* tab);
    void ProcessCommand(const QString& str) override;
    void DialNumberFromCommandLine(const QString& params);
    void DialNumber(const QString& params);
    bool MakeCall(const QString& number, bool hasVideo = false, bool fromCommandLine = false,
                  bool noTransform = false, const QString& name = QString());
    bool MessagesOpen(const QString& number, bool forCall = false, bool noTransform = false,
                      const QString& name = QString());
    bool AutoAnswer(pjsua_call_id call_id, bool force = false);
    pjsua_call_id CurrentCallId();
    QString GetNameForCall(SIPURI& sipuri, call_user_data* user_data, QString& numberOriginal);

    void ShortcutAction(Shortcut* shortcut, bool block = false, bool second = false);
    void ShortcutsRemoveAll();
    bool isSubscribed = false;
    void SubsribeNumber(QString* number);
    void UnsubscribeNumber(QString* number);
    void Subscribe();
    void Unsubscribe();
    void PlayerPlay(const QString& filename, bool noLoop = false, bool inCall = false, bool isAA = false);
    bool CopyStringToClipboard(const QString& str);
    void OnTimerProgress();
    void OnTimerCall();
    void OnTimerNetworkChange();

    void UsersDirectoryLoad(bool update = false);
    void ShortcutsURLLoad();
    void SetupJumpList();
    void RemoveJumpList();
    void MainPopupMenu(bool isMenuButton = false);
    void SetPaneText2(const QString& str = QString());
    void AccountSettingsPendingSave();
    void OnAccountChanged(bool init = false);
    void OpenTransferDlg(QWidget* pParent, onyx_action action,
                         pjsua_call_id call_id = PJSUA_INVALID_ID,
                         Contact* selectedContact = nullptr);
    void UpdateSoundDevicesIds();
    void PlayerStop();
#ifdef _GLOBAL_VIDEO
    Preview* previewWin = nullptr;
    int VideoCaptureDeviceId(const QString& name = QString());
    void createPreviewWin();
#endif
    void MessagesIncoming(QString* number, QString* message, QDateTime* pTime = nullptr);

    bool CommandCallAnswer();
    bool CommandCallReject();
    bool CommandCallPickup(const QString& number);

protected:
    void closeEvent(QCloseEvent* event) override;
    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    void onUpdateWindowText(int wParam = 0, const QString& text = QString());
    void onTrayNotify(QSystemTrayIcon::ActivationReason reason);
    void onCreateRingingDlg(pjsua_call_id call_id);
    void onRefreshLevels();
    void onRegState2(int code, const QString& headerError);
    void onCallState(pjsua_call_info* call_info, call_user_data* user_data);
    void onIncomingCall(pjsua_call_info* call_info, call_user_data* user_data);
    void onMWIInfo(WPARAM wParam, LPARAM lParam);
    void onCallMediaState(pjsua_call_info* call_info, call_user_data* user_data);
    void onCallTransferStatus(pjsua_call_info* call_info, const QString& str);
    void onPager(QString* number, QString* message);
    void onPagerStatus(QString* number, QString* message);
    void onBuddyState(pjsua_buddy_id buddy_id);
    void onCopyData(const QString& data);
    void CreationComplete();
    void OnNetworkChange();
    void OnRestart();
    void PostRestart();
    void OnPowerBroadcast(quint32);
    void OnSysCommand(UINT nID, LPARAM lParam = 0);
    void OnBnClickedOk();
    void OnBnClickedMenu();
    void OnContextMenu(QWidget* w, const QPoint& pos);
    BOOL OnQueryEndSession();
    void OnClose();
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
    void OnSessionChange(UINT nSessionState, UINT nId);
    void OnMove(int x, int y);
    void OnSize(UINT type, int w, int h);
    void onShellHookMessage();
    LRESULT onCallAnswer(int call_id, int lParam = 0);
    LRESULT onCallHangup(int call_id);
    LRESULT onTabIconUpdate(int call_id);
    void onPlayerPlay(int sound, const QString& custom = QString());
    void onPlayerStop();
    void onCommandLine(const QString& params);
    void OnAccount();
    void OnMenuAccountAdd();
    void OnMenuAccountEdit(int nID);
    void OnMenuAccountChange(int nID);
    void OnMenuAccountLocalEdit();
    void OnMenuCustomRange(int nID);
    void OnMenuSettings();
    void OnMenuShortcuts();
    void OnMenuAlwaysOnTop();
    void OnMenuLog();
    void OnMenuExit();
    void OnTimer(int timerId);
    void OnTcnSelchangeTab(int index);
    void OnTcnSelchangingTab(int index);
    void OnMenuWebsite();
    void OnMenuHelp();
    void OnMenuAddl();
    void OnMuteInput();
    void OnMuteOutput();
    void OnCheckUpdates();
    void CheckUpdates();
    LRESULT OnUpdateCheckerLoaded(URLGetAsyncData* response);
    LRESULT onUsersDirectoryLoaded(URLGetAsyncData* response);
    void onShortcutsURLLoaded(URLGetAsyncData* response);
    LRESULT onCustomLoaded(WPARAM wParam, LPARAM lParam);
    void OnUpdatePane(CCmdUI* pCmdUI);

private:
    void setupUi();
    void createTrayIcon();
    void createStatusBar();
    void createTimers();
    QTimer* timerForId(int timerId) const;
    void showNotPortedMessage(const QString& feature);

    QIcon m_hIcon;
    QIcon iconSmall;
    QIcon iconInactive;
    QIcon iconMissed;

    QTabWidget* mainTab = nullptr;
    QLabel* pane2Label = nullptr;
    QWidget* centralHost = nullptr;
    QTabWidget* tabWidget = nullptr;
    QStatusBar* statusBar = nullptr;
    QMap<int, QLabel*> m_barLabels;

    QSystemTrayIcon* trayIcon = nullptr;
    QMenu* trayMenu = nullptr;
    QStatusBar* m_bar = nullptr;

    QTimer* timerIdle = nullptr;
    QTimer* timerTone = nullptr;
    QTimer* timerBalance = nullptr;
    QTimer* timerInitRingin = nullptr;
    QTimer* timerCall = nullptr;
    QTimer* timerContactsBlink = nullptr;
    QTimer* timerShortcutsBlink = nullptr;
    QTimer* timerDirectory = nullptr;
    QTimer* timerContacts = nullptr;
    QTimer* timerCalls = nullptr;
    QTimer* timerSave = nullptr;
    QTimer* timerNetworkChanged = nullptr;
    QTimer* timerSwitchDevices = nullptr;
    QTimer* timerHeadset = nullptr;
    QTimer* timerVuMeter = nullptr;
    QTimer* timerAutoAnswer = nullptr;
    QTimer* timerForwarding = nullptr;
    QTimer* timerProgress = nullptr;
    QTimer* timerDTMF = nullptr;
    QTimer* timerCustom = nullptr;
    QMap<int, QPair<int, std::function<void()>>> m_timers;

    unsigned char m_tabPrev = 0;
    qint64 m_lastInputTime = 0;
    int m_idleCounter = 0;
    pjrpid_activity m_PresenceStatus = PJRPID_ACTIVITY_UNKNOWN;
    bool newMessages = false;

    static QMutex gethostbyaddrThreadCS;
    static QString gethostbyaddrThreadResult;
};

extern CmainDlg* mainDlg;

void on_buddy_state(pjsua_buddy_id buddy_id);
