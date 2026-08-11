#include "OnyxUtils.h"
#include "global.h"

#include <QNetworkInterface>
#include <QUdpSocket>
#include <QDesktopServices>
#include <QProcess>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include <QSettings>
#include <QHostAddress>
#include <QUrl>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <sddl.h>

// PJSIP utilities
#include <pjsip/sip_types.h>

// ----------------------------------------------------------------------
// Global helper functions (as in original)
// ----------------------------------------------------------------------

QByteArray onyx_md5sum(const QByteArray& str)
{
    QByteArray md5sum;
    pj_md5_context ctx;
    pj_uint8_t digest[16];
    pj_md5_init(&ctx);
    pj_md5_update(&ctx, (pj_uint8_t*)str.constData(), str.size());
    pj_md5_final(&ctx, digest);
    md5sum.resize(32);
    char* p = md5sum.data();
    for (int i = 0; i < 16; ++i) {
        pj_val_to_hex_digit(digest[i], p);
        p += 2;
    }
    return md5sum;
}

QByteArray onyx_md5sum(const QString& str)
{
    return onyx_md5sum(str.toUtf8());
}

void onyx_audio_conf_set_volume(int val, bool mute)
{
    if (!is_pjsua_running())
        return;
    if (mute)
        val = 0;
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            pjsua_conf_port_id conf_port_id = pjsua_call_get_conf_port(call_ids[i]);
            if (conf_port_id != PJSUA_INVALID_ID) {
                pjsua_conf_adjust_rx_level(conf_port_id, (float)val / 100);
            }
        }
    }
}

void onyx_audio_input_set_volume(int val, bool mute)
{
    if (!is_pjsua_running())
        return;
    if (mute) {
        val = 0;
    } else {
        pj_status_t status = -1;
        // external accountSettings not ported; we'll assume a global or pass as param
        // For this port we skip the hardware volume adjustment and just use software.
        // We'll set val as is.
    }
    pjsua_conf_adjust_rx_level(0, (float)val / 100);
}

pj_status_t onyx_verify_sip_url(const QString& url)
{
    if (!is_pjsua_running())
        return PJSIP_ENOTINITIALIZED;
    QByteArray urlA = url.toUtf8();
    return urlA.size() > 900 ? PJSIP_EURITOOLONG : pjsua_verify_sip_url(urlA.constData());
}

int onyx_get_duration(pj_time_val* time_val)
{
    int res = time_val->sec;
    if (time_val->msec >= 500)
        res++;
    return res;
}

// ----------------------------------------------------------------------
// Onyx namespace implementation
// ----------------------------------------------------------------------

namespace Onyx {

void GetScreenRect(QRect* rect)
{
    rect->setLeft(GetSystemMetrics(SM_XVIRTUALSCREEN));
    rect->setTop(GetSystemMetrics(SM_YVIRTUALSCREEN));
    rect->setRight(GetSystemMetrics(SM_CXVIRTUALSCREEN) - rect->left());
    rect->setBottom(GetSystemMetrics(SM_CYVIRTUALSCREEN) - rect->top());
}

QString GetErrorMessage(pj_status_t status)
{
    QString str;
    if (!is_pjsua_running()) {
        str = "Softphone is not initialized. Check your settings.";
    } else if (status == 171039 || status == 171042) {
        str = "Invalid Number";
    } else if (status == PJSIP_EAUTHACCNOTFOUND || status == PJSIP_EAUTHACCDISABLED) {
        str = "Account or credentials not found.";
    } else if (status == 130051) {
        str = "Unable to connect to remote server.";
    } else {
        char buf[PJ_ERR_MSG_SIZE];
        pj_strerror(status, buf, sizeof(buf));
        str = QString::fromLocal8Bit(buf);
        int i = str.lastIndexOf('(');
        if (i != -1)
            str = str.left(i - 1);
    }
    // Assume translation is done elsewhere; return as is.
    return str;
}

bool ShowErrorMessage(pj_status_t status)
{
    if (status != PJ_SUCCESS) {
        // Use QMessageBox in a real app, but here just print
        qDebug() << "Error:" << GetErrorMessage(status);
        return true;
    }
    return false;
}

bool IsIP(const QString& host)
{
    QHostAddress addr;
    return addr.setAddress(host);
}

QString RemovePort(const QString& domain)
{
    int pos = domain.indexOf(':');
    if (pos != -1)
        return domain.left(pos);
    return domain;
}

void ParseSIPURI(const QString& in, SIPURI* out)
{
    out->name.clear();
    out->user.clear();
    out->domain.clear();
    out->parameters.clear();
    out->commands.clear();

    QString tmp = in;
    if (tmp.endsWith('>'))
        tmp.chop(1);

    int start = tmp.indexOf("sip:");
    if (start > 0) {
        out->name = tmp.left(start);
        out->name = out->name.trimmed();
        out->name.remove('"');
        out->name.remove('<');
        if (out->name.compare("unknown", Qt::CaseInsensitive) == 0)
            out->name.clear();
    }
    if (start >= 0)
        start += 4;
    else
        start = 0;

    int end = tmp.indexOf('@', start);
    if (end >= 0) {
        out->user = tmp.mid(start, end - start);
        start = end + 1;
    }

    end = tmp.indexOf(';', start);
    if (end >= 0) {
        out->domain = tmp.mid(start, end - start);
        start = end;
        out->parameters = tmp.mid(start);
    } else {
        end = tmp.indexOf('?', start);
        if (end >= 0) {
            out->domain = tmp.mid(start, end - start);
            start = end;
            out->parameters = tmp.mid(start);
        } else {
            out->domain = tmp.mid(start);
            int comma = out->domain.indexOf(',');
            if (comma >= 0) {
                out->commands = out->domain.mid(comma);
                out->domain = out->domain.left(comma);
            }
        }
    }
}

QString BuildSIPURI(const SIPURI* in)
{
    QString res = in->user + "@" + in->domain;
    if (!in->parameters.isEmpty())
        res += ";" + in->parameters;
    res = "<sip:" + res + ">";
    if (!in->name.isEmpty())
        res = "\"" + in->name + "\" " + res;
    if (!in->commands.isEmpty())
        res += "," + in->commands;
    return res;
}

QString PjToStr(const pj_str_t* str, bool utf)
{
    QByteArray ba(str->ptr, str->slen);
    if (utf)
        return QString::fromUtf8(ba);
    else
        return QString::fromLocal8Bit(ba);
}

QString Utf8DecodeUni(const char* str)
{
    return QString::fromUtf8(str);
}

QByteArray Utf8EncodeUni(const QString& str)
{
    return str.toUtf8();
}

QString AnsiToWideChar(const char* str)
{
    return QString::fromLocal8Bit(str);
}

pj_str_t StrToPjStr(const QString& str)
{
    // caller must free memory
    char* buf = WideCharToPjStr(str);
    return pj_str(buf);
}

char* WideCharToPjStr(const QString& str)
{
    QByteArray utf8 = str.toUtf8();
    char* buf = (char*)malloc(utf8.size() + 1);
    if (!buf) {
        return nullptr;
    }
    memcpy(buf, utf8.constData(), utf8.size());
    buf[utf8.size()] = '\0';
    return buf;
}

void OpenURL(const QString& url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void OpenFile(const QString& filename)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}

QString GetDuration(int sec, bool zero)
{
    if (!sec && !zero)
        return QString();
    int h = sec / 3600;
    sec %= 3600;
    int m = sec / 60;
    sec %= 60;
    if (h)
        return QString("%1:%2:%3").arg(h, 2, 10, QChar('0'))
                                  .arg(m, 2, 10, QChar('0'))
                                  .arg(sec, 2, 10, QChar('0'));
    else
        return QString("%1:%2").arg(m, 2, 10, QChar('0'))
                               .arg(sec, 2, 10, QChar('0'));
}

bool IsPSTNNnmber(const QString& number)
{
    for (int i = 0; i < number.size(); ++i) {
        QChar c = number[i];
        if (!c.isDigit() && c != '*' && c != '#' && c != '.' &&
            c != '-' && c != '(' && c != ')' && c != '/' &&
            c != ' ' && !(i == 0 && c == '+')) {
            return false;
        }
    }
    return true;
}

bool IniSectionExists(const QString& section, const QString& iniFile)
{
    QSettings settings(iniFile, QSettings::IniFormat);
    return settings.childGroups().contains(section);
}

QString Bin2String(const QByteArray& ba)
{
    QString res;
    for (int i = 0; i < ba.size(); ++i)
        res += QString("%1").arg((unsigned char)ba[i], 2, 16, QChar('0'));
    return res;
}

void String2Bin(const QString& str, QByteArray* res)
{
    res->clear();
    for (int i = 0; i < str.size(); i += 2) {
        bool ok;
        unsigned int val = str.mid(i, 2).toUInt(&ok, 16);
        if (ok)
            res->append((char)val);
    }
}

void CommandLineToShell(const QString& cmd, QString& command, QString& params)
{
    command.clear();
    params.clear();
    QString trimmed = cmd.trimmed();
    // Simple split: first token is command, rest are parameters
    int firstSpace = trimmed.indexOf(' ');
    if (firstSpace == -1) {
        command = trimmed;
    } else {
        command = trimmed.left(firstSpace);
        params = trimmed.mid(firstSpace + 1).trimmed();
    }
    // Handle quoted command with spaces
    if (command.startsWith('"') && command.endsWith('"'))
        command = command.mid(1, command.size() - 2);
}

void RunCmd(const QString& cmdLine, const QString& addParams, bool noWait)
{
    QString command, params;
    CommandLineToShell(cmdLine, command, params);
    if (!addParams.isEmpty())
        params += " " + addParams;

    QProcess process;
    process.setProgram(command);
    process.setArguments(params.split(' ', Qt::SkipEmptyParts));
    process.setProcessChannelMode(QProcess::MergedChannels);
    if (noWait) {
        process.startDetached();
    } else {
        process.start();
        process.waitForFinished(10000);
    }
}

void PortKnock()
{
    // accountSettings not available; we'll use external variables
    // For this port we assume they are global or passed.
    // We'll use dummy values or skip.
    // We'll keep the function but not implement fully.
    qDebug() << "PortKnock not implemented in Qt port";
}

bool IsConnectedToNetwork(bool checkInternet)
{
    // Use QNetworkInterface to see if there's any active interface
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : ifaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            return true;
        }
    }
    return false;
}

QString FormatDateTime(const QDateTime& time, const QDateTime* timeNow)
{
    QDateTime now = timeNow ? *timeNow : QDateTime::currentDateTime();
    QString format = (now.date().year() == time.date().year() &&
                      now.date().month() == time.date().month() &&
                      now.date().day() == time.date().day())
                     ? "hh:mm:ss" : "yyyy-MM-dd hh:mm:ss";
    return time.toString(format);
}

void ExpandEnvironmentStrings(QString& str)
{
    if (str.isEmpty())
        return;
    wchar_t buf[32768]; // typical max
    DWORD len = ::ExpandEnvironmentStringsW(str.toStdWString().c_str(), buf, sizeof(buf)/sizeof(wchar_t));
    if (len > 0 && len < sizeof(buf)/sizeof(wchar_t))
        str = QString::fromWCharArray(buf);
}

QString GetSID()
{
    QString res;
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        DWORD size = 0;
        GetTokenInformation(hToken, TokenUser, nullptr, 0, &size);
        BYTE* buffer = new BYTE[size];
        TOKEN_USER* tokenUser = (TOKEN_USER*)buffer;
        if (GetTokenInformation(hToken, TokenUser, tokenUser, size, &size)) {
            wchar_t* stringSid = nullptr;
            ConvertSidToStringSidW(tokenUser->User.Sid, &stringSid);
            res = QString::fromWCharArray(stringSid);
            LocalFree(stringSid);
        }
        CloseHandle(hToken);
        delete[] buffer;
    }
    return res;
}

} // namespace Onyx
