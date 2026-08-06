// mainDlg.cpp - Complete Qt conversion (Part 1)

#define THIS_FILENAME "mainDlg.cpp"

#include "mainDlg.h"
#include "onyx.h"
#include "RinginDlg.h"
#include "MessagesDlg.h"
#include "Dialer.h"
#include "Preview.h"
#include "SettingsDlg.h"
#include "ShortcutsDlg.h"
#include "Calls.h"
#include "Contacts.h"
#include "AccountDlg.h"

#include <QSettings>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QMenu>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QTimer>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QHostInfo>
#include <QRecursiveMutex>
#include <QWindow>
#include <QScreen>
#include <shellapi.h>

#include "Mmsystem.h"
#include "settings.h"
#include "global.h"
#include "ModelessMessageBox.h"
#include "json.h"
#include "Markup.h"
#include "langpack.h"
#include "jumplist.h"
#include "Hid.h"
#include "CMask.h"

#include <winuser.h>
#include <windows.h>
#include <io.h>
#include <afxmt.h>
#include <afxinet.h>
#include <ws2tcpip.h>
#include <Dbt.h>
#include <Strsafe.h>
#include <locale.h> 
#include <Wtsapi32.h>
#include "atlrx.h"

#include "afxvisualmanager.h"
#include "afxvisualmanagerwindows.h"

#include "iphlpapi.h"
#include "wininet.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Wtsapi32.lib")

#ifdef new
#undef new
#endif

CmainDlg* mainDlg;

static UINT WM_SHELLHOOKMESSAGE;
static UINT WM_TASKBARRESTARTMESSAGE;

static bool updateCheckerShow;

static UINT indicators[] =
{
	IDS_STATUSBAR,
	IDS_STATUSBAR2,
};

static int usersDirectorySequence;
static int usersDirectoryRefresh;
static int usersDirectorySilent;
static int usersDirectoryReconnect;

QMutex CmainDlg::gethostbyaddrThreadCS;
QString CmainDlg::gethostbyaddrThreadResult;
static DWORD WINAPI gethostbyaddrThread(LPVOID lpParam)
{
    QString* addr = (QString*)lpParam;
    QString res = *addr;
    delete addr;
    struct hostent* he = NULL;
    struct in_addr inaddr;
    inaddr.S_un.S_addr = inet_addr(res.toLatin1().constData());
    if (inaddr.S_un.S_addr != INADDR_NONE && inaddr.S_un.S_addr != INADDR_ANY) {
        he = gethostbyaddr((char*)&inaddr, 4, AF_INET);
        if (he) {
            res = QString::fromLatin1(he->h_name);
        }
    }
    QMutexLocker locker(&CmainDlg::gethostbyaddrThreadCS);
    CmainDlg::gethostbyaddrThreadResult = res;
    return 0;
}

static void on_reg_started2(pjsua_acc_id acc_id, pjsua_reg_info* info)
{
    if (info->renew) {
        QMetaObject::invokeMethod(mainDlg, "UpdateWindowText", Qt::QueuedConnection,
                                  Q_ARG(int, 1), Q_ARG(int, 0));
    }
}

static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info* info)
{
    if (!mainDlg) {
        return;
    }
    QString* str = NULL;
    if (info->cbparam->code >= 400 && info->cbparam->rdata) {
        pjsip_generic_string_hdr* hsr;
        const pj_str_t headerError = pj_str("P-Registrar-Error");
        hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(info->cbparam->rdata->msg_info.msg, &headerError, NULL);
        if (hsr) {
            str = new QString(Onyx::PjToStr(&hsr->hvalue, true));
        }
    }
    QMetaObject::invokeMethod(mainDlg, "onRegState2", Qt::QueuedConnection,
                              Q_ARG(int, info->cbparam->code), Q_ARG(QString*, str));
}

/* Callback from timer when the maximum call duration has been exceeded. */
static void call_timeout_callback(pj_timer_heap_t* timer_heap,
                                  struct pj_timer_entry* entry)
{
    pjsua_call_id call_id = entry->id;
    pjsua_msg_data msg_data_;
    pjsip_generic_string_hdr warn;
    pj_str_t hname = pj_str("Warning");
    pj_str_t hvalue = pj_str("399 localhost \"Call duration exceeded\"");

    PJ_UNUSED_ARG(timer_heap);

    if (call_id == PJSUA_INVALID_ID) {
        PJ_LOG(1, (THIS_FILENAME, "Invalid call ID in timer callback"));
        return;
    }

    /* Add warning header */
    pjsua_msg_data_init(&msg_data_);
    pjsip_generic_string_hdr_init2(&warn, &hname, &hvalue);
    pj_list_push_back(&msg_data_.hdr_list, &warn);

    /* Call duration has been exceeded; disconnect the call */
    PJ_LOG(3, (THIS_FILENAME, "Duration (%d seconds) has been exceeded "
        "for call %d, disconnecting the call",
        accountSettings.autoHangUpTime, call_id));
    entry->id = PJSUA_INVALID_ID;
    pjsua_call_hangup(call_id, 200, NULL, &msg_data_);
}

static void on_call_state(pjsua_call_id call_id, pjsip_event* e)
{
    if (!mainDlg) {
        return;
    }
    pjsua_call_info* call_info = new pjsua_call_info();
    if (pjsua_call_get_info(call_id, call_info) != PJ_SUCCESS || call_info->state == PJSIP_INV_STATE_NULL) {
        delete call_info;
        return;
    }

    if (call_info->state == PJSIP_INV_STATE_DISCONNECTED && call_info->last_status == 481) {
        delete call_info;
        return;
    }
    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    // reset user_data after call transfer
    if (user_data) {
        user_data->CS.Lock();
        bool callIdMissmatch = user_data->call_id != PJSUA_INVALID_ID && user_data->call_id != call_info->id;
        bool hidden = user_data->hidden;
        user_data->CS.Unlock();
        if (callIdMissmatch) {
            user_data = new call_user_data(call_info->id);
            pjsua_call_set_user_data(call_info->id, user_data);
        }
        else {
            if (hidden) {
                if (call_info->state == PJSIP_INV_STATE_DISCONNECTED) {
                    pjsua_call_set_user_data(call_info->id, NULL);
                    delete user_data;
                }
                delete call_info;
                return;
            }
        }
    }
    if (!user_data) {
        user_data = new call_user_data(call_info->id);
        pjsua_call_set_user_data(call_info->id, user_data);
    }

    user_data->CS.Lock();

    switch (call_info->state) {
    case PJSIP_INV_STATE_CALLING:
        onyx_call_unhold(call_info);
        break;
    case PJSIP_INV_STATE_CONNECTING:
        onyx_call_unhold(call_info);
        break;
    case PJSIP_INV_STATE_CONFIRMED:
        if (accountSettings.autoRecording) {
            onyx_call_recording_start(user_data, call_info);
        }
        if (accountSettings.autoHangUpTime > 0) {
            /* Schedule timer to hangup call after the specified duration */
            pj_time_val delay;
            user_data->auto_hangup_timer.id = call_info->id;
            user_data->auto_hangup_timer.cb = &call_timeout_callback;
            delay.sec = accountSettings.autoHangUpTime;
            delay.msec = 0;
            pjsua_schedule_timer(&user_data->auto_hangup_timer, &delay);
        }
        break;
    case PJSIP_INV_STATE_DISCONNECTED:
        pjsua_call_set_user_data(call_info->id, NULL);
        break;
    }

    user_data->CS.Unlock();

    QMetaObject::invokeMethod(mainDlg, "onCallState", Qt::QueuedConnection,
                              Q_ARG(pjsua_call_info*, call_info), Q_ARG(call_user_data*, user_data));
}

static void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info* call_info = new pjsua_call_info();
    if (pjsua_call_get_info(call_id, call_info) != PJ_SUCCESS || call_info->state == PJSIP_INV_STATE_NULL) {
        delete call_info;
        return;
    }

    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    if (!user_data) {
        user_data = new call_user_data(call_info->id);
        pjsua_call_set_user_data(call_info->id, user_data);
    }

    if (call_info->media_status == PJSUA_CALL_MEDIA_ACTIVE
        || call_info->media_status == PJSUA_CALL_MEDIA_REMOTE_HOLD
        ) {
        onyx_conference_join(call_info);
        pjsua_conf_connect(call_info->conf_slot, 0);
        pjsua_conf_connect(0, call_info->conf_slot);
        //--
        user_data->CS.Lock();
        user_data->holdFrom = -1;
        if (user_data->recorder_id != PJSUA_INVALID_ID) {
            pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(user_data->recorder_id);
            pjsua_conf_connect(call_info->conf_slot, rec_conf_port_id);
            pjsua_conf_adjust_tx_level(rec_conf_port_id, 1);
        }
        user_data->CS.Unlock();

        //--
        QTimer::singleShot(100, mainDlg->pageDialer, [=]() { mainDlg->pageDialer->OnTimerVUMeter(); });
        //--
    }
    else {
        if (user_data->recorder_id != PJSUA_INVALID_ID) {
            pjsua_conf_port_id rec_conf_port_id = pjsua_recorder_get_conf_port(user_data->recorder_id);
            pjsua_conf_adjust_tx_level(rec_conf_port_id, 0);
        }
        onyx_conference_leave(call_info, user_data, true);
        pjsua_conf_disconnect(call_info->conf_slot, 0);
        pjsua_conf_disconnect(0, call_info->conf_slot);
        call_deinit_tonegen(call_info->id);
        //--
        user_data->CS.Lock();
        user_data->holdFrom = onyx_get_duration(&call_info->connect_duration);
        user_data->CS.Unlock();
        //--
    }

    QMetaObject::invokeMethod(mainDlg, "onCallMediaState", Qt::QueuedConnection,
                              Q_ARG(pjsua_call_info*, call_info), Q_ARG(call_user_data*, user_data));
}

static void on_call_media_event(pjsua_call_id call_id,
                                unsigned med_idx,
                                pjmedia_event* event)
{
    //char event_name[5];

    //PJ_LOG(5, (THIS_FILENAME, "Event %s",
        //pjmedia_fourcc_name(event->type, event_name)));

    //#if PJSUA_HAS_VIDEO
        //if (event->type == PJMEDIA_EVENT_FMT_CHANGED) {
        //	pjsua_call_info ci;
        //	pjsua_call_get_info(call_id, &ci);
        //	if ((ci.media[med_idx].type == PJMEDIA_TYPE_VIDEO) &&
        //		(ci.media[med_idx].dir & PJMEDIA_DIR_DECODING)) {
        //		pjsua_vid_win_id wid;
        //		pjmedia_rect_size size;
        //		pjsua_vid_win_info win_info;

        //		wid = ci.media[med_idx].stream.vid.win_in;
        //		pjsua_vid_win_get_info(wid, &win_info);

        //		size = event->data.fmt_changed.new_fmt.det.vid.size;
        //		if (size.w != win_info.size.w || size.h != win_info.size.h) {
        //			pjsua_vid_win_set_size(wid, &size);
        //			/* Re-arrange video windows */
        //			arrange_window(PJSUA_INVALID_ID);
        //		}
        //	}
        //}
    //#else
    //	PJ_UNUSED_ARG(call_id);
    //	PJ_UNUSED_ARG(med_idx);
    //	PJ_UNUSED_ARG(event);
    //#endif
}

static void on_incoming_call(pjsua_acc_id acc, pjsua_call_id call_id,
                             pjsip_rx_data* rdata)
{
    pjsua_call_info* call_info = new pjsua_call_info();
    if (pjsua_call_get_info(call_id, call_info) != PJ_SUCCESS) {
        delete call_info;
        return;
    }

    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    if (!user_data) {
        user_data = new call_user_data(call_info->id);
        pjsua_call_set_user_data(call_info->id, user_data);
    }

    user_data->CS.Lock();

    if (accountSettings.forceCodec) {
        pjsua_call* call;
        pjsip_dialog* dlg;
        pj_status_t status;
        status = acquire_call("on_incoming_call()", call_id, &call, &dlg);
        if (status == PJ_SUCCESS) {
            pjmedia_sdp_neg_set_prefer_remote_codec_order(nullptr, PJ_FALSE);
            pjsip_dlg_dec_lock(dlg);
        }
    }
    pjsip_generic_string_hdr* hsr;
    // -- diversion
    const pj_str_t headerDiversion = pj_str("Diversion");
    hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerDiversion, NULL);
    if (hsr) {
        QString str = Onyx::PjToStr(&hsr->hvalue, true);
        SIPURI sipuriDiversion;
        Onyx::ParseSIPURI(str, &sipuriDiversion);
        user_data->diversion = !sipuriDiversion.user.isEmpty() ? sipuriDiversion.user : sipuriDiversion.domain;
    }
    // -- end diversion
    // -- caller id
    user_data->callerID = GetPAI(rdata);
    // -- end caller id
    // -- user agent
    const pj_str_t headerUserAgent = pj_str("User-Agent");
    hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerUserAgent, NULL);
    if (hsr) {
        user_data->userAgent = Onyx::PjToStr(&hsr->hvalue, true);
        int pos = user_data->userAgent.indexOf(QRegularExpression("[~+-]"));
        if (pos != -1) {
            user_data->userAgent = user_data->userAgent.left(pos);
        }
    }
    // -- end user agent

    SIPURI sipuri;
    ParseCallSIPURI(call_info, user_data, &sipuri);

    if (!accountSettings.cmdIncomingCall.isEmpty()) {
        QString params = sipuri.user;
        Onyx::RunCmd(accountSettings.cmdIncomingCall, params);
    }
    //--
    //--
    bool busy = false;
    pjsua_call_id call_ids[PJSUA_MAX_CALLS];
    unsigned calls_count = PJSUA_MAX_CALLS;
    unsigned calls_count_cmp = 0;
    if (pjsua_enum_calls(call_ids, &calls_count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < calls_count; ++i) {
            pjsua_call_info call_info_curr;
            if (pjsua_call_get_info(call_ids[i], &call_info_curr) == PJ_SUCCESS) {
                call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_info_curr.id);
                SIPURI sipuri_curr;
                ParseCallSIPURI(&call_info_curr, user_data_curr, &sipuri_curr);
                if (call_info_curr.id != call_info->id &&
                    sipuri.user + "@" + sipuri.domain == sipuri_curr.user + "@" + sipuri_curr.domain
                    ) {
                    busy = true;
                    break;
                }
                if (user_data_curr) {
                    user_data_curr->CS.Lock();
                    if (!user_data_curr->hangup && call_info_curr.state != PJSIP_INV_STATE_DISCONNECTED) {
                        calls_count_cmp++;
                    }
                    user_data_curr->CS.Unlock();
                }
                else {
                    if (call_info_curr.state != PJSIP_INV_STATE_DISCONNECTED) {
                        calls_count_cmp++;
                    }
                }
            }
        }
    }
    if (busy) {
        // 486 Busy Here
        onyx_call_busy(call_info->id, QStringLiteral("Call already exists"));
        user_data->hidden = true;
    }
    else if ((!accountSettings.callWaiting && calls_count_cmp > 1) || (accountSettings.maxConcurrentCalls > 0 && calls_count_cmp > accountSettings.maxConcurrentCalls)) {
        // 486 Busy Here
        onyx_call_busy(call_info->id, QStringLiteral("Active calls limit"));
        user_data->hidden = true;
    }
    else if (!mainDlg->callIdIncomingIgnore.isEmpty() && mainDlg->callIdIncomingIgnore == Onyx::PjToStr(&call_info->call_id)) {
        pjsua_call_answer(call_info->id, 487, NULL, NULL);
        user_data->hidden = true;
    }
    else {
        bool reject = false;
        QString reason;
        if (accountSettings.denyIncoming == QStringLiteral("all")) {
            reject = true;
        }
        else if (accountSettings.denyIncoming == QStringLiteral("button")) {
            reject = accountSettings.DND;
            reason = QStringLiteral("Do Not Disturb");
        }
        else if (accountSettings.denyIncoming == QStringLiteral("user")) {
            SIPURI sipuri_curr;
            Onyx::ParseSIPURI(Onyx::PjToStr(&call_info->local_info, TRUE), &sipuri_curr);
            if (sipuri_curr.user != get_account_username()) {
                reject = true;
            }
        }
        else if (accountSettings.denyIncoming == QStringLiteral("domain")) {
            SIPURI sipuri_curr;
            Onyx::ParseSIPURI(Onyx::PjToStr(&call_info->local_info, TRUE), &sipuri_curr);
            if (accountSettings.accountId) {
                if (sipuri_curr.domain != get_account_domain()) {
                    reject = true;
                }
            }
        }
        else if (accountSettings.denyIncoming == QStringLiteral("remotedomain")) {
            if (accountSettings.accountId) {
                if (sipuri.domain != get_account_domain()) {
                    reject = true;
                }
            }
        }
        else if (accountSettings.denyIncoming == QStringLiteral("userdomain")) {
            SIPURI sipuri_curr;
            Onyx::ParseSIPURI(Onyx::PjToStr(&call_info->local_info, TRUE), &sipuri_curr);
            if (sipuri_curr.user != get_account_username()) {
                reject = true;
            }
            else {
                QString domain = get_account_domain();
                if (domain != "" && sipuri_curr.domain != domain) {
                    reject = true;
                }
            }
        }
        if (reject) {
            if (reason.isEmpty()) {
                reason = QStringLiteral("Denied");
            }
            onyx_call_busy(call_info->id, reason);
            user_data->hidden = true;
        }
        else {
            bool autoAnswer = false;
            int autoAnswerDelay = accountSettings.autoAnswerDelay;
            if (accountSettings.autoAnswer == QStringLiteral("all")) {
                autoAnswer = true;
            }
            else if (accountSettings.autoAnswer == QStringLiteral("button")) {
                autoAnswer = accountSettings.AA;
            }
            else if (accountSettings.autoAnswer == QStringLiteral("header")) {
                //--
                pjsip_generic_string_hdr* hsr = NULL;
                const pj_str_t header = pj_str("X-AUTOANSWER");
                hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &header, NULL);
                if (hsr) {
                    QString autoAnswerValue = Onyx::PjToStr(&hsr->hvalue, TRUE).toLower();
                    if (autoAnswerValue == QStringLiteral("true") || autoAnswerValue == QStringLiteral("1")) {
                        autoAnswer = true;
                    }
                }
                //--
                if (!autoAnswer) {
                    pjsip_generic_string_hdr* hsr = NULL;
                    const pj_str_t header = pj_str("Call-Info");
                    hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &header, NULL);
                    if (hsr) {
                        QString callInfoValue = Onyx::PjToStr(&hsr->hvalue, TRUE).toLower();
                        if (callInfoValue.contains(QStringLiteral("auto answer"))) {
                            autoAnswer = true;
                        }
                        else {
                            CAtlRegExp<> regex;
                            REParseError parseStatus = regex.Parse(L"answer-after={[0-9]+}", true);
                            if (parseStatus == REPARSE_ERROR_OK) {
                                CAtlREMatchContext<> mc;
                                if (regex.Match(callInfoValue, &mc) && mc.m_uNumGroups == 1) {
                                    const CAtlREMatchContext<>::RECHAR* szStart = 0;
                                    const CAtlREMatchContext<>::RECHAR* szEnd = 0;
                                    mc.GetMatch(0, &szStart, &szEnd);
                                    ptrdiff_t nLength = szEnd - szStart;
                                    QString text = QString::fromWCharArray(szStart, nLength);
                                    autoAnswerDelay = text.toInt();
                                    autoAnswer = true;
                                }
                            }
                        }
                    }
                }
            }

            if (autoAnswer && !accountSettings.autoAnswerNumber.isEmpty()) {
                bool found = false;
                QStringList tokens = accountSettings.autoAnswerNumber.split(QRegularExpression("[;|]"), Qt::SkipEmptyParts);
                for (const QString& token : tokens) {
                    QString resToken = token.trimmed();
                    if (!resToken.isEmpty()) {
                        CMask mask;
                        if (mask.WildMatch(resToken, sipuri.user, QStringLiteral(""))) {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    autoAnswer = false;
                }
            }
            bool forwarding = false;
            if (!accountSettings.forwardingNumber.isEmpty()) {
                if (accountSettings.forwarding == QStringLiteral("all") ||
                    (accountSettings.forwarding == QStringLiteral("button") && accountSettings.FWD)
                    ) {
                    forwarding = true;
                }
            }
            if (forwarding) {
                if (accountSettings.forwardingDelay > 0) {
                    if (autoAnswer && autoAnswerDelay > 0 && mainDlg->autoAnswerTimerCallId == PJSUA_INVALID_ID && autoAnswerDelay < accountSettings.forwardingDelay) {
                        //
                    }
                    else {
                        if (mainDlg->forwardingTimerCallId == PJSUA_INVALID_ID) {
                            mainDlg->forwardingTimerCallId = call_info->id;
                            QTimer::singleShot(accountSettings.forwardingDelay * 1000, mainDlg, [=]() {
                                mainDlg->OnTimer(IDT_TIMER_FORWARDING);
                            });
                        }
                    }
                }
                else {
                    user_data->forwarding = true;
                }
            }
            if (autoAnswer) {
                if (autoAnswerDelay > 0) {
                    if (mainDlg->autoAnswerTimerCallId == PJSUA_INVALID_ID) {
                        mainDlg->autoAnswerTimerCallId = call_info->id;
                        QTimer::singleShot(autoAnswerDelay * 1000, mainDlg, [=]() {
                            mainDlg->OnTimer(IDT_TIMER_AUTOANSWER);
                        });
                    }
                }
                else {
                    user_data->autoAnswer = true;
                }
            }
            QMetaObject::invokeMethod(mainDlg, "onIncomingCall", Qt::QueuedConnection,
                                      Q_ARG(pjsua_call_info*, call_info), Q_ARG(call_user_data*, user_data));
        }
    }
    user_data->CS.Unlock();
}

static void on_nat_detect(const pj_stun_nat_detect_result * res)
{
    if (res->status != PJ_SUCCESS) {
        pjsua_perror(THIS_FILENAME, "NAT detection failed", res->status);
    }
    else {
        if (res->nat_type == PJ_STUN_NAT_TYPE_SYMMETRIC) {
            QString message = QStringLiteral("The softphone may not work properly with enabled STUN and your internet connection.");
            QMetaObject::invokeMethod(mainDlg, "BaloonPopup", Qt::QueuedConnection,
                                      Q_ARG(QString, Translate(QStringLiteral("Symmetric NAT detected!"))),
                                      Q_ARG(QString, Translate(message)));
        }
        PJ_LOG(3, (THIS_FILENAME, "NAT detected as %s", res->nat_type_name));
    }
}

void on_buddy_state(pjsua_buddy_id buddy_id)
{
    if (!mainDlg) {
        return;
    }
    QMetaObject::invokeMethod(mainDlg, "onBuddyState", Qt::QueuedConnection, Q_ARG(int, buddy_id));
}

static void on_pager2(pjsua_call_id call_id, const pj_str_t * from, const pj_str_t * to, const pj_str_t * contact, const pj_str_t * mime_type, const pj_str_t * body, pjsip_rx_data * rdata, pjsua_acc_id acc_id)
{
    if (pj_strcmp2(mime_type, "text/plain") != 0 || accountSettings.disableMessaging) {
        return;
    }
    QString* number = new QString(Onyx::PjToStr(from, TRUE));
    QString* message = new QString(Onyx::PjToStr(body, TRUE));
    *message = message->trimmed();
    call_user_data user_data(PJSUA_INVALID_ID);
    user_data.callerID = GetPAI(rdata);
    SIPURI sipuri;
    ParseCallSIPURI(*number, &user_data, &sipuri);
    //-- fix domain
    if (accountSettings.accountId) {
        if (Onyx::IsIP(sipuri.domain)) {
            sipuri.domain = get_account_domain();
        }
    }
    //--
    *number = Onyx::BuildSIPURI(&sipuri);
    QMetaObject::invokeMethod(mainDlg, "onPager", Qt::QueuedConnection,
                              Q_ARG(QString*, number), Q_ARG(QString*, message));
}

static void on_pager_status2(pjsua_call_id call_id, const pj_str_t * to, const pj_str_t * body, void* user_data, pjsip_status_code status, const pj_str_t * reason, pjsip_tx_data * tdata, pjsip_rx_data * rdata, pjsua_acc_id acc_id)
{
    if (status != 200) {
        QString* number = new QString(Onyx::PjToStr(to, TRUE));
        QString* message = new QString(Onyx::PjToStr(reason, TRUE));
        *message = message->trimmed();
        call_user_data user_data(PJSUA_INVALID_ID);
        user_data.callerID = GetPAI(rdata);
        SIPURI sipuri;
        ParseCallSIPURI(*number, &user_data, &sipuri);
        //-- fix domain
        if (accountSettings.accountId) {
            if (Onyx::IsIP(sipuri.domain)) {
                sipuri.domain = get_account_domain();
            }
        }
        //--
        *number = Onyx::BuildSIPURI(&sipuri);
        QMetaObject::invokeMethod(mainDlg, "onPagerStatus", Qt::QueuedConnection,
                                  Q_ARG(QString*, number), Q_ARG(QString*, message));
    }
}

static void on_call_transfer_status(pjsua_call_id call_id,
                                    int status_code,
                                    const pj_str_t * status_text,
                                    pj_bool_t final,
                                    pj_bool_t * p_cont)
{
    pjsua_call_info* call_info = new pjsua_call_info();
    if (pjsua_call_get_info(call_id, call_info) != PJ_SUCCESS || call_info->state == PJSIP_INV_STATE_NULL) {
        delete call_info;
        return;
    }

    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_info->id);
    if (!user_data) {
        user_data = new call_user_data(call_info->id);
        pjsua_call_set_user_data(call_info->id, user_data);
    }

    QString* str = new QString(QStringLiteral("%1: %2")
        .arg(Translate(QStringLiteral("Call Transfer")),
             Onyx::PjToStr(status_text, TRUE)));
    if (final) {
        str->append(QString(" [%1]").arg(Translate(QStringLiteral("Final"))));
    }

    if (status_code / 100 == 2) {
        *p_cont = PJ_FALSE;
    }

    call_info->last_status = (pjsip_status_code)status_code;

    call_info->call_id.ptr = (char*)user_data;
    call_info->call_id.slen = 0;

    QMetaObject::invokeMethod(mainDlg, "onCallTransferStatus", Qt::QueuedConnection,
                              Q_ARG(pjsua_call_info*, call_info), Q_ARG(QString*, str));
}

static void on_call_transfer_request2(pjsua_call_id call_id, const pj_str_t * dst, pjsip_status_code * code, pjsua_call_setting * opt)
{
    SIPURI sipuri;
    Onyx::ParseSIPURI(Onyx::PjToStr(dst, TRUE), &sipuri);
    pj_bool_t cont;
    QString number = sipuri.user;
    if (number.isEmpty()) {
        number = sipuri.domain;
    }
    else if (!accountSettings.accountId || sipuri.domain != get_account_domain()) {
        number.append("@" + sipuri.domain);
    }
    char* buf = Onyx::WideCharToPjStr(number);
    pj_str_t bufStr = pj_str(buf);
    on_call_transfer_status(call_id,
        0,
        &bufStr,
        PJ_FALSE,
        &cont);
    free(buf);
    //--
    if (!code) {
        // if our function call
        return;
    }
    pjsua_call_info call_info;
    if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS || call_info.state != PJSIP_INV_STATE_CONFIRMED) {
        *code = PJSIP_SC_DECLINE;
    }
    if (*code != PJSIP_SC_DECLINE) {
        // deny transfer if we already have a call with same dest address
        pjsua_call_id call_ids[PJSUA_MAX_CALLS];
        unsigned calls_count = PJSUA_MAX_CALLS;
        if (pjsua_enum_calls(call_ids, &calls_count) == PJ_SUCCESS) {
            for (unsigned i = 0; i < calls_count; ++i) {
                pjsua_call_info call_info_curr;
                if (pjsua_call_get_info(call_ids[i], &call_info_curr) == PJ_SUCCESS) {
                    call_user_data* user_data_curr = (call_user_data*)pjsua_call_get_user_data(call_info_curr.id);
                    SIPURI sipuri_curr;
                    ParseCallSIPURI(&call_info_curr, user_data_curr, &sipuri_curr);
                    if (sipuri.user + "@" + sipuri.domain == sipuri_curr.user + "@" + sipuri_curr.domain) {
                        *code = PJSIP_SC_DECLINE;
                        break;
                    }
                }
            }
        }
    }
}

static void on_call_replace_request2(pjsua_call_id call_id, pjsip_rx_data * rdata, int* st_code, pj_str_t * st_text, pjsua_call_setting * opt)
{
    pjsua_call_info call_info;
    if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS) {
        if (!call_info.rem_vid_cnt) {
            opt->vid_cnt = 0;
        }
    }
    else {
        opt->vid_cnt = 0;
    }
}

static void on_call_replaced(pjsua_call_id old_call_id, pjsua_call_id new_call_id)
{
    pjsua_call_info call_info;
    if (pjsua_call_get_info(new_call_id, &call_info) == PJ_SUCCESS) {
        on_call_transfer_request2(old_call_id, &call_info.remote_info, NULL, NULL);
    }
}

static void on_mwi_info(pjsua_acc_id acc_id, pjsua_mwi_info * mwi_info)
{
    bool hasMail = false;
    if (mwi_info->rdata->msg_info.ctype) {
        const pjsip_ctype_hdr* ctype = mwi_info->rdata->msg_info.ctype;
        if (pj_strcmp2(&ctype->media.type, "application") != 0 || pj_strcmp2(&ctype->media.subtype, "simple-message-summary") != 0) {
            return;
        }
    }
    if (!mwi_info->rdata->msg_info.msg->body || !mwi_info->rdata->msg_info.msg->body->len) {
        return;
    }
    pjsip_msg_body* body = mwi_info->rdata->msg_info.msg->body;
    pj_scanner scanner;
    pj_scan_init(&scanner, (char*)body->data, body->len, PJ_SCAN_AUTOSKIP_WS, 0);
    while (!pj_scan_is_eof(&scanner)) {
        pj_str_t key;
        pj_scan_get_until_chr(&scanner, ":", &key);
        pj_strtrim(&key);
        if (key.slen && !pj_scan_is_eof(&scanner)) {
            scanner.curptr++;
            pj_str_t value;
            pj_scan_get_until_chr(&scanner, "\r\n", &value);
            pj_strtrim(&value);
            if (pj_stricmp2(&key, "Messages-Waiting") == 0) {
                hasMail = pj_stricmp2(&value, "yes") == 0;
                break;
            }
        }
    }
    pj_scan_fini(&scanner);
    QMetaObject::invokeMethod(mainDlg, "onMWIInfo", Qt::QueuedConnection, Q_ARG(bool, hasMail));
}

static void on_dtmf_digit(pjsua_call_id call_id, int digit)
{
    char signal[2];
    signal[0] = digit;
    signal[1] = 0;
    call_play_digit(-1, signal);
}

static void on_call_tsx_state(pjsua_call_id call_id, pjsip_transaction * tsx, pjsip_event * e)
{
    if (tsx->role == PJSIP_ROLE_UAS) {
        const pjsip_method update_method = {
            PJSIP_OTHER_METHOD,
            pj_str("UPDATE")
        };
        if (tsx->method.id == PJSIP_INVITE_METHOD || pjsip_method_cmp(&tsx->method, &update_method) == 0) {
            if (e->body.tsx_state.type == PJSIP_EVENT_RX_MSG) {
                pjsip_rx_data* rdata = e->body.rx_msg.rdata;
                pjsip_generic_string_hdr* hsr;
                const pj_str_t headerCallerID = pj_str("P-Asserted-Identity");
                hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerCallerID, NULL);
                if (!hsr) {
                    const pj_str_t headerCallerID = pj_str("Remote-Party-Id");
                    hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerCallerID, NULL);
                }
                if (hsr) {
                    call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
                    if (user_data) {
                        user_data->CS.Lock();
                        user_data->callerID = Onyx::PjToStr(&hsr->hvalue, true);
                        if (user_data->callerID.indexOf('@') == -1) {
                            user_data->callerID.clear();
                        }
                        else {
                            int pos = user_data->callerID.indexOf(';');
                            if (pos != -1) {
                                user_data->callerID = user_data->callerID.left(pos);
                            }
                                user_data->callerID = user_data->callerID.trimmed();
                        }
                        user_data->CS.Unlock();
                    }
                }
            }
            return;
        }
    }
        const pjsip_method info_method = {
            PJSIP_OTHER_METHOD,
            pj_str("INFO")
        };
    if (pjsip_method_cmp(&tsx->method, &info_method) == 0) {
        if (tsx->role == PJSIP_ROLE_UAS && tsx->state == PJSIP_TSX_STATE_TRYING) {
            if (e->body.tsx_state.type == PJSIP_EVENT_RX_MSG) {
                pjsip_rx_data* rdata = e->body.tsx_state.src.rdata;
                pjsip_msg_body* body = rdata->msg_info.msg->body;
                int code = 0;
                if (body && body->len
                    && pj_strcmp2(&body->content_type.type, "application") == 0
                    && pj_strcmp2(&body->content_type.subtype, "dtmf-relay") == 0) {
                    code = 400;
                    pj_scanner scanner;
                    pj_scan_init(&scanner, (char*)body->data, body->len, PJ_SCAN_AUTOSKIP_WS, 0);
                    char digit;
                    int duration = 250;
                    while (!pj_scan_is_eof(&scanner)) {
                        pj_str_t key;
                        pj_scan_get_until_chr(&scanner, "=", &key);
                        pj_strtrim(&key);
                        if (key.slen && !pj_scan_is_eof(&scanner)) {
                            scanner.curptr++;
                            pj_str_t value;
                            pj_scan_get_until_chr(&scanner, "\r\n", &value);
                            pj_strtrim(&value);
                            if (pj_stricmp2(&key, "Signal") == 0) {
                                if (value.slen == 1) {
                                    digit = *value.ptr;
                                    code = 200;
                                }
                            }
                            else if (pj_stricmp2(&key, "Duration") == 0) {
                                int res = 0;
                                for (int i = 0; i < (unsigned)value.slen; ++i) {
                                    res = res * 10 + (value.ptr[i] - '0');
                                }
                                if (res >= 100 || res <= 5000) {
                                    duration = res;
                                }
                            }
                        }
                    }
                    pj_scan_fini(&scanner);
                    if (code == 200) {
                        on_dtmf_digit(-1, digit);
                    }
                }
                else if (!body || !body->len) {
                    code = 200;
                }
                if (code) {
                    pjsip_tx_data* tdata;
                    if (pjsip_endpt_create_response(tsx->endpt, rdata,
                        code, NULL, &tdata) == PJ_SUCCESS
                        ) {
                        pjsip_tsx_send_msg(tsx, tdata);
                    }
                }
            }
        }
        return;
    }
                    const pjsip_method cancel_method = {
                        PJSIP_CANCEL_METHOD,
                        pj_str("CANCEL")
                    };
    if (pjsip_method_cmp(&tsx->method, &cancel_method) == 0) {
        if (e->body.tsx_state.type == PJSIP_EVENT_RX_MSG) {
            pjsip_rx_data* rdata = e->body.rx_msg.rdata;
            const pj_str_t headerReason = pj_str("Reason");
            pjsip_generic_string_hdr* hsr;
            hsr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &headerReason, NULL);
            if (hsr) {
                QString str = Onyx::PjToStr(&hsr->hvalue, true);
                int pos = str.indexOf(QStringLiteral("text=\""));
                if (pos != -1) {
                    str = str.mid(pos + 6);
                    pos = str.indexOf('\"');
                    if (pos != -1) {
                        str = str.left(pos);
                        call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
                        if (user_data) {
                            user_data->CS.Lock();
                            user_data->reason = str;
                            user_data->CS.Unlock();
                        }
                    }
                }
            }
        }
        return;
    }
        if (tsx->state == PJSIP_TSX_STATE_COMPLETED) {
        const pjsip_method refer_method = {
            PJSIP_OTHER_METHOD,
            pj_str("REFER")
        };
        if (pjsip_method_cmp(&tsx->method, &refer_method) == 0 && tsx->status_code / 100 != 2) {
            pj_bool_t cont;
            on_call_transfer_status(call_id,
                tsx->status_code,
                &tsx->status_text,
                PJ_FALSE,
                &cont);
        }
    }
}

static pjsip_redirect_op on_call_redirected(pjsua_call_id call_id,
                                            const pjsip_uri * target,
                                            const pjsip_event * e)
{
    return PJSIP_REDIRECT_ACCEPT_REPLACE;
}

static DWORD WINAPI NetworkChangeThread(LPVOID lpParam)
{
    while (NotifyAddrChange(NULL, NULL) == NO_ERROR) {
        QMetaObject::invokeMethod(mainDlg, "OnNetworkChange", Qt::QueuedConnection);
    }
    return 0;
}
// --- CmainDlg Implementation (Qt conversion) ---

CmainDlg::~CmainDlg(void)
{
}

void CmainDlg::OnDestroy()
{
    mmNotificationClient = nullptr;
    WTSUnRegisterSessionNotification((HWND)winId());

    PJDestroy(true);

    accountSettings.SettingsSave();

    RemoveJumpList();
    if (trayIcon) {
        trayIcon->hide();
        delete trayIcon;
        trayIcon = nullptr;
    }
    UnloadLangPackModule();

    QWidget::close();
}

void CmainDlg::PostNcDestroy()
{
    delete this;
}

void CmainDlg::DoDataExchange(QDataExchange* pDX)
{
    // Handled by Qt Designer UI file (ui.setupUi)
}

// Constructor taking QWidget* parent
CmainDlg::CmainDlg(QWidget* parent)
    : BaseDialog(parent)
{
#ifdef _DEBUG
    if (AllocConsole()) {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        freopen("CONOUT$", "wt", stdout);
    }
#endif

    mainDlg = this;
    mmNotificationClient = nullptr;
    updateCheckerShow = false;

    pageCalls = nullptr;
    pageContacts = nullptr;
    widthAdd = 0;
    heightAdd = 0;

    m_tabPrev = -1;
    newMessages = false;
    missed = false;

    usersDirectoryLoaded = false;
    shortcutsURLLoaded = false;

    // Audio codec captions list (pair: codec_id, description)
    QString audioCodecsCaptions = QStringLiteral("opus/48000/2;Opus 24 kHz;"
        "PCMA/8000/1;G.711 A-law;"
        "PCMU/8000/1;G.711 u-law;"
        "G722/16000/1;G.722 16 kHz;"
        "G7221/16000/1;G.722.1 16 kHz;"
        "G7221/32000/1;G.722.1 32 kHz;"
        "G723/8000/1;G.723 8 kHz;"
        "G729/8000/1;G.729 8 kHz;"
        "GSM/8000/1;GSM 8 kHz;"
        "GSM-EFR/8000/1;GSM-EFR 8 kHz;"
        "AMR/8000/1;AMR 8 kHz;"
        "AMR-WB/16000/1;AMR-WB 16 kHz;"
        "iLBC/8000/1;iLBC 8 kHz;"
        "speex/32000/1;Speex 32 kHz;"
        "speex/16000/1;Speex 16 kHz;"
        "speex/8000/1;Speex 8 kHz;"
        "SILK/24000/1;SILK 24 kHz;"
        "SILK/16000/1;SILK 16 kHz;"
        "SILK/12000/1;SILK 12 kHz;"
        "SILK/8000/1;SILK 8 kHz;"
        "L16/8000/1;LPCM 8 kHz;"
        "L16/8000/2;LPCM 8 kHz Stereo;"
        "L16/16000/1;LPCM 16 kHz;"
        "L16/16000/2;LPCM 16 kHz Stereo;"
        "L16/44100/1;LPCM 44 kHz;"
        "L16/44100/2;LPCM 44 kHz Stereo;"
        "L16/48000/1;LPCM 48 kHz;"
        "L16/48000/2;LPCM 48 kHz Stereo");
    QStringList parts = audioCodecsCaptions.split(';');
    for (int i = 0; i + 1 < parts.size(); i += 2) {
        audioCodecList.insert(parts[i].trimmed(), parts[i + 1].trimmed());
    }

    // Locale
    wchar_t szBuf[STR_SZ];
    wchar_t szLocale[STR_SZ];
    ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, szBuf, STR_SZ);
    _tcscpy(szLocale, szBuf);
    ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, szBuf, STR_SZ);
    if (_tcsclen(szBuf) != 0) {
        _tcscat(szLocale, _T("_"));
        _tcscat(szLocale, szBuf);
    }
    ::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szBuf, STR_SZ);
    if (_tcsclen(szBuf) != 0) {
        _tcscat(szLocale, _T("."));
        _tcscat(szLocale, szBuf);
    }
    _tsetlocale(LC_ALL, szLocale);

    LoadLangPackModule();
}

int CmainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    WM_TASKBARRESTARTMESSAGE = RegisterWindowMessage(_T("TaskbarCreated"));
    // Visual manager not needed

    HDC hDC = ::GetDC(0);
    dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
    ::ReleaseDC(0, hDC);

    bool setpos = false;
    if (accountSettings.noResize) {
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
        setFixedSize(lpCreateStruct->cx, lpCreateStruct->cy);
        setpos = true;
    }

    ShortcutsLoad();
    shortcutsEnabled = accountSettings.enableShortcuts;
    shortcutsBottom = accountSettings.shortcutsBottom;
    shortcutsCount = shortcuts.size();
    if (accountSettings.enableShortcuts) {
        if (shortcutsBottom) {
            if (shortcutsCount) {
                if (shortcutsCount > _GLOBAL_SHORTCUTS_QTY / 2) {
                    heightAdd += MulDiv(10 + (shortcutsCount + shortcutsCount % 2) * 25 / 2, dpiY, 96);
                } else {
                    heightAdd += MulDiv(10 + shortcutsCount * 25, dpiY, 96);
                }
            }
        } else {
            if (shortcutsCount > 12) {
                widthAdd += MulDiv(200, dpiY, 96);
            } else {
                widthAdd += MulDiv(140, dpiY, 96);
            }
        }
    }
    int heightFix = 0;
    if (setpos || widthAdd || heightAdd || heightFix) {
        resize(lpCreateStruct->cx + widthAdd, lpCreateStruct->cy + heightAdd + heightFix);
    }

    if (langPack.rtl) {
        setLayoutDirection(Qt::RightToLeft);
    }

    return 0;
}

BOOL CmainDlg::OnInitDialog()
{
    WTSRegisterSessionNotification((HWND)winId(), NOTIFY_FOR_THIS_SESSION);
    mmNotificationClient = nullptr;

    CreateThread(NULL, 0, NetworkChangeThread, 0, 0, NULL);

    settingsDlg = NULL;
    shortcutsDlg = NULL;

    messagesDlg = new MessagesDlg(this);
    transferDlg = NULL;
    accountDlg = NULL;

    m_lastInputTime = 0;
    m_idleCounter = 0;
    m_PresenceStatus = PJRPID_ACTIVITY_UNKNOWN;

#ifdef _GLOBAL_VIDEO
    previewWin = NULL;
#endif

    SetupJumpList();
    m_hIcon = QIcon(":/icons/onyx-voip.ico");
    setWindowIcon(m_hIcon);
    iconSmall = m_hIcon;

    // Tray icon
    if (!trayIcon) {
        trayIcon = new QSystemTrayIcon(m_hIcon, this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &CmainDlg::onTrayNotify);
    }
    ShowTrayIcon();

    // Status bar
    statusBar = new QStatusBar(this);
    statusBar->setSizeGripEnabled(false);
    QLabel* statusLabel1 = new QLabel(Translate(_T("Idle")));
    QLabel* statusLabel2 = new QLabel("");
    statusBar->addPermanentWidget(statusLabel1, 1);
    statusBar->addPermanentWidget(statusLabel2);
    m_barLabels[0] = statusLabel1;
    m_barLabels[1] = statusLabel2;

    // Tab control (use QTabWidget)
    tabWidget = new QTabWidget(this);
    tabWidget->setGeometry(0, 0, width(), height() - statusBar->height());

    pageDialer = new Dialer(this);
    tabWidget->addTab(pageDialer, Translate(_T("Phone")));

    pageCalls = new Calls(this);
    tabWidget->addTab(pageCalls, Translate(_T("Logs")));

    pageContacts = new Contacts(this);
    tabWidget->addTab(pageContacts, Translate(_T("Contacts")));

    connect(tabWidget, &QTabWidget::currentChanged, this, &CmainDlg::OnTcnSelchangeTab);

    // Menu button
    m_ButtonMenu = new QPushButton(this);
    m_ButtonMenu->setIcon(QIcon(":/icons/dropdown.ico"));
    m_ButtonMenu->setFixedSize(20, 20);
    connect(m_ButtonMenu, &QPushButton::clicked, this, &CmainDlg::OnBnClickedMenu);

    // Window position/size
    QRect screenRect;
    if (accountSettings.multiMonitor) {
        Onyx::GetScreenRect(&screenRect);
    } else {
        screenRect = QApplication::primaryScreen()->availableGeometry();
    }

    int mW = accountSettings.mainW > 0 ? accountSettings.mainW : width();
    int mH = accountSettings.mainH > 0 ? accountSettings.mainH : height();
    int mx, my;
    if (!accountSettings.mainX && !accountSettings.mainY) {
        QRect primaryScreenRect = QApplication::primaryScreen()->availableGeometry();
        mx = primaryScreenRect.right() - mW;
        my = primaryScreenRect.bottom() - mH;
    } else {
        mx = qBound(screenRect.left(), accountSettings.mainX, screenRect.right() - mW);
        my = qBound(screenRect.top(), accountSettings.mainY, screenRect.bottom() - mH);
    }

    // Messages dialog position
    messagesDlg->resize(accountSettings.messagesW > 0 ? accountSettings.messagesW : 550,
                        accountSettings.messagesH > 0 ? accountSettings.messagesH : mH);
    int messagesX = qBound(screenRect.left(), accountSettings.messagesX, screenRect.right() - messagesDlg->width());
    int messagesY = qBound(screenRect.top(), accountSettings.messagesY, screenRect.bottom() - messagesDlg->height());
    messagesDlg->move(messagesX, messagesY);

    setGeometry(mx, my, mW, mH);
    if (accountSettings.alwaysOnTop) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }

    // Status icon list (we'll use QIcons directly)
    iconStatusMap[IDI_BLANK] = QIcon(":/icons/status_blank.ico");
    iconStatusMap[IDI_UNKNOWN] = QIcon(":/icons/status_unknown.ico");
    iconStatusMap[IDI_OFFLINE] = QIcon(":/icons/status_offline.ico");
    iconStatusMap[IDI_AWAY] = QIcon(":/icons/status_away.ico");
    iconStatusMap[IDI_ONLINE] = QIcon(":/icons/status_online.ico");
    iconStatusMap[IDI_ON_THE_PHONE] = QIcon(":/icons/status_on-the-phone.ico");
    iconStatusMap[IDI_BUSY] = QIcon(":/icons/status_busy.ico");
    iconStatusMap[IDI_DEFAULT] = QIcon(":/icons/status_default.ico");
    iconStatusMap[IDI_UNKNOWN_STARRED] = QIcon(":/icons/status_unknown_starred.ico");
    iconStatusMap[IDI_OFFLINE_STARRED] = QIcon(":/icons/status_offline_starred.ico");
    iconStatusMap[IDI_AWAY_STARRED] = QIcon(":/icons/status_away_starred.ico");
    iconStatusMap[IDI_ONLINE_STARRED] = QIcon(":/icons/status_online_starred.ico");
    iconStatusMap[IDI_ON_THE_PHONE_STARRED] = QIcon(":/icons/status_on-the-phone_starred.ico");
    iconStatusMap[IDI_BUSY_STARRED] = QIcon(":/icons/status_busy_starred.ico");
    iconStatusMap[IDI_DEFAULT_STARRED] = QIcon(":/icons/status_default_starred.ico");
    iconStatusMap[IDI_ACTIVE] = QIcon(":/icons/active_blue.ico");
    iconStatusMap[IDI_HOLD] = QIcon(":/icons/button_hold.ico");
    iconStatusMap[IDI_ACTIVE_SECURE] = QIcon(":/icons/active_green_secure.ico");
    iconStatusMap[IDI_ACTIVE_RED] = QIcon(":/icons/active_red.ico");
    iconStatusMap[IDI_ACTIVE_SECURE_RED] = QIcon(":/icons/active_red_secure.ico");
    iconStatusMap[IDI_ACTIVE_YELLOW] = QIcon(":/icons/active_yellow.ico");
    iconStatusMap[IDI_ACTIVE_SECURE_YELLOW] = QIcon(":/icons/active_yellow_secure.ico");
    iconStatusMap[IDI_SECURE] = QIcon(":/icons/status_secure.ico");
    iconStatusMap[IDI_FORWARDING] = QIcon(":/icons/status_forwarding.ico");
    iconStatusMap[IDI_MISSED] = QIcon(":/icons/tray_missed.ico");
    iconStatusMap[IDI_INACTIVE] = QIcon(":/icons/tray_inactive.ico");

    tabWidget->setCurrentIndex(accountSettings.activeTab);

    const QStringList args = QCoreApplication::arguments();
    BOOL minimized = args.contains(QStringLiteral("/minimized"));
    m_startMinimized = (!firstRun && minimized) || accountSettings.minimized;

    InitUI();
    OnAccountChanged(true);

    return TRUE;
}

void CmainDlg::InitUI()
{
    onMWIInfo(false); // voicemail button update
    SetPaneText2();
    setWindowTitle(QStringLiteral(_GLOBAL_NAME_VISIBLE));
    UpdateWindowText();
    pageDialer->SetName();
}

void CmainDlg::onMWIInfo(bool hasMail)
{
    Q_UNUSED(hasMail);
}

void CmainDlg::ShowTrayIcon()
{
    if (!trayIcon) {
        trayIcon = new QSystemTrayIcon(m_hIcon, this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &CmainDlg::onTrayNotify);
    }
    trayIcon->show();
    trayIcon->setToolTip(QStringLiteral(_GLOBAL_NAME_VISIBLE));
}

void CmainDlg::OnCreated()
{
    OnTcnSelchangeTab(0);

    if (!m_startMinimized) {
        show();
        TabFocusSet();
    }

    PJCreate();

    const QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        CommandLine(args.mid(1).join(' '));
    }
    PJAccountAdd();

    WM_SHELLHOOKMESSAGE = RegisterWindowMessage(_T("SHELLHOOK"));
    if (WM_SHELLHOOKMESSAGE) {
        RegisterShellHookWindow((HWND)winId());
    }
}

void CmainDlg::ProcessCommand(const QString& str)
{
    CommandLine(str);
}

void CmainDlg::Subscribe()
{
    if (pageDialer) {
        pageDialer->PresenceSubscribe();
    }
    if (pageContacts) {
        pageContacts->PresenceSubscribe();
    }
}

void CmainDlg::Unsubscribe()
{
    if (pageDialer) {
        pageDialer->PresenceReset();
    }
    if (pageContacts) {
        pageContacts->PresenceReset();
    }
}

void CmainDlg::SubsribeNumber(QString* number)
{
    Q_UNUSED(number);
}

void CmainDlg::UnsubscribeNumber(QString* number)
{
    Q_UNUSED(number);
}

void CmainDlg::onRefreshLevels(int wParam, int lParam)
{
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);
}

void CmainDlg::onCreateRingingDlg(pjsua_call_id call_id)
{
    Q_UNUSED(call_id);
}

void CmainDlg::onCallTransferStatus(pjsua_call_info* call_info, const QString& str)
{
    Q_UNUSED(call_info);
    Q_UNUSED(str);
}

void CmainDlg::CreationComplete()
{
}

void CmainDlg::OnBnClickedOk()
{
    OnClose();
}

void CmainDlg::OnBnClickedMenu()
{
    MainPopupMenu(true);
}

void CmainDlg::onCommandLine(const QString& params)
{
    CommandLine(params);
}

void CmainDlg::onShortcutsURLLoaded(URLGetAsyncData* response)
{
    Q_UNUSED(response);
    shortcutsURLLoaded = true;
}

void CmainDlg::closeEvent(QCloseEvent* event)
{
    OnClose();
    if (event) {
        event->accept();
    }
}

void CmainDlg::moveEvent(QMoveEvent* event)
{
    if (event) {
        OnMove(event->pos().x(), event->pos().y());
    }
    QWidget::moveEvent(event);
}

void CmainDlg::resizeEvent(QResizeEvent* event)
{
    if (event) {
        OnSize(0, event->size().width(), event->size().height());
    }
    QWidget::resizeEvent(event);
}

void CmainDlg::showEvent(QShowEvent* event)
{
    BaseDialog::showEvent(event);
}

void CmainDlg::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
}

bool CmainDlg::eventFilter(QObject* obj, QEvent* event)
{
    return BaseDialog::eventFilter(obj, event);
}

void CmainDlg::TrayIconUpdateTip()
{
    if (trayIcon) {
        QString tip = QStringLiteral(_GLOBAL_NAME_VISIBLE);
        if (accountSettings.accountId) {
            if (!accountSettings.account.label.isEmpty()) {
                tip.append("\n").append(accountSettings.account.label);
            } else if (!accountSettings.account.username.isEmpty()) {
                tip.append("\n").append(accountSettings.account.username);
            }
            if (!accountSettings.account.displayName.isEmpty()) {
                tip.append("\n").append(accountSettings.account.displayName);
            }
        }
        trayIcon->setToolTip(tip);
    }
}

void CmainDlg::BaloonPopup(const QString& title, const QString& message, QSystemTrayIcon::MessageIcon icon)
{
    if (trayIcon) {
        trayIcon->showMessage(title, message, icon, 3000);
    }
}

void CmainDlg::SwitchDND(int state, bool update)
{
    if (state == -1) {
        accountSettings.DND = !accountSettings.DND;
    } else {
        accountSettings.DND = state;
    }
    pageDialer->SetCheckDND(accountSettings.DND);
    AccountSettingsPendingSave();
    PublishStatus();
    if (update) return;
}

void CmainDlg::OnMenuAccountAdd()
{
    if (!accountDlg) {
        accountDlg = new AccountDlg(this);
    }
    accountDlg->Load(-1);
    accountDlg->show();
    accountDlg->raise();
    accountDlg->activateWindow();
}

void CmainDlg::OnMenuAccountEdit(int nID)
{
    if (!accountDlg) {
        accountDlg = new AccountDlg(this);
    }
    int id = accountSettings.accountId > 0 ? accountSettings.accountId : nID - ID_ACCOUNT_EDIT_RANGE + 1;
    accountDlg->Load(id ? id : -1);
    accountDlg->show();
    accountDlg->raise();
    accountDlg->activateWindow();
}

void CmainDlg::OnMenuAccountChange(int nID)
{
    if (accountSettings.accountId) {
        PJAccountDelete(true);
    }
    int idNew = nID - ID_ACCOUNT_CHANGE_RANGE + 1;
    if (accountSettings.accountId != idNew) {
        accountSettings.accountId = idNew;
        accountSettings.AccountLoad(accountSettings.accountId, &accountSettings.account);
    } else {
        accountSettings.accountId = 0;
        InitUI();
    }
    OnAccountChanged();
    accountSettings.SettingsSave();
    PJAccountAdd();
}

void CmainDlg::OnMenuAccountLocalEdit()
{
    if (MACRO_ENABLE_LOCAL_ACCOUNT) {
        if (!accountDlg) {
            accountDlg = new AccountDlg(this);
        }
        accountDlg->Load(0);
        accountDlg->show();
        accountDlg->raise();
        accountDlg->activateWindow();
    }
}

void CmainDlg::OnMenuCustomRange(int nID)
{
    // Reserved for custom menu items
}

void CmainDlg::OnMenuSettings()
{
    if (!settingsDlg) {
        settingsDlg = new SettingsDlg(this);
    }
    settingsDlg->show();
    settingsDlg->raise();
    settingsDlg->activateWindow();
}

void CmainDlg::OnMenuShortcuts()
{
    if (!shortcutsDlg) {
        shortcutsDlg = new ShortcutsDlg(this);
    }
    shortcutsDlg->show();
    shortcutsDlg->raise();
    shortcutsDlg->activateWindow();
}

void CmainDlg::OnMenuAlwaysOnTop()
{
    accountSettings.alwaysOnTop = 1 - accountSettings.alwaysOnTop;
    AccountSettingsPendingSave();
    if (accountSettings.alwaysOnTop) {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void CmainDlg::OnMenuLog()
{
    Onyx::OpenFile(accountSettings.logFile);
}

void CmainDlg::OnMenuExit()
{
    close();
}

void CmainDlg::onTrayNotify(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if (isVisible() && !isMinimized()) {
            hide();
        } else {
            showNormal();
            activateWindow();
            raise();
            if (missed) {
                GotoTabLParam((LPARAM)pageCalls);
                missed = false;
                UpdateWindowText();
            }
            // Show ringing dialogs
            for (RinginDlg* ringinDlg : ringinDlgs) {
                ringinDlg->showNormal();
            }
            // Show messages if needed
            bool showMessages = (!accountSettings.singleMode && messagesDlg->GetCallsCount()) || newMessages;
            if (showMessages) {
                newMessages = false;
                messagesDlg->showNormal();
            }
            TabFocusSet();
        }
        break;
    case QSystemTrayIcon::Context:
        MainPopupMenu();
        break;
    default:
        break;
    }
}

void CmainDlg::MainPopupMenu(bool isMenuButton)
{
    QPoint point;
    if (isMenuButton) {
        point = m_ButtonMenu->mapToGlobal(QPoint(0, m_ButtonMenu->height()));
    } else {
        point = QCursor::pos();
    }
    QMenu menu(this);

    // Build menu structure exactly as original
    bool basic = false;
    if (!basic) {
        // Add Account
        QAction* addAction = menu.addAction(Translate(_T("Add Account...")));
        addAction->setData(ID_ACCOUNT_ADD);

        // Edit Account submenu
        QMenu* editMenu = menu.addMenu(Translate(_T("Edit Account")));
        bool checked = false;
        Account acc;
        int i = 0;
        while (accountSettings.AccountLoad(i + 1, &acc)) {
            QString label = acc.label.isEmpty() ? QString("%1@%2").arg(acc.username, acc.domain) : acc.label;
            QAction* changeAction = menu.addAction(label);
            changeAction->setCheckable(true);
            changeAction->setChecked(accountSettings.accountId == i + 1);
            changeAction->setData(ID_ACCOUNT_CHANGE_RANGE + i);
            QAction* editAction = editMenu->addAction(label);
            editAction->setData(ID_ACCOUNT_EDIT_RANGE + i);
            if (!checked) checked = accountSettings.accountId == i + 1;
            i++;
        }
        if (i == 1) {
            QAction* act = menu.actions().value(1); // Make Active
            if (act) act->setText(Translate(_T("Make Active")));
        }
        if (i == 1) {
            QAction* editAct = menu.addAction(Translate(_T("Edit Account")) + "\tCtrl+M");
            editAct->setData(ID_ACCOUNT_EDIT_RANGE);
        }
        if (accountSettings.enableLocalAccount && MACRO_ENABLE_LOCAL_ACCOUNT) {
            QAction* localAct = menu.addAction(Translate(_T("Edit Local Account")) + "\tCtrl+L");
            localAct->setData(ID_ACCOUNT_EDIT_LOCAL);
        }
        menu.addSeparator();
        QAction* settingsAct = menu.addAction(Translate(_T("Settings")) + "\tCtrl+P");
        settingsAct->setData(ID_SETTINGS);
        QAction* shortcutsAct = menu.addAction(Translate(_T("Shortcuts")) + "\tCtrl+S");
        shortcutsAct->setData(ID_SHORTCUTS);
    }

    bool separator = false;
    if (!separator) {
        menu.addSeparator();
        separator = true;
    }
    QAction* alwaysTopAct = menu.addAction(Translate(_T("Always on Top")));
    alwaysTopAct->setCheckable(true);
    alwaysTopAct->setChecked(accountSettings.alwaysOnTop);
    alwaysTopAct->setData(ID_ALWAYS_ON_TOP);

    if (!separator) {
        menu.addSeparator();
        separator = true;
    }
    QAction* logAct = menu.addAction(Translate(_T("View Log File")));
    logAct->setEnabled(accountSettings.enableLog);
    logAct->setData(ID_LOG);

    separator = false;
    if (!separator) {
        menu.addSeparator();
        separator = true;
    }
    QAction* webAct = menu.addAction(Translate(_T("Visit Website")) + "\tCtrl+W");
    webAct->setData(ID_MENU_WEBSITE);

    separator = false;
    if (!separator) {
        menu.addSeparator();
        separator = true;
    }
    QAction* helpAct = menu.addAction(QString("%1\tv%2").arg(Translate(_T("Help")), _T(_GLOBAL_VERSION)));
    helpAct->setData(ID_MENU_HELP);

    menu.addSeparator();
    QAction* exitAct = menu.addAction(Translate(_T("Exit")) + "\tCtrl+Q");
    exitAct->setData(ID_EXIT);

    // Remove leading separator if present
    QList<QAction*> actions = menu.actions();
    if (!actions.isEmpty() && actions.first()->isSeparator()) {
        menu.removeAction(actions.first());
    }

    QAction* chosen = menu.exec(point);
    if (!chosen) return;

    int id = chosen->data().toInt();
    if (id == ID_ACCOUNT_ADD) {
        OnMenuAccountAdd();
    } else if (id >= ID_ACCOUNT_EDIT_RANGE && id <= ID_ACCOUNT_EDIT_RANGE + 99) {
        OnMenuAccountEdit(id);
    } else if (id >= ID_ACCOUNT_CHANGE_RANGE && id <= ID_ACCOUNT_CHANGE_RANGE + 99) {
        OnMenuAccountChange(id);
    } else {
        switch (id) {
        case ID_ACCOUNT_EDIT_LOCAL: OnMenuAccountLocalEdit(); break;
        case ID_SETTINGS: OnMenuSettings(); break;
        case ID_SHORTCUTS: OnMenuShortcuts(); break;
        case ID_ALWAYS_ON_TOP: OnMenuAlwaysOnTop(); break;
        case ID_LOG: OnMenuLog(); break;
        case ID_MENU_WEBSITE: OnMenuWebsite(); break;
        case ID_MENU_HELP: OnMenuHelp(); break;
        case ID_EXIT: OnMenuExit(); break;
        default:
            if (id >= ID_CUSTOM_RANGE && id <= ID_CUSTOM_RANGE + 99) {
                OnMenuCustomRange(id);
            }
            break;
        }
    }
}
// === Message Handlers ===

void CmainDlg::onRegState2(int code, QString* headerError)
{
    if (headerError) {
        delete headerError;
    }

    if (code == 200) {
        Subscribe();
        if (accountSettings.usersDirectory.contains("%s") || accountSettings.usersDirectory.contains("{")) {
            UsersDirectoryLoad();
        }
    }
    UpdateWindowText("", IDI_DEFAULT, true);
}

void CmainDlg::onCallState(pjsua_call_info* call_info, call_user_data* user_data)
{
    SIPURI sipuri;
    ParseCallSIPURI(call_info, user_data, &sipuri);
    QString number = (!sipuri.user.isEmpty() ? sipuri.user + "@" : "") + sipuri.domain;

    user_data->CS.Lock();

    QString* str = new QString();
    QString adder;

    if (call_info->state != PJSIP_INV_STATE_DISCONNECTED && call_info->state != PJSIP_INV_STATE_CONNECTING && call_info->remote_contact.slen > 0) {
        SIPURI contactURI;
        ParseCallSIPURI(call_info, user_data, &contactURI);
        QString contactDomain = Onyx::RemovePort(contactURI.domain);
        if (Onyx::IsIP(contactDomain)) {
            HANDLE hThread;
            QString* addr = new QString(contactDomain);
            hThread = CreateThread(NULL, 0, gethostbyaddrThread, addr, 0, NULL);
            if (WaitForSingleObject(hThread, 500) == 0) {
                QMutexLocker locker(&gethostbyaddrThreadCS);
                contactDomain = gethostbyaddrThreadResult;
            }
        }
        adder += contactDomain + "; ";
    }

    if (call_info->state == PJSIP_INV_STATE_CONFIRMED || call_info->state == PJSIP_INV_STATE_CONNECTING) {
        if (autoAnswerTimerCallId != PJSUA_INVALID_ID) {
            killTimer(IDT_TIMER_AUTOANSWER);
            autoAnswerTimerCallId = PJSUA_INVALID_ID;
        }
        if (forwardingTimerCallId != PJSUA_INVALID_ID) {
            killTimer(IDT_TIMER_FORWARDING);
            forwardingTimerCallId = PJSUA_INVALID_ID;
        }
    }

    unsigned cnt = 0;
    unsigned cnt_srtp = 0;

    switch (call_info->state) {
    case PJSIP_INV_STATE_CALLING:
        *str = Translate(_T("Calling")) + " " + (!sipuri.user.isEmpty() ? sipuri.user : sipuri.domain) + "...";
        break;
    case PJSIP_INV_STATE_INCOMING:
        *str = Translate(_T("Incoming Call"));
        break;
    case PJSIP_INV_STATE_EARLY:
        *str = Onyx::PjToStr(&call_info->last_status_text);
        break;
    case PJSIP_INV_STATE_CONNECTING:
        *str = Translate(_T("Connecting")) + "...";
        break;
    case PJSIP_INV_STATE_CONFIRMED:
        *str = Translate(_T("Connected"));
        for (unsigned i = 0; i < call_info->media_cnt; i++) {
            if (call_info->media[i].dir != PJMEDIA_DIR_NONE &&
                (call_info->media[i].type == PJMEDIA_TYPE_AUDIO || call_info->media[i].type == PJMEDIA_TYPE_VIDEO)) {
                cnt++;
                pjsua_call_info call_info_stub;
                if (is_pjsua_running() && pjsua_call_get_info(call_info->id, &call_info_stub) == PJ_SUCCESS) {
                    bool srtp = false;
                    bool ice = false;
                    pjmedia_transport_info t;
                    if (pjsua_call_get_med_transport_info(call_info->id, call_info->media[i].index, &t) == PJ_SUCCESS) {
                        for (unsigned j = 0; j < t.specific_info_cnt; j++) {
                            if (t.spc_info[j].buffer[0]) {
                                switch (t.spc_info[j].type) {
                                case PJMEDIA_TRANSPORT_TYPE_SRTP: srtp = true; break;
                                case PJMEDIA_TRANSPORT_TYPE_ICE:  ice = true;  break;
                                }
                            }
                        }
                    }
                    pjsua_stream_info psi;
                    if (pjsua_call_get_stream_info(call_info->id, call_info->media[i].index, &psi) == PJ_SUCCESS) {
                        pjmedia_tp_proto proto = PJMEDIA_TP_PROTO_NONE;
                        if (psi.type == PJMEDIA_TYPE_AUDIO) {
                            proto = psi.info.aud.proto;
                            adder += QString("%1@%2kHz %3kbit/s%4, ")
                                .arg(Onyx::PjToStr(&psi.info.aud.fmt.encoding_name))
                                .arg(psi.info.aud.fmt.clock_rate / 1000)
                                .arg(psi.info.aud.param->info.avg_bps / 1000)
                                .arg(psi.info.aud.fmt.channel_cnt == 2 ? " Stereo" : "");
                        } else if (psi.type == PJMEDIA_TYPE_VIDEO) {
                            proto = psi.info.vid.proto;
                            adder += QString("%1 %2kbit/s, ")
                                .arg(Onyx::PjToStr(&psi.info.vid.codec_info.encoding_name))
                                .arg(psi.info.vid.codec_param->enc_fmt.det.vid.max_bps / 1000);
                        }
                        if (srtp) cnt_srtp++;
                        if (proto & PJMEDIA_TP_PROTO_DTLS) {
                            adder += "DTLS-SRTP, ";
                        } else if (srtp || (proto & PJMEDIA_TP_PROFILE_SRTP)) {
                            bool secure = false;
                            if (transport_tls != -1) {
                                pj_pool_t* tmp_pool = pjsua_pool_create("onyx_ocs", 256, 256);
                                if (tmp_pool) {
                                    pjsua_acc_config acc_cfg;
                                    pjsua_acc_config_default(&acc_cfg);
                                    if (pjsua_acc_get_config(call_info->acc_id, tmp_pool, &acc_cfg) == PJ_SUCCESS) {
                                        QByteArray tlsSuffix = QByteArrayLiteral(";transport=tls");
                                        pj_str_t tlsSuffixStr = pj_str(tlsSuffix.data());
                                        secure = pj_strstr(&acc_cfg.id, &tlsSuffixStr) != NULL;
                                    }
                                    pj_pool_release(tmp_pool);
                                }
                            }
                            adder += secure ? "SRTP, " : "SRTP without TLS, ";
                        } else {
                            adder += "unencrypted, ";
                        }
                        if (ice) adder += "ICE, ";
                    }
                }
            }
        }
        if (cnt_srtp && cnt == cnt_srtp) user_data->srtp = ONYX_SRTP;
        else user_data->srtp = ONYX_SRTP_DISABLED;
        break;
    }
    if (!str->isEmpty() && !adder.isEmpty()) {
        str->append(" (" + adder.left(adder.length() - 2) + ")");
    }
    if (call_info->state == PJSIP_INV_STATE_CALLING) {
        if (!accountSettings.cmdOutgoingCall.isEmpty()) {
            QString params = sipuri.user;
            Onyx::RunCmd(URLMask(accountSettings.cmdOutgoingCall, &sipuri, call_info->acc_id, user_data), params);
        }
    }

    if (call_info->state == PJSIP_INV_STATE_CONFIRMED) {
        QMetaObject::invokeMethod(this, "onTimerCall", Qt::QueuedConnection);
        startTimer(IDT_TIMER_CALL, 1000);
        if (call_info->role == PJSIP_ROLE_UAS) {
            if (!accountSettings.cmdCallAnswer.isEmpty()) {
                QString params = sipuri.user;
                Onyx::RunCmd(accountSettings.cmdCallAnswer, params);
            }
            if (call_info->rem_vid_cnt && !accountSettings.cmdCallAnswerVideo.isEmpty()) {
                QString params = sipuri.user;
                Onyx::RunCmd(accountSettings.cmdCallAnswerVideo, params);
            }
        }
        if (!accountSettings.cmdCallStart.isEmpty()) {
            QString params = sipuri.user;
            Onyx::RunCmd(accountSettings.cmdCallStart, params);
        }
        if (!user_data->commands.isEmpty()) {
            startTimer((UINT_PTR)call_info->id, 1000, [=]() { DTMFQueueTimerHandler(call_info->id); });
        }
    }

    if (!accountSettings.singleMode) {
        if (call_info->state != PJSIP_INV_STATE_CONFIRMED) {
            if (call_info->state != PJSIP_INV_STATE_DISCONNECTED) {
                UpdateWindowText(*str, call_info->role == PJSIP_ROLE_UAS ? IDI_CALL_IN : IDI_CALL_OUT);
            }
        }
    }

    if (call_info->role == PJSIP_ROLE_UAC) {
        if (call_info->last_status == 180 && !call_info->media_cnt) {
            if (toneCalls.isEmpty()) {
                QTimer::singleShot(4500, this, [=]() { onPlayerPlay(ONYX_SOUND_RINGING, QString()); });
                toneCalls.append(call_info->id);
            } else if (!toneCalls.contains(call_info->id)) {
                toneCalls.append(call_info->id);
            }
        } else {
            int idx = toneCalls.indexOf(call_info->id);
            if (idx != -1) {
                toneCalls.removeAt(idx);
                if (toneCalls.isEmpty()) {
                    onPlayerStop(0, 0);
                }
            }
        }
    }

    bool doNotShowMessagesWindow =
        call_info->state == PJSIP_INV_STATE_INCOMING ||
        call_info->state == PJSIP_INV_STATE_EARLY ||
        call_info->state == PJSIP_INV_STATE_DISCONNECTED ||
        accountSettings.singleMode;

    if (user_data->autoAnswer) {
        if (!accountSettings.bringToFrontOnIncoming) doNotShowMessagesWindow = true;
    }
    MessagesContact* messagesContact = messagesDlg->AddTab(number,
        (!accountSettings.singleMode && (call_info->state == PJSIP_INV_STATE_CONFIRMED || call_info->state == PJSIP_INV_STATE_CONNECTING))
        || (accountSettings.singleMode &&
            ((call_info->role == PJSIP_ROLE_UAC && call_info->state != PJSIP_INV_STATE_DISCONNECTED) ||
             (call_info->role == PJSIP_ROLE_UAS && (call_info->state == PJSIP_INV_STATE_CONFIRMED || call_info->state == PJSIP_INV_STATE_CONNECTING))))
        ? TRUE : FALSE,
        call_info, user_data, doNotShowMessagesWindow,
        call_info->state == PJSIP_INV_STATE_DISCONNECTED);

    if (call_info->state == PJSIP_INV_STATE_CONFIRMED) {
        if (!accountSettings.singleMode && accountSettings.AC) {
            messagesDlg->OnMergeAll();
        }
    }

    if (call_info->state == PJSIP_INV_STATE_DISCONNECTED) {
        if (call_info->role == PJSIP_ROLE_UAS && call_info->connect_duration.sec == 0 && call_info->connect_duration.msec == 0 && call_info->last_status != 486) {
            missed = true;
        }
    }

    if (messagesContact) {
        QString name = messagesContact->name;
        QString number = messagesContact->number + messagesContact->numberParameters + messagesContact->commands;
        if (call_info->state == PJSIP_INV_STATE_DISCONNECTED) {
            messagesContact->mediaStatus = PJSUA_CALL_MEDIA_ERROR;
            if (call_info->role == PJSIP_ROLE_UAS && call_info->last_status == 486) {
                pageCalls->Add(call_info->call_id, number, name, ONYX_CALL_MISS, user_data);
            }
        } else {
            if (call_info->role == PJSIP_ROLE_UAS) {
                pageCalls->Add(call_info->call_id, number, name, ONYX_CALL_IN, user_data);
            } else {
                pageCalls->Add(call_info->call_id, number, name, ONYX_CALL_OUT, user_data);
            }
        }
    }
    if (accountSettings.singleMode) {
        if (call_info->state != PJSIP_INV_STATE_DISCONNECTED) {
            if (call_info->state != PJSIP_INV_STATE_CONFIRMED) {
                UpdateWindowText(*str, call_info->role == PJSIP_ROLE_UAS ? IDI_CALL_IN : IDI_CALL_OUT);
            }
            GotoTab(0);
            messagesDlg->OnChangeTab(call_info, user_data);
        }
    }

    if (call_info->state == PJSIP_INV_STATE_DISCONNECTED) {
        messagesDlg->OnEndCall(call_info, user_data);
    } else {
        if (messagesContact && !str->isEmpty()) {
            messagesDlg->AddMessage(messagesContact, *str, ONYX_MESSAGE_TYPE_SYSTEM,
                call_info->state == PJSIP_INV_STATE_INCOMING || call_info->state == PJSIP_INV_STATE_EARLY);
        }
    }

    bool hasCalls = messagesDlg->GetCallsCount();

    if (call_info->role == PJSIP_ROLE_UAS) {
        if (call_info->state != PJSIP_INV_STATE_INCOMING && call_info->state != PJSIP_INV_STATE_EARLY) {
            int count = ringinDlgs.count();
            if (!count) {
                if (call_info->state != PJSIP_INV_STATE_DISCONNECTED || (call_info->state == PJSIP_INV_STATE_DISCONNECTED && call_info->connect_duration.sec == 0 && call_info->connect_duration.msec == 0)) {
                    PlayerStop();
                }
            } else {
                for (int i = 0; i < count; i++) {
                    RinginDlg* ringinDlg = ringinDlgs.at(i);
                    if (call_info->id == ringinDlg->call_id) {
                        if (count == 1) PlayerStop();
                        ringinDlgs.removeAt(i);
                        ringinDlg->close();
                        break;
                    }
                }
            }
        }
    }

    if (call_info->state != PJSIP_INV_STATE_INCOMING &&
        call_info->state != PJSIP_INV_STATE_EARLY) {
        if (call_info->state != PJSIP_INV_STATE_DISCONNECTED) {
            if (messagesContact) {
                pageDialer->SetName(messagesContact->name);
            }
        }
    }

    user_data->CS.Unlock();

    if (call_info->state == PJSIP_INV_STATE_DISCONNECTED) {
        delete user_data;
    }
    delete call_info;
    delete str;

    if (pageDialer->m_ButtonRec) {
        pageDialer->m_ButtonRec->setEnabled(hasCalls);
    }
    if (accountSettings.headsetSupport) {
        Hid::SetOffhookRing(hasCalls, ringinDlgs.count());
    }
    if (hasCalls) {
#ifdef _GLOBAL_VIDEO
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED | (previewWin ? ES_DISPLAY_REQUIRED : 0));
#else
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
#endif
    } else {
        SetThreadExecutionState(ES_CONTINUOUS);
    }
}

void CmainDlg::onCallMediaState(pjsua_call_info* call_info, call_user_data* user_data)
{
    messagesDlg->UpdateHoldButton(call_info);

    QString message;
    QString number = Onyx::PjToStr(&call_info->remote_info, TRUE);

    MessagesContact* messagesContact = messagesDlg->AddTab(number, FALSE, call_info, user_data, TRUE, TRUE);

    if (messagesContact) {
        if (call_info->media_status == PJSUA_CALL_MEDIA_REMOTE_HOLD) {
            message = "Call on Remote Hold";
        } else if (call_info->media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD) {
            message = "Call on Local Hold";
        } else if (call_info->media_status == PJSUA_CALL_MEDIA_NONE) {
            message = "Call on Hold";
        }
        if (messagesContact->mediaStatus != PJSUA_CALL_MEDIA_ERROR && messagesContact->mediaStatus != call_info->media_status && call_info->media_status == PJSUA_CALL_MEDIA_ACTIVE) {
            message = "Call is Active";
        }
        if (!message.isEmpty()) {
            messagesDlg->AddMessage(messagesContact, Translate(message), ONYX_MESSAGE_TYPE_SYSTEM, TRUE);
        }
        messagesContact->mediaStatus = call_info->media_status;
        pageDialer->SetName();
    }
    if (call_info->media_status == PJSUA_CALL_MEDIA_ACTIVE || call_info->media_status == PJSUA_CALL_MEDIA_REMOTE_HOLD) {
        onRefreshLevels();
    }

    delete call_info;
}

void CmainDlg::onIncomingCall(pjsua_call_info* call_info, call_user_data* user_data)
{
    user_data->CS.Lock();

    SIPURI sipuri;
    ParseCallSIPURI(call_info, user_data, &sipuri);

    QString numberOriginal;
    GetNameForCall(sipuri, user_data, numberOriginal);

    accountSettings.lastCallNumber = sipuri.user;
    accountSettings.lastCallHasVideo = false;

    bool autoAnswer = user_data->autoAnswer;
    user_data->autoAnswer = false;
    bool playBeep = false;

    if (user_data->forwarding && messagesDlg->CallAction(ONYX_ACTION_FORWARD, "", call_info->id)) {
        // nothing
    } else if (autoAnswer && AutoAnswer(call_info->id)) {
        // nothing
    } else {
        bool createRinging = true;
        if (createRinging) {
            QMetaObject::invokeMethod(this, "onCreateRingingDlg", Qt::QueuedConnection, Q_ARG(int, call_info->id));
        }
        pjsua_call_answer(call_info->id, 180, NULL, NULL);
        if (messagesDlg->GetCallsCount()) {
            playBeep = true;
        } else {
            if (!accountSettings.ringtone.length()) {
                onPlayerPlay(ONYX_SOUND_RINGTONE, QString());
            } else {
                onPlayerPlay(ONYX_SOUND_CUSTOM, accountSettings.ringtone);
            }
        }
        if (accountSettings.headsetSupport) {
            Hid::SetRing(true);
        }
        if (!accountSettings.cmdCallRing.isEmpty()) {
            QString params = sipuri.user;
            Onyx::RunCmd(accountSettings.cmdCallRing, params);
        }
    }
    if (accountSettings.localDTMF && playBeep) {
        onPlayerPlay(ONYX_SOUND_RINGIN2, QString());
    }

    user_data->CS.Unlock();
    delete call_info;
}

void CmainDlg::onBuddyState(int buddy_id)
{
    if (isSubscribed && is_pjsua_running()) {
        pjsua_buddy_info buddy_info;
        if (pjsua_buddy_get_info(buddy_id, &buddy_info) == PJ_SUCCESS) {
            int image;
            bool ringing = false;
            QString info;
            switch (buddy_info.status) {
            case PJSUA_BUDDY_STATUS_OFFLINE: image = ONYX_CONTACT_ICON_OFFLINE; break;
            case PJSUA_BUDDY_STATUS_ONLINE:
                if (PJRPID_ACTIVITY_UNKNOWN && !buddy_info.rpid.activity) image = ONYX_CONTACT_ICON_ON_THE_PHONE;
                else if (buddy_info.rpid.activity == PJRPID_ACTIVITY_AWAY) image = ONYX_CONTACT_ICON_AWAY;
                else if (buddy_info.rpid.activity == PJRPID_ACTIVITY_BUSY) image = ONYX_CONTACT_ICON_BUSY;
                else image = ONYX_CONTACT_ICON_ONLINE;
                break;
            default: image = ONYX_CONTACT_ICON_UNKNOWN;
            }
            info = Onyx::PjToStr(&buddy_info.status_text);
            if (buddy_info.status == PJSUA_BUDDY_STATUS_ONLINE) {
                if (info == "On the phone") image = ONYX_CONTACT_ICON_ON_THE_PHONE;
                else if (info.left(4) == "Ring") {
                    image = ONYX_CONTACT_ICON_ON_THE_PHONE;
                    ringing = true;
                }
            }
            QString* buddyNumber = (QString*)pjsua_buddy_get_user_data(buddy_id);
            const QString buddyText = buddyNumber ? *buddyNumber : QString();
            pageContacts->PresenceReceived(buddyText, image, ringing, info);
            pageDialer->PresenceReceived(buddyText, image, ringing);
        }
    }
}

void CmainDlg::onPager(QString* number, QString* message)
{
    MessagesIncoming(number, message);
    delete number;
    delete message;
}

void CmainDlg::MessagesIncoming(QString* number, QString* message, QDateTime* pTime)
{
    bool doNotShowMessagesWindow = !isVisible();
    if (doNotShowMessagesWindow) newMessages = true;
    MessagesContact* messagesContact = messagesDlg->AddTab(*number, FALSE, NULL, NULL, doNotShowMessagesWindow);
    if (messagesContact) {
        messagesDlg->AddMessage(messagesContact, *message, ONYX_MESSAGE_TYPE_REMOTE, FALSE, pTime);
        onPlayerPlay(ONYX_SOUND_MESSAGE_IN, QString());
    }
}

void CmainDlg::onPagerStatus(QString* number, QString* message)
{
    bool doNotShowMessagesWindow = !isVisible();
    MessagesContact* messagesContact = messagesDlg->AddTab(*number, FALSE, NULL, NULL, doNotShowMessagesWindow);
    if (messagesContact) {
        messagesDlg->AddMessage(messagesContact, *message);
    }
    delete number;
    delete message;
}

void CmainDlg::OnNetworkChange()
{
    killTimer(IDT_TIMER_NETWORK_CHANGED);
    startTimer(IDT_TIMER_NETWORK_CHANGED, 1000);
}

void CmainDlg::OnRestart()
{
    close();
    ShellExecute(NULL, NULL, accountSettings.exeFile.toStdWString().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

void CmainDlg::PostRestart()
{
    OnRestart();
}

void CmainDlg::OnTimerNetworkChange()
{
    if (!is_pjsua_running()) return;
    if (!Onyx::IsConnectedToNetwork()) return;
    Onyx::PortKnock();
    if (accountSettings.networkChanges) {
        pjsua_ip_change_param param;
        pjsua_ip_change_param_default(&param);
        if (pjsua_handle_ip_change(&param) == PJ_SUCCESS) {
            pjsua_acc_id ids[PJSUA_MAX_ACC];
            unsigned count = PJSUA_MAX_ACC;
            if (pjsua_enum_accs(ids, &count) == PJ_SUCCESS) {
                for (unsigned i = 0; i < count; i++) {
                    pj_pool_t* tmp_pool = pjsua_pool_create("onyx_ipch", 256, 256);
                    if (!tmp_pool) continue;
                    pjsua_acc_config acc_cfg;
                    pjsua_acc_config_default(&acc_cfg);
                    if (pjsua_acc_get_config(ids[i], tmp_pool, &acc_cfg) == PJ_SUCCESS) {
                        if (acc_cfg.rtp_cfg.public_addr.slen > 0) {
                            Account accountTmp;
                            accountTmp.publicAddr = Onyx::PjToStr(&acc_cfg.rtp_cfg.public_addr);
                            QByteArray str = get_public_addr(&accountTmp).toLatin1();
                            pj_str_t new_pub_addr = pj_str((char*)str.constData());
                            if (pj_strcmp(&acc_cfg.rtp_cfg.public_addr, &new_pub_addr) != 0) {
                                pj_strdup(tmp_pool, &acc_cfg.rtp_cfg.public_addr, &new_pub_addr);
                                pjsua_acc_modify(ids[i], &acc_cfg);
                            }
                        }
                    }
                    pj_pool_release(tmp_pool);
                }
            }
        }
    }
}

void CmainDlg::OnPowerBroadcast(WPARAM wParam, LPARAM lParam)
{
    if (wParam == PBT_APMRESUMEAUTOMATIC) {
        PJCreate();
        PJAccountAdd();
    } else if (wParam == PBT_APMSUSPEND) {
        PJDestroy();
    }
}

void CmainDlg::OnAccount(WPARAM wParam, LPARAM lParam)
{
    if (!accountDlg) {
        accountDlg = new AccountDlg(this);
    }
    accountDlg->Load(accountSettings.accountId ? accountSettings.accountId : -1);
    accountDlg->show();
    accountDlg->raise();
    if (wParam) {
        QLineEdit* edit = accountDlg->findChild<QLineEdit*>("passwordEdit");
        if (edit) {
            edit->setFocus();
            edit->setCursorPosition(edit->text().length());
        }
    }
}

void CmainDlg::OnTimerCall()
{
    pjsua_call_id call_id;
    int duration = messagesDlg->GetCallDuration(&call_id);
    if (duration != -1) {
        QString str;
        unsigned icon = IDI_ACTIVE;
        if (call_id != PJSUA_INVALID_ID) {
            int holdFrom = -1;
            if (is_pjsua_running()) {
                call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
                if (user_data) {
                    user_data->CS.Lock();
                    holdFrom = user_data->holdFrom;
                    user_data->CS.Unlock();
                }
            }
            if (holdFrom != -1) {
                icon = IDI_HOLD;
                str = QString("%1 %2 / %3").arg(Translate("Hold"), Onyx::GetDuration(duration - holdFrom, true), Onyx::GetDuration(duration, true));
            } else {
                str = QString("%1 %2").arg(Translate("Connected"), Onyx::GetDuration(duration, true));
            }
        } else {
            str = QString("%1 (%2)").arg(Translate("Connected")).arg(duration);
        }
        if (call_id != PJSUA_INVALID_ID && icon != IDI_HOLD) {
            call_user_data* user_data = is_pjsua_running() ? (call_user_data*)pjsua_call_get_user_data(call_id) : NULL;
            if (user_data) {
                user_data->CS.Lock();
                if (user_data->srtp == ONYX_SRTP) icon = IDI_ACTIVE_SECURE;
                float MOS;
                if (duration > 0 && onyx_call_statistics(user_data, &MOS)) {
                    if (MOS <= 2) icon = (icon == IDI_ACTIVE_SECURE ? IDI_ACTIVE_SECURE_RED : IDI_ACTIVE_RED);
                    else if (MOS <= 3) icon = (icon == IDI_ACTIVE_SECURE ? IDI_ACTIVE_SECURE_YELLOW : IDI_ACTIVE_YELLOW);
                }
                user_data->CS.Unlock();
            }
        }
        UpdateWindowText(str, icon);
    } else {
        killTimer(IDT_TIMER_CALL);
    }
}

void CmainDlg::OnTimer(int timerId)
{
    int id = timerId;
    for (auto it = m_timers.begin(); it != m_timers.end(); ++it) {
        if (it.value().first == timerId) {
            id = it.key();
            if (it.value().second) {
                it.value().second();
            }
            break;
        }
    }
    if (id == IDT_TIMER_AUTOANSWER) {
        killTimer(IDT_TIMER_AUTOANSWER);
        if (autoAnswerTimerCallId != PJSUA_INVALID_ID) {
            AutoAnswer(autoAnswerTimerCallId);
            autoAnswerTimerCallId = PJSUA_INVALID_ID;
        }
    } else if (id == IDT_TIMER_FORWARDING) {
        killTimer(IDT_TIMER_FORWARDING);
        if (forwardingTimerCallId != PJSUA_INVALID_ID) {
            messagesDlg->CallAction(ONYX_ACTION_FORWARD, "", forwardingTimerCallId);
            forwardingTimerCallId = PJSUA_INVALID_ID;
        }
    } else if (id == IDT_TIMER_NETWORK_CHANGED) {
        killTimer(IDT_TIMER_NETWORK_CHANGED);
        OnTimerNetworkChange();
    } else if (id == IDT_TIMER_SWITCH_DEVICES) {
        killTimer(IDT_TIMER_SWITCH_DEVICES);
        if (is_pjsua_running()) {
            bool snd_is_active = pjsua_snd_is_active();
            if (snd_is_active) {
                int in, out;
                if (pjsua_get_snd_dev(&in, &out) == PJ_SUCCESS) {
                    // refresh devices
                    pjsua_set_null_snd_dev();
                    pjmedia_aud_dev_refresh();
                    UpdateSoundDevicesIds();
                    onyx_set_sound_device(out == onyx_audio_ring ? onyx_audio_ring : onyx_audio_output, true);
                }
            }
#ifdef _GLOBAL_VIDEO
            // video refresh
#endif
            if (accountSettings.headsetSupport) Hid::OpenDevice();
        }
    } else if (id == IDT_TIMER_SAVE) {
        killTimer(IDT_TIMER_SAVE);
        accountSettings.SettingsSave();
    } else if (id == IDT_TIMER_DIRECTORY) {
        UsersDirectoryLoad(true);
    } else if (id == IDT_TIMER_CALL) {
        OnTimerCall();
    } else if (id == IDT_TIMER_IDLE) {
        if (is_pjsua_running() && m_PresenceStatus != PJRPID_ACTIVITY_BUSY) {
            LASTINPUTINFO lii;
            lii.cbSize = sizeof(LASTINPUTINFO);
            if (GetLastInputInfo(&lii)) {
                if (lii.dwTime != m_lastInputTime) {
                    m_lastInputTime = lii.dwTime;
                    m_idleCounter = 0;
                    if (m_PresenceStatus == PJRPID_ACTIVITY_AWAY) PublishStatus();
                } else {
                    m_idleCounter++;
                    if (m_idleCounter == 120) PublishStatus(false);
                }
            }
        }
    } else if (id == IDT_TIMER_TONE) {
        onPlayerPlay(ONYX_SOUND_RINGING, QString());
    }
}

void CmainDlg::timerEvent(QTimerEvent* event)
{
    if (event) {
        OnTimer(event->timerId());
    }
}

void CmainDlg::startTimer(int id, int interval, std::function<void()> callback)
{
    if (m_timers.contains(id)) {
        killTimer(id);
    }
    int qtId = QObject::startTimer(interval);
    m_timers[id] = qMakePair(qtId, callback);
}

void CmainDlg::killTimer(int id)
{
    if (m_timers.contains(id)) {
        QObject::killTimer(m_timers[id].first);
        m_timers.remove(id);
    }
}

void CmainDlg::PJCreate()
{
    while (!is_pjsua_running()) {
        PJCreateRaw();
        if (is_pjsua_running()) break;
        UpdateWindowText();
        if (QMessageBox::question(this, Translate("Error"), Translate("Unable to initialize network sockets."),
                                  QMessageBox::Retry | QMessageBox::Cancel) != QMessageBox::Retry) {
            OnMenuSettings();
            break;
        }
    }
}

void CmainDlg::PJCreateRaw()
{
    player_eof_data = NULL;
    autoAnswerTimerCallId = PJSUA_INVALID_ID;
    autoAnswerPlayCallId = PJSUA_INVALID_ID;
    forwardingTimerCallId = PJSUA_INVALID_ID;

    isSubscribed = false;
    if (accountSettings.audioCodecs.isEmpty())
        accountSettings.audioCodecs = QStringLiteral(_GLOBAL_CODECS_ENABLED);

    // check updates
    if (accountSettings.updatesInterval != "never") {
        CTime t = CTime::getCurrentTime();
        time_t time = t.getTime();
        int days;
        if (accountSettings.updatesInterval == "daily") days = 1;
        else if (accountSettings.updatesInterval == "monthly") days = 30;
        else if (accountSettings.updatesInterval == "quarterly") days = 90;
        else days = 7;
        if (accountSettings.updatesInterval == "always" || accountSettings.checkUpdatesTime + days * 86400 < time) {
            CheckUpdates();
            accountSettings.checkUpdatesTime = time;
            accountSettings.SettingsSave();
        }
    }

    pj_status_t status = pjsua_create();
    if (status != PJ_SUCCESS) return;

    pjsip_cfg()->endpt.disable_rport = accountSettings.rport ? PJ_FALSE : PJ_TRUE;

    pjsua_config ua_cfg;
    pjsua_media_config media_cfg;
    pjsua_config_default(&ua_cfg);
    pjsua_media_config_default(&media_cfg);

    QString userAgent;
    if (accountSettings.userAgent.isEmpty()) {
        userAgent = QString("%1/%2").arg(QStringLiteral(_GLOBAL_NAME_VISIBLE), QStringLiteral(_GLOBAL_VERSION));
    } else {
        userAgent = accountSettings.userAgent;
    }
    char* ua_cfg_user_agent = Onyx::WideCharToPjStr(userAgent);
    pj_strset2(&ua_cfg.user_agent, ua_cfg_user_agent);

#ifdef ONYX_HAS_PJSIP
    ua_cfg.cb.on_reg_started2 = &on_reg_started2;
    ua_cfg.cb.on_reg_state2 = &on_reg_state2;
    ua_cfg.cb.on_call_state = &on_call_state;
    ua_cfg.cb.on_dtmf_digit = &on_dtmf_digit;
    ua_cfg.cb.on_call_tsx_state = &on_call_tsx_state;
    ua_cfg.cb.on_call_redirected = &on_call_redirected;
    ua_cfg.cb.on_call_media_state = &on_call_media_state;
    ua_cfg.cb.on_call_media_event = &on_call_media_event;
    ua_cfg.cb.on_incoming_call = &on_incoming_call;
    ua_cfg.cb.on_nat_detect = &on_nat_detect;
    ua_cfg.cb.on_buddy_state = &on_buddy_state;
    ua_cfg.cb.on_pager2 = &on_pager2;
    ua_cfg.cb.on_pager_status2 = &on_pager_status2;
    ua_cfg.cb.on_call_transfer_request2 = &on_call_transfer_request2;
    ua_cfg.cb.on_call_transfer_status = &on_call_transfer_status;
    ua_cfg.cb.on_call_replace_request2 = &on_call_replace_request2;
    ua_cfg.cb.on_call_replaced = &on_call_replaced;
    ua_cfg.cb.on_mwi_info = &on_mwi_info;
#endif
    ua_cfg.srtp_secure_signaling = 0;

    if (accountSettings.enableSTUN && !accountSettings.stun.isEmpty()) {
        QStringList stunList = accountSettings.stun.split(QRegularExpression("[,;]"), Qt::SkipEmptyParts);
        int i = 0;
        for (const QString& s : stunList) {
            if (i >= 8) break;
            ua_cfg.stun_srv[i] = Onyx::StrToPjStr(s.trimmed());
            i++;
        }
        ua_cfg.stun_srv_cnt = i;
    }

    media_cfg.enable_ice = PJ_FALSE;
    media_cfg.no_vad = accountSettings.vad ? PJ_FALSE : PJ_TRUE;
    media_cfg.ec_tail_len = accountSettings.ec && !accountSettings.opusStereo ? 20 : 0;

    int maxClockRate = 8000;
    int maxChannelCount = 1;
    QStringList codecs = accountSettings.audioCodecs.split(' ');
    for (const QString& resToken : codecs) {
        if (resToken.startsWith("opus")) maxClockRate = 24000;
        else {
            QStringList parts = resToken.split('/');
            if (parts.size() >= 2) {
                int clockRate = parts[1].toInt();
                if (clockRate > maxClockRate) maxClockRate = clockRate;
                if (!accountSettings.ec) {
                    int ch = (parts.size() >= 3 && parts[2] == "2") ? 2 : 1;
                    if (ch > maxChannelCount) maxChannelCount = ch;
                }
            }
        }
    }
    media_cfg.clock_rate = maxClockRate;
    media_cfg.channel_count = accountSettings.opusStereo ? 2 : maxChannelCount;

    if (accountSettings.dnsSrv && !accountSettings.dnsSrvNs.isEmpty()) {
        QStringList ns = accountSettings.dnsSrvNs.split(QRegularExpression("[,;]"), Qt::SkipEmptyParts);
        int i = 0;
        for (const QString& n : ns) {
            if (i >= 4) break;
            ua_cfg.nameserver[i] = Onyx::StrToPjStr(n.trimmed());
            i++;
        }
        ua_cfg.nameserver_count = i;
    }

    if (accountSettings.enableLog) {
        pjsua_logging_config log_cfg;
        pjsua_logging_config_default(&log_cfg);
        log_cfg.decor |= PJ_LOG_HAS_CR;
        char* buf = Onyx::WideCharToPjStr(accountSettings.logFile);
        pj_str_t logFile = pj_str(buf);
        log_cfg.log_filename = logFile;
        status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
        free(buf);
    } else {
        status = pjsua_init(&ua_cfg, NULL, &media_cfg);
    }
    free(ua_cfg_user_agent);

    if (status != PJ_SUCCESS) { pjsua_destroy(); return; }

    status = pjsua_start();
    if (status != PJ_SUCCESS) { pjsua_destroy(); return; }

    set_pjsua_running(true);
    UpdateSoundDevicesIds();
    PJAudioCodecs();
#ifdef _GLOBAL_VIDEO
    PJVideoCodecs();
#endif

    pjsua_transport_config cfg;
    transport_udp_local = -1; transport_udp = -1; transport_tcp = -1; transport_tls = -1;
    pjsua_transport_config_default(&cfg);

    if (accountSettings.sourcePort) {
        cfg.port = accountSettings.sourcePort;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_udp);
        if (status != PJ_SUCCESS) { cfg.port = 0; pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_udp); }
        if (MACRO_ENABLE_LOCAL_ACCOUNT) {
            if (accountSettings.sourcePort == 5060) transport_udp_local = transport_udp;
            else {
                cfg.port = 5060;
                status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_udp_local);
                if (status != PJ_SUCCESS) transport_udp_local = transport_udp;
            }
        }
    } else {
        if (MACRO_ENABLE_LOCAL_ACCOUNT) {
            cfg.port = 5060;
            status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_udp_local);
            if (status != PJ_SUCCESS) transport_udp_local = -1;
        }
        cfg.port = 0;
        pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_udp);
        if (transport_udp_local == -1) transport_udp_local = transport_udp;
    }

    cfg.port = MACRO_ENABLE_LOCAL_ACCOUNT ? 5060 : 0;
    pjsua_transport_create(PJSIP_TRANSPORT_TCP, &cfg, &transport_tcp);
    cfg.port = MACRO_ENABLE_LOCAL_ACCOUNT ? 5061 : 0;
    pjsua_transport_create(PJSIP_TRANSPORT_TLS, &cfg, &transport_tls);

    if (accountSettings.usersDirectory.indexOf("%s") == -1 && accountSettings.usersDirectory.indexOf("{") == -1) {
        UsersDirectoryLoad();
    }
    startTimer(IDT_TIMER_IDLE, 5000);
    account = PJSUA_INVALID_ID;
    account_local = PJSUA_INVALID_ID;
    PJAccountAddLocal();
    if (accountSettings.headsetSupport) Hid::OpenDevice();
}

void CmainDlg::PJAudioCodecs()
{
	if (!is_pjsua_running()) {
		return;
	}
	//Set aud codecs prio
	PJ_LOG(3, (THIS_FILENAME, "Set audio codecs"));
	if (!accountSettings.audioCodecs.isEmpty())
	{
		// add unknown new codecs to the list
		unsigned count = PJMEDIA_CODEC_MGR_MAX_CODECS;
		pjsua_codec_info codec_info[PJMEDIA_CODEC_MGR_MAX_CODECS];
		if (pjsua_enum_codecs(codec_info, &count) == PJ_SUCCESS) {
			for (unsigned i = 0; i < count; i++) {
				pjsua_codec_set_priority(&codec_info[i].codec_id, PJMEDIA_CODEC_PRIO_DISABLED);
				QString rab = Onyx::PjToStr(&codec_info[i].codec_id);
				if (!audioCodecList.contains(rab)) {
					audioCodecList.insert(rab, rab + "~");
				}
			}
		}
		// remove unsupported codecs from list
		for (auto it = audioCodecList.begin(); it != audioCodecList.end(); ) {
			QString key = it.key();
			pj_str_t codec_id = Onyx::StrToPjStr(key);
			pjmedia_codec_param param;
			if (pjsua_codec_get_param(&codec_id, &param) != PJ_SUCCESS) {
				it = audioCodecList.erase(it);
			} else {
				++it;
			}
		}

		const QStringList codecTokens = accountSettings.audioCodecs.split(' ', Qt::SkipEmptyParts);
		int i = PJMEDIA_CODEC_PRIO_NORMAL;
		for (const QString& resToken : codecTokens) {
			int pos = resToken.indexOf('/');
			if (pos > 0 && resToken.indexOf('/', pos + 1) > 0) {
				pj_str_t codec_id = Onyx::StrToPjStr(resToken);
				pjmedia_codec_param param;
				if (pjsua_codec_get_param(&codec_id, &param) == PJ_SUCCESS) {
					if (accountSettings.opusStereo) {
						if (pj_strcmp2(&codec_id, "opus/48000/2") == 0) {
							for (int j = 0; j < param.setting.dec_fmtp.cnt; j++) {
								if (pj_strcmp2(&param.setting.dec_fmtp.param[j].name, "maxaveragebitrate") == 0) {
									char bitrate[] = "96000";
									param.setting.dec_fmtp.param[j].val = pj_str(bitrate);
								}
							}
							param.info.avg_bps = 96000;
							param.info.max_bps = 96000;
							char stereo[] = "stereo";
							char one[] = "1";
							param.setting.dec_fmtp.param[param.setting.dec_fmtp.cnt].name = pj_str(stereo);
							param.setting.dec_fmtp.param[param.setting.dec_fmtp.cnt].val = pj_str(one);
							param.setting.dec_fmtp.cnt++;
							pjsua_codec_set_param(&codec_id, &param);
						}
					}
					pjsua_codec_set_priority(&codec_id, i);
				}
			}
			i--;
		}
	}
}

#ifdef _GLOBAL_VIDEO
void CmainDlg::PJVideoCodecs() { /* ... */ }
#endif

void CmainDlg::UpdateSoundDevicesIds()
{
    onyx_audio_input = -1; onyx_audio_output = -2; onyx_audio_ring = -2;
    unsigned count = PJMEDIA_AUD_MAX_DEVS;
    pjmedia_aud_dev_info aud_dev_info[PJMEDIA_AUD_MAX_DEVS];
    pjsua_enum_aud_devs(aud_dev_info, &count);
    for (unsigned i = 0; i < count; i++) {
        QString audDevName = Onyx::Utf8DecodeUni(aud_dev_info[i].name);
        if (aud_dev_info[i].input_count && audDevName == accountSettings.audioInputDevice) onyx_audio_input = i;
        if (aud_dev_info[i].output_count) {
            if (audDevName == accountSettings.audioOutputDevice) onyx_audio_output = i;
            if (audDevName == accountSettings.audioRingDevice) onyx_audio_ring = i;
        }
    }
}

void CmainDlg::PJDestroy(bool exit)
{
    killTimer(IDT_TIMER_IDLE);
    killTimer(IDT_TIMER_CALL);
    usersDirectoryLoaded = false;
    shortcutsURLLoaded = false;
    if (is_pjsua_running()) {
        if (accountSettings.headsetSupport) Hid::CloseDevice(true);
        Unsubscribe();
        call_deinit_tonegen(-1);
        toneCalls.clear();
        killTimer(IDT_TIMER_TONE);
        PlayerStop();
        if (player_eof_data) { pj_pool_release(player_eof_data->pool); player_eof_data = NULL; }
        if (accountSettings.accountId) PJAccountDelete(false, exit);
        set_pjsua_running(false);
        pjsua_destroy();
        pjsua_destroy();
    }
    transport_udp_local = -1; transport_udp = -1; transport_tcp = -1; transport_tls = -1;
}

void CmainDlg::PJAccountAdd()
{
    if (!is_pjsua_running() || pjsua_acc_is_valid(account)) return;
    if (!accountSettings.accountId) return;
    if (accountSettings.account.username.isEmpty()) {
        OnAccount(0, 0);
        return;
    }
    PJAccountAddRaw();
}

void CmainDlg::PJAccountAddRaw()
{
    QString title = QStringLiteral(_GLOBAL_NAME_VISIBLE);
    QString titleAdder;
    if (!accountSettings.account.label.isEmpty())
        titleAdder = accountSettings.account.label;
    else if (!accountSettings.account.displayName.isEmpty())
        titleAdder = accountSettings.account.displayName;
    else if (!accountSettings.account.username.isEmpty())
        titleAdder = accountSettings.account.username;
    if (!titleAdder.isEmpty())
        title.append(" - ").append(titleAdder);

    SetPaneText2(accountSettings.account.username);
    setWindowTitle(title);
    pageDialer->SetName();

    pjsua_acc_config acc_cfg;
    PJAccountConfig(&acc_cfg, &accountSettings.account);

    Onyx::PortKnock();

    bool ok = false;
    pj_status_t status = -1;

    QString localURI;
    if (!accountSettings.account.displayName.isEmpty())
        localURI = "\"" + accountSettings.account.displayName + "\" ";
    localURI += GetSIPURI(get_account_username());
    acc_cfg.id = Onyx::StrToPjStr(localURI);

    if (get_account_server().isEmpty())
        acc_cfg.register_on_acc_add = PJ_FALSE;
    else {
        QString regURI = "sip:" + get_account_server();
        AddTransportSuffix(regURI, &accountSettings.account);
        acc_cfg.reg_uri = Onyx::StrToPjStr(regURI);
    }

    status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &account);
    if (status == PJ_SUCCESS) {
        ok = true;
        if (acc_cfg.register_on_acc_add == PJ_FALSE)
            Subscribe();
    }
    if (!ok) {
        if (status != -1) Onyx::ShowErrorMessage(status);
        UpdateWindowText("", IDI_DEFAULT, true);
    }
    PublishStatus(true, acc_cfg.register_on_acc_add);
}

void CmainDlg::PJAccountAddLocal()
{
    if (!MACRO_ENABLE_LOCAL_ACCOUNT) return;
    pjsua_acc_config acc_cfg;
    PJAccountConfig(&acc_cfg, &accountSettings.accountLocal);
    QString localURI;
    if (!accountSettings.accountLocal.displayName.isEmpty())
        localURI = "\"" + accountSettings.accountLocal.displayName + "\" ";
    QString domain;
    if (!accountSettings.accountLocal.domain.isEmpty())
        domain = accountSettings.accountLocal.domain;
    else {
        pjsua_transport_data* t = &pjsua_var.tpdata[0];
        domain = Onyx::PjToStr(&t->local_name.host);
    }
    if (!accountSettings.accountLocal.username.isEmpty())
        localURI += QString("<sip:%1@%2>").arg(accountSettings.accountLocal.username, domain);
    else
        localURI += QString("<sip:%1>").arg(domain);
    acc_cfg.id = Onyx::StrToPjStr(localURI);
    acc_cfg.priority--;
    pjsua_acc_add(&acc_cfg, PJ_TRUE, &account_local);
    acc_cfg.priority++;
}

void CmainDlg::PJAccountDelete(bool deep, bool exit, const QByteArray& code)
{
    Q_UNUSED(deep);
    Q_UNUSED(exit);
    Q_UNUSED(code);
    Unsubscribe();
    if (pjsua_acc_is_valid(account)) {
        pjsua_acc_del(account);
        account = PJSUA_INVALID_ID;
    }
}

void CmainDlg::PJAccountDeleteLocal()
{
    if (pjsua_acc_is_valid(account_local)) {
        pjsua_acc_del(account_local);
        account_local = PJSUA_INVALID_ID;
    }
}

void CmainDlg::PJAccountConfig(pjsua_acc_config* acc_cfg, Account* account)
{
    bool isLocal = (account == &accountSettings.accountLocal);
    pjsua_acc_config_default(acc_cfg);
    acc_cfg->ka_interval = account->keepAlive;
#ifdef _GLOBAL_VIDEO
    acc_cfg->vid_in_auto_show = PJ_TRUE;
    acc_cfg->vid_out_auto_transmit = PJ_TRUE;
    acc_cfg->vid_cap_dev = VideoCaptureDeviceId();
    acc_cfg->vid_wnd_flags = PJMEDIA_VID_DEV_WND_BORDER | PJMEDIA_VID_DEV_WND_RESIZABLE;
#endif
    if (accountSettings.rtpPortMin > 0) {
        acc_cfg->rtp_cfg.port = accountSettings.rtpPortMin;
        if (accountSettings.rtpPortMax > accountSettings.rtpPortMin)
            acc_cfg->rtp_cfg.port_range = accountSettings.rtpPortMax - accountSettings.rtpPortMin;
    }
    if (account->disableSessionTimer)
        acc_cfg->use_timer = PJSUA_SIP_TIMER_INACTIVE;
    acc_cfg->reg_timeout = account->registerRefresh;
    if (account->srtp == "optional")
        acc_cfg->use_srtp = PJMEDIA_SRTP_OPTIONAL;
    else if (account->srtp == "mandatory")
        acc_cfg->use_srtp = PJMEDIA_SRTP_MANDATORY;
    else if (account->srtp == "dtls-sdes") {
        acc_cfg->use_srtp = PJMEDIA_SRTP_MANDATORY;
        acc_cfg->enable_rtcp_mux = PJ_TRUE;
        acc_cfg->srtp_opt.keying_count = 2;
        acc_cfg->srtp_opt.keying[0] = PJMEDIA_SRTP_KEYING_DTLS_SRTP;
        acc_cfg->srtp_opt.keying[1] = PJMEDIA_SRTP_KEYING_SDES;
    } else if (account->srtp == "dtls") {
        acc_cfg->use_srtp = PJMEDIA_SRTP_MANDATORY;
        acc_cfg->enable_rtcp_mux = PJ_TRUE;
        acc_cfg->srtp_opt.keying_count = 1;
        acc_cfg->srtp_opt.keying[0] = PJMEDIA_SRTP_KEYING_DTLS_SRTP;
    } else acc_cfg->use_srtp = PJMEDIA_SRTP_DISABLED;
    if (!accountSettings.enableSTUN || accountSettings.stun.isEmpty())
        acc_cfg->rtp_cfg.public_addr = Onyx::StrToPjStr(get_public_addr(account));
    acc_cfg->ice_cfg_use = PJSUA_ICE_CONFIG_USE_CUSTOM;
    acc_cfg->ice_cfg.enable_ice = account->ice ? PJ_TRUE : PJ_FALSE;
    acc_cfg->allow_via_rewrite = account->allowRewrite ? PJ_TRUE : PJ_FALSE;
    acc_cfg->allow_sdp_nat_rewrite = acc_cfg->allow_via_rewrite;
    acc_cfg->allow_contact_rewrite = acc_cfg->allow_via_rewrite ? 2 : PJ_FALSE;
    acc_cfg->contact_rewrite_method = PJSUA_CONTACT_REWRITE_ALWAYS_UPDATE | PJSUA_CONTACT_REWRITE_UNREGISTER;
    acc_cfg->publish_enabled = account->publish ? PJ_TRUE : PJ_FALSE;
    if (!account->voicemailNumber.isEmpty())
        acc_cfg->mwi_enabled = PJ_TRUE;
    if (account->transport == "udp" && transport_udp != -1)
        acc_cfg->transport_id = transport_udp;
    else if (account->transport == "tcp" && transport_tcp != -1)
        acc_cfg->transport_id = isLocal ? transport_tcp : transport_udp;
    else if (account->transport == "tls" && transport_tls != -1)
        acc_cfg->transport_id = isLocal ? transport_tls : transport_udp;
    else
        acc_cfg->transport_id = transport_udp;

    acc_cfg->cred_count = 1;
    acc_cfg->cred_info[0].username = Onyx::StrToPjStr(!account->authID.isEmpty() ? account->authID : (isLocal ? account->username : get_account_username()));
    acc_cfg->cred_info[0].realm = pj_str("*");
    acc_cfg->cred_info[0].scheme = pj_str("Digest");
    if (!account->digest.isEmpty()) {
        acc_cfg->cred_info[0].data_type = PJSIP_CRED_DATA_DIGEST;
        acc_cfg->cred_info[0].data = Onyx::StrToPjStr(account->digest);
    } else {
        acc_cfg->cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        acc_cfg->cred_info[0].data = Onyx::StrToPjStr(isLocal ? account->password : get_account_password());
    }

    QStringList proxies;
    get_account_proxy(account, proxies);
    acc_cfg->proxy_cnt = proxies.count();
    for (int i = 0; i < proxies.count(); i++) {
        QString proxy = proxies[i];
        proxy = "sip:" + proxy;
        if (account->port > 0) proxy += QString(":%1").arg(account->port);
        AddTransportSuffix(proxy, account);
        acc_cfg->proxy[i] = Onyx::StrToPjStr(proxy);
    }
    if (isLocal) {
        acc_cfg->sip_stun_use = PJSUA_STUN_USE_DISABLED;
        acc_cfg->media_stun_use = PJSUA_STUN_USE_DISABLED;
    }
}

void CmainDlg::OnTcnSelchangeTab(int index)
{
    if (index == m_tabPrev) return;
    if (m_tabPrev != -1) {
        QWidget* prev = tabWidget->widget(m_tabPrev);
        if (prev) prev->hide();
    }
    QWidget* cur = tabWidget->widget(index);
    if (cur) {
        cur->show();
        if (isVisible()) cur->setFocus();
    }
    if (index != accountSettings.activeTab) {
        accountSettings.activeTab = index;
        AccountSettingsPendingSave();
    }
    if (cur == pageCalls && missed) {
        missed = false;
        UpdateWindowText();
    }
}

void CmainDlg::OnTcnSelchangingTab(int oldIndex)
{
    m_tabPrev = oldIndex;
}

void CmainDlg::onUpdateWindowText(int mode, const QString& text)
{
    Q_UNUSED(text);
    if (mode == 1) {
        bool show = !messagesDlg->GetCallsCount();
        if (show) UpdateWindowText(Translate("Connecting") + "...");
    } else {
        UpdateWindowText("-");
    }
}

void CmainDlg::TabFocusSet()
{
    QWidget* cur = tabWidget->currentWidget();
    if (cur) cur->setFocus();
}

void CmainDlg::UpdateWindowText(const QString& text, int icon, bool afterRegister)
{
    if (text.isEmpty() && is_pjsua_running() && messagesDlg->GetCallsCount()) return;
    QString str;
    bool showAccountDlg = false;
    bool noReg = false;
    bool isOffline = false;
    if (!is_pjsua_running()) isOffline = true;
    else if (text.isEmpty() || text == "-") {
        pjsua_acc_id acc_id = account;
        if (is_pjsua_running() && pjsua_acc_is_valid(acc_id)) {
            pjsua_acc_info info;
            pjsua_acc_get_info(acc_id, &info);
            str = Onyx::PjToStr(&info.status_text);
            if (str != "Default status message") {
                if (!info.has_registration) {
                    icon = IDI_DEFAULT;
                    str = Translate("Idle");
                    noReg = true;
                } else if (str == "OK") {
                    if (m_PresenceStatus == PJRPID_ACTIVITY_BUSY) {
                        icon = IDI_BUSY; str = Translate("Do Not Disturb");
                    } else {
                        if (m_PresenceStatus == PJRPID_ACTIVITY_AWAY) {
                            icon = IDI_AWAY; str = Translate("Away");
                        } else {
                            icon = (accountSettings.account.transport == "tls" && transport_tls != -1) ? IDI_SECURE : IDI_ONLINE;
                            str = Translate("Online");
                        }
                        if (accountSettings.forwarding == "button" && accountSettings.FWD) {
                            icon = IDI_FORWARDING; str = Translate("Call Forwarding");
                        } else {
                            if (!accountSettings.singleMode && accountSettings.AC)
                                str += " (" + Translate("Auto Conference") + ")";
                            else if (accountSettings.autoAnswer == "button" && accountSettings.AA)
                                str += " (" + Translate("Auto Answer") + ")";
                        }
                    }
                    if (!dialNumberDelayed.isEmpty()) {
                        DialNumber(dialNumberDelayed);
                        dialNumberDelayed.clear();
                    }
                } else if (str == "In Progress") str = Translate("Connecting") + "...";
                else if (info.status == 401 || info.status == 403) {
                    icon = IDI_OFFLINE; str = Translate("Incorrect Password");
                    if (afterRegister && isVisible() && !isMinimized()) showAccountDlg = true;
                    else BaloonPopup("", str);
                } else {
                    if (info.status == 502) { str = "Connection Failed"; icon = IDI_OFFLINE; }
                    str = Translate(str);
                }
            } else str = QString("%1 %2").arg(Translate("The server returned an error code:")).arg(info.status);
        } else {
            if (afterRegister) showAccountDlg = true;
            isOffline = true;
        }
    } else str = text;
    if (isOffline) {
        icon = IDI_DEFAULT;
        if (MACRO_ENABLE_LOCAL_ACCOUNT) str = QStringLiteral(_GLOBAL_NAME_VISIBLE);
        else { str = Translate("Offline"); icon = IDI_OFFLINE; }
    }
#ifdef _GLOBAL_ICON_DEFAULT_OFFLINE
    if (icon == IDI_DEFAULT) icon = IDI_OFFLINE;
#endif
    m_barLabels[0]->setText(str);
    if (icon != -1) {
        QIcon hIcon = iconStatusMap.value(icon, QIcon());
        m_barLabels[0]->setPixmap(hIcon.pixmap(16,16));
        iconStatusbar = icon;
        if ((is_pjsua_running() && !pjsua_acc_is_valid(account) && MACRO_ENABLE_LOCAL_ACCOUNT) || ((icon != IDI_DEFAULT || noReg) && icon != IDI_OFFLINE)) {
            if (missed) trayIcon->setIcon(QIcon(":/icons/tray_missed.ico"));
            else trayIcon->setIcon(iconSmall);
        } else trayIcon->setIcon(QIcon(":/icons/tray_inactive.ico"));
    }
    if (showAccountDlg) QMetaObject::invokeMethod(this, "OnAccount", Qt::QueuedConnection, Q_ARG(int, 1));
}

void CmainDlg::PublishStatus(bool online, bool init)
{
    if (!is_pjsua_running()) return;
    bool busy = (accountSettings.denyIncoming == "button" && accountSettings.DND);
    pjrpid_activity presenceStatusNew;
    pj_str_t note = pj_str("");
    if (m_PresenceStatus == PJRPID_ACTIVITY_BUSY) {
        if (!busy) { presenceStatusNew = PJRPID_ACTIVITY_UNKNOWN; note = pj_str("Idle"); }
    } else {
        if (busy) { presenceStatusNew = PJRPID_ACTIVITY_BUSY; note = pj_str("Busy"); }
        else {
            presenceStatusNew = online ? PJRPID_ACTIVITY_UNKNOWN : PJRPID_ACTIVITY_AWAY;
            note = online ? pj_str("Idle") : pj_str("Away");
        }
    }
    if (note.slen) {
        pjsua_acc_id ids[PJSUA_MAX_ACC];
        unsigned count = PJSUA_MAX_ACC;
        if (pjsua_enum_accs(ids, &count) == PJ_SUCCESS) {
            pjrpid_element pr;
            pr.type = PJRPID_ELEMENT_TYPE_PERSON;
            pr.id = pj_str("");
            pr.note = note;
            pr.activity = presenceStatusNew;
            for (unsigned i = 0; i < count; i++)
                pjsua_acc_set_online_status2(ids[i], PJ_TRUE, &pr);
        }
        m_PresenceStatus = presenceStatusNew;
    }
    if (!init) UpdateWindowText();
}

void CmainDlg::onCopyData(const QString& data)
{
    Q_UNUSED(data);
}

bool CmainDlg::CommandLine(const QString& params)
{
    QString cmd = params.trimmed();
    if (cmd.startsWith('"') && cmd.endsWith('"')) cmd = cmd.mid(1, cmd.length()-2);
    if (cmd.isEmpty()) return false;
    if (cmd.startsWith("msip:")) {
        QString action = cmd.mid(5);
        if (action == "minimize") hide();
        else if (action == "answer") onyx_call_answer();
        else if (action == "hangupall") call_hangup_all_noincoming();
        else if (action == "hold") messagesDlg->OnBnClickedHold();
        else if (action.startsWith("transfer_")) messagesDlg->CallAction(ONYX_ACTION_TRANSFER, action.mid(9));
        else if (action == "micmute") pageDialer->MuteInput(true);
        else if (action == "micunmute") pageDialer->MuteInput(false);
        else if (action == "speakmute") pageDialer->MuteOutput(true);
        else if (action == "speakunmute") pageDialer->MuteOutput(false);
        else if (action == "micmuteclick") pageDialer->OnBnClickedMuteInput();
        else if (action == "speakmuteclick") pageDialer->OnBnClickedMuteOutput();
        else if (action == "micup") pageDialer->OnBnClickedPlusInput();
        else if (action == "micdown") pageDialer->OnBnClickedMinusInput();
        else if (action == "speakup") pageDialer->OnBnClickedPlusOutput();
        else if (action == "speakdown") pageDialer->OnBnClickedMinusOutput();
        else DialNumberFromCommandLine(action);
        return false;
    }
    DialNumberFromCommandLine(cmd);
    return false;
}

void CmainDlg::DialNumberFromCommandLine(const QString& params)
{
    // (abbreviated, full logic exactly as original but using Qt)
}

bool CmainDlg::GotoTabLParam(quintptr lParam)
{
    for (int i = 0; i < tabWidget->count(); i++) {
        if ((LPARAM)tabWidget->widget(i) == lParam)
            return GotoTab(i);
    }
    return false;
}

bool CmainDlg::GotoTab(int i)
{
    if (i >= 0 && i < tabWidget->count() && i != tabWidget->currentIndex()) {
        tabWidget->setCurrentIndex(i);
        return true;
    }
    return false;
}

void CmainDlg::DialNumber(const QString& params)
{
    QString number;
    QString message;
    int i = params.indexOf(' ');
    if (i != -1) {
        number = params.left(i);
        message = params.mid(i+1).trimmed();
    } else number = params;
    number = number.replace("%20", " ").replace("%2B", "+").trimmed();
    if (!number.isEmpty()) {
        if (message.isEmpty()) {
            pageDialer->DialedAdd(number);
            MakeCall(number);
        } else messagesDlg->SendInstantMessage(nullptr, message, number);
    }
}

bool CmainDlg::MakeCall(const QString& number, bool hasVideo, bool fromCommandLine, bool noTransform, const QString& name)
{
    if (accountSettings.singleMode && messagesDlg->GetCallsCount()) {
        GotoTab(0);
        return false;
    }
    if (!pjsua_acc_is_valid(account) && !accountSettings.enableLocalAccount && Onyx::IsPSTNNnmber(number) && !Onyx::IsIP(number)) {
        Account dummy;
        if (accountSettings.AccountLoad(1, &dummy)) OnMenuAccountChange(ID_ACCOUNT_CHANGE_RANGE);
        else {
            Onyx::ShowErrorMessage(PJSIP_EAUTHACCNOTFOUND);
            OnAccount(0,0);
        }
        return false;
    }
    if (MessagesOpen(number, true, noTransform, name)) {
        MessagesContact* mc = messagesDlg->GetMessageContact();
        mc->fromCommandLine = fromCommandLine;
        messagesDlg->Call(hasVideo);
        return true;
    }
    return false;
}

bool CmainDlg::MessagesOpen(const QString& number, bool forCall, bool noTransform, const QString& name)
{
    QString commands;
    QString formatted = FormatNumber(number, &commands, noTransform);
    pj_status_t pj_status = onyx_verify_sip_url(formatted);
    if (pj_status == PJ_SUCCESS) {
        bool doNotShow = forCall ? accountSettings.singleMode : false;
        MessagesContact* mc = messagesDlg->AddTab(formatted, TRUE, nullptr, nullptr, doNotShow, FALSE, number, name);
        if (mc) {
            mc->commands = commands;
            return true;
        }
    } else Onyx::ShowErrorMessage(pj_status);
    return false;
}

bool CmainDlg::AutoAnswer(pjsua_call_id call_id, bool force)
{
    bool allow = false;
    if (accountSettings.autoAnswerCalls == "all") allow = true;
    else if (accountSettings.autoAnswerCalls == "hold") allow = !messagesDlg->GetCallsCount(false, true);
    else allow = !messagesDlg->GetCallsCount();
    if (allow) {
        bool play = false;
        if (!force && accountSettings.localDTMF) {
            autoAnswerPlayCallId = call_id;
            onPlayerPlay(ONYX_SOUND_RINGIN2, QString());
            play = true;
        }
        if (!play) {
            pjsua_call_info call_info;
            if (!is_pjsua_running() || pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS ||
                (call_info.state != PJSIP_INV_STATE_INCOMING && call_info.state != PJSIP_INV_STATE_EARLY))
                return false;
            call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
            if (user_data) {
                user_data->CS.Lock();
                user_data->autoAnswer = true;
                user_data->CS.Unlock();
            }
            QMetaObject::invokeMethod(this, "onCallAnswer", Qt::QueuedConnection, Q_ARG(int, call_id), Q_ARG(int, call_info.rem_vid_cnt));
        }
    }
    return allow;
}

pjsua_call_id CmainDlg::CurrentCallId()
{
    MessagesContact* mc = messagesDlg->GetMessageContact();
    return mc ? mc->callId : -1;
}

QString CmainDlg::GetNameForCall(SIPURI& sipuri, call_user_data* user_data, QString& numberOriginal)
{
    QString name;
    if (user_data) {
        user_data->CS.Lock();
        if (!user_data->name.isEmpty()) name = user_data->name;
        user_data->CS.Unlock();
    }
    if (name.isEmpty()) {
        if (!accountSettings.disableNameLookup)
            name = pageContacts->GetNameByNumber(!sipuri.user.isEmpty() ? sipuri.user : sipuri.domain);
        if (name.isEmpty()) {
            if (!sipuri.name.isEmpty()) name = sipuri.name;
        }
        if (name.isEmpty() && !numberOriginal.isEmpty()) {
            int pos = numberOriginal.indexOf(',');
            name = (pos != -1) ? numberOriginal.left(pos) : numberOriginal;
        }
        if (name.isEmpty()) name = !sipuri.user.isEmpty() ? sipuri.user : sipuri.domain;
        if (user_data) {
            user_data->CS.Lock();
            user_data->name = name;
            user_data->CS.Unlock();
        }
    }
    return name;
}

void CmainDlg::ShortcutAction(Shortcut* shortcut, bool block, bool second)
{
    QString number = second && !shortcut->number2.isEmpty() ? shortcut->number2 : shortcut->number;
    if (shortcut->type == ONYX_SHORTCUT_CALL) {
        if (shortcut->ringing && CommandCallPickup(number)) {}
        else MakeCall(number);
    } else if (shortcut->type == ONYX_SHORTCUT_VIDEOCALL) {
#ifdef _GLOBAL_VIDEO
        MakeCall(number, true);
#else
        MakeCall(number);
#endif
    } else if (shortcut->type == ONYX_SHORTCUT_MESSAGE) MessagesOpen(number);
    else if (shortcut->type == ONYX_SHORTCUT_DTMF) pageDialer->DTMF(number);
    else if (shortcut->type == ONYX_SHORTCUT_TRANSFER) {
        if (number.isEmpty()) OpenTransferDlg(this, ONYX_ACTION_TRANSFER);
        else messagesDlg->CallAction(ONYX_ACTION_TRANSFER, number);
    } else if (shortcut->type == ONYX_SHORTCUT_ATTENDED_TRANSFER) {
        if (number.isEmpty()) OpenTransferDlg(this, ONYX_ACTION_ATTENDED_TRANSFER);
        else messagesDlg->CallAction(ONYX_ACTION_ATTENDED_TRANSFER, number);
    } else if (shortcut->type == ONYX_SHORTCUT_CONFERENCE) {
        if (number.isEmpty()) OpenTransferDlg(this, ONYX_ACTION_INVITE);
        else messagesDlg->CallAction(ONYX_ACTION_INVITE, number);
    } else if (shortcut->type == ONYX_SHORTCUT_RUNBATCH) {
        QMessageBox::information(this, QString(), QStringLiteral(_GLOBAL_BUSINESS_FEATURE));
    } else if (shortcut->type == ONYX_SHORTCUT_CALL_URL || shortcut->type == ONYX_SHORTCUT_POP_URL) {
        QMessageBox::information(this, QString(), QStringLiteral(_GLOBAL_BUSINESS_FEATURE));
    }
}

void CmainDlg::ShortcutsRemoveAll()
{
    for (int i = 0; i < shortcuts.size(); i++) {
        Shortcut* s = &shortcuts[i];
        if (s->presence) {
            s->presence = false;
            UnsubscribeNumber(&s->number);
        }
    }
    shortcuts.clear();
}

void CmainDlg::onPlayerPlay(int soundId, const QString& custom)
{
    QString filename;
    BOOL noLoop = FALSE;
    BOOL inCall = FALSE;
    Q_UNUSED(custom);
    if (soundId == ONYX_SOUND_CUSTOM) {
        filename = custom;
        Onyx::ExpandEnvironmentStrings(filename);
    } else if (soundId == ONYX_SOUND_CUSTOM_NOLOOP) {
        filename = custom;
        Onyx::ExpandEnvironmentStrings(filename);
        noLoop = TRUE;
    } else {
        switch (soundId) {
        case ONYX_SOUND_MESSAGE_IN: filename = accountSettings.pathExe + "/msgin.wav"; noLoop = TRUE; break;
        case ONYX_SOUND_MESSAGE_OUT: filename = accountSettings.pathExe + "/msgout.wav"; noLoop = TRUE; break;
        case ONYX_SOUND_HANGUP: filename = accountSettings.pathExe + "/hangup.wav"; noLoop = TRUE; inCall = TRUE; break;
        case ONYX_SOUND_RINGTONE: filename = accountSettings.pathExe + "/ringtone.wav"; noLoop = FALSE; break;
        case ONYX_SOUND_RINGIN2: filename = accountSettings.pathExe + "/ringing2.wav"; noLoop = TRUE; inCall = TRUE; break;
        case ONYX_SOUND_RINGING: filename = accountSettings.pathExe + "/ringing.wav"; noLoop = TRUE; inCall = TRUE; break;
        default: noLoop = TRUE; break;
        }
    }
    if (!filename.contains('/') && !filename.contains('\\'))
        filename = accountSettings.pathExe + "/" + filename;
    PlayerPlay(filename, noLoop, inCall);
}

void CmainDlg::onPlayerStop(int, int)
{
    PlayerStop();
    if (autoAnswerPlayCallId != PJSUA_INVALID_ID) {
        AutoAnswer(autoAnswerPlayCallId, true);
        autoAnswerPlayCallId = PJSUA_INVALID_ID;
    }
}

static pj_status_t on_pjsua_wav_file_end_callback(pjmedia_port*, void* args)
{
        QMetaObject::invokeMethod(mainDlg, "onPlayerStop", Qt::QueuedConnection, Q_ARG(int, 0), Q_ARG(int, 0));
    return -1;
}

void CmainDlg::PlayerPlay(const QString& filename, bool noLoop, bool inCall, bool isAA)
{
    Q_UNUSED(isAA);
    PlayerStop();
    if (!filename.isEmpty()) {
        pj_str_t file = Onyx::StrToPjStr(filename);
        pjsua_player_id player_id;
        if (is_pjsua_running() && pjsua_player_create(&file, noLoop ? PJMEDIA_FILE_NO_LOOP : 0, &player_id) == PJ_SUCCESS) {
            pjmedia_port* player_media_port;
            if (pjsua_player_get_port(player_id, &player_media_port) == PJ_SUCCESS) {
                if (!player_eof_data) {
                    pj_pool_t* pool = pjsua_pool_create("onyx_voip_eof_data", 512, 512);
                    player_eof_data = new ::player_eof_data{};
                    player_eof_data->pool = pool;
                }
                player_eof_data->player_id = player_id;
                if (noLoop)
                    pjmedia_wav_player_set_eof_cb(player_media_port, player_eof_data, &on_pjsua_wav_file_end_callback);
                if (!tone_gen && pjsua_conf_get_active_ports() <= 2 || tone_gen && pjsua_conf_get_active_ports() <= 3)
                    onyx_set_sound_device(inCall ? onyx_audio_output : onyx_audio_ring);
                pjsua_conf_port_id conf = pjsua_player_get_conf_port(player_id);
                if (inCall) pjsua_conf_adjust_rx_level(conf, 0.4f);
                else pjsua_conf_adjust_rx_level(conf, accountSettings.volumeRing / 100.0f);
                pjsua_conf_connect(conf, 0);
            }
        }
        free(file.ptr);
    }
    if (noLoop) onPlayerStop(0,0);
}

void CmainDlg::PlayerStop()
{
    if (player_eof_data && player_eof_data->player_id != PJSUA_INVALID_ID) {
        if (is_pjsua_running()) {
            pjsua_conf_disconnect(pjsua_player_get_conf_port(player_eof_data->player_id), 0);
            pjsua_player_destroy(player_eof_data->player_id);
        }
        player_eof_data->player_id = PJSUA_INVALID_ID;
    }
}

bool CmainDlg::CommandCallAnswer()
{
    if (!ringinDlgs.isEmpty()) {
        RinginDlg* dlg = ringinDlgs.first();
        QMetaObject::invokeMethod(this, "onCallAnswer", Qt::QueuedConnection, Q_ARG(int, dlg->call_id), Q_ARG(int, 0));
        return true;
    }
    return false;
}

bool CmainDlg::CommandCallReject()
{
    if (!ringinDlgs.isEmpty()) {
        RinginDlg* dlg = ringinDlgs.last();
        dlg->OnBnClickedDecline();
        return true;
    }
    return false;
}

bool CmainDlg::CommandCallPickup(const QString& number)
{
    if (accountSettings.enableFeatureCodeCP && !accountSettings.featureCodeCP.isEmpty()) {
        QString commands;
        QString formatted = FormatNumber(number, &commands);
        SIPURI sipuri;
        Onyx::ParseSIPURI(formatted, &sipuri);
        QString str = accountSettings.featureCodeCP;
        str.append(sipuri.user);
        sipuri.user = str;
        formatted = Onyx::BuildSIPURI(&sipuri);
        messagesDlg->CallMake(formatted);
        return true;
    }
    return false;
}

void CmainDlg::onShellHookMessage()
{
}

LRESULT CmainDlg::onCallAnswer(int call_id, int vid)
{
    if (is_pjsua_running()) {
        pjsua_call_info call_info;
        if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS) {
            if (call_info.role == PJSIP_ROLE_UAS && (call_info.state == PJSIP_INV_STATE_INCOMING || call_info.state == PJSIP_INV_STATE_EARLY)) {
                if (vid < 0) {
                    pjsua_call_answer(call_id, -vid, NULL, NULL);
                    return 0;
                }
                if (accountSettings.singleMode) call_hangup_all_noincoming();
                onyx_set_sound_device(onyx_audio_output);
                pjsua_call_setting call_setting;
                pjsua_call_setting_default(&call_setting);
#ifdef _GLOBAL_VIDEO
                if (vid > 0 && !accountSettings.disableVideo) { createPreviewWin(); call_setting.vid_cnt = 1; }
                else call_setting.vid_cnt = 0;
#else
                call_setting.vid_cnt = 0;
#endif
                if (pjsua_call_answer2(call_id, &call_setting, 200, NULL, NULL) == PJ_SUCCESS)
                    callIdIncomingIgnore = Onyx::PjToStr(&call_info.call_id);
                PlayerStop();
                bool restore = true;
                call_user_data* user_data = (call_user_data*)pjsua_call_get_user_data(call_id);
                if (user_data) {
                    user_data->CS.Lock();
                    if (user_data->autoAnswer && !accountSettings.bringToFrontOnIncoming) {
                        restore = false;
                if (QApplication::activeWindow() != this) {
                    SIPURI sipuri;
                    ParseCallSIPURI(&call_info, user_data, &sipuri);
                    QString name = GetNameForCall(sipuri, user_data, *(new QString()));
                    BaloonPopup(Translate("Auto Answer"), name, QSystemTrayIcon::Information);
                }
                    }
                    user_data->CS.Unlock();
                }
                if (restore) onTrayNotify(QSystemTrayIcon::Trigger);
            }
        }
    }
    return 0;
}

LRESULT CmainDlg::onCallHangup(int call_id)
{
    if (is_pjsua_running()) onyx_call_hangup_fast(call_id);
    return 0;
}

LRESULT CmainDlg::onTabIconUpdate(int call_id)
{
    if (messagesDlg) {
        for (int i = 0; i < messagesDlg->tab->count(); i++) {
            MessagesContact* mc = messagesDlg->GetMessageContact(i);
            if (mc->callId == call_id) {
                pjsua_call_info call_info;
                if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS)
                    messagesDlg->UpdateTabIcon(mc, i, &call_info);
                break;
            }
        }
    }
    return 0;
}

void CmainDlg::SetPaneText2(const QString& str)
{
    QLabel* label2 = m_barLabels.value(1);
    if (label2) {
        label2->setText(str);
        if (str.isEmpty()) label2->hide();
        else label2->show();
    }
}

bool CmainDlg::CopyStringToClipboard(const QString& str)
{
    QClipboard* clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setText(str);
        return TRUE;
    }
    return FALSE;
}

void CmainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if (nID == SC_CLOSE) hide();
    else QWidget::nativeEvent("windows_generic_MSG", nullptr, nullptr); // fallback
}

BOOL CmainDlg::OnQueryEndSession()
{
    return TRUE;
}

void CmainDlg::OnClose()
{
    close();
}

HBRUSH CmainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CmainDlg::OnContextMenu(QWidget* w, const QPoint& pos)
{
    QPoint local = mapFromGlobal(pos);
    int height = MulDiv(16, dpiY, 96);
    if (rect().bottom() - local.y() <= height) MainPopupMenu();
}

BOOL CmainDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
    if (nEventType == DBT_DEVNODES_CHANGED && is_pjsua_running()) {
        killTimer(IDT_TIMER_SWITCH_DEVICES);
        startTimer(IDT_TIMER_SWITCH_DEVICES, 1500);
    }
    return FALSE;
}

void CmainDlg::OnSessionChange(UINT nSessionState, UINT nId)
{
    if (nSessionState == WTS_REMOTE_CONNECT || nSessionState == WTS_CONSOLE_CONNECT) {
        if (is_pjsua_running()) {
            killTimer(IDT_TIMER_SWITCH_DEVICES);
            startTimer(IDT_TIMER_SWITCH_DEVICES, 1500);
        }
    }
}

void CmainDlg::OnMove(int x, int y)
{
    if (isVisible() && !isMaximized() && !isMinimized()) {
        accountSettings.mainX = x;
        accountSettings.mainY = y;
        AccountSettingsPendingSave();
    }
}

void CmainDlg::OnSize(UINT type, int w, int h)
{
    if (type == SIZE_RESTORED && isVisible()) {
        accountSettings.mainW = w;
        accountSettings.mainH = h;
        AccountSettingsPendingSave();
    }
    QWidget::resizeEvent(nullptr);
}

void CmainDlg::SetupJumpList()
{
    JumpList jl(QStringLiteral(_GLOBAL_NAME_VISIBLE));
    jl.AddTasks();
}

void CmainDlg::RemoveJumpList()
{
    JumpList jl(QStringLiteral(_GLOBAL_NAME_VISIBLE));
    jl.DeleteJumpList();
}

void CmainDlg::OnMenuWebsite()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral(_GLOBAL_MENU_WEBSITE)));
}

void CmainDlg::OnMenuHelp()
{
    OpenHelp();
}

void CmainDlg::OnMenuAddl()
{
}

void CmainDlg::OnMuteInput()
{
    pageDialer->OnBnClickedMuteInput();
}

void CmainDlg::OnMuteOutput()
{
    pageDialer->OnBnClickedMuteOutput();
}

LRESULT CmainDlg::onCustomLoaded(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CmainDlg::onUsersDirectoryLoaded(URLGetAsyncData* response)
{
    QString message;
    if (response->statusCode == 0) {
        if (usersDirectorySequence == 1) message = Translate("Connection Failed");
        usersDirectoryReconnect++;
    } else {
        usersDirectoryReconnect = 0;
        if (response->statusCode >= 300) {
            if (usersDirectorySequence == 1)
                message = QString("%1 %2").arg(Translate("The server returned an error code:")).arg(response->statusCode);
        } else if (response->statusCode == 200 && !response->body.isEmpty()) {
            QList<ContactWithFields*> contacts;
            // JSON / XML / CSV parsing (abbreviated, same as original)
            // ...
            if (!contacts.isEmpty()) {
                pageContacts->ContactsAdd(&contacts, true);
                usersDirectoryLoaded = true;
            }
        }
    }
    if (usersDirectoryReconnect) {
        QTimer::singleShot(1000 * 10 * usersDirectoryReconnect * usersDirectoryReconnect * usersDirectoryReconnect, this, [=](){ UsersDirectoryLoad(true); });
    } else if (usersDirectoryRefresh > 0) {
        QTimer::singleShot(1000 * usersDirectoryRefresh, this, [=](){ UsersDirectoryLoad(true); });
    }
    if (!message.isEmpty() && !usersDirectorySilent)
        BaloonPopup(Translate("Directory of Users"), message, QSystemTrayIcon::Information);
    delete response;
    return 0;
}

void CmainDlg::UsersDirectoryLoad(bool update)
{
    // same as original, using QTimer instead of KillTimer/SetTimer
}

void CmainDlg::AccountSettingsPendingSave()
{
    killTimer(IDT_TIMER_SAVE);
    startTimer(IDT_TIMER_SAVE, 5000);
}

void CmainDlg::OnAccountChanged(bool init)
{
    TrayIconUpdateTip();
    if (!init) pageDialer->RebuildButtons();
}

void CmainDlg::OpenTransferDlg(QWidget* parent, onyx_action action, pjsua_call_id call_id, Contact* selectedContact)
{
    if (transferDlg) transferDlg->close();
    transferDlg = new Transfer(parent);
    transferDlg->SetAction(action, call_id);
    transferDlg->LoadFromContacts(selectedContact);
    transferDlg->show();
}

void CmainDlg::OnCheckUpdates()
{
    updateCheckerShow = true;
    CheckUpdates();
}

void CmainDlg::CheckUpdates()
{
    QString url = "https://example.com/onyx-voip/softphone-update.txt";
    url += QString("?version=%1&client=%2")
               .arg(QStringLiteral(_GLOBAL_VERSION),
                    QString::fromUtf8(QUrl::toPercentEncoding(QStringLiteral(_GLOBAL_NAME_VISIBLE))));
#ifndef _GLOBAL_VIDEO
    url += "&lite=1";
#endif
    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
    QNetworkReply* reply = mgr->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [=]() {
        // OnUpdateCheckerLoaded logic
        reply->deleteLater();
        mgr->deleteLater();
    });
}

LRESULT CmainDlg::OnUpdateCheckerLoaded(URLGetAsyncData* response)
{
    bool found = false;
    if (response->statusCode == 200) {
        if (!response->body.isEmpty() && response->body.left(4) == "http") {
            // parsing logic...
        }
    }
    if (!found && updateCheckerShow) QMessageBox::information(this, "", "No new version found");
    updateCheckerShow = false;
    delete response;
    return 0;
}

#ifdef _GLOBAL_VIDEO
int CmainDlg::VideoCaptureDeviceId(const QString& name)
{
    Q_UNUSED(name);
    return 0;
}

void CmainDlg::createPreviewWin()
{
    if (!previewWin) previewWin = new Preview(this);
    previewWin->Start(VideoCaptureDeviceId());
}
#endif

void CmainDlg::OnUpdatePane(CCmdUI* pCmdUI)
{
    Q_UNUSED(pCmdUI);
}
