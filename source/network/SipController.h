#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

class ConfigManager;

#ifdef ONYX_HAS_PJSIP
#include <pjsua-lib/pjsua.h>
#endif

class SipController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString regStatus READ regStatus NOTIFY regStatusChanged)
    Q_PROPERTY(bool isEngineReady READ isEngineReady NOTIFY engineReadyChanged)

public:
    static SipController *instance();

    QString regStatus() const { return m_regStatus; }
    bool isEngineReady() const { return m_engineReady; }

    bool initializeEngine(ConfigManager *config);
    void shutdownEngine();

    Q_INVOKABLE void initiateCall(const QString &uri);
    Q_INVOKABLE void hangUpCall(int callId);
    Q_INVOKABLE void answerCall(int callId);
    Q_INVOKABLE void sendInstantMessage(const QString &targetUri, const QString &message);
    Q_INVOKABLE QStringList getAudioDevices();

signals:
    void regStatusChanged();
    void engineReadyChanged();
    void callStateChanged(int callId, int state, const QString &stateName);
    void incomingCall(int callId, const QString &caller);
    void messageReceived(const QString &sender, const QString &message);
    void natWarningTriggered(const QString &warning);

private:
    explicit SipController(QObject *parent = nullptr);
    ~SipController() override;

#ifdef ONYX_HAS_PJSIP
    static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);
    static void on_call_state(pjsua_call_id call_id, pjsip_event *event);
    static void on_call_media_state(pjsua_call_id call_id);
    static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
    static void on_pager(pjsua_call_id call_id,
                         const pj_str_t *from,
                         const pj_str_t *to,
                         const pj_str_t *contact,
                         const pj_str_t *mime_type,
                         const pj_str_t *body);
    static void on_nat_detect(const pj_stun_nat_detect_result *result);
#endif

    static SipController *m_instance;

    ConfigManager *m_config;
    QString m_regStatus;
    bool m_engineReady;
};
