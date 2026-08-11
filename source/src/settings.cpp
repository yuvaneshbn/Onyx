#include "settings.h"
#include "Crypto.h"
#include "global.h"

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QDateTime>
#include <QDebug>
#include <algorithm>
#include <vector>

#ifdef Q_OS_WIN
#include <windows.h>
#include <Msi.h>
#pragma comment(lib, "Msi.lib")
#endif

AccountSettings accountSettings;
int dpiY = 96;
bool firstRun = false;
QDateTime startTime;
QVector<Shortcut> shortcuts;

// Helper: ANSI to wide conversion (used only for old INI conversion)
[[maybe_unused]] static QString AnsiToWideChar(const QByteArray &ansi) {
    return QString::fromLocal8Bit(ansi);
}

// Hex string <-> binary helpers (Onyx replacements)
static QByteArray String2Bin(const QString &str) {
    return QByteArray::fromHex(str.toLatin1());
}
static QString Bin2String(const QByteArray &bin) {
    return QString::fromLatin1(bin.toHex().toUpper());
}

// Encryption/decryption using existing Crypto class (kept unchanged)
static bool IniDecrypt(QString &str) {
    if (!str.isEmpty()) {
        bool res = false;
        QByteArray arPassword = String2Bin(str);
        MFC::CCrypto crypto;
        QString key = QString::fromLatin1(_GLOBAL_KEY);
        if (crypto.DeriveKey(key)) {
            try {
                QString decrypted;
                if (crypto.Decrypt(arPassword, decrypted)) {
                    str = decrypted;
                    res = true;
                }
            } catch (...) { // simplified catch; original used CArchiveException
            }
        }
        return res;
    } else {
        return true;
    }
}

static QString IniEncrypt(const QString &str) {
    QString res;
    MFC::CCrypto crypto;
    QByteArray arPassword;
    QString key = QString::fromLatin1(_GLOBAL_KEY);
    if (!str.isEmpty() && crypto.DeriveKey(key) && crypto.Encrypt(str, arPassword)) {
        res = Bin2String(arPassword);
    } else {
        res = str;
    }
    return res;
}

// Check if an INI section exists using QSettings
static bool IniSectionExists(const QString &section, const QString &iniFilePath) {
    QSettings settings(iniFilePath, QSettings::IniFormat);
    return settings.childGroups().contains(section, Qt::CaseInsensitive);
}

// File size helper
static qint64 FileSize(const QString &name) {
    QFileInfo fi(name);
    return fi.exists() ? fi.size() : -1;
}

AccountSettings::AccountSettings() {
    // Initialize members to safe defaults (not present in original but good practice)
    singleMode = true;
    volumeRing = 100;
    volumeOutput = 100;
    volumeInput = 100;
    // ... other defaults could be set here, but original relies on Init()
}

void AccountSettings::Init() {
    bool isPortable = false;
    firstRun = false;
    QString str;
    accountId = 0;
    startTime = QDateTime::currentDateTime();

    // Get executable path and directory
    exeFile = QCoreApplication::applicationFilePath();
    pathExe = QFileInfo(exeFile).absolutePath();

    QString fileName = QFileInfo(exeFile).completeBaseName();
    QString version = QString::fromLatin1(_GLOBAL_VERSION);
    logFile = QString("%1_log.txt").arg(fileName);
    iniFile = QString("%1.ini").arg(fileName);
    pathRoaming.clear();
    pathLocal.clear();

    // AppData paths using QStandardPaths
    appDataRoamingRoot = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QStandardPaths already appends the app name, but original built it manually.
    // We need to match the original structure: <AppDataRoamingRoot>\<AppName>
    // We'll keep the root as the base, then append the nice name.
    QDir().mkpath(appDataRoamingRoot); // ensure exists
    appDataRoaming = appDataRoamingRoot + "/" + QString::fromLatin1(_GLOBAL_NAME_NICE) + "/";

    appDataLocalRoot = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(appDataLocalRoot);
    appDataLocal = appDataLocalRoot + "/" + QString::fromLatin1(_GLOBAL_NAME_NICE) + "/";

    // Determine roaming/local paths (installer detection)
    if (pathRoaming.isEmpty()) {
        QString contactsFile = "Contacts.xml";
        QString pathInstaller;
#ifdef Q_OS_WIN
        // Registry: HKEY_CURRENT_USER\Software\<AppName>
        QSettings regCurrentUser("HKEY_CURRENT_USER\\Software\\" + QString::fromLatin1(_GLOBAL_NAME_NICE),
                                 QSettings::NativeFormat);
        pathInstaller = regCurrentUser.value("Default").toString(); // original used NULL value name
        if (pathInstaller.isEmpty()) {
            QSettings regLocalMachine("HKEY_LOCAL_MACHINE\\Software\\" + QString::fromLatin1(_GLOBAL_NAME_NICE),
                                      QSettings::NativeFormat);
            pathInstaller = regLocalMachine.value("Default").toString();
        }
        // MSI detection
        if (pathInstaller.isEmpty()) {
            DWORD i = 0;
            wchar_t lpProductBuf[39];
            while (MsiEnumProducts(i, lpProductBuf) == ERROR_SUCCESS) {
                wchar_t valueBuf[256];
                DWORD cchValueBuf = 256;
                if (MsiGetProductInfo(lpProductBuf, INSTALLPROPERTY_INSTALLEDPRODUCTNAME, valueBuf, &cchValueBuf) == ERROR_SUCCESS) {
                    if (QString::compare(QString::fromWCharArray(valueBuf),
                                         QString::fromLatin1(_GLOBAL_NAME_NICE),
                                         Qt::CaseInsensitive) == 0) {
                        cchValueBuf = 256;
                        if (MsiGetProductInfo(lpProductBuf, INSTALLPROPERTY_INSTALLLOCATION, valueBuf, &cchValueBuf) == ERROR_SUCCESS) {
                            pathInstaller = QString::fromWCharArray(valueBuf);
                        }
                        if (!pathInstaller.isEmpty()) {
                            if (pathInstaller.endsWith('\\'))
                                pathInstaller.chop(1);
                        } else {
                            pathInstaller = pathExe;
                        }
                        break;
                    }
                }
                i++;
            }
        }
#else
        // Non-Windows: always portable
        pathInstaller.clear();
#endif

        // Compare installer path with exe path (case insensitive)
        bool installerMatch = !pathInstaller.isEmpty() &&
                              (QString::compare(pathInstaller, pathExe, Qt::CaseInsensitive) == 0);

        if (!pathInstaller.isEmpty() && installerMatch) {
            // Installed mode
            QDir().mkpath(appDataRoaming);
            pathRoaming = appDataRoaming;
            QDir().mkpath(appDataLocal);
            pathLocal = appDataLocal;
            logFile = pathLocal + logFile;
            // Move legacy files if needed
            if (!QFile::exists(pathRoaming + iniFile) && QFile::exists(pathLocal + iniFile)) {
                QFile::rename(pathLocal + iniFile, pathRoaming + iniFile);
            }
            if (!QFile::exists(pathRoaming + contactsFile) && QFile::exists(pathLocal + contactsFile)) {
                QFile::rename(pathLocal + contactsFile, pathRoaming + contactsFile);
            }
        } else {
            // Portable mode
            isPortable = true;
            pathRoaming = pathExe + "/";
            pathLocal = pathRoaming;
            logFile = pathLocal + logFile;
        }
        iniFile = pathRoaming + iniFile;

        // Legacy INI name migration
        if (!QFile::exists(iniFile)) {
            QString name = QString::fromLatin1(_GLOBAL_NAME_NICE);
            if (name.contains(' ')) {
                QString iniFileNice = pathRoaming + name + ".ini";
                if (QFile::exists(iniFileNice)) {
                    QFile::rename(iniFileNice, iniFile);
                }
            }
        }

        // Handle /reset command line
        if (QCoreApplication::arguments().contains("/reset")) {
            return;
        }

        // First run: create INI file if missing
        if (!QFile::exists(iniFile) || FileSize(iniFile) == 0) {
            firstRun = true;
            // QSettings will create the file automatically when we write. We just need to mark first run.
        } else {
            // Original code converted non-BOM files to UTF-16LE. QSettings handles encoding,
            // so we skip manual conversion. We assume the file is readable by QSettings.
        }

        // First-run actions
        if (firstRun) {
            if (!isPortable) {
                bool runAtSystemStartup = false;
#ifdef Q_OS_WIN
                QSettings regCur("HKEY_CURRENT_USER\\Software\\" + QString::fromLatin1(_GLOBAL_NAME_NICE),
                                 QSettings::NativeFormat);
                bool found = regCur.contains("RunAtSystemStartup");
                if (found) {
                    runAtSystemStartup = regCur.value("RunAtSystemStartup").toBool();
                } else {
                    QSettings regLoc("HKEY_LOCAL_MACHINE\\Software\\" + QString::fromLatin1(_GLOBAL_NAME_NICE),
                                     QSettings::NativeFormat);
                    if (regLoc.contains("RunAtSystemStartup"))
                        runAtSystemStartup = regLoc.value("RunAtSystemStartup").toBool();
                }
                // onyx_startup_set(runAtSystemStartup); // platform-specific; keep as comment
#endif
            }
        }
    }

    // Load user settings from INI
    QSettings settings(iniFile, QSettings::IniFormat);
    settings.beginGroup("Settings");

    singleMode = settings.value("singleMode", 1).toInt() != 0;
    ringtone = settings.value("ringingSound").toString();
    volumeRing = settings.value("volumeRing", 100).toInt();
    audioRingDevice = settings.value("audioRingDevice").toString();
    audioOutputDevice = settings.value("audioOutputDevice").toString();
    audioInputDevice = settings.value("audioInputDevice").toString();

    micAmplification = settings.value("micAmplification", 0).toInt() != 0;
    swLevelAdjustment = settings.value("swLevelAdjustment", 0).toInt() != 0;
    audioCodecs = settings.value("audioCodecs", QString::fromLatin1(_GLOBAL_CODECS_ENABLED)).toString();
    vad = settings.value("VAD", 0).toInt() != 0;
    ec = settings.value("EC", 1).toInt() != 0;
    forceCodec = settings.value("forceCodec", 0).toInt() != 0;
    opusStereo = settings.value("opusStereo", 0).toInt() != 0;
    disableMessaging = settings.value("disableMessaging", 0).toInt() != 0;

#ifdef _GLOBAL_VIDEO
    disableVideo = settings.value("disableVideo", 0).toInt() != 0;
    videoCaptureDevice = settings.value("videoCaptureDevice").toString();
    videoCodec = settings.value("videoCodec").toString();
    videoH264 = settings.value("videoH264", 1).toInt() != 0;
    videoH263 = settings.value("videoH263", 1).toInt() != 0;
    videoVP8 = settings.value("videoVP8", 1).toInt() != 0;
    videoVP9 = settings.value("videoVP9", 1).toInt() != 0;
    videoBitrate = settings.value("videoBitrate", 0).toInt();
#endif

    rport = settings.value("rport", 0).toInt() != 0;
    sourcePort = settings.value("sourcePort", 0).toInt();
    rtpPortMin = settings.value("rtpPortMin", 0).toInt();
    rtpPortMax = settings.value("rtpPortMax", 0).toInt();
    dnsSrvNs = settings.value("dnsSrvNs").toString();
    dnsSrv = settings.value("dnsSrv", 0).toInt() != 0;
    stun = settings.value("STUN").toString();
    enableSTUN = settings.value("enableSTUN", 0).toInt() != 0;

    // Recording
    QString defaultRecordingPath;
    if (isPortable) {
        defaultRecordingPath = "Recordings";
    } else {
        defaultRecordingPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/Recordings";
    }
    recordingPath = settings.value("recordingPath", defaultRecordingPath).toString();
    recordingFormat = settings.value("recordingFormat").toString();
    autoRecording = settings.value("autoRecording", 0).toInt() != 0;
    recordingButton = settings.value("recordingButton", 1).toInt() != 0;

    buttonAC = settings.value("buttonAC", 1).toInt() != 0;
    buttonCONF = settings.value("buttonCONF", 1).toInt() != 0;
    DTMFMethod = settings.value("DTMFMethod", 0).toInt();

    autoAnswer = settings.value("autoAnswer", QString::fromLatin1(_GLOBAL_SETT_AA_DEFAULT)).toString();
    autoAnswerDelay = settings.value("autoAnswerDelay", 0).toInt();
    autoAnswerNumber = settings.value("autoAnswerNumber").toString();
    autoAnswerCalls = settings.value("autoAnswerCalls").toString();
    forwarding = settings.value("forwarding").toString();
    forwardingNumber = settings.value("forwardingNumber").toString();
    forwardingDelay = settings.value("forwardingDelay", 0).toInt();

    featureCodeCP = settings.value("featureCodeCP", "**").toString();
    featureCodeBT = settings.value("featureCodeBT", "##").toString();
    featureCodeAT = settings.value("featureCodeAT", "*2").toString();
    enableFeatureCodeCP = settings.value("enableFeatureCodeCP", 1).toInt() != 0;
    enableFeatureCodeBT = settings.value("enableFeatureCodeBT", 0).toInt() != 0;
    enableFeatureCodeAT = settings.value("enableFeatureCodeAT", 0).toInt() != 0;

    denyIncoming = settings.value("denyIncoming", QString::fromLatin1(_GLOBAL_SETT_DENYINC_DEFAULT)).toString();

    usersDirectory = settings.value("usersDirectory").toString();
    defaultAction = settings.value("defaultAction").toString();
    enableMediaButtons = settings.value("enableMediaButtons", 0).toInt() != 0;
    headsetSupport = settings.value("headsetSupport", 0).toInt() != 0;
    localDTMF = settings.value("localDTMF", 1).toInt() != 0;
    enableLog = settings.value("enableLog", 0).toInt() != 0;
    bringToFrontOnIncoming = settings.value("bringToFrontOnIncoming", 1).toInt() != 0;
    enableLocalAccount = settings.value("enableLocalAccount", 0).toInt() != 0;
    randomAnswerBox = settings.value("randomAnswerBox", 0).toInt() != 0;
    disableNameLookup = settings.value("disableNameLookup", 0).toInt() != 0;
    crashReport = 0; // kept as 0
    callWaiting = settings.value("callWaiting", 1).toInt() != 0;
    multiMonitor = settings.value("multiMonitor", 0).toInt() != 0;
    networkChanges = settings.value("networkChanges", 1).toInt() != 0;
    updatesInterval = settings.value("updatesInterval").toString();
    checkUpdatesTime = settings.value("checkUpdatesTime", 0).toInt();

    // INI / system settings
    noResize = settings.value("noResize", 0).toInt() != 0;
    userAgent = settings.value("userAgent").toString();
    autoHangUpTime = settings.value("autoHangUpTime", 0).toInt();
    maxConcurrentCalls = settings.value("maxConcurrentCalls", 0).toInt();
    noIgnoreCall = settings.value("noIgnoreCall", 0).toInt() != 0;
    cmdOutgoingCall = settings.value("cmdOutgoingCall").toString();
    cmdIncomingCall = settings.value("cmdIncomingCall").toString();
    cmdCallRing = settings.value("cmdCallRing").toString();
    cmdCallAnswer = settings.value("cmdCallAnswer").toString();
    cmdCallAnswerVideo = settings.value("cmdCallAnswerVideo").toString();
    cmdCallBusy = settings.value("cmdCallBusy").toString();
    cmdCallStart = settings.value("cmdCallStart").toString();
    cmdCallEnd = settings.value("cmdCallEnd").toString();
    minimized = settings.value("minimized", 0).toInt() != 0;
    portKnockerHost = settings.value("portKnockerHost").toString();
    portKnockerPorts = settings.value("portKnockerPorts").toString();

    mainX = settings.value("mainX", 0).toInt();
    mainY = settings.value("mainY", 0).toInt();
    mainW = settings.value("mainW", 0).toInt();
    mainH = settings.value("mainH", 0).toInt();
    messagesX = settings.value("messagesX", 0).toInt();
    messagesY = settings.value("messagesY", 0).toInt();
    messagesW = settings.value("messagesW", 0).toInt();
    messagesH = settings.value("messagesH", 0).toInt();
    ringinX = settings.value("ringinX", 0).toInt();
    ringinY = settings.value("ringinY", 0).toInt();

    callsWidth0 = settings.value("callsWidth0", 0).toInt();
    callsWidth1 = settings.value("callsWidth1", 0).toInt();
    callsWidth2 = settings.value("callsWidth2", 0).toInt();
    callsWidth3 = settings.value("callsWidth3", 0).toInt();
    callsWidth4 = settings.value("callsWidth4", 0).toInt();
    callsWidth5 = settings.value("callsWidth5", 0).toInt();
    contactsWidth0 = settings.value("contactsWidth0", 0).toInt();
    contactsWidth1 = settings.value("contactsWidth1", 0).toInt();
    contactsWidth2 = settings.value("contactsWidth2", 0).toInt();

    volumeOutput = settings.value("volumeOutput", 100).toInt();
    volumeInput = settings.value("volumeInput", 100).toInt();
    activeTab = settings.value("activeTab", 0).toInt();
    FWD = settings.value("FWD", 0).toInt() != 0;
    AA = settings.value("AA", 0).toInt() != 0;
    AC = settings.value("AC", 0).toInt() != 0;
    DND = settings.value("DND", 0).toInt() != 0;
    alwaysOnTop = settings.value("alwaysOnTop", 0).toInt() != 0;
    enableShortcuts = settings.value("enableShortcuts", 0).toInt() != 0;
    shortcutsBottom = settings.value("shortcutsBottom", 0).toInt() != 0;
    lastCallNumber = settings.value("lastCallNumber").toString();
    lastCallHasVideo = settings.value("lastCallHasVideo", 0).toInt() != 0;

    // Version check
    QString currentVersion = settings.value("version").toString();
    if (currentVersion != version) {
        settings.setValue("version", version);
    }

    settings.endGroup();

    // Account loading
    int savedAccountId = settings.value("Settings/accountId", -1).toInt();
    if (savedAccountId == -1) {
        // Not set yet, try old account format
        if (AccountLoad(-2, &account)) {
            accountId = 1;
            QSettings s(iniFile, QSettings::IniFormat);
            s.setValue("Settings/accountId", 1);
        }
    } else {
        accountId = savedAccountId;
        if (!accountId && !enableLocalAccount) {
            accountId = 1;
        }
        if (accountId > 0) {
            if (!AccountLoad(accountId, &account)) {
                accountId = 0;
            }
        }
    }
    AccountLoad(0, &accountLocal);
}

void AccountSettings::AccountDelete(int id) {
    QString section = QString("Account%1").arg(id);
    QSettings settings(iniFile, QSettings::IniFormat);
    settings.remove(section); // Removes the whole group
}

bool AccountSettings::AccountLoad(int id, Account *account) {
    QString section;
    if (id == -2) {
        section = "Settings";
    } else {
        section = QString("Account%1").arg(id);
    }

    bool sectionExists = IniSectionExists(section, iniFile);

    QSettings settings(iniFile, QSettings::IniFormat);
    settings.beginGroup(section);

    account->label = settings.value("label", (id ? QString() : "Local (call by IP address)")).toString();

    account->server = settings.value("server").toString();
    account->proxy = settings.value("proxy").toString();
    account->domain = settings.value("domain").toString();
    account->username = settings.value("username").toString();
    account->password = settings.value("password").toString();

    if (!account->password.isEmpty() && !IniDecrypt(account->password)) {
        // Re-encrypt with new key? Actually original re-encrypts using IniEncrypt.
        // We'll write back the encrypted password.
        settings.setValue("password", IniEncrypt(account->password));
    }

    account->rememberPassword = (account->username.length() > 0) ? 1 : 0;

    account->authID = settings.value("authID").toString();
    account->displayName = settings.value("displayName").toString();
    account->dialingPrefix = settings.value("dialingPrefix").toString();
    account->dialPlan = settings.value("dialPlan").toString();
    account->hideCID = settings.value("hideCID", 0).toInt() != 0;
    account->voicemailNumber = settings.value("voicemailNumber").toString();
    account->srtp = settings.value("SRTP").toString();
    account->transport = settings.value("transport", "udp").toString();
    account->publicAddr = settings.value("publicAddr").toString();
    account->registerRefresh = settings.value("registerRefresh", 300).toInt();
    if (account->registerRefresh <= 0) account->registerRefresh = 300;
    else if (account->registerRefresh <= 10) account->registerRefresh = 10;
    account->keepAlive = settings.value("keepAlive", 15).toInt();
    if (account->keepAlive <= 0) account->keepAlive = 15;
    else if (account->keepAlive == 1) account->keepAlive = 2;
    account->publish = settings.value("publish", 0).toInt() != 0;
    account->allowRewrite = settings.value("allowRewrite", 0).toInt() != 0;
    account->ice = settings.value("ICE", 0).toInt() != 0;
    account->disableSessionTimer = settings.value("disableSessionTimer", 0).toInt() != 0;

    settings.endGroup();

    if (id == -2) {
        // Delete old keys and save to Account1 if section existed and domain is not empty
        QSettings s(iniFile, QSettings::IniFormat);
        s.beginGroup("Settings");
        s.remove("server");
        s.remove("proxy");
        s.remove("SRTP");
        s.remove("transport");
        s.remove("publicAddr");
        s.remove("publish");
        s.remove("STUN");
        s.remove("ICE");
        s.remove("allowRewrite");
        s.remove("domain");
        s.remove("authID");
        s.remove("username");
        s.remove("passwordSize");
        s.remove("password");
        s.remove("id");
        s.remove("displayName");
        s.endGroup();

        if (sectionExists && !account->domain.isEmpty()) {
            AccountSave(1, account);
        }
    }

    if (id == 0)
        return true; // Local account always ok

    return sectionExists && !account->domain.isEmpty();
}

void AccountSettings::AccountSave(int id, Account *account) {
    QString section = QString("Account%1").arg(id);
    QSettings settings(iniFile, QSettings::IniFormat);
    settings.beginGroup(section);

    settings.setValue("label", account->label);
    settings.setValue("server", account->server);
    settings.setValue("proxy", account->proxy);
    settings.setValue("domain", account->domain);
    if (!account->rememberPassword) {
        settings.setValue("username", "");
        settings.setValue("password", "");
    } else {
        settings.setValue("username", account->username);
        settings.setValue("password", IniEncrypt(account->password));
    }
    settings.setValue("authID", account->authID);
    settings.setValue("displayName", account->displayName);
    settings.setValue("dialingPrefix", account->dialingPrefix);
    settings.setValue("dialPlan", account->dialPlan);
    settings.setValue("hideCID", account->hideCID ? "1" : "0");
    settings.setValue("voicemailNumber", account->voicemailNumber);
    settings.setValue("transport", account->transport);
    settings.setValue("publicAddr", account->publicAddr);
    settings.setValue("SRTP", account->srtp);
    settings.setValue("registerRefresh", QString::number(account->registerRefresh));
    settings.setValue("keepAlive", QString::number(account->keepAlive));
    settings.setValue("publish", account->publish ? "1" : "0");
    settings.setValue("ICE", account->ice ? "1" : "0");
    settings.setValue("allowRewrite", account->allowRewrite ? "1" : "0");
    settings.setValue("disableSessionTimer", account->disableSessionTimer ? "1" : "0");

    settings.endGroup();
}

void AccountSettings::SettingsSave() {
    QSettings settings(iniFile, QSettings::IniFormat);
    settings.beginGroup("Settings");

    settings.setValue("accountId", accountId);
    settings.setValue("singleMode", singleMode ? "1" : "0");
    settings.setValue("ringingSound", ringtone);
    settings.setValue("volumeRing", volumeRing);
    settings.setValue("audioRingDevice", audioRingDevice);
    settings.setValue("audioOutputDevice", audioOutputDevice);
    settings.setValue("audioInputDevice", audioInputDevice);
    settings.setValue("micAmplification", micAmplification ? "1" : "0");
    settings.setValue("swLevelAdjustment", swLevelAdjustment ? "1" : "0");
    settings.setValue("audioCodecs", audioCodecs);
    settings.setValue("VAD", vad ? "1" : "0");
    settings.setValue("EC", ec ? "1" : "0");
    settings.setValue("forceCodec", forceCodec ? "1" : "0");
    settings.setValue("opusStereo", opusStereo ? "1" : "0");
    settings.setValue("disableMessaging", disableMessaging ? "1" : "0");
#ifdef _GLOBAL_VIDEO
    settings.setValue("disableVideo", disableVideo ? "1" : "0");
    settings.setValue("videoCaptureDevice", videoCaptureDevice);
    settings.setValue("videoCodec", videoCodec);
    settings.setValue("videoH264", videoH264 ? "1" : "0");
    settings.setValue("videoH263", videoH263 ? "1" : "0");
    settings.setValue("videoVP8", videoVP8 ? "1" : "0");
    settings.setValue("videoVP9", videoVP9 ? "1" : "0");
    settings.setValue("videoBitrate", videoBitrate);
#endif
    settings.setValue("rport", rport ? "1" : "0");
    settings.setValue("sourcePort", sourcePort);
    settings.setValue("rtpPortMin", rtpPortMin);
    settings.setValue("rtpPortMax", rtpPortMax);
    settings.setValue("dnsSrvNs", dnsSrvNs);
    settings.setValue("dnsSrv", dnsSrv ? "1" : "0");
    settings.setValue("STUN", stun);
    settings.setValue("enableSTUN", enableSTUN ? "1" : "0");
    settings.setValue("recordingPath", recordingPath);
    settings.setValue("recordingFormat", recordingFormat);
    settings.setValue("autoRecording", autoRecording ? "1" : "0");
    settings.setValue("recordingButton", recordingButton ? "1" : "0");
    settings.setValue("buttonAC", buttonAC ? "1" : "0");
    settings.setValue("buttonCONF", buttonCONF ? "1" : "0");
    settings.setValue("DTMFMethod", DTMFMethod); // original wrote string
    settings.setValue("autoAnswer", autoAnswer);
    settings.setValue("autoAnswerDelay", autoAnswerDelay);
    settings.setValue("autoAnswerNumber", autoAnswerNumber);
    settings.setValue("autoAnswerCalls", autoAnswerCalls);
    settings.setValue("forwarding", forwarding);
    settings.setValue("forwardingNumber", forwardingNumber);
    settings.setValue("forwardingDelay", forwardingDelay);
    settings.setValue("featureCodeCP", featureCodeCP);
    settings.setValue("featureCodeBT", featureCodeBT);
    settings.setValue("featureCodeAT", featureCodeAT);
    settings.setValue("enableFeatureCodeCP", enableFeatureCodeCP ? "1" : "0");
    settings.setValue("enableFeatureCodeBT", enableFeatureCodeBT ? "1" : "0");
    settings.setValue("enableFeatureCodeAT", enableFeatureCodeAT ? "1" : "0");
    settings.setValue("denyIncoming", denyIncoming);
    settings.setValue("usersDirectory", usersDirectory);
    settings.setValue("defaultAction", defaultAction);
    settings.setValue("enableMediaButtons", enableMediaButtons ? "1" : "0");
    settings.setValue("headsetSupport", headsetSupport ? "1" : "0");
    settings.setValue("localDTMF", localDTMF ? "1" : "0");
    settings.setValue("enableLog", enableLog ? "1" : "0");
    settings.setValue("bringToFrontOnIncoming", bringToFrontOnIncoming ? "1" : "0");
    settings.setValue("enableLocalAccount", enableLocalAccount ? "1" : "0");
    settings.setValue("randomAnswerBox", randomAnswerBox ? "1" : "0");
    settings.setValue("disableNameLookup", disableNameLookup ? "1" : "0");
    settings.setValue("callWaiting", callWaiting ? "1" : "0");
    settings.setValue("multiMonitor", multiMonitor ? "1" : "0");
    settings.setValue("networkChanges", networkChanges ? "1" : "0");
    settings.setValue("updatesInterval", updatesInterval);
    settings.setValue("checkUpdatesTime", checkUpdatesTime);
    settings.setValue("noResize", noResize ? "1" : "0");
    settings.setValue("userAgent", userAgent);
    settings.setValue("autoHangUpTime", autoHangUpTime);
    settings.setValue("maxConcurrentCalls", maxConcurrentCalls);
    settings.setValue("noIgnoreCall", noIgnoreCall ? "1" : "0");
    settings.setValue("cmdOutgoingCall", cmdOutgoingCall);
    settings.setValue("cmdIncomingCall", cmdIncomingCall);
    settings.setValue("cmdCallRing", cmdCallRing);
    settings.setValue("cmdCallAnswer", cmdCallAnswer);
    settings.setValue("cmdCallAnswerVideo", cmdCallAnswerVideo);
    settings.setValue("cmdCallBusy", cmdCallBusy);
    settings.setValue("cmdCallStart", cmdCallStart);
    settings.setValue("cmdCallEnd", cmdCallEnd);
    settings.setValue("minimized", minimized ? "1" : "0");
    settings.setValue("portKnockerHost", portKnockerHost);
    settings.setValue("portKnockerPorts", portKnockerPorts);

    settings.setValue("mainX", mainX);
    settings.setValue("mainY", mainY);
    settings.setValue("mainW", mainW);
    settings.setValue("mainH", mainH);
    settings.setValue("messagesX", messagesX);
    settings.setValue("messagesY", messagesY);
    settings.setValue("messagesW", messagesW);
    settings.setValue("messagesH", messagesH);
    settings.setValue("ringinX", ringinX);
    settings.setValue("ringinY", ringinY);

    settings.setValue("callsWidth0", callsWidth0);
    settings.setValue("callsWidth1", callsWidth1);
    settings.setValue("callsWidth2", callsWidth2);
    settings.setValue("callsWidth3", callsWidth3);
    settings.setValue("callsWidth4", callsWidth4);
    settings.setValue("callsWidth5", callsWidth5);
    settings.setValue("contactsWidth0", contactsWidth0);
    settings.setValue("contactsWidth1", contactsWidth1);
    settings.setValue("contactsWidth2", contactsWidth2);

    settings.setValue("volumeOutput", volumeOutput);
    settings.setValue("volumeInput", volumeInput);
    settings.setValue("activeTab", activeTab);
    settings.setValue("FWD", FWD ? "1" : "0");
    settings.setValue("AA", AA ? "1" : "0");
    settings.setValue("AC", AC ? "1" : "0");
    settings.setValue("DND", DND ? "1" : "0");
    settings.setValue("alwaysOnTop", alwaysOnTop ? "1" : "0");
    settings.setValue("enableShortcuts", enableShortcuts ? "1" : "0");
    settings.setValue("shortcutsBottom", shortcutsBottom ? "1" : "0");
    settings.setValue("lastCallNumber", lastCallNumber);
    settings.setValue("lastCallHasVideo", lastCallHasVideo ? "1" : "0");

    settings.endGroup();
}

// Shortcut encode/decode
QString ShortcutEncode(Shortcut *pShortcut) {
    return QString("%1;%2;%3;%4;%5")
        .arg(pShortcut->label,
             pShortcut->number,
             pShortcut->type,
             pShortcut->number2,
             pShortcut->presence ? "1" : "0");
}

void ShortcutDecode(const QString &str, Shortcut *pShortcut) {
    pShortcut->label.clear();
    pShortcut->number.clear();
    pShortcut->type = "ONYX_SHORTCUT_DTMF"; // keep as string originally? In original type was CString, but later converted to integer codes. We'll keep as string for compatibility.
    pShortcut->number2.clear();
    pShortcut->presence = false;

    QStringList parts = str.split(';');
    if (parts.size() > 0) pShortcut->label = parts[0];
    if (parts.size() > 1) pShortcut->number = parts[1];
    if (parts.size() > 2) {
        QString typeStr = parts[2];
        if (typeStr == "0") pShortcut->type = "ONYX_SHORTCUT_CALL";
        else if (typeStr == "1") pShortcut->type = "ONYX_SHORTCUT_VIDEOCALL";
        else if (typeStr == "2") pShortcut->type = "ONYX_SHORTCUT_MESSAGE";
        else if (typeStr == "3") pShortcut->type = "ONYX_SHORTCUT_DTMF";
        else if (typeStr == "4") pShortcut->type = "ONYX_SHORTCUT_TRANSFER";
        else pShortcut->type = typeStr; // fallback
    }
    if (parts.size() > 3) pShortcut->number2 = parts[3];
    if (parts.size() > 4) pShortcut->presence = (parts[4] == "1");
}

void ShortcutsLoad() {
    shortcuts.clear();
    QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
    settings.beginGroup("Shortcuts");
    QStringList keys = settings.childKeys();
    // Original iterated i from 0 to max using GetPrivateProfileString; here we load all keys sorted
    keys.sort(); // assume numeric keys
    for (const QString &key : keys) {
        QString val = settings.value(key).toString();
        Shortcut shortcut;
        ShortcutDecode(val, &shortcut);
        shortcuts.append(shortcut);
    }
    settings.endGroup();
}

void ShortcutsSave() {
    QSettings settings(accountSettings.iniFile, QSettings::IniFormat);
    settings.remove("Shortcuts"); // clear existing
    settings.beginGroup("Shortcuts");
    for (int i = 0; i < shortcuts.size(); ++i) {
        settings.setValue(QString::number(i), ShortcutEncode(&shortcuts[i]));
    }
    settings.endGroup();
}
