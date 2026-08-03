#include "network/SipController.h"

#include "core/ConfigManager.h"
#include "network/PortKnocker.h"

#include <QDebug>
#include <QMetaObject>

SipController *SipController::m_instance = nullptr;

SipController::SipController(QObject *parent)
    : QObject(parent),
      m_config(nullptr),
      m_regStatus(QStringLiteral("Not initialized")),
      m_engineReady(false)
{
}

SipController::~SipController() = default;

SipController *SipController::instance()
{
    if (!m_instance) {
        m_instance = new SipController();
    }
    return m_instance;
}

bool SipController::initializeEngine(ConfigManager *config)
{
    m_config = config;

#ifdef ONYX_HAS_PJSIP
    const QString serverIp = m_config->getValue("Account", "SipServer", "127.0.0.1");
    const QString knockPorts = m_config->getValue("Account", "PortKnockingCSV", "");
    if (!knockPorts.isEmpty()) {
        PortKnocker knocker;
        knocker.executeKnockSequence(serverIp, knockPorts);
    }

    pj_status_t status = pjsua_create();
    if (status != PJ_SUCCESS) {
        m_regStatus = QStringLiteral("pjsua_create failed");
        emit regStatusChanged();
        return false;
    }

    pjsua_config cfg;
    pjsua_logging_config logCfg;
    pjsua_media_config mediaCfg;

    pjsua_config_default(&cfg);
    pjsua_logging_config_default(&logCfg);
    pjsua_media_config_default(&mediaCfg);

    cfg.cb.on_reg_state2 = &SipController::on_reg_state2;
    cfg.cb.on_call_state = &SipController::on_call_state;
    cfg.cb.on_call_media_state = &SipController::on_call_media_state;
    cfg.cb.on_incoming_call = &SipController::on_incoming_call;
    cfg.cb.on_pager = &SipController::on_pager;
    cfg.cb.on_nat_detect = &SipController::on_nat_detect;
    logCfg.console_level = 3;

    status = pjsua_init(&cfg, &logCfg, &mediaCfg);
    if (status != PJ_SUCCESS) {
        m_regStatus = QStringLiteral("pjsua_init failed");
        emit regStatusChanged();
        pjsua_destroy();
        return false;
    }

    pjsua_transport_config transportCfg;
    pjsua_transport_config_default(&transportCfg);
    transportCfg.port = m_config->getValue("Network", "LocalSipPort", "5060").toUShort();

    pjsua_transport_id transportId = PJSUA_INVALID_ID;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportCfg, &transportId);
    if (status != PJ_SUCCESS) {
        m_regStatus = QStringLiteral("transport creation failed");
        emit regStatusChanged();
        pjsua_destroy();
        return false;
    }

    status = pjsua_start();
    if (status != PJ_SUCCESS) {
        m_regStatus = QStringLiteral("pjsua_start failed");
        emit regStatusChanged();
        pjsua_destroy();
        return false;
    }

    const QString username = m_config->getValue("Account", "Username", "");
    const QString domain = m_config->getValue("Account", "Domain", "");
    const QString password = m_config->getValue("Account", "Password", "");

    if (!username.isEmpty() && !domain.isEmpty()) {
        pjsua_acc_config accCfg;
        pjsua_acc_config_default(&accCfg);

        const QByteArray registrarBytes = QStringLiteral("sip:%1").arg(domain).toUtf8();
        const QByteArray identityBytes = QStringLiteral("sip:%1@%2").arg(username, domain).toUtf8();
        const QByteArray domainBytes = domain.toUtf8();
        const QByteArray usernameBytes = username.toUtf8();
        const QByteArray passwordBytes = password.toUtf8();

        accCfg.id = pj_str(const_cast<char *>(identityBytes.constData()));
        accCfg.reg_uri = pj_str(const_cast<char *>(registrarBytes.constData()));
        accCfg.cred_count = 1;
        accCfg.cred_info[0].realm = pj_str(const_cast<char *>(domainBytes.constData()));
        accCfg.cred_info[0].scheme = pj_str(const_cast<char *>("digest"));
        accCfg.cred_info[0].username = pj_str(const_cast<char *>(usernameBytes.constData()));
        accCfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        accCfg.cred_info[0].data = pj_str(const_cast<char *>(passwordBytes.constData()));

        pjsua_acc_id accId = PJSUA_INVALID_ID;
        status = pjsua_acc_add(&accCfg, PJ_TRUE, &accId);
        if (status != PJ_SUCCESS) {
            m_regStatus = QStringLiteral("account registration failed");
            emit regStatusChanged();
            pjsua_destroy();
            return false;
        }
    } else {
        m_regStatus = QStringLiteral("Ready (LAN mode)");
        emit regStatusChanged();
    }

    m_engineReady = true;
    emit engineReadyChanged();
    return true;
#else
    Q_UNUSED(config);
    m_regStatus = QStringLiteral("Built without pjproject; LAN services only");
    m_engineReady = true;
    emit regStatusChanged();
    emit engineReadyChanged();
    return true;
#endif
}

void SipController::shutdownEngine()
{
#ifdef ONYX_HAS_PJSIP
    if (m_engineReady) {
        pjsua_destroy();
    }
#endif
    m_engineReady = false;
    emit engineReadyChanged();
}

void SipController::initiateCall(const QString &uri)
{
#ifdef ONYX_HAS_PJSIP
    if (!m_engineReady || uri.isEmpty()) {
        return;
    }

    QByteArray destinationBytes = uri.toUtf8();
    pj_str_t destUri = pj_str(destinationBytes.data());
    pjsua_call_id callId = PJSUA_INVALID_ID;
    pjsua_call_make_call(0, &destUri, nullptr, nullptr, nullptr, &callId);
#else
    Q_UNUSED(uri);
    qWarning() << "Call requested, but PJSIP is not linked into this build.";
#endif
}

void SipController::hangUpCall(int callId)
{
#ifdef ONYX_HAS_PJSIP
    if (!m_engineReady) {
        return;
    }

    if (callId < 0) {
        pjsua_call_hangup_all();
    } else {
        pjsua_call_hangup(callId, 0, nullptr, nullptr);
    }
#else
    Q_UNUSED(callId);
#endif
}

void SipController::answerCall(int callId)
{
#ifdef ONYX_HAS_PJSIP
    if (m_engineReady) {
        pjsua_call_answer(callId, 200, nullptr, nullptr);
    }
#else
    Q_UNUSED(callId);
#endif
}

void SipController::sendInstantMessage(const QString &targetUri, const QString &message)
{
#ifdef ONYX_HAS_PJSIP
    if (!m_engineReady || targetUri.isEmpty() || message.isEmpty()) {
        return;
    }

    QByteArray toBytes = targetUri.toUtf8();
    QByteArray bodyBytes = message.toUtf8();
    pj_str_t toUri = pj_str(toBytes.data());
    pj_str_t payload = pj_str(bodyBytes.data());
    pjsua_im_send(0, &toUri, nullptr, &payload, nullptr, nullptr);
#else
    Q_UNUSED(targetUri);
    Q_UNUSED(message);
#endif
}

QStringList SipController::getAudioDevices()
{
#ifdef ONYX_HAS_PJSIP
    QStringList devices;
    unsigned count = 32;
    pjmedia_snd_dev_info info[32];
    if (pjsua_enum_snd_devs(info, &count) == PJ_SUCCESS) {
        for (unsigned i = 0; i < count; ++i) {
            devices << QString::fromUtf8(info[i].name);
        }
    }
    return devices;
#else
    return {};
#endif
}

#ifdef ONYX_HAS_PJSIP
void SipController::on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
    Q_UNUSED(info);

    SipController *self = SipController::instance();
    pjsua_acc_info accInfo;
    if (pjsua_acc_get_info(acc_id, &accInfo) == PJ_SUCCESS) {
        const QString statusText =
            QString::fromUtf8(accInfo.status_text.ptr, static_cast<int>(accInfo.status_text.slen));

        QMetaObject::invokeMethod(
            self,
            [self, statusText]() {
                self->m_regStatus = statusText;
                emit self->regStatusChanged();
            },
            Qt::QueuedConnection);
    }
}

void SipController::on_call_state(pjsua_call_id call_id, pjsip_event *event)
{
    Q_UNUSED(event);

    SipController *self = SipController::instance();
    pjsua_call_info callInfo;
    if (pjsua_call_get_info(call_id, &callInfo) == PJ_SUCCESS) {
        const QString stateName =
            QString::fromUtf8(callInfo.state_text.ptr, static_cast<int>(callInfo.state_text.slen));

        QMetaObject::invokeMethod(self,
                                  "callStateChanged",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, call_id),
                                  Q_ARG(int, static_cast<int>(callInfo.state)),
                                  Q_ARG(QString, stateName));

        if (self->m_config) {
            const QString peer =
                QString::fromUtf8(callInfo.remote_info.ptr, static_cast<int>(callInfo.remote_info.slen));

            if (callInfo.state == PJSIP_INV_STATE_CALLING) {
                self->m_config->triggerShellCommand(QStringLiteral("cmdCallStart"), peer);
            } else if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED) {
                self->m_config->triggerShellCommand(QStringLiteral("cmdCallEnd"), peer);
            }
        }
    }
}

void SipController::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info callInfo;
    if (pjsua_call_get_info(call_id, &callInfo) == PJ_SUCCESS &&
        callInfo.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        pjsua_conf_connect(callInfo.conf_slot, 0);
        pjsua_conf_connect(0, callInfo.conf_slot);
    }
}

void SipController::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    Q_UNUSED(acc_id);
    Q_UNUSED(rdata);

    SipController *self = SipController::instance();
    pjsua_call_info callInfo;
    if (pjsua_call_get_info(call_id, &callInfo) == PJ_SUCCESS) {
        const QString caller =
            QString::fromUtf8(callInfo.remote_info.ptr, static_cast<int>(callInfo.remote_info.slen));

        QMetaObject::invokeMethod(
            self, "incomingCall", Qt::QueuedConnection, Q_ARG(int, call_id), Q_ARG(QString, caller));
    }
}

void SipController::on_pager(pjsua_call_id call_id,
                             const pj_str_t *from,
                             const pj_str_t *to,
                             const pj_str_t *contact,
                             const pj_str_t *mime_type,
                             const pj_str_t *body)
{
    Q_UNUSED(call_id);
    Q_UNUSED(to);
    Q_UNUSED(contact);
    Q_UNUSED(mime_type);

    SipController *self = SipController::instance();
    const QString sender = QString::fromUtf8(from->ptr, static_cast<int>(from->slen));
    const QString message = QString::fromUtf8(body->ptr, static_cast<int>(body->slen));

    QMetaObject::invokeMethod(
        self, "messageReceived", Qt::QueuedConnection, Q_ARG(QString, sender), Q_ARG(QString, message));
}

void SipController::on_nat_detect(const pj_stun_nat_detect_result *result)
{
    SipController *self = SipController::instance();
    if (result->status != PJ_SUCCESS || result->nat_type != PJ_STUN_NAT_TYPE_SYMMETRIC) {
        return;
    }

    pjsua_acc_id accountIds[16];
    unsigned accountCount = 16;
    if (pjsua_enum_accs(accountIds, &accountCount) != PJ_SUCCESS) {
        return;
    }

    for (unsigned i = 0; i < accountCount; ++i) {
        pj_pool_t *pool = pjsua_pool_create("onyx-stun", 1000, 1000);
        if (!pool) {
            continue;
        }

        pjsua_acc_config accCfg;
        if (pjsua_acc_get_config(accountIds[i], pool, &accCfg) == PJ_SUCCESS) {
            accCfg.sip_stun_use = PJSUA_STUN_USE_DISABLED;
            accCfg.media_stun_use = PJSUA_STUN_USE_DISABLED;
            pjsua_acc_modify(accountIds[i], &accCfg);
        }
        pj_pool_release(pool);
    }

    QMetaObject::invokeMethod(
        self,
        "natWarningTriggered",
        Qt::QueuedConnection,
        Q_ARG(QString, QStringLiteral("Symmetric NAT detected. STUN has been disabled.")));
}
#endif
