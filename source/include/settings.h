#pragma once

#include "global.h"
#include <QString>
#include <QDateTime>
#include <QVector>

struct AccountSettings {

    int accountId;
    Account account;
    Account accountLocal;
    bool singleMode;
    QString ringtone;
    QString audioRingDevice;
    QString audioOutputDevice;
    QString audioInputDevice;
    bool micAmplification;
    bool swLevelAdjustment;
    QString audioCodecs;
    bool vad;
    bool ec;
    bool forceCodec;
    bool opusStereo;
    QString recordingPath;
    QString recordingFormat;
    bool autoRecording;
    bool recordingButton;
    bool buttonAC;
    bool buttonCONF;

    bool disableMessaging;
#ifdef _GLOBAL_VIDEO
    bool disableVideo;
    QString videoCaptureDevice;
    QString videoCodec;
    bool videoH264;
    bool videoH263;
    bool videoVP8;
    bool videoVP9;
    int videoBitrate;
#endif
    bool rport;
    int sourcePort;
    int rtpPortMin;
    int rtpPortMax;
    bool dnsSrv;
    QString dnsSrvNs;
    QString stun;
    bool enableSTUN;
    int DTMFMethod;
    bool AA;
    bool AC;
    bool DND;
    QString autoAnswer;
    int autoAnswerDelay;
    QString autoAnswerNumber;
    QString autoAnswerCalls;
    QString forwarding;
    QString forwardingNumber;
    int forwardingDelay;
    bool FWD;
    QString featureCodeCP;
    QString featureCodeBT;
    QString featureCodeAT;
    bool enableFeatureCodeCP;
    bool enableFeatureCodeBT;
    bool enableFeatureCodeAT;

    QString denyIncoming;
    QString usersDirectory;
    QString defaultAction;
    bool enableMediaButtons;
    bool headsetSupport;
    bool localDTMF;
    bool enableLocalAccount;
    bool crashReport;
    bool enableLog;
    bool bringToFrontOnIncoming;
    bool randomAnswerBox;
    bool disableNameLookup;
    QString userAgent;
    QString portKnockerHost;
    QString portKnockerPorts;

    QString lastCallNumber;
    bool lastCallHasVideo;

    QString updatesInterval;

    int activeTab;
    bool alwaysOnTop;
    bool multiMonitor;
    bool networkChanges;

    int mainX;
    int mainY;
    int mainW;
    int mainH;
    bool noResize;

    int messagesX;
    int messagesY;
    int messagesW;
    int messagesH;

    int ringinX;
    int ringinY;

    int callsWidth0;
    int callsWidth1;
    int callsWidth2;
    int callsWidth3;
    int callsWidth4;
    int callsWidth5;

    int contactsWidth0;
    int contactsWidth1;
    int contactsWidth2;

    int volumeRing;
    int volumeOutput;
    int volumeInput;

    QString iniFile;
    QString callsFile;
    QString logFile;
    QString exeFile;
    QString pathRoaming;
    QString pathLocal;
    QString pathExe;
    QString appDataRoaming;
    QString appDataRoamingRoot;
    QString appDataLocal;
    QString appDataLocalRoot;

    int checkUpdatesTime;

    bool minimized;

    int autoHangUpTime;
    int maxConcurrentCalls;
    bool callWaiting;
    bool noIgnoreCall;

    QString cmdOutgoingCall;
    QString cmdIncomingCall;
    QString cmdCallRing;
    QString cmdCallAnswer;
    QString cmdCallAnswerVideo;
    QString cmdCallBusy;
    QString cmdCallStart;
    QString cmdCallEnd;

    bool enableShortcuts;
    bool shortcutsBottom;

    AccountSettings();
    void Init();
    bool AccountLoad(int id, Account *account);
    void AccountSave(int id, Account *account);
    void AccountDelete(int id);
    void SettingsSave();
};

extern AccountSettings accountSettings;
extern int dpiY;
extern bool firstRun;
extern QDateTime startTime;

QString ShortcutEncode(Shortcut *pShortcut);
void ShortcutDecode(const QString &str, Shortcut *pShortcut);
void ShortcutsLoad();
void ShortcutsSave();
extern QVector<Shortcut> shortcuts;