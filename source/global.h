#pragma once

#include "define.h"
#include "OnyxUtils.h"

#include <QString>
#include <QByteArray>
#include <QList>
#include <QMutex>
#include <QDateTime>
#include <QIcon>
#include <QWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include <QRegularExpression>
#include <atomic>
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>


#ifndef Lock
#define Lock lock
#endif
#ifndef Unlock
#define Unlock unlock
#endif

enum EUserWndMessages
{
    UM_FIRST_USER_MSG = (0x0400 + 0x100 + 1),   // WM_USER equivalent

    UM_UPDATEWINDOWTEXT,
    UM_NOTIFYICON,

    UM_UPDATE_SETTINGS,

    UM_CREATE_RINGING,
    UM_CALL_ANSWER,
    UM_CALL_HANGUP,
    UM_TAB_ICON_UPDATE,
    UM_ON_ACCOUNT,
    UM_ON_REG_STATE2,
    UM_ON_CALL_STATE,
    UM_ON_INCOMING_CALL,
    UM_ON_CALL_TRANSFER_STATUS,
    UM_ON_MWI_INFO,
    UM_ON_CALL_MEDIA_STATE,
    UM_ON_PAGER,
    UM_ON_PAGER_STATUS,
    UM_ON_BUDDY_STATE,
    UM_ON_PLAYER_STOP,
    UM_SET_PANE_TEXT,
    UM_REFRESH_LEVELS,
    UM_USERS_DIRECTORY,
    UM_CUSTOM,
    UM_ON_BALANCE_PLAIN,
    UM_ON_BALANCE_OPTIONS,
    UM_ON_COMMAND_LINE,
    UM_NETWORK_CHANGE,
    UM_RESTART,

    IDT_TIMER_IDLE,
    IDT_TIMER_TONE,
    IDT_TIMER_BALANCE,
    IDT_TIMER_INIT_RINGIN,
    IDT_TIMER_CALL,
    IDT_TIMER_CONTACTS_BLINK,
    IDT_TIMER_SHORTCUTS_BLINK,
    IDT_TIMER_DIRECTORY,
    IDT_TIMER_CONTACTS,
    IDT_TIMER_CALLS,
    IDT_TIMER_SAVE,
    IDT_TIMER_NETWORK_CHANGED,
    IDT_TIMER_SWITCH_DEVICES,
    IDT_TIMER_HEADSET,
    IDT_TIMER_VU_METER,
    IDT_TIMER_AUTOANSWER,
    IDT_TIMER_FORWARDING,
    IDT_TIMER_PROGRESS,
    IDT_TIMER_DTMF,
    IDT_TIMER_CUSTOM,
    UM_CLOSETAB,
    UM_DBLCLICKTAB,
    UM_QUERYTAB,
    UM_UPDATE_CHECKER_LOADED,
};

enum {
    IDS_STATUSBAR,
    IDS_STATUSBAR2,
};

enum {ONYX_MESSAGE_TYPE_LOCAL, ONYX_MESSAGE_TYPE_REMOTE, ONYX_MESSAGE_TYPE_SYSTEM};
enum {ONYX_CALL_OUT, ONYX_CALL_IN, ONYX_CALL_MISS, ONYX_CALL_ELSE};
enum { ONYX_SOUND_CUSTOM, ONYX_SOUND_CUSTOM_NOLOOP, ONYX_SOUND_MESSAGE_IN, ONYX_SOUND_MESSAGE_OUT, ONYX_SOUND_HANGUP, ONYX_SOUND_RINGTONE, ONYX_SOUND_RINGIN2, ONYX_SOUND_RINGING };
enum onyx_srtp_type { ONYX_SRTP_DISABLED, ONYX_SRTP };

#define ONYX_SHORTCUT_CALL QStringLiteral("call")
#define ONYX_SHORTCUT_VIDEOCALL QStringLiteral("video")
#define ONYX_SHORTCUT_MESSAGE QStringLiteral("message")
#define ONYX_SHORTCUT_DTMF QStringLiteral("dtmf")
#define ONYX_SHORTCUT_TRANSFER QStringLiteral("transfer")
#define ONYX_SHORTCUT_ATTENDED_TRANSFER QStringLiteral("attendedTransfer")
#define ONYX_SHORTCUT_CONFERENCE QStringLiteral("conference")
#define ONYX_SHORTCUT_RUNBATCH QStringLiteral("runBatch")
#define ONYX_SHORTCUT_CALL_URL QStringLiteral("callURL")
#define ONYX_SHORTCUT_POP_URL QStringLiteral("popURL")

enum {
    ONYX_CONTACT_ICON_BLANK,
    ONYX_CONTACT_ICON_UNKNOWN,
    ONYX_CONTACT_ICON_OFFLINE,
    ONYX_CONTACT_ICON_AWAY,
    ONYX_CONTACT_ICON_ONLINE,
    ONYX_CONTACT_ICON_ON_THE_PHONE,
    ONYX_CONTACT_ICON_BUSY,
    ONYX_CONTACT_ICON_DEFAULT,
    ONYX_CONTACT_ICON_UNKNOWN_STARRED,
    ONYX_CONTACT_ICON_OFFLINE_STARRED,
    ONYX_CONTACT_ICON_AWAY_STARRED,
    ONYX_CONTACT_ICON_ONLINE_STARRED,
    ONYX_CONTACT_ICON_ON_THE_PHONE_STARRED,
    ONYX_CONTACT_ICON_BUSY_STARRED,
    ONYX_CONTACT_ICON_DEFAULT_STARRED
};

struct Account {
    QString label;
    QString server;
    QString proxy;
    QString username;
    QString domain;
    int port;
    QString authID;
    QString password;
    QString digest;
    bool rememberPassword;
    QString displayName;
    QString dialingPrefix;
    QString dialPlan;
    bool hideCID;
    QString voicemailNumber;
    QString srtp;
    QString transport;
    QString publicAddr;
    int registerRefresh;
    int keepAlive;
    bool publish;
    bool ice;
    bool allowRewrite;
    bool disableSessionTimer;

    bool operator==(const Account& a) const {
        return label == a.label
            && server == a.server
            && proxy == a.proxy
            && username == a.username
            && domain == a.domain
            && port == a.port
            && authID == a.authID
            && password == a.password
            && digest == a.digest
            && displayName == a.displayName
            && dialingPrefix == a.dialingPrefix
            && dialPlan == a.dialPlan
            && hideCID == a.hideCID
            && voicemailNumber == a.voicemailNumber
            && srtp == a.srtp
            && transport == a.transport
            && publicAddr == a.publicAddr
            && registerRefresh == a.registerRefresh
            && keepAlive == a.keepAlive
            && publish == a.publish
            && ice == a.ice
            && allowRewrite == a.allowRewrite
            && disableSessionTimer == a.disableSessionTimer;
    }
    bool operator!=(const Account& a) const {
        return !(*this == a);
    }
    Account& operator=(const Account& a) {
        label = a.label;
        server = a.server;
        proxy = a.proxy;
        username = a.username;
        domain = a.domain;
        port = a.port;
        authID = a.authID;
        password = a.password;
        digest = a.digest;
        displayName = a.displayName;
        dialingPrefix = a.dialingPrefix;
        dialPlan = a.dialPlan;
        hideCID = a.hideCID;
        voicemailNumber = a.voicemailNumber;
        srtp = a.srtp;
        transport = a.transport;
        publicAddr = a.publicAddr;
        registerRefresh = a.registerRefresh;
        keepAlive = a.keepAlive;
        publish = a.publish;
        ice = a.ice;
        allowRewrite = a.allowRewrite;
        disableSessionTimer = a.disableSessionTimer;
        return *this;
    }
    Account() : port(0)
        , rememberPassword(false)
        , registerRefresh(0)
        , keepAlive(0)
        , publish(false)
        , ice(false)
        , allowRewrite(false)
        , disableSessionTimer(false)
    {}
};

struct player_eof_data
{
    pj_pool_t          *pool;
    pjsua_player_id player_id;
    void *callback;
};

struct Shortcut {
    QString label;
    QString number;
    QString number2;
    QString type;
    bool presence;
    bool ringing;
    int image;
    Shortcut() : presence(false)
        ,ringing(false)
        ,image(0)
    {}
};

struct Prensence {
    QString number;
    int image;
    bool ringing;
    QString info;
};

struct Contact {
    QString name;
    QString number;
    QString firstname;
    QString lastname;
    QString phone;
    QString mobile;
    QString email;
    QString address;
    QString city;
    QString state;
    QString zip;
    QString comment;
    QString id;
    bool presence;
    bool starred;
    bool directory;
    QString info;
    bool ringing;
    int image;
    bool candidate;
    Contact():presence(false)
        ,starred(false)
        ,directory(false)
        ,ringing(false)
        ,image(0)
        ,candidate(false)
    {}
};

struct ContactWithFields {
    Contact contact;
    QStringList fields;
    bool processed;
    ContactWithFields():processed(false)
    {}
};

struct MessagesContact {
    QString name;
    QString number;
    QString numberOriginal;
    QString commands;
    QString numberParameters;
    QString messages;
    QString message;
    bool hasNewMessages;
    bool fromCommandLine;
    QString lastSystemMessage;
    QDateTime lastSystemMessageTime;
    pjsua_call_id callId;
    QString callIdStr;
    int mediaStatus;
    MessagesContact():mediaStatus(PJSUA_CALL_MEDIA_ERROR)
        ,callId(-1)
        ,hasNewMessages(false)
        ,fromCommandLine(false)
    {}
};

struct Call {
    int key;
    QString id;
    QString name;
    QString number;
    int type;
    int time;
    int duration;
    QString info;
};

struct call_tonegen_data
{
   pj_pool_t          *pool;
   pjmedia_port       *tonegen;
   pjsua_conf_port_id  toneslot;
};

struct call_user_data
{
    QMutex CS;
    pjsua_call_id call_id;
    call_tonegen_data *tonegen_data;
    pjsua_recorder_id recorder_id;
    pj_timer_entry auto_hangup_timer;
    bool hangup;
    onyx_srtp_type srtp;
    int rx_pkt_prev;
    int rx_loss_prev;
    QString name;
    QString userAgent;
    QString diversion;
    QString callerID;
    QString commands;
    QString reason;
    bool inConference;
    bool autoAnswer;
    bool forwarding;
    bool hidden;
    int holdFrom;
    int duration;
    call_user_data(pjsua_call_id call_id): tonegen_data(NULL)
        ,recorder_id(PJSUA_INVALID_ID)
        ,hangup(false)
        ,inConference(false)
        ,autoAnswer(false)
        ,forwarding(false)
        ,hidden(false)
        ,holdFrom(-1)
        ,duration(-1)
        ,srtp(ONYX_SRTP_DISABLED)
        ,rx_pkt_prev(0)
        ,rx_loss_prev(0)
        {
            this->call_id = call_id;
            pj_bzero(&auto_hangup_timer, sizeof(auto_hangup_timer));
            auto_hangup_timer.id = PJSUA_INVALID_ID;
        }
};

extern pjsua_transport_id transport_udp_local;
extern pjsua_transport_id transport_udp;
extern pjsua_transport_id transport_tcp;
extern pjsua_transport_id transport_tls;

extern struct call_tonegen_data *tone_gen;
extern pjsua_acc_id account;
extern QString password;
extern pjsua_acc_id account_local;
extern pjsua_conf_port_id onyx_conf_port_id;
extern pjsua_call_id onyx_conf_port_call_id;

extern int onyx_audio_input;
extern int onyx_audio_output;
extern int onyx_audio_ring;

extern QString customString;

bool is_pjsua_running();
void set_pjsua_running(bool running);

QString FormatNumber(QString number, QString *commands = nullptr, bool noTransform = false);
void AddTransportSuffix(QString &str, Account *account);
QString GetSIPURI(QString str, bool isSimple = false, bool isLocal = false, QString domain = QString());
void ParseCallSIPURI(QString &number, call_user_data* user_data, SIPURI* out);
void ParseCallSIPURI(pjsua_call_info* call_info, call_user_data* user_data, SIPURI* out);
QString GetPAI(pjsip_rx_data* rdata);
bool SelectSIPAccount(QString number, pjsua_acc_id& acc_id, pj_str_t* pj_uri = nullptr);
QString get_account_username();
QString get_account_password();
QString get_account_domain();
QString get_account_server();
void get_account_proxy(Account *account, QStringList &proxies);
QString get_public_addr(Account *account = nullptr);

struct call_tonegen_data *call_init_tonegen(pjsua_call_id call_id);
BOOL call_play_digit(pjsua_call_id call_id, const char *digits, int duration = 160);
void call_deinit_tonegen(pjsua_call_id call_id);

void onyx_call_hangup_fast(pjsua_call_id call_id, pjsua_call_info *p_call_info = nullptr);

bool call_hangup_all_noincoming(bool onHold = false);
bool call_hangup_incoming();
void call_hangup_calling();
void call_hangup_all();

void OpenHelp(QString code);

struct URLGetAsyncData {
    QWidget* hWnd = nullptr;
    int message = 0;
    QString url;
    QString username;
    QString password;
    bool post = false;
    QString postData;
    int statusCode = 0;
    QString headers;
    QByteArray body;
    void* userData = nullptr;
};

void URLGetAsync(QString url, QWidget* hWnd = nullptr, int message = 0, bool post = false,
                 QString postData = QString(), QString headers = QString(),
                 QString username = QString(), QString password = QString(), void* userData = nullptr);
URLGetAsyncData URLGetSync(QString url, bool post = false, QString postData = QString(),
                           QString headers = QString(), QString username = QString(),
                           QString password = QString(), void* userData = nullptr);

QByteArray urldecode(const QByteArray& str);
QByteArray urlencode(const QByteArray& str);
QByteArray char2hex(char dec);

QString URLMask(QString url, SIPURI* sipuri = nullptr, pjsua_acc_id acc = -1,
                call_user_data *user_data = nullptr, pjsua_call_info* call_info = nullptr);
QIcon LoadImageIcon(int i, int w = 0, int h = 0);

void onyx_set_sound_device(int outDev, bool forse = false);
bool onyx_call_statistics(call_user_data *user_data, float *MOS);
void onyx_call_dial_dtmf(pjsua_call_id call_id, QString digits);
void onyx_call_send_dtmf_info(pjsua_call_id current_call, pj_str_t digits);
void onyx_call_end(pjsua_call_id call_id);
void onyx_conference_join(pjsua_call_info *call_info);
void onyx_conference_leave(pjsua_call_info *call_info, call_user_data *user_data = nullptr, bool hold = false);
void onyx_call_hold(pjsua_call_info *call_info);
void onyx_call_unhold(pjsua_call_info *call_info = nullptr);
bool onyx_call_answer(pjsua_call_id call_id = PJSUA_INVALID_ID);
void onyx_call_busy(pjsua_call_id call_id, QString reason = QString());
void onyx_call_recording_start(call_user_data *user_data, pjsua_call_info *call_info = nullptr, int id = 0);
void onyx_call_recording_stop(call_user_data *user_data, int id = 0, bool force = false);
QString onyx_url_mask(QString url);
void onyx_startup_set(bool enable);
