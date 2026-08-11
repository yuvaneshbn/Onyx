#ifndef ONYX_UTILS_H
#define ONYX_UTILS_H

#include <QString>
#include <QByteArray>
#include <QRect>
#include <QDateTime>
#include <QList>

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

struct SIPURI {
    QString name;
    QString user;
    QString domain;
    QString parameters;
    QString commands;
};

// Global helper functions (as in original)
QByteArray onyx_md5sum(const QByteArray& str);
QByteArray onyx_md5sum(const QString& str);
void onyx_audio_conf_set_volume(int val, bool mute);
void onyx_audio_input_set_volume(int val, bool mute = false);
pj_status_t onyx_verify_sip_url(const QString& url);
int onyx_get_duration(pj_time_val* time_val);

namespace Onyx
{
    void GetScreenRect(QRect* rect);
    QString GetErrorMessage(pj_status_t status);
    bool ShowErrorMessage(pj_status_t status);
    bool IsIP(const QString& host);
    QString RemovePort(const QString& domain);
    void ParseSIPURI(const QString& in, SIPURI* out);
    QString BuildSIPURI(const SIPURI* in);
    QString PjToStr(const pj_str_t* str, bool utf = false);
    QString Utf8DecodeUni(const char* str);
    QByteArray Utf8EncodeUni(const QString& str);
    QString AnsiToWideChar(const char* str);
    pj_str_t StrToPjStr(const QString& str);
    char* WideCharToPjStr(const QString& str);
    void OpenURL(const QString& url);
    void OpenFile(const QString& filename);
    QString GetDuration(int sec, bool zero = false);
    bool IsPSTNNnmber(const QString& number);
    bool IniSectionExists(const QString& section, const QString& iniFile);
    QString Bin2String(const QByteArray& ba);
    void String2Bin(const QString& str, QByteArray* res);
    void CommandLineToShell(const QString& cmd, QString& command, QString& params);
    void RunCmd(const QString& cmdLine, const QString& addParams = QString(), bool noWait = false);
    void PortKnock();
    bool IsConnectedToNetwork(bool checkInternet = false);
    QString FormatDateTime(const QDateTime& time, const QDateTime* timeNow = nullptr);
    void ExpandEnvironmentStrings(QString& str);
    QString GetSID();
}

#endif // ONYX_UTILS_H
