#define THIS_FILENAME "global.cpp"

#include "global.h"
#include "settings.h"
#include "langpack.h"
#include "addons.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QRegularExpression>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QApplication>
#include <QWidget>
#include <QMetaObject>
#include <QThread>
#include <QDebug>
#include <cmath>

static std::atomic<bool> pjsua_running(false);

pjsua_transport_id transport_udp_local;
pjsua_transport_id transport_udp;
pjsua_transport_id transport_tcp;
pjsua_transport_id transport_tls;

struct call_tonegen_data* tone_gen = nullptr;
pjsua_acc_id account;
QString password;
pjsua_acc_id account_local;
pjsua_conf_port_id onyx_conf_port_id;
pjsua_call_id onyx_conf_port_call_id;

int onyx_audio_input;
int onyx_audio_output;
int onyx_audio_ring;

QList<pjmedia_port*> DTMFTonegens;

QString customString;

bool is_pjsua_running() {
    return pjsua_running.load();
}

void set_pjsua_running(bool running) {
    pjsua_running.store(running);
}

QString FormatNumber(QString number, QString* commands, bool noTransform) {
    int pos = number.indexOf(',');
    if (pos > 0 && pos < number.length() - 1) {
        if (commands) {
            *commands = number.mid(pos);
        }
        number = number.left(pos);
    }
    QString numberFormated = number;
    pjsua_acc_id acc_id;
    bool isLocal = SelectSIPAccount(number, acc_id) && acc_id == account_local;
    if (!noTransform) {
        if (number.indexOf('<') == -1 || number.indexOf('>') == -1) {
            if (!isLocal) {
                bool addPrefix = false;
                if (Onyx::IsPSTNNnmber(number)) {
                    numberFormated.remove('.');
                    numberFormated.remove('-');
                    numberFormated.remove('(');
                    numberFormated.remove(')');
                    numberFormated.remove('/');
                    numberFormated.remove(' ');
                    if (!accountSettings.account.dialingPrefix.isEmpty() && numberFormated.length() > 3) {
                        if (numberFormated.left(1) == QLatin1String("+")) {
                            numberFormated = numberFormated.mid(1);
                        }
                        addPrefix = true;
                    }
                }
                if (addPrefix) {
                    numberFormated = accountSettings.account.dialingPrefix + numberFormated;
                }
                if (!accountSettings.account.dialPlan.isEmpty()) {
                    QString dialPlan = accountSettings.account.dialPlan;
                    dialPlan = dialPlan.trimmed();
                    if (dialPlan.startsWith('(') && dialPlan.endsWith(')'))
                        dialPlan = dialPlan.mid(1, dialPlan.length() - 2);
                    pos = 0;
                    bool matched = false;
                    QStringList tokens = dialPlan.split('|', Qt::SkipEmptyParts);
                    for (const QString& resTokenOrig : tokens) {
                        QString resToken = resTokenOrig;
                        QString newToken;
                        QString replaceGroup;
                        QStringList delayedReplaces;
                        bool group = false;
                        for (int i = 0; i < resToken.length(); ++i) {
                            QChar c = resToken.at(i);
                            if (!group && c == '<') {
                                group = true;
                            }
                            else if (group) {
                                if (c != '>') {
                                    replaceGroup.append(c);
                                }
                                else {
                                    if (!replaceGroup.isEmpty()) {
                                        int p = replaceGroup.indexOf(':');
                                        if (p == -1) {
                                            newToken.append(replaceGroup);
                                        }
                                        else {
                                            QString match = replaceGroup.left(p);
                                            QString replace = replaceGroup.mid(p + 1);
                                            newToken.append(QString("{%1}").arg(match));
                                            delayedReplaces.append(replace);
                                        }
                                    }
                                    replaceGroup.clear();
                                    group = false;
                                }
                            }
                            else {
                                newToken.append(c);
                            }
                        }
                        newToken.replace('.', '*');
                        newToken.replace('x', '.');
                        newToken.replace('X', '.');
                        QString pattern = QString("^%1$").arg(newToken);
                        QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
                        QRegularExpressionMatch match = regex.match(numberFormated);
                        if (match.hasMatch()) {
                            if (!delayedReplaces.isEmpty()) {
                                QString numberFormatedNew;
                                int i = 0;
                                int prev = 0;
                                for (const QString& replace : delayedReplaces) {
                                    int start = match.capturedStart(i);
                                    int end = match.capturedEnd(i);
                                    numberFormatedNew.append(numberFormated.mid(prev, start - prev));
                                    numberFormatedNew.append(replace);
                                    prev = end;
                                    ++i;
                                }
                                numberFormatedNew.append(numberFormated.mid(prev));
                                numberFormated = numberFormatedNew;
                            }
                            matched = true;
                            break;
                        }
                    }
                    if (!matched) {
                        numberFormated.clear();
                    }
                }
            }
        }
    }
    return GetSIPURI(numberFormated, true, isLocal);
}

void AddTransportSuffix(QString& str, Account* account)
{
    if (account) {
        QString suffix;
        if (account->transport == QLatin1String("tcp") && transport_tcp != -1) {
            suffix = QLatin1String(";transport=tcp");
        }
        else if (account->transport == QLatin1String("tls") && transport_tls != -1) {
            suffix = QLatin1String(";transport=tls");
        }
        if (!suffix.isEmpty()) {
            int pos = str.indexOf('?');
            if (pos != -1) {
                str.insert(pos, suffix);
            }
            else {
                str.append(suffix);
            }
        }
    }
}

QString GetSIPURI(QString str, bool isSimple, bool isLocal, QString domain)
{
    QString rab = str.toLower();
    int pos = rab.indexOf(QLatin1String("sip:"));
    if (pos == -1)
    {
        str = QLatin1String("sip:") + str;
    }
    pos = str.indexOf('@');
    if (!isLocal) {
        if (accountSettings.accountId && pos == -1) {
            str.append('@' + (!domain.isEmpty() ? domain : get_account_domain()));
        }
    }
    else {
        if (pos == -1 && !accountSettings.accountLocal.domain.isEmpty()) {
            str.append('@' + accountSettings.accountLocal.domain);
        }
    }
    if (!str.isEmpty() && str.at(str.length() - 1) == '>')
    {
        str = str.left(str.length() - 1);
        if (!isSimple) {
            AddTransportSuffix(str, isLocal ? &accountSettings.accountLocal : &accountSettings.account);
        }
        str += '>';
    }
    else {
        if (!isSimple) {
            AddTransportSuffix(str, isLocal ? &accountSettings.accountLocal : &accountSettings.account);
        }
        str = '<' + str + '>';
    }
    return str;
}

void ParseCallSIPURI(QString &number, call_user_data* user_data, SIPURI* out)
{
    SIPURI sipuriCallerID;
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        if (!user_data->callerID.isEmpty()) {
            Onyx::ParseSIPURI(user_data->callerID, &sipuriCallerID);
        }
    }
    Onyx::ParseSIPURI(number, out);
    if (!sipuriCallerID.user.isEmpty()) {
        out->user = sipuriCallerID.user;
    }
    if (!sipuriCallerID.domain.isEmpty()) {
        out->domain = sipuriCallerID.domain;
    }
    if (!sipuriCallerID.name.isEmpty()) {
        out->name = sipuriCallerID.name;
    }
}

void ParseCallSIPURI(pjsua_call_info* call_info, call_user_data* user_data, SIPURI* out)
{
    QString number = Onyx::PjToStr(&call_info->remote_info, TRUE);
    ParseCallSIPURI(number, user_data, out);
}

QString GetPAI(pjsip_rx_data* rdata)
{
    QString res;
    if (rdata) {
        pjsip_generic_string_hdr* hsr;
        const pj_str_t headerCallerID = { (char*)"P-Asserted-Identity", 19 };
        hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerCallerID, NULL);
        if (!hsr) {
            const pj_str_t headerCallerID2 = { (char*)"Remote-Party-Id", 15 };
            hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerCallerID2, NULL);
        }
        if (hsr) {
            res = Onyx::PjToStr(&hsr->hvalue, true);
            if (res.indexOf('@') == -1) {
                res.clear();
            }
            else {
                int pos = res.indexOf(';');
                if (pos != -1) {
                    res = res.left(pos);
                }
                res = res.trimmed();
            }
        }
    }
    return res;
}

bool SelectSIPAccount(QString number, pjsua_acc_id& acc_id, pj_str_t* pj_uri)
{
    if (!is_pjsua_running()) {
        return false;
    }
    SIPURI sipuri;
    Onyx::ParseSIPURI(number, &sipuri);
    if (pjsua_acc_is_valid(account) && pjsua_acc_is_valid(account_local)) {
        acc_id = account;
        if (get_account_domain() != sipuri.domain) {
            int pos = sipuri.domain.indexOf(':');
            QString domainWithoutPort = Onyx::RemovePort(sipuri.domain);
            if (domainWithoutPort.compare(QLatin1String("localhost"), Qt::CaseInsensitive) == 0 || Onyx::IsIP(domainWithoutPort)) {
                acc_id = account_local;
            }
        }
    }
    else if (pjsua_acc_is_valid(account)) {
        acc_id = account;
    }
    else if (pjsua_acc_is_valid(account_local)) {
        acc_id = account_local;
    }
    else {
        return false;
    }
    if (pj_uri) {
        *pj_uri = Onyx::StrToPjStr(GetSIPURI(number, false, acc_id == account_local));
    }
    return true;
}

void OpenHelp(QString code)
{
    QString url = QStringLiteral(_GLOBAL_HELP_WEBSITE);
    url.append('#');
    Onyx::OpenURL(url + code);
}

struct call_tonegen_data* call_init_tonegen(pjsua_call_id call_id)
{
    if (!is_pjsua_running()) {
        return nullptr;
    }
    pj_status_t status;
    pj_pool_t* pool;
    struct call_tonegen_data* cd;
    pjsua_call_info ci;

    if (call_id != -1) {
        pjsua_call_get_info(call_id, &ci);

        if (ci.media_status != PJSUA_CALL_MEDIA_ACTIVE)
            return nullptr;
    }

    pool = pjsua_pool_create("onyx_tonegen", 512, 512);
    cd = new call_tonegen_data{};
    cd->pool = pool;

    status = pjmedia_tonegen_create(cd->pool, 8000, 1, 64, 16, 0, &cd->tonegen);
    if (status != PJ_SUCCESS) {
        return nullptr;
    }
    pjsua_conf_add_port(cd->pool, cd->tonegen, &cd->toneslot);
    if (call_id != -1) {
        pjsua_conf_connect(cd->toneslot, ci.conf_slot);
    }
    else {
        if (accountSettings.localDTMF) {
            pjsua_conf_adjust_rx_level(cd->toneslot, 0.4);
            pjsua_conf_connect(cd->toneslot, 0);
        }
    }
    if (call_id != -1) {
        call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
        if (!user_data) {
            user_data = new call_user_data(call_id);
            pjsua_call_set_user_data(call_id, user_data);
        }
        QMutexLocker lock(&user_data->CS);
        user_data->tonegen_data = cd;
    }
    return cd;
}

static QTimer* destroyDTMFPlayerTimer = nullptr;
static QTimer* tonegenBusyTimer = nullptr;

void destroyDTMFPlayerTimerHandler()
{
    if (!tone_gen || !is_pjsua_running() || !pjmedia_tonegen_is_busy(tone_gen->tonegen)) {
        if (destroyDTMFPlayerTimer) {
            destroyDTMFPlayerTimer->stop();
            destroyDTMFPlayerTimer->deleteLater();
            destroyDTMFPlayerTimer = nullptr;
        }
        call_deinit_tonegen(-1);
    }
}

void DTMFQueueTimerHandler(pjsua_call_id call_id)
{
    if (is_pjsua_running() && pjsua_call_is_active(call_id)) {
        call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
        if (user_data) {
            QMutexLocker lock(&user_data->CS);
            if (!user_data->commands.isEmpty()) {
                QString dtmf;
                int pos = user_data->commands.indexOf(',');
                if (pos != -1) {
                    dtmf = user_data->commands.left(pos);
                    user_data->commands = user_data->commands.mid(pos + 1);
                }
                else {
                    dtmf = user_data->commands;
                    user_data->commands.clear();
                }
                if (!dtmf.isEmpty()) {
                    onyx_call_dial_dtmf(call_id, dtmf);
                }
                if (!user_data->commands.isEmpty()) {
                    QTimer::singleShot(1000 + 200 * dtmf.length(), [call_id]() {
                        DTMFQueueTimerHandler(call_id);
                    });
                }
            }
        }
    }
}

void tonegenBusyHandler()
{
    for (int i = DTMFTonegens.size() - 1; i >= 0; --i) {
        pjmedia_port* port = DTMFTonegens.at(i);
        if (!is_pjsua_running() || pjmedia_tonegen_is_busy(port) == PJ_FALSE) {
            DTMFTonegens.removeAt(i);
        }
    }
    if (DTMFTonegens.isEmpty()) {
        if (tonegenBusyTimer) {
            tonegenBusyTimer->stop();
            tonegenBusyTimer->deleteLater();
            tonegenBusyTimer = nullptr;
        }
        QWidget* hWnd = qApp->activeWindow();
        if (hWnd) {
            QMetaObject::invokeMethod(hWnd, "onRefreshLevels", Qt::QueuedConnection);
        }
    }
}

void onyx_set_sound_device(int outDev, bool forse) {
    if (!is_pjsua_running()) {
        return;
    }
    int in, out;
    if (forse || onyx_audio_input == -1 || pjsua_get_snd_dev(&in, &out) != PJ_SUCCESS || onyx_audio_input != in || outDev != out) {
        pjsua_snd_dev_param params;
        pjsua_snd_dev_param_default(&params);
        params.capture_dev = onyx_audio_input;
        params.playback_dev = outDev;
        if (pjsua_set_snd_dev2(&params) != PJ_SUCCESS) {
            params.mode |= PJSUA_SND_DEV_SPEAKER_ONLY;
            pjsua_set_snd_dev2(&params);
        }
    }
}

bool onyx_call_statistics(call_user_data* user_data, float* MOS)
{
    if (!is_pjsua_running()) {
        return false;
    }
    if (user_data->call_id == PJSUA_INVALID_ID) {
        return false;
    }
    if (!pjsua_call_has_media(user_data->call_id)) {
        return false;
    }
    pjsua_stream_stat stat;
    pj_status_t status = pjsua_call_get_stream_stat(user_data->call_id, 0, &stat);
    if (status != PJ_SUCCESS) {
        return false;
    }

    int LOCAL_DELAY = 30;
    float R;
    float a = 0.0f;
    float b = 19.8f;
    float c = 29.7f;
    float rx_loss = 0.0;
    float rx_jit = 0.0;
    float avg_latency = 0.0;

    int pkt_last = stat.rtcp.rx.pkt - user_data->rx_pkt_prev;
    int loss_last = stat.rtcp.rx.loss - user_data->rx_loss_prev;
    rx_loss = (pkt_last == 0) ? 1.0f : ((float)loss_last / (float)(pkt_last + loss_last));
    user_data->rx_pkt_prev = stat.rtcp.rx.pkt;
    user_data->rx_loss_prev = stat.rtcp.rx.loss;

    rx_jit = (float)stat.rtcp.rx.jitter.last / 1000;

    avg_latency = (stat.rtcp.rtt.last / 2000.0f) + LOCAL_DELAY + PJMEDIA_SND_DEFAULT_PLAY_LATENCY +
        PJMEDIA_SND_DEFAULT_REC_LATENCY + rx_jit;

    {
        float d = avg_latency;
        float d2 = d - 177.3f;
        float Id = 0.024f * d + 0.11f * (d - 177.3f) * (d2 < 0 ? 0 : 1);
        float P = rx_loss;
        float Ie = a + b * (float)log(1 + c * P);
        R = 94.2f - Id - Ie;
    }
    if (R < 0) {
        *MOS = 1;
    }
    else if (R > 100) {
        *MOS = 4.5;
    }
    else {
        *MOS = 1 + 0.035f * R + 7.10f / 1000000 * R * (R - 60) * (100 - R);
    }
    return true;
}

void onyx_call_dial_dtmf(pjsua_call_id call_id, QString digits)
{
    if (!is_pjsua_running()) {
        return;
    }
    bool simulate = true;
    if (call_id != PJSUA_INVALID_ID) {
        pjsua_call_info call_info;
        pjsua_call_get_info(call_id, &call_info);
        if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
            pj_str_t pj_digits = Onyx::StrToPjStr(digits);
            if (accountSettings.DTMFMethod == 1) {
                // in-band
                simulate = !call_play_digit(call_id, digits.toLatin1().constData());
            }
            else if (accountSettings.DTMFMethod == 2) {
                // RFC2833
                pjsua_call_dial_dtmf(call_id, &pj_digits);
            }
            else if (accountSettings.DTMFMethod == 3) {
                // sip-info
                onyx_call_send_dtmf_info(call_id, pj_digits);
            }
            else {
                // auto
                if (pjsua_call_dial_dtmf(call_id, &pj_digits) != PJ_SUCCESS) {
                    simulate = !call_play_digit(call_id, digits.toLatin1().constData());
                }
            }
            free(pj_digits.ptr);
        }
    }
    if (simulate && accountSettings.localDTMF) {
        onyx_set_sound_device(onyx_audio_output);
        call_play_digit(-1, digits.toLatin1().constData());
    }
}

BOOL call_play_digit(pjsua_call_id call_id, const char* digits, int duration)
{
    if (!is_pjsua_running()) {
        return FALSE;
    }
    pjmedia_tone_digit d[16];
    unsigned i, count = strlen(digits);
    struct call_tonegen_data* cd;
    call_user_data* user_data = nullptr;
    if (call_id != -1) {
        user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
        cd = nullptr;
        if (user_data) {
            QMutexLocker lock(&user_data->CS);
            if (user_data->tonegen_data) {
                cd = user_data->tonegen_data;
            }
        }
    }
    else {
        cd = tone_gen;
    }
    if (!cd)
        cd = call_init_tonegen(call_id);
    if (!cd)
        return FALSE;
    if (call_id == -1) {
        tone_gen = cd;
    }

    if (count > PJ_ARRAY_SIZE(d))
        count = PJ_ARRAY_SIZE(d);

    pj_bzero(d, sizeof(d));
    for (i = 0; i < count; ++i) {
        d[i].digit = digits[i];
        d[i].on_msec = duration;
        d[i].off_msec = 50;
        d[i].volume = call_id == -1 ? (1 + accountSettings.volumeRing * PJMEDIA_TONEGEN_VOLUME / 100) : 0;
    }

    if (call_id != -1) {
        // mute microphone before play in-band tones
        onyx_audio_input_set_volume(0, true);
        if (!DTMFTonegens.contains(cd->tonegen)) {
            DTMFTonegens.append(cd->tonegen);
        }
        if (tonegenBusyTimer) {
            tonegenBusyTimer->stop();
            tonegenBusyTimer->deleteLater();
        }
        tonegenBusyTimer = new QTimer();
        QObject::connect(tonegenBusyTimer, &QTimer::timeout, tonegenBusyHandler);
        tonegenBusyTimer->start(800);
    }

    pjmedia_tonegen_play_digits(cd->tonegen, count, d, 0);

    if (call_id == -1) {
        if (destroyDTMFPlayerTimer) {
            destroyDTMFPlayerTimer->stop();
            destroyDTMFPlayerTimer->deleteLater();
        }
        destroyDTMFPlayerTimer = new QTimer();
        QObject::connect(destroyDTMFPlayerTimer, &QTimer::timeout, destroyDTMFPlayerTimerHandler);
        destroyDTMFPlayerTimer->start(5000);
    }
    return TRUE;
}

void call_deinit_tonegen(pjsua_call_id call_id)
{
    struct call_tonegen_data* cd;
    call_user_data* user_data = nullptr;

    if (call_id != -1) {
        if (is_pjsua_running()) {
            user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
        }
        cd = nullptr;
        if (user_data) {
            QMutexLocker lock(&user_data->CS);
            if (user_data->tonegen_data) {
                cd = user_data->tonegen_data;
                DTMFTonegens.removeAll(cd->tonegen);
            }
        }
    }
    else {
        cd = tone_gen;
    }
    if (!cd)
        return;

    if (is_pjsua_running()) {
        pjsua_conf_remove_port(cd->toneslot);
        pjmedia_port_destroy(cd->tonegen);
        pj_pool_release(cd->pool);
    }

    if (call_id != -1) {
        if (user_data) {
            QMutexLocker lock(&user_data->CS);
            user_data->tonegen_data = nullptr;
        }
    }
    else {
        tone_gen = nullptr;
    }
}

bool call_hangup_all_noincoming(bool onHold)
{
    bool res = false;
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (is_pjsua_running() && pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            pjsua_call_info call_info;
            pjsua_call_get_info(call_ids[i], &call_info);
            if (call_info.role != PJSIP_ROLE_UAS || (call_info.state != PJSIP_INV_STATE_INCOMING && call_info.state != PJSIP_INV_STATE_EARLY)) {
                if (onHold && call_info.media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD) {
                    continue;
                }
                onyx_call_hangup_fast(call_ids[i], &call_info);
                res = true;
            }
        }
    }
    return res;
}

bool call_hangup_incoming()
{
    bool res = false;
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (is_pjsua_running() && pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            pjsua_call_info call_info;
            pjsua_call_get_info(call_ids[i], &call_info);
            if (call_info.role == PJSIP_ROLE_UAS && (call_info.state == PJSIP_INV_STATE_INCOMING || call_info.state == PJSIP_INV_STATE_EARLY)) {
                onyx_call_hangup_fast(call_ids[i], &call_info);
                res = true;
            }
        }
    }
    return res;
}

void call_hangup_calling()
{
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (is_pjsua_running() && pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            pjsua_call_info call_info;
            pjsua_call_get_info(call_ids[i], &call_info);
            if (call_info.role == PJSIP_ROLE_UAC && call_info.state != PJSIP_INV_STATE_CONFIRMED) {
                onyx_call_hangup_fast(call_ids[i], &call_info);
            }
        }
    }
}

void call_hangup_all()
{
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (is_pjsua_running() && pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            onyx_call_end(call_ids[i]);
        }
    }
}

static char _x2c(char hex_up, char hex_low)
{
    char digit;
    digit = 16 * (hex_up >= 'A'
        ? ((hex_up & 0xdf) - 'A') + 10 : (hex_up - '0'));
    digit += (hex_low >= 'A'
        ? ((hex_low & 0xdf) - 'A') + 10 : (hex_low - '0'));
    return (digit);
}

QByteArray urldecode(const QByteArray& str)
{
    QByteArray res;
    for (int j = 0; j < str.length(); ++j) {
        switch (str.at(j)) {
        case '+':
            res.append(' ');
            break;
        case '%':
            if (j + 2 < str.length()) {
                res.append(_x2c(str.at(j + 1), str.at(j + 2)));
                j += 2;
            }
            break;
        default:
            res.append(str.at(j));
            break;
        }
    }
    return res;
}

QByteArray urlencode(const QByteArray& str)
{
    QByteArray escaped;
    int max = str.length();
    for (int i = 0; i < max; ++i)
    {
        const char chr = str.at(i);
        if ((48 <= chr && chr <= 57) ||//0-9
            (65 <= chr && chr <= 90) ||//ABC...XYZ
            (97 <= chr && chr <= 122) || //abc...xyz
            (chr == '~' || chr == '!' || chr == '*' || chr == '(' || chr == ')' || chr == '\'')
            )
        {
            escaped.append(chr);
        }
        else
        {
            escaped.append('%');
            escaped.append(char2hex(chr));
        }
    }
    return escaped;
}

QByteArray char2hex(char dec)
{
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);
    if (0 <= dig1 && dig1 <= 9) dig1 += 48;
    if (10 <= dig1 && dig1 <= 15) dig1 += 97 - 10;
    if (0 <= dig2 && dig2 <= 9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 97 - 10;

    QByteArray r;
    r.append(dig1);
    r.append(dig2);
    return r;
}

// Network helpers (Qt replacement for CInternetSession)
static QNetworkAccessManager* g_nam = nullptr;
static QNetworkAccessManager* getNAM() {
    if (!g_nam) {
        g_nam = new QNetworkAccessManager();
    }
    return g_nam;
}

static void URLGetAsyncThreadImpl(URLGetAsyncData* data)
{
    data->body.clear();
    data->statusCode = 0;
    if (!data->url.isEmpty()) {
        QUrl qurl(data->url);
        QNetworkRequest request(qurl);
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

        if (!data->username.isEmpty() && !data->password.isEmpty()) {
            QString auth = data->username + ':' + data->password;
            request.setRawHeader("Authorization", "Basic " + auth.toUtf8().toBase64());
        }

        if (!data->headers.isEmpty()) {
            // simple header parsing (Content-Type etc.)
            const QStringList lines = data->headers.split('\n', Qt::SkipEmptyParts);
            for (const QString& line : lines) {
                int colon = line.indexOf(':');
                if (colon > 0) {
                    request.setRawHeader(line.left(colon).trimmed().toUtf8(),
                                         line.mid(colon + 1).trimmed().toUtf8());
                }
            }
        }

        QNetworkReply* reply = nullptr;
        if (data->post) {
            QByteArray postBody = data->postData.toUtf8();
            if (postBody.isEmpty()) {
                int pos = qurl.query().isEmpty() ? -1 : 0;
                // already handled by caller usually
            }
            if (data->headers.isEmpty()) {
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            }
            reply = getNAM()->post(request, postBody);
        } else {
            reply = getNAM()->get(request);
        }

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        QTimer::singleShot(10000, &loop, &QEventLoop::quit); // timeout
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            data->statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            data->body = reply->readAll();
            data->headers = QString::fromUtf8(reply->rawHeaderPairs().value(0).second); // simplified
        } else {
            data->statusCode = 0;
        }
        reply->deleteLater();
    }

    if (data->message && data->hWnd) {
        QMetaObject::invokeMethod(data->hWnd, "onUrlGetFinished",
                                  Qt::QueuedConnection,
                                  Q_ARG(void*, data));
    } else {
        delete data;
    }
}

void URLGetAsync(QString url, QWidget* hWnd, int message, bool post,
                 QString postData, QString headers, QString username, QString password, void* userData)
{
    URLGetAsyncData* data = new URLGetAsyncData();
    data->hWnd = hWnd;
    data->message = message;
    data->statusCode = 0;
    data->url = url;
    data->post = post;
    data->postData = postData;
    data->headers = headers;
    data->username = username;
    data->password = password;
    data->userData = userData;

    // run in background thread
    QThread* thread = QThread::create([data]() {
        URLGetAsyncThreadImpl(data);
    });
    QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

URLGetAsyncData URLGetSync(QString url, bool post, QString postData, QString headers,
                           QString username, QString password, void* userData)
{
    URLGetAsyncData data;
    data.hWnd = nullptr;
    data.message = 1;
    data.statusCode = 0;
    data.url = url;
    data.post = post;
    data.postData = postData;
    data.headers = headers;
    data.username = username;
    data.password = password;
    data.userData = userData;
    URLGetAsyncThreadImpl(&data);
    return data;
}

QString get_account_username()
{
    return accountSettings.account.username;
}

QString get_account_password()
{
    QString res = accountSettings.account.password;
    if (!password.isEmpty()) {
        res = password;
    }
    return res;
}

QString get_account_domain()
{
    return accountSettings.account.domain;
}

QString get_account_server()
{
    return accountSettings.account.server;
}

void get_account_proxy(Account* account, QStringList& proxies)
{
    proxies.clear();
    if (!account) return;
    const QStringList tokens = account->proxy.split(' ', Qt::SkipEmptyParts);
    for (const QString& t : tokens) {
        proxies.append(t);
    }
}

QString get_public_addr(Account* account)
{
    QString res = account ? account->publicAddr : accountSettings.account.publicAddr;
    if (!res.isEmpty()) {
        // simplified: return configured public address
        // full original logic used getaddrinfo + private network matching;
        // kept as-is for structure, can be expanded with QNetworkInterface if needed
    }
    return res;
}

QString URLMask(QString url, SIPURI* sipuri, pjsua_acc_id acc, call_user_data* user_data, pjsua_call_info* call_info)
{
    QString str;
    if (accountSettings.accountId) {
        str = get_account_server();
    }
    url.replace(QLatin1String("{server}"), str.isEmpty() ? QLatin1String("localhost") : str);
    if (accountSettings.accountId) {
        url.replace(QLatin1String("{extension}"), get_account_username());
        url.replace(QLatin1String("{password}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(accountSettings.account.password))));
        url.replace(QLatin1String("{md5_password}"), QString(onyx_md5sum(accountSettings.account.password)));
    }
    else {
        url.replace(QLatin1String("{extension}"), QString());
        url.replace(QLatin1String("{password}"), QString());
        url.replace(QLatin1String("{md5_password}"), QString());
    }
    int duration = 0;
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        duration = user_data->duration;
    }
    str = QString::number(duration);
    url.replace(QLatin1String("{duration}"), str);

    qint64 time = QDateTime::currentSecsSinceEpoch();
    str = QString::number(time);
    url.replace(QLatin1String("{time}"), str);

    url.replace(QLatin1String("{callid}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(
        call_info ? Onyx::PjToStr(&call_info->call_id, TRUE) : QString()
    ))));
    url.replace(QLatin1String("{direction}"),
        call_info ? (call_info->role ? QLatin1String("out") : QLatin1String("in")) : QString()
    );
    if (sipuri) {
        QString num = !sipuri->name.isEmpty() ? sipuri->name : sipuri->user;
        url.replace(QLatin1String("{callerid}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(num))));
        url.replace(QLatin1String("{user}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(sipuri->user))));
        url.replace(QLatin1String("{number}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(sipuri->user))));
        url.replace(QLatin1String("{domain}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(sipuri->domain))));
        url.replace(QLatin1String("{name}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(sipuri->name))));
    }
    return url;
}

QIcon LoadImageIcon(int i, int w, int h)
{
    // Qt equivalent – load from resources
    QString path = QString(":/icons/%1").arg(i);
    QIcon icon(path);
    if (w > 0 && h > 0) {
        return QIcon(icon.pixmap(w, h));
    }
    return icon;
}

void onyx_call_send_dtmf_info(pjsua_call_id current_call, pj_str_t digits)
{
    if (!is_pjsua_running()) {
        return;
    }
    if (current_call == -1) {
        PJ_LOG(3, (THIS_FILENAME, "No current call"));
    }
    else {
        const pj_str_t SIP_INFO = pj_str((char*)"INFO");
        pj_status_t status;
        for (int i = 0; i < digits.slen; ++i) {
            char body[80];
            pjsua_msg_data msg_data_;

            pjsua_msg_data_init(&msg_data_);
            msg_data_.content_type = pj_str((char*)"application/dtmf-relay");

            pj_ansi_snprintf(body, sizeof(body),
                "Signal=%c\r\n"
                "Duration=160",
                digits.ptr[i]);
            msg_data_.msg_body = pj_str(body);

            status = pjsua_call_send_request(current_call, &SIP_INFO, &msg_data_);
            if (status != PJ_SUCCESS) {
                return;
            }
        }
    }
}

void onyx_call_hangup_fast(pjsua_call_id call_id, pjsua_call_info* p_call_info)
{
    if (!is_pjsua_running()) {
        return;
    }
    pjsua_call_info call_info;
    if (!p_call_info) {
        if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS) {
            p_call_info = &call_info;
        }
    }
    if (!p_call_info) {
        return;
    }
    if (p_call_info->conf_slot != PJSUA_INVALID_ID) {
        pjsua_conf_disconnect(p_call_info->conf_slot, 0);
        pjsua_conf_disconnect(0, p_call_info->conf_slot);
    }
    if (pjsua_call_hangup(call_id, 0, NULL, NULL) == PJ_SUCCESS) {
        call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
        // mainDlg->messagesDlg->OnEndCall(p_call_info, user_data);  // keep external
        QWidget* main = qApp->activeWindow();
        if (main) {
            QMetaObject::invokeMethod(main, "onEndCall", Qt::QueuedConnection,
                                      Q_ARG(pjsua_call_info*, p_call_info),
                                      Q_ARG(call_user_data*, user_data));
        }
    }
}

void onyx_call_end(pjsua_call_id call_id)
{
    if (!is_pjsua_running()) {
        return;
    }
    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        user_data->hangup = true;
        if (user_data->inConference) {
            pjsua_call_info call_info;
            if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS && call_info.state == PJSIP_INV_STATE_CONFIRMED) {
                pjsua_call_id call_ids[PJSUA_MAX_CALLS];
                unsigned count = PJSUA_MAX_CALLS;
                if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                    for (unsigned i = 0; i < count; ++i) {
                        if (call_id == call_ids[i]) {
                            continue;
                        }
                        call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                        bool inConferenceCurr = false;
                        if (user_data_curr) {
                            QMutexLocker lock2(&user_data_curr->CS);
                            if (user_data_curr->inConference) {
                                inConferenceCurr = true;
                            }
                        }
                        if (inConferenceCurr) {
                            onyx_call_hangup_fast(call_ids[i]);
                        }
                    }
                }
            }
        }
    }
    onyx_call_hangup_fast(call_id);
}

void onyx_conference_join(pjsua_call_info* call_info)
{
    if (!is_pjsua_running()) {
        return;
    }
    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        if (user_data->inConference) {
            pjsua_call_id call_ids[PJSUA_MAX_CALLS];
            unsigned count = PJSUA_MAX_CALLS;
            if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                for (unsigned i = 0; i < count; ++i) {
                    if (call_info->id == call_ids[i]) {
                        continue;
                    }
                    if (!pjsua_call_has_media(call_ids[i])) {
                        continue;
                    }
                    call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                    bool inConferenceCurr = false;
                    bool isRecordingCurr = false;
                    if (user_data_curr) {
                        QMutexLocker lock2(&user_data_curr->CS);
                        if (user_data_curr->inConference) {
                            inConferenceCurr = true;
                        }
                        if (user_data_curr->recorder_id != PJSUA_INVALID_ID) {
                            isRecordingCurr = true;
                        }
                    }
                    if (inConferenceCurr) {
                        if (call_info->conf_slot != PJSUA_INVALID_ID) {
                            pjsua_conf_port_id conf_port_id = pjsua_call_get_conf_port(call_ids[i]);
                            if (conf_port_id != PJSUA_INVALID_ID) {
                                pjsua_conf_connect(call_info->conf_slot, conf_port_id);
                                pjsua_conf_connect(conf_port_id, call_info->conf_slot);
                            }
                        }
                        if (isRecordingCurr) {
                            onyx_call_recording_start(user_data, call_info);
                        }
                        else if (user_data->recorder_id != PJSUA_INVALID_ID) {
                            onyx_call_recording_start(user_data_curr);
                        }
                        QWidget* hWnd = qApp->activeWindow();
                        if (hWnd) {
                            QMetaObject::invokeMethod(hWnd, "onTabIconUpdate", Qt::QueuedConnection,
                                                      Q_ARG(int, call_ids[i]));
                        }
                    }
                }
            }
            QWidget* hWnd = qApp->activeWindow();
            if (hWnd) {
                QMetaObject::invokeMethod(hWnd, "onTabIconUpdate", Qt::QueuedConnection,
                                          Q_ARG(int, call_info->id));
            }
        }
    }
}

void onyx_conference_leave(pjsua_call_info* call_info, call_user_data* user_data, bool hold)
{
    if (!is_pjsua_running()) {
        return;
    }
    if (!user_data) {
        user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    }
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        if (user_data->inConference) {
            if (user_data->recorder_id != PJSUA_INVALID_ID) {
                onyx_call_recording_stop(user_data);
            }
            pjsua_call_id call_ids[PJSUA_MAX_CALLS];
            unsigned count = PJSUA_MAX_CALLS;
            if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                int qty = 0;
                call_user_data* last_conf_user_data = nullptr;
                for (unsigned i = 0; i < count; ++i) {
                    if (call_info->id == call_ids[i]) {
                        continue;
                    }
                    call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                    bool inConferenceCurr = false;
                    if (user_data_curr) {
                        QMutexLocker lock2(&user_data_curr->CS);
                        if (user_data_curr->inConference) {
                            inConferenceCurr = true;
                        }
                    }
                    if (inConferenceCurr) {
                        last_conf_user_data = user_data_curr;
                        qty++;
                        if (call_info->conf_slot != PJSUA_INVALID_ID) {
                            pjsua_conf_port_id conf_port_id = pjsua_call_get_conf_port(call_ids[i]);
                            if (conf_port_id != PJSUA_INVALID_ID) {
                                pjsua_conf_disconnect(call_info->conf_slot, conf_port_id);
                                pjsua_conf_disconnect(conf_port_id, call_info->conf_slot);
                            }
                        }
                        if (!hold) {
                            QWidget* hWnd = qApp->activeWindow();
                            if (hWnd) {
                                QMetaObject::invokeMethod(hWnd, "onTabIconUpdate", Qt::QueuedConnection,
                                                          Q_ARG(int, call_ids[i]));
                            }
                        }
                    }
                }
                if (qty == 1) {
                    if (!hold) {
                        QMutexLocker lock2(&last_conf_user_data->CS);
                        last_conf_user_data->inConference = false;
                        QWidget* hWnd = qApp->activeWindow();
                        if (hWnd) {
                            QMetaObject::invokeMethod(hWnd, "onTabIconUpdate", Qt::QueuedConnection,
                                                      Q_ARG(int, call_info->id));
                        }
                    }
                }
            }
            if (!hold) {
                user_data->inConference = false;
            }
        }
    }
}

void onyx_call_hold(pjsua_call_info* call_info)
{
    if (!is_pjsua_running()) {
        return;
    }
    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        if (user_data->inConference) {
            pjsua_call_id call_ids[PJSUA_MAX_CALLS];
            unsigned count = PJSUA_MAX_CALLS;
            if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                for (unsigned i = 0; i < count; ++i) {
                    if (call_ids[i] != call_info->id) {
                        call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                        bool inConferenceCurr = false;
                        if (user_data_curr) {
                            QMutexLocker lock2(&user_data_curr->CS);
                            if (user_data_curr->inConference) {
                                inConferenceCurr = true;
                            }
                        }
                        if (inConferenceCurr) {
                            pjsua_call_info call_info_curr;
                            pjsua_call_get_info(call_ids[i], &call_info_curr);
                            if (call_info_curr.state == PJSIP_INV_STATE_CONFIRMED) {
                                if (call_info_curr.media_status != PJSUA_CALL_MEDIA_LOCAL_HOLD && call_info_curr.media_status != PJSUA_CALL_MEDIA_NONE) {
                                    pjsua_call_set_hold(call_info_curr.id, NULL);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (call_info->state == PJSIP_INV_STATE_CONFIRMED) {
        if (call_info->media_status != PJSUA_CALL_MEDIA_LOCAL_HOLD && call_info->media_status != PJSUA_CALL_MEDIA_NONE) {
            pjsua_call_set_hold(call_info->id, NULL);
        }
    }
}

void onyx_call_unhold(pjsua_call_info* call_info)
{
    if (!is_pjsua_running()) {
        return;
    }
    call_user_data* user_data = nullptr;
    if (call_info) {
        user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    }
    bool inConference = false;
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        if (user_data->inConference) {
            inConference = true;
        }
    }
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned count = PJSUA_MAX_CALLS;
    if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            if (!call_info || call_ids[i] != call_info->id) {
                pjsua_call_info call_info_curr;
                pjsua_call_get_info(call_ids[i], &call_info_curr);
                if (call_info_curr.state == PJSIP_INV_STATE_CONFIRMED) {
                    call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                    bool inConferenceCurr = false;
                    if (user_data_curr) {
                        QMutexLocker lock2(&user_data_curr->CS);
                        if (user_data_curr->inConference) {
                            inConferenceCurr = true;
                        }
                    }
                    if (inConference && inConferenceCurr) {
                        if (call_info_curr.media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD || call_info_curr.media_status == PJSUA_CALL_MEDIA_NONE) {
                            pjsua_call_reinvite(call_ids[i], PJSUA_CALL_UNHOLD, NULL);
                        }
                    }
                    else {
                        if (call_info_curr.media_status != PJSUA_CALL_MEDIA_LOCAL_HOLD && call_info_curr.media_status != PJSUA_CALL_MEDIA_NONE) {
                            if (accountSettings.singleMode || !accountSettings.AC) {
                                pjsua_call_set_hold(call_ids[i], NULL);
                            }
                        }
                    }
                }
            }
        }
    }
    if (call_info && call_info->state == PJSIP_INV_STATE_CONFIRMED) {
        if (call_info->media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD || call_info->media_status == PJSUA_CALL_MEDIA_NONE) {
            pjsua_call_reinvite(call_info->id, PJSUA_CALL_UNHOLD, NULL);
        }
    }
}

bool onyx_call_answer(pjsua_call_id call_id)
{
    if (!is_pjsua_running()) {
        return false;
    }
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned calls_count = PJSUA_MAX_CALLS;
    if (pjsua_enum_calls(call_ids, &calls_count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < calls_count; ++i) {
            pjsua_call_info call_info;
            if (pjsua_call_get_info(call_ids[i], &call_info) == PJ_SUCCESS) {
                if (call_info.role == PJSIP_ROLE_UAS && (call_info.state == PJSIP_INV_STATE_INCOMING || call_info.state == PJSIP_INV_STATE_EARLY)) {
                    QWidget* hWnd = qApp->activeWindow();
                    if (hWnd) {
                        QMetaObject::invokeMethod(hWnd, "onCallAnswer", Qt::QueuedConnection,
                                                  Q_ARG(int, call_ids[i]));
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return false;
}

void onyx_call_busy(pjsua_call_id call_id, QString reason)
{
    if (!reason.isEmpty()) {
        pj_str_t pj_reason = Onyx::StrToPjStr(reason);
        pjsua_call_hangup(call_id, 486, &pj_reason, NULL);
        free(pj_reason.ptr);
    }
    else {
        pjsua_call_hangup(call_id, 486, NULL, NULL);
    }
}

void onyx_call_recording_start(call_user_data* user_data, pjsua_call_info* call_info, int id)
{
    if (!is_pjsua_running()) {
        return;
    }
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        pjsua_recorder_id* recorder_id = &user_data->recorder_id;
        if (*recorder_id == PJSUA_INVALID_ID) {
            pjsua_call_info call_info_loc;
            if (!call_info) {
                if (pjsua_call_get_info(user_data->call_id, &call_info_loc) == PJ_SUCCESS) {
                    call_info = &call_info_loc;
                }
            }
            if (call_info && call_info->conf_slot != PJSUA_INVALID_ID) {
                if (user_data->inConference) {
                    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
                    unsigned count = PJSUA_MAX_CALLS;
                    if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                        for (unsigned i = 0; i < count; ++i) {
                            if (call_info->id == call_ids[i]) {
                                continue;
                            }
                            call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                            if (user_data_curr) {
                                QMutexLocker lock2(&user_data_curr->CS);
                                if (user_data_curr->inConference && user_data_curr->recorder_id != PJSUA_INVALID_ID) {
                                    pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(user_data_curr->recorder_id);
                                    pjsua_conf_connect(call_info->conf_slot, rec_conf_port_id);
                                    *recorder_id = user_data_curr->recorder_id;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (*recorder_id == PJSUA_INVALID_ID) {
                    QString filename;

                    SIPURI remoteURI;
                    ParseCallSIPURI(call_info, user_data, &remoteURI);

                    QDateTime tm = QDateTime::currentDateTime();
                    QString recordingPath = accountSettings.recordingPath;
                    if (!recordingPath.isEmpty() && !recordingPath.endsWith('/') && !recordingPath.endsWith('\\')) {
                        recordingPath.append('/');
                    }
                    SIPURI localURI;
                    Onyx::ParseSIPURI(Onyx::PjToStr(&call_info->local_info, TRUE), &localURI);
                    filename = QString("%1-%2-%3-%4")
                        .arg(tm.toString("yyyyMMdd-HHmmss"))
                        .arg(remoteURI.user)
                        .arg(call_info->role == PJSIP_ROLE_UAC ? "outgoing" : "incoming")
                        .arg(accountSettings.accountId && !accountSettings.account.label.isEmpty()
                             ? accountSettings.account.label : localURI.user);

                    if (!recordingPath.isEmpty()) {
                        QDir().mkpath(recordingPath);
                    }
                    const char spec[] = { '/','\\', '?', '%', '*', ':', '|', '"', '<', '>', '.', ' ' };
                    for (int i = 0; i < (int)sizeof(spec); ++i) {
                        filename.replace(spec[i], '_');
                    }
                    filename = recordingPath + filename;
                    if (accountSettings.recordingFormat == QLatin1String("wav")) {
                        filename.append(".wav");
                    }
                    else {
                        filename.append(".mp3");
                    }
                    char* buf = Onyx::WideCharToPjStr(filename);
                    pj_str_t filenameStr = pj_str(buf);
                    if (pjsua_recorder_create(&filenameStr, 0, NULL, -1, 0, recorder_id) == PJ_SUCCESS) {
                        pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(*recorder_id);
                        pjsua_conf_connect(call_info->conf_slot, rec_conf_port_id);
                        pjsua_conf_connect(0, rec_conf_port_id);
                        if (user_data->inConference) {
                            pjsua_call_id call_ids[PJSUA_MAX_CALLS];
                            unsigned count = PJSUA_MAX_CALLS;
                            if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                                for (unsigned i = 0; i < count; ++i) {
                                    if (call_info->id == call_ids[i]) {
                                        continue;
                                    }
                                    call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                                    if (user_data_curr) {
                                        QMutexLocker lock2(&user_data_curr->CS);
                                        if (user_data_curr->inConference && user_data_curr->recorder_id == PJSUA_INVALID_ID) {
                                            pjsua_call_info call_info_curr;
                                            pjsua_call_get_info(call_ids[i], &call_info_curr);
                                            if (call_info_curr.conf_slot != PJSUA_INVALID_ID) {
                                                pjsua_conf_connect(call_info_curr.conf_slot, rec_conf_port_id);
                                                user_data_curr->recorder_id = *recorder_id;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    free(buf);
                }
            }
        }
    }
}

void onyx_call_recording_stop(call_user_data* user_data, int id, bool force)
{
    if (user_data) {
        QMutexLocker lock(&user_data->CS);
        pjsua_recorder_id* recorder_id = &user_data->recorder_id;
        if (*recorder_id != PJSUA_INVALID_ID) {
            if (is_pjsua_running()) {
                bool block = false;
                pjsua_call_id call_ids[PJSUA_MAX_CALLS];
                unsigned count = PJSUA_MAX_CALLS;
                if (pjsua_enum_calls(call_ids, &count) == PJ_SUCCESS) {
                    for (unsigned i = 0; i < count; ++i) {
                        if (user_data->call_id == call_ids[i]) {
                            continue;
                        }
                        call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_ids[i]);
                        if (user_data_curr) {
                            QMutexLocker lock2(&user_data_curr->CS);
                            if (user_data_curr->recorder_id == *recorder_id) {
                                if (force) {
                                    pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(user_data_curr->recorder_id);
                                    pjsua_call_info call_info_curr;
                                    pjsua_call_get_info(call_ids[i], &call_info_curr);
                                    if (call_info_curr.conf_slot != PJSUA_INVALID_ID) {
                                        pjsua_conf_disconnect(call_info_curr.conf_slot, rec_conf_port_id);
                                    }
                                    user_data_curr->recorder_id = PJSUA_INVALID_ID;
                                }
                                else {
                                    block = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(*recorder_id);
                pjsua_call_info call_info;
                pjsua_call_get_info(user_data->call_id, &call_info);
                if (call_info.conf_slot != PJSUA_INVALID_ID) {
                    pjsua_conf_disconnect(call_info.conf_slot, rec_conf_port_id);
                }
                if (!block) {
                    pjsua_recorder_destroy(*recorder_id);
                }
            }
            *recorder_id = PJSUA_INVALID_ID;
        }
    }
}

QString onyx_url_mask(QString url)
{
    qint64 time = QDateTime::currentSecsSinceEpoch();
    QString str = QString::number(time);
    url.replace(QLatin1String("{time}"), str);
    url.replace(QLatin1String("{version}"), QStringLiteral(_GLOBAL_VERSION));
    if (accountSettings.accountId) {
        url.replace(QLatin1String("{label}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(accountSettings.account.label))));
        url.replace(QLatin1String("{server}"), Onyx::RemovePort(get_account_server()));
        url.replace(QLatin1String("{domain}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(accountSettings.account.domain))));
        url.replace(QLatin1String("{username}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(accountSettings.account.username))));
        url.replace(QLatin1String("{password}"), QString::fromUtf8(urlencode(Onyx::Utf8EncodeUni(accountSettings.account.password))));
        url.replace(QLatin1String("{md5_password}"), QString(onyx_md5sum(accountSettings.account.password)));
    }
    else {
        url.replace(QLatin1String("{label}"), QString());
        url.replace(QLatin1String("{server}"), QLatin1String("localhost"));
        url.replace(QLatin1String("{domain}"), QString());
        url.replace(QLatin1String("{username}"), QString());
        url.replace(QLatin1String("{password}"), QString());
        url.replace(QLatin1String("{md5_password}"), QString());
    }
    return url;
}

void onyx_startup_set(bool enable)
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (enable) {
        QString str = QString("\"%1\" /minimized").arg(accountSettings.exeFile);
        settings.setValue(QStringLiteral(_GLOBAL_NAME_NICE), str);
    }
    else {
        settings.remove(QStringLiteral(_GLOBAL_NAME_NICE));
    }
}
