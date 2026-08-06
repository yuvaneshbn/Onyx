#pragma once

#include <cstddef>
#include <cstdint>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>

using pj_bool_t = int;
using pj_status_t = int;
using pj_uint8_t = std::uint8_t;
using pj_uint16_t = std::uint16_t;
using pj_uint32_t = std::uint32_t;

using pjsua_transport_id = int;
using pjsua_acc_id = int;
using pjsua_call_id = int;
using pjsua_conf_port_id = int;
using pjsua_buddy_id = int;
using pjsua_player_id = int;
using pjsua_recorder_id = int;
using pjsua_vid_win_id = int;
using pjmedia_vid_dev_index = int;
using pjmedia_type = int;
using pjmedia_tp_proto = int;
using pjrpid_activity = int;
using pjrpid_element_type = int;
using pjsip_status_code = int;

struct pj_timer_heap_t;
struct pjsip_hdr;
struct pjsip_msg;
struct pjsip_msg_body;
struct pjsip_ctype_hdr;
struct pjsip_generic_string_hdr;
struct pjsip_dialog;
struct pjsip_transaction;
struct pjsip_method;
struct pjsip_tx_data;

struct pj_str_t
{
    char *ptr = nullptr;
    std::size_t slen = 0;
};

struct pj_time_val
{
    long sec = 0;
    long msec = 0;
};

struct pj_timer_entry
{
    int id = 0;
    void *user_data = nullptr;
    void (*cb)(pj_timer_heap_t *, pj_timer_entry *) = nullptr;
    bool cancelled = false;
};

struct pj_timer_heap_t
{
    int unused = 0;
};

struct pj_md5_context
{
    std::uint32_t state[4] = {};
};

struct pj_pool_t
{
    int unused = 0;
};

struct pjmedia_port
{
    int unused = 0;
};

struct pjmedia_tonegen
{
    int unused = 0;
};

struct pj_scanner
{
    char *start = nullptr;
    char *curptr = nullptr;
    char *end = nullptr;
};

struct pjmedia_coord
{
    int x = 0;
    int y = 0;
};

struct pjmedia_rect_size
{
    int w = 0;
    int h = 0;
};

struct pjmedia_tone_digit
{
    char digit = 0;
    int on_msec = 0;
    int off_msec = 0;
    int volume = 0;
};

struct pjmedia_snd_dev_info
{
    char name[64] = {};
};

struct pjmedia_aud_dev_info
{
    char name[64] = {};
    int input_count = 0;
    int output_count = 0;
};

struct pjmedia_vid_dev_info
{
    char name[64] = {};
    int fmt_cnt = 0;
    int dir = 0;
};

struct pjmedia_codec_param
{
    struct
    {
        struct
        {
            int cnt = 0;
            struct
            {
                pj_str_t name;
                pj_str_t val;
            } param[8];
        } dec_fmtp;
    } setting;

    struct
    {
        int avg_bps = 0;
        int max_bps = 0;
    } info;
};

struct pjmedia_vid_codec_param
{
    struct
    {
        struct
        {
            struct
            {
                int max_bps = 0;
            } vid;
        } det;
    } enc_fmt;
};

struct pjmedia_event
{
    int type = 0;
};

struct pjrpid_element
{
    pjrpid_element_type type = 0;
    pj_str_t id;
    pjrpid_activity activity = 0;
    pj_str_t note;
};

struct pjmedia_rtcp_stat
{
    struct
    {
        int pkt = 0;
        int loss = 0;
        struct
        {
            int last = 0;
        } jitter;
    } rx;

    struct
    {
        int last = 0;
    } rtt;
};

struct pjsip_hdr
{
    pjsip_hdr *next = nullptr;
    pjsip_hdr *prev = nullptr;
    pj_str_t name;
};

struct pjsip_generic_string_hdr
{
    pjsip_hdr hdr;
    pj_str_t hvalue;
};

struct pjsip_msg_body
{
    struct
    {
        pj_str_t type;
        pj_str_t subtype;
    } content_type;
    std::size_t len = 0;
    char *data = nullptr;
};

struct pjsip_msg
{
    pjsip_msg_body *body = nullptr;
};

struct pjsip_ctype_hdr
{
    struct
    {
        pj_str_t type;
        pj_str_t subtype;
    } media;
};

struct pjsip_rx_data
{
    struct
    {
        pjsip_msg *msg = nullptr;
        pjsip_ctype_hdr *ctype = nullptr;
    } msg_info;
};

struct pjsip_event
{
    struct
    {
        struct
        {
            int type = 0;
            struct
            {
                pjsip_rx_data *rdata = nullptr;
            } src;
        } tsx_state;
        struct
        {
            pjsip_rx_data *rdata = nullptr;
        } rx_msg;
    } body;
};

struct pjsip_method
{
    int id = 0;
    pj_str_t name;
};

struct pjsip_transaction
{
    int role = 0;
    pjsip_method method;
    int state = 0;
    int status_code = 0;
    pj_str_t status_text;
    void *endpt = nullptr;
};

struct pjsip_dialog
{
    int unused = 0;
};

struct pjsip_uri;

using pjsip_redirect_op = int;
constexpr int PJSIP_REDIRECT_ACCEPT_REPLACE = 0;

struct pjsip_tx_data
{
    int unused = 0;
};

struct pj_stun_nat_detect_result
{
    pj_status_t status = 0;
    int nat_type = 0;
    const char *nat_type_name = "";
};

struct pjsua_reg_info
{
    bool renew = false;
    struct cbparam_t
    {
        int code = 0;
        pjsip_rx_data *rdata = nullptr;
    } *cbparam = nullptr;
};

struct pjsua_call_media_info
{
    int dir = 0;
    int type = 0;
    int index = 0;
};

struct pjsua_call_info
{
    pj_str_t remote_info;
    pj_str_t remote_contact;
    pj_str_t state_text;
    pj_str_t last_status_text;
    pj_str_t call_id;
    pj_str_t local_info;
    int id = 0;
    int acc_id = 0;
    int state = 0;
    int role = 0;
    int media_status = 0;
    int conf_slot = 0;
    unsigned media_cnt = 0;
    pjsua_call_media_info media[4];
    int rem_vid_cnt = 0;
    int last_status = 0;
    pj_time_val connect_duration;
    pj_time_val total_duration;
};

struct pjsua_call_inv
{
    int neg = 0;
};

struct pjsua_call
{
    pjsua_call_inv *inv = nullptr;
};

struct pjsua_acc_info
{
    pj_str_t status_text;
    pj_bool_t has_registration = 0;
    int status = 0;
};

struct pjsua_buddy_info
{
    int status = 0;
    struct
    {
        int activity = 0;
    } rpid;
    pj_str_t status_text;
};

struct pjsua_conf_port_info
{
    int slot_id = 0;
    float rx_level_adj = 0.0f;
    float tx_level_adj = 0.0f;
};

struct pjmedia_transport_info
{
    unsigned specific_info_cnt = 0;
    struct
    {
        unsigned type = 0;
        unsigned char buffer[16] = {};
    } spc_info[4];
};

struct pjsua_stream_info
{
    pjmedia_type type = 0;
    struct
    {
        struct
        {
            int proto = 0;
            struct
            {
                pj_str_t encoding_name;
                int clock_rate = 0;
                int channel_cnt = 0;
            } fmt;
            pjmedia_codec_param* param = nullptr;
        } aud;
        struct
        {
            int proto = 0;
            struct
            {
                pj_str_t encoding_name;
            } codec_info;
            pjmedia_vid_codec_param* codec_param = nullptr;
        } vid;
    } info;
};

struct pjsua_stream_stat
{
    pjmedia_rtcp_stat rtcp;
};

struct pjsua_snd_dev_param
{
    int mode = 0;
    int capture_dev = 0;
    int playback_dev = 0;
};

struct pjsua_transport_config
{
    unsigned port = 0;
};

struct pjsua_transport_data
{
    struct
    {
        pj_str_t host;
    } local_name;
};

struct pjsua_var_t
{
    pjsua_transport_data tpdata[1];
};

inline pjsua_var_t pjsua_var{};

struct pjsua_logging_config
{
    int console_level = 0;
    int decor = 0;
    pj_str_t log_filename;
};

struct pjsua_config
{
    pj_str_t user_agent;
    pj_str_t stun_srv[8];
    unsigned stun_srv_cnt = 0;
    pj_str_t nameserver[4];
    unsigned nameserver_count = 0;
    int srtp_secure_signaling = 0;
};

struct pjsua_media_config
{
    int enable_ice = 0;
    int no_vad = 0;
    int ec_tail_len = 0;
    int clock_rate = 0;
    int channel_count = 0;
};

struct pjsua_cred_info
{
    pj_str_t realm;
    pj_str_t scheme;
    pj_str_t username;
    int data_type = 0;
    pj_str_t data;
};

struct pjsua_acc_config
{
    pj_str_t id;
    pj_str_t reg_uri;
    unsigned cred_count = 0;
    pjsua_cred_info cred_info[8];
    pj_bool_t register_on_acc_add = 1;
    int priority = 0;
    int ka_interval = 0;
    int vid_in_auto_show = 0;
    int vid_out_auto_transmit = 0;
    int vid_cap_dev = 0;
    int vid_wnd_flags = 0;
    unsigned reg_timeout = 0;
    int use_timer = 0;
    int use_srtp = 0;
    int enable_rtcp_mux = 0;
    struct
    {
        unsigned keying_count = 0;
        int keying[4] = {};
    } srtp_opt;
    int ice_cfg_use = 0;
    struct
    {
        int enable_ice = 0;
    } ice_cfg;
    int allow_via_rewrite = 0;
    int allow_sdp_nat_rewrite = 0;
    int allow_contact_rewrite = 0;
    int contact_rewrite_method = 0;
    int publish_enabled = 0;
    int mwi_enabled = 0;
    int transport_id = 0;
    unsigned proxy_cnt = 0;
    pj_str_t proxy[8];
    int sip_stun_use = 0;
    int media_stun_use = 0;
    struct
    {
        unsigned port = 0;
        unsigned port_range = 0;
        pj_str_t public_addr;
    } rtp_cfg;
};

struct pjsua_codec_info
{
    pj_str_t codec_id;
    int priority = 0;
};

struct pjsua_msg_data
{
    void *hdr_list = nullptr;
    pj_str_t content_type;
    pj_str_t msg_body;
};

struct pjsua_call_setting
{
    unsigned flag = 0;
    unsigned req_keyframe_method = 0;
    unsigned aud_cnt = 1;
    unsigned vid_cnt = 0;
    unsigned txt_cnt = 1;
};

struct pjsua_ip_change_param
{
    int unused = 0;
};

struct pjsua_vid_preview_param
{
    int unused = 0;
};

struct pjsua_vid_win_info
{
    struct
    {
        struct
        {
            struct
            {
                void *hwnd = nullptr;
            } win;
        } info;
    } hwnd;
};

struct pjsip_endpoint_cfg
{
    struct
    {
        int disable_rport = 0;
    } endpt;
};

inline pjsip_endpoint_cfg *pjsip_cfg()
{
    static pjsip_endpoint_cfg cfg;
    return &cfg;
}

constexpr pj_status_t PJ_SUCCESS = 0;
constexpr pj_bool_t PJ_TRUE = 1;
constexpr pj_bool_t PJ_FALSE = 0;

constexpr int PJ_ERR_MSG_SIZE = 256;
constexpr int PJSUA_INVALID_ID = -1;
constexpr int PJSUA_MAX_CALLS = 32;
constexpr int PJSUA_MAX_ACC = 32;
constexpr int PJSUA_MAX_CONF_PORTS = 32;
constexpr int PJSUA_REG_INTERVAL = 60;

constexpr int PJSUA_CALL_MEDIA_NONE = 0;
constexpr int PJSUA_CALL_MEDIA_ACTIVE = 1;
constexpr int PJSUA_CALL_MEDIA_REMOTE_HOLD = 2;
constexpr int PJSUA_CALL_MEDIA_LOCAL_HOLD = 3;
constexpr int PJSUA_CALL_MEDIA_ERROR = 4;

constexpr int PJSIP_ROLE_UAC = 0;
constexpr int PJSIP_ROLE_UAS = 1;

constexpr int PJSIP_INV_STATE_NULL = 0;
constexpr int PJSIP_INV_STATE_CALLING = 1;
constexpr int PJSIP_INV_STATE_INCOMING = 2;
constexpr int PJSIP_INV_STATE_EARLY = 3;
constexpr int PJSIP_INV_STATE_CONNECTING = 4;
constexpr int PJSIP_INV_STATE_CONFIRMED = 5;
constexpr int PJSIP_INV_STATE_DISCONNECTED = 6;

constexpr int PJSIP_TSX_STATE_TRYING = 1;
constexpr int PJSIP_TSX_STATE_TERMINATED = 2;
constexpr int PJSIP_TSX_STATE_COMPLETED = 3;

constexpr int PJSUA_BUDDY_STATUS_OFFLINE = 0;
constexpr int PJSUA_BUDDY_STATUS_ONLINE = 1;

constexpr int PJRPID_ACTIVITY_UNKNOWN = 0;
constexpr int PJRPID_ACTIVITY_AWAY = 1;
constexpr int PJRPID_ACTIVITY_BUSY = 2;
constexpr int PJRPID_ELEMENT_TYPE_PERSON = 0;

constexpr int PJSUA_STUN_USE_DISABLED = 0;
constexpr int PJMEDIA_SRTP_OPTIONAL = 0;
constexpr int PJMEDIA_SRTP_MANDATORY = 1;
constexpr int PJMEDIA_SRTP_DISABLED = 2;
constexpr int PJMEDIA_SRTP_KEYING_DTLS_SRTP = 0;
constexpr int PJMEDIA_SRTP_KEYING_SDES = 1;
constexpr int PJSUA_ICE_CONFIG_USE_CUSTOM = 1;
constexpr int PJSUA_CONTACT_REWRITE_ALWAYS_UPDATE = 1;
constexpr int PJSUA_CONTACT_REWRITE_UNREGISTER = 2;
constexpr int PJMEDIA_VID_DEV_WND_BORDER = 0x0001;
constexpr int PJMEDIA_VID_DEV_WND_RESIZABLE = 0x0002;
constexpr int PJSUA_SIP_TIMER_INACTIVE = 0;
constexpr int PJ_STUN_NAT_TYPE_SYMMETRIC = 1;
constexpr int PJ_SCAN_AUTOSKIP_WS = 0;

constexpr int PJSIP_TRANSPORT_UDP = 0;
constexpr int PJSIP_TRANSPORT_TCP = 1;
constexpr int PJSIP_TRANSPORT_TLS = 2;

constexpr int PJSIP_CRED_DATA_PLAIN_PASSWD = 0;
constexpr int PJSIP_CRED_DATA_DIGEST = 1;

constexpr int PJMEDIA_TYPE_NONE = 0;
constexpr int PJMEDIA_TYPE_AUDIO = 1;
constexpr int PJMEDIA_TYPE_VIDEO = 2;

constexpr int PJMEDIA_DIR_NONE = 0;
constexpr int PJMEDIA_DIR_ENCODING = 1;
constexpr int PJMEDIA_DIR_DECODING = 2;
constexpr int PJMEDIA_DIR_ENCODING_DECODING = 3;

constexpr int PJMEDIA_TRANSPORT_TYPE_SRTP = 1;
constexpr int PJMEDIA_TRANSPORT_TYPE_ICE = 2;

constexpr int PJMEDIA_TP_PROTO_NONE = 0;
constexpr int PJMEDIA_TP_PROTO_DTLS = 0x01;
constexpr int PJMEDIA_TP_PROFILE_SRTP = 0x02;

constexpr int PJMEDIA_CODEC_MGR_MAX_CODECS = 64;
constexpr int PJMEDIA_CODEC_PRIO_DISABLED = 0;
constexpr int PJMEDIA_CODEC_PRIO_NORMAL = 128;
constexpr int PJMEDIA_AUD_MAX_DEVS = 32;
constexpr int PJMEDIA_VID_DEV_MAX_DEVS = 32;
constexpr int PJMEDIA_TONEGEN_VOLUME = 100;
constexpr int PJMEDIA_SND_DEFAULT_PLAY_LATENCY = 0;
constexpr int PJMEDIA_SND_DEFAULT_REC_LATENCY = 0;
constexpr int PJMEDIA_FILE_NO_LOOP = 1;

constexpr int PJSUA_SND_DEV_SPEAKER_ONLY = 0x0001;
constexpr int PJ_LOG_HAS_CR = 0x0001;

constexpr int PJSIP_EURITOOLONG = -1;
constexpr int PJSIP_ENOTINITIALIZED = -2;
constexpr int PJSIP_EAUTHACCNOTFOUND = -3;
constexpr int PJSIP_EAUTHACCDISABLED = -4;
constexpr int PJSIP_EINVALIDREQURI = -5;
constexpr int PJ_ERESOLVE = -6;
constexpr int PJMEDIA_EAUD_NODEFDEV = -7;
constexpr int PJSIP_SC_NULL = 0;
constexpr int PJSIP_SC_TRYING = 100;
constexpr int PJSIP_SC_RINGING = 180;
constexpr int PJSIP_SC_OK = 200;
constexpr int PJSIP_SC_MOVED_TEMPORARILY = 302;
constexpr int PJSIP_SC_NOT_FOUND = 404;
constexpr int PJSIP_SC_DECLINE = 603;
constexpr int PJSIP_SC_REQUEST_TERMINATED = 487;
constexpr int PJSIP_OTHER_METHOD = 0;
constexpr int PJSIP_INVITE_METHOD = 1;
constexpr int PJSIP_CANCEL_METHOD = 2;
constexpr int PJSIP_EVENT_RX_MSG = 1;

#ifndef PJ_UNUSED_ARG
#define PJ_UNUSED_ARG(arg) (void)(arg)
#endif

#ifndef PJ_STATUS_TO_OS
#define PJ_STATUS_TO_OS(e) (-(e))
#endif

#ifndef PJSUA_CALL_UNHOLD
#define PJSUA_CALL_UNHOLD 0
#endif

constexpr int PJSUA_TRANSPORT_UDP = PJSIP_TRANSPORT_UDP;
constexpr int PJSUA_TRANSPORT_TCP = PJSIP_TRANSPORT_TCP;
constexpr int PJSUA_TRANSPORT_TLS = PJSIP_TRANSPORT_TLS;

#ifndef PJ_ARRAY_SIZE
#define PJ_ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef pj_bzero
#define pj_bzero(ptr, size) std::memset((ptr), 0, (size))
#endif

#ifndef PJ_LOG
#define PJ_LOG(level, arg) do { (void)(level); } while (false)
#endif

#ifndef PJ_POOL_ZALLOC_T
#define PJ_POOL_ZALLOC_T(pool, type) new type{}
#endif

inline pj_str_t pj_str(char *ptr)
{
    return pj_str_t{ptr, ptr ? std::strlen(ptr) : 0};
}

inline pj_str_t pj_str(const char *ptr)
{
    return pj_str_t{const_cast<char*>(ptr), ptr ? std::strlen(ptr) : 0};
}

inline void pj_strset2(pj_str_t *dst, char *src)
{
    if (dst)
        *dst = pj_str(src);
}

inline void pj_strset2(pj_str_t *dst, const char *src)
{
    if (dst)
        *dst = pj_str(src);
}

inline void pj_strtrim(pj_str_t *s)
{
    if (!s || !s->ptr)
        return;
    while (s->slen && std::isspace(static_cast<unsigned char>(s->ptr[0]))) {
        ++s->ptr;
        --s->slen;
    }
    while (s->slen && std::isspace(static_cast<unsigned char>(s->ptr[s->slen - 1]))) {
        --s->slen;
    }
}

inline int pj_stricmp2(const pj_str_t *lhs, const char *rhs)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);
    const std::size_t rhsLen = std::strlen(rhs);
    const std::size_t len = lhs->slen < rhsLen ? lhs->slen : rhsLen;
    for (std::size_t i = 0; i < len; ++i) {
        const int a = std::tolower(static_cast<unsigned char>(lhs->ptr[i]));
        const int b = std::tolower(static_cast<unsigned char>(rhs[i]));
        if (a != b)
            return a - b;
    }
    if (lhs->slen == rhsLen)
        return 0;
    return lhs->slen < rhsLen ? -1 : 1;
}

inline void pj_ansi_snprintf(char *buf, std::size_t len, const char *fmt, ...)
{
    if (!buf || !len)
        return;
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buf, len, fmt, args);
    va_end(args);
    buf[len - 1] = '\0';
}

inline void pj_scan_init(pj_scanner *scanner, char *buf, std::size_t len, int, int)
{
    if (!scanner)
        return;
    scanner->start = buf;
    scanner->curptr = buf;
    scanner->end = buf ? buf + len : nullptr;
}

inline pj_bool_t pj_scan_is_eof(const pj_scanner *scanner)
{
    return !scanner || !scanner->curptr || !scanner->end || scanner->curptr >= scanner->end;
}

inline void pj_scan_get_until_chr(pj_scanner *scanner, const char *chars, pj_str_t *out)
{
    if (!scanner || !out || pj_scan_is_eof(scanner)) {
        if (out) *out = {};
        return;
    }
    char *start = scanner->curptr;
    char *pos = start;
    while (pos < scanner->end && !std::strchr(chars, *pos))
        ++pos;
    *out = pj_str_t{start, static_cast<std::size_t>(pos - start)};
    scanner->curptr = pos;
}

inline void pj_scan_fini(pj_scanner *) {}

inline int pj_strcmp2(const pj_str_t *lhs, const char *rhs)
{
    if (!lhs || !lhs->ptr || !rhs)
        return lhs && lhs->ptr ? 1 : (rhs ? -1 : 0);

    const std::size_t lhsLen = lhs->slen;
    const std::size_t rhsLen = std::strlen(rhs);
    const std::size_t len = lhsLen < rhsLen ? lhsLen : rhsLen;
    const int cmp = std::strncmp(lhs->ptr, rhs, len);
    if (cmp != 0)
        return cmp;
    if (lhsLen == rhsLen)
        return 0;
    return lhsLen < rhsLen ? -1 : 1;
}

inline int pj_strcmp(const pj_str_t *lhs, const pj_str_t *rhs)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);
    return pj_strcmp2(lhs, rhs->ptr);
}

inline char *pj_strstr(const pj_str_t *haystack, const pj_str_t *needle)
{
    if (!haystack || !needle || !haystack->ptr || !needle->ptr)
        return nullptr;
    const char *found = std::strstr(haystack->ptr, needle->ptr);
    return const_cast<char *>(found);
}

inline void pj_md5_init(pj_md5_context *) {}
inline void pj_md5_update(pj_md5_context *, const pj_uint8_t *, unsigned) {}
inline void pj_md5_final(pj_md5_context *, pj_uint8_t digest[16])
{
    if (digest)
        std::memset(digest, 0, 16);
}

inline void pj_val_to_hex_digit(pj_uint8_t, char *out)
{
    if (out)
    {
        out[0] = '0';
        out[1] = '0';
    }
}

inline void pj_strerror(pj_status_t, char *buf, std::size_t len)
{
    if (buf && len)
    {
        std::strncpy(buf, "pjcompat", len - 1);
        buf[len - 1] = '\0';
    }
}

inline void pj_strdup(pj_pool_t *, pj_str_t *dst, const pj_str_t *src)
{
    if (dst)
        *dst = src ? *src : pj_str("");
}

inline void pj_list_push_back(void *, void *) {}

inline void *pj_pool_create(const char *, std::size_t, std::size_t)
{
    return new pj_pool_t{};
}

inline void pj_pool_release(void *pool)
{
    delete static_cast<pj_pool_t *>(pool);
}

inline void pjmedia_port_destroy(void *) {}
inline void pjmedia_sdp_neg_set_prefer_remote_codec_order(void *, pj_bool_t) {}
inline void pjmedia_aud_dev_refresh() {}
using pjmedia_wav_player_eof_cb = pj_status_t (*)(pjmedia_port *, void *);
inline void pjmedia_wav_player_set_eof_cb(void *, void *, pjmedia_wav_player_eof_cb) {}
inline void pjsua_perror(const char *, const char *, pj_status_t) {}

struct pjsua_mwi_info
{
    pjsip_rx_data *rdata = nullptr;
};

inline void pjsua_config_default(void *) {}
inline void pjsua_media_config_default(void *) {}
inline void pjsua_logging_config_default(void *) {}
inline void pjsua_transport_config_default(void *) {}
inline void pjsua_acc_config_default(void *) {}
inline void pjsua_ip_change_param_default(void *) {}
inline void pjsua_snd_dev_param_default(pjsua_snd_dev_param *param)
{
    if (param)
        *param = {};
}
inline void pjsua_msg_data_init(pjsua_msg_data *data)
{
    if (data)
        *data = {};
}
inline void pjsua_call_setting_default(pjsua_call_setting *opt)
{
    if (opt)
        *opt = {};
}
inline pj_bool_t pjsua_acc_is_valid(pjsua_acc_id acc_id) { return acc_id >= 0 ? PJ_TRUE : PJ_FALSE; }
inline pj_pool_t *pjsua_pool_create(const char *, std::size_t, std::size_t) { return static_cast<pj_pool_t *>(pj_pool_create(nullptr, 0, 0)); }
inline pj_status_t pjmedia_tonegen_create(pj_pool_t *, unsigned, unsigned, unsigned, unsigned, unsigned, pjmedia_port **tonegen)
{
    if (tonegen)
        *tonegen = new pjmedia_port{};
    return PJ_SUCCESS;
}
inline pj_bool_t pjmedia_tonegen_is_busy(pjmedia_port *) { return PJ_FALSE; }
inline void pjmedia_tonegen_play_digits(pjmedia_port *, unsigned, const pjmedia_tone_digit *, unsigned) {}
inline pj_status_t pjsua_schedule_timer(pj_timer_entry *entry, const pj_time_val *)
{
    if (entry)
        entry->cancelled = false;
    return PJ_SUCCESS;
}
inline void pjsua_cancel_timer(pj_timer_entry *entry)
{
    if (entry)
        entry->cancelled = true;
}
inline pj_status_t pjsua_call_xfer(pjsua_call_id, const pj_str_t *, const pjsua_msg_data *) { return PJ_SUCCESS; }
inline void *pjsip_msg_find_hdr_by_name(pjsip_msg *, const pj_str_t *, const void *) { return nullptr; }
inline void pjsip_generic_string_hdr_init2(pjsip_generic_string_hdr *h, pj_str_t *hname, pj_str_t *hvalue)
{
    if (!h)
        return;
    h->hdr.name = hname ? *hname : pj_str("");
    h->hvalue = hvalue ? *hvalue : pj_str("");
}
inline pjsip_generic_string_hdr *pjsip_generic_string_hdr_create(pj_pool_t *, const pj_str_t *hname, const pj_str_t *hvalue)
{
    auto *hdr = new pjsip_generic_string_hdr{};
    hdr->hdr.name = hname ? *hname : pj_str("");
    hdr->hvalue = hvalue ? *hvalue : pj_str("");
    return hdr;
}
inline pjsip_generic_string_hdr *pjsip_generic_string_hdr_init(pj_pool_t *, void *mem, const pj_str_t *hname, const pj_str_t *hvalue)
{
    auto *hdr = static_cast<pjsip_generic_string_hdr *>(mem);
    if (hdr)
        pjsip_generic_string_hdr_init2(hdr, const_cast<pj_str_t *>(hname), const_cast<pj_str_t *>(hvalue));
    return hdr;
}
inline int pjsip_method_cmp(const pjsip_method *lhs, const pjsip_method *rhs)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);
    return pj_stricmp2(&lhs->name, rhs->name.ptr) - 0;
}
inline pj_status_t pjsip_endpt_create_response(void *, pjsip_rx_data *, int, const void *, pjsip_tx_data **tdata)
{
    if (tdata)
        *tdata = new pjsip_tx_data{};
    return PJ_SUCCESS;
}
inline pj_status_t pjsip_tsx_send_msg(pjsip_transaction *, pjsip_tx_data *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_create() { return PJ_SUCCESS; }
inline pj_status_t pjsua_init(void *, void *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_start() { return PJ_SUCCESS; }
inline void pjsua_destroy() {}
inline pj_status_t pjsua_transport_create(int, void *, pjsua_transport_id *id)
{
    if (id)
        *id = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_calls(pjsua_call_id *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_conf_ports(pjsua_conf_port_id *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_accs(pjsua_acc_id *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_snd_devs(pjmedia_snd_dev_info *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_aud_devs(pjmedia_aud_dev_info *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_vid_enum_devs(pjmedia_vid_dev_info *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_enum_codecs(pjsua_codec_info *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_vid_enum_codecs(pjsua_codec_info *, unsigned *count)
{
    if (count)
        *count = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_call_get_info(pjsua_call_id, pjsua_call_info *info)
{
    if (info)
        *info = {};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_call_get_med_transport_info(pjsua_call_id, int, pjmedia_transport_info *info)
{
    if (info)
        *info = {};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_call_get_stream_info(pjsua_call_id, int, pjsua_stream_info *info)
{
    if (info)
        *info = {};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_acc_get_info(pjsua_acc_id, pjsua_acc_info *info)
{
    if (info)
        *info = pjsua_acc_info{};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_acc_get_config(pjsua_acc_id, pj_pool_t *, pjsua_acc_config *cfg)
{
    if (cfg)
        *cfg = pjsua_acc_config{};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_acc_modify(pjsua_acc_id, const pjsua_acc_config *)
{
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_acc_add(const pjsua_acc_config *, pj_bool_t, pjsua_acc_id *acc_id)
{
    if (acc_id)
        *acc_id = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_acc_del(pjsua_acc_id) { return PJ_SUCCESS; }
inline pj_status_t pjsua_acc_set_online_status2(pjsua_acc_id, pj_bool_t, const pjrpid_element *) { return PJ_SUCCESS; }

inline pj_status_t pjsua_buddy_get_info(pjsua_buddy_id, pjsua_buddy_info *info)
{
    if (info)
        *info = {};
    return PJ_SUCCESS;
}

inline void *pjsua_buddy_get_user_data(pjsua_buddy_id) { return nullptr; }
inline void *pjsua_call_get_user_data(pjsua_call_id) { return nullptr; }
inline pj_status_t pjsua_call_set_user_data(pjsua_call_id, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_hangup_all() { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_hangup(pjsua_call_id, int, void *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_answer(pjsua_call_id, unsigned, void *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_answer2(pjsua_call_id, const pjsua_call_setting *, unsigned, void *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_make_call(pjsua_acc_id, const pj_str_t *, void *, void *, void *, pjsua_call_id *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_send_request(pjsua_call_id, const pj_str_t *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_set_hold(pjsua_call_id, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_reinvite(pjsua_call_id, int, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_dial_dtmf(pjsua_call_id, const pj_str_t *) { return PJ_SUCCESS; }
inline pj_bool_t pjsua_call_is_active(pjsua_call_id) { return PJ_FALSE; }
inline pj_bool_t pjsua_call_has_media(pjsua_call_id) { return PJ_FALSE; }
inline unsigned pjsua_call_get_count() { return 0; }
inline pjsua_conf_port_id pjsua_call_get_conf_port(pjsua_call_id) { return 0; }
inline pj_status_t acquire_call(const char *, pjsua_call_id, pjsua_call **call, pjsip_dialog **dlg)
{
    static pjsua_call_inv inv;
    static pjsua_call dummyCall;
    dummyCall.inv = &inv;
    if (call)
        *call = &dummyCall;
    if (dlg)
        *dlg = nullptr;
    return PJ_SUCCESS;
}
inline void pjsip_dlg_dec_lock(pjsip_dialog *) {}

inline pj_status_t pjsua_conf_add_port(pj_pool_t *, pjmedia_port *, pjsua_conf_port_id *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_conf_connect(pjsua_conf_port_id, pjsua_conf_port_id) { return PJ_SUCCESS; }
inline pj_status_t pjsua_conf_disconnect(pjsua_conf_port_id, pjsua_conf_port_id) { return PJ_SUCCESS; }
inline pj_status_t pjsua_conf_remove_port(pjsua_conf_port_id) { return PJ_SUCCESS; }
inline unsigned pjsua_conf_get_active_ports() { return 0; }
inline pj_status_t pjsua_conf_adjust_rx_level(pjsua_conf_port_id, float) { return PJ_SUCCESS; }
inline pj_status_t pjsua_conf_adjust_tx_level(pjsua_conf_port_id, float) { return PJ_SUCCESS; }
inline pj_status_t pjsua_conf_get_port_info(pjsua_conf_port_id, pjsua_conf_port_info *info)
{
    if (info)
        *info = {};
    return PJ_SUCCESS;
}
inline pj_status_t pjsua_conf_get_signal_level(pjsua_conf_port_id, int *tx, int *rx)
{
    if (tx)
        *tx = 0;
    if (rx)
        *rx = 0;
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_recorder_create(const pj_str_t *, unsigned, void *, int, pjsua_recorder_id *id)
{
    if (id)
        *id = 0;
    return PJ_SUCCESS;
}
inline pj_status_t pjsua_recorder_create(const pj_str_t *, unsigned, void *, int, int, pjsua_recorder_id *id)
{
    if (id)
        *id = 0;
    return PJ_SUCCESS;
}
inline pjsua_conf_port_id pjsua_recorder_get_conf_port(pjsua_recorder_id) { return 0; }
inline pj_status_t pjsua_recorder_destroy(pjsua_recorder_id) { return PJ_SUCCESS; }

inline pj_status_t pjsua_player_create(const pj_str_t *, unsigned, pjsua_player_id *id)
{
    if (id)
        *id = 0;
    return PJ_SUCCESS;
}
inline pj_status_t pjsua_player_get_port(pjsua_player_id, pjmedia_port **port)
{
    if (port)
        *port = nullptr;
    return PJ_SUCCESS;
}
inline pjmedia_port *pjsua_player_get_port(pjsua_player_id) { return nullptr; }
inline pjsua_conf_port_id pjsua_player_get_conf_port(pjsua_player_id) { return 0; }
inline pj_status_t pjsua_player_destroy(pjsua_player_id) { return PJ_SUCCESS; }

inline pj_status_t pjsua_get_snd_dev(int *in, int *out)
{
    if (in)
        *in = 0;
    if (out)
        *out = 0;
    return PJ_SUCCESS;
}
inline pj_status_t pjsua_set_snd_dev2(pjsua_snd_dev_param *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_set_null_snd_dev() { return PJ_SUCCESS; }
inline pj_bool_t pjsua_snd_is_active() { return PJ_FALSE; }

inline pj_status_t pjsua_verify_sip_url(const char *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_im_send(pjsua_acc_id, const pj_str_t *, const pj_str_t *, const pj_str_t *, void *, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_handle_ip_change(const pjsua_ip_change_param *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_schedule_timer(int, unsigned, void *) { return PJ_SUCCESS; }

inline pj_status_t pjsua_codec_get_param(const pj_str_t *, pjmedia_codec_param *param)
{
    if (param)
        *param = {};
    return PJ_SUCCESS;
}
inline pj_status_t pjsua_codec_set_param(const pj_str_t *, const pjmedia_codec_param *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_codec_set_priority(const pj_str_t *, unsigned) { return PJ_SUCCESS; }

inline pj_status_t pjsua_vid_codec_get_param(const pj_str_t *, pjmedia_vid_codec_param *param)
{
    if (param)
        *param = {};
    return PJ_SUCCESS;
}

inline pj_status_t pjsua_vid_preview_start(pjmedia_vid_dev_index, pjsua_vid_preview_param *) { return PJ_SUCCESS; }
inline void pjsua_vid_preview_stop(pjmedia_vid_dev_index) {}
inline pjsua_vid_win_id pjsua_vid_preview_get_win(pjmedia_vid_dev_index) { return PJSUA_INVALID_ID; }
inline void pjsua_vid_win_get_info(pjsua_vid_win_id, pjsua_vid_win_info *info)
{
    if (info)
        *info = {};
}
inline void pjsua_vid_win_set_pos(pjsua_vid_win_id, const pjmedia_coord *) {}
inline void pjsua_vid_win_set_size(pjsua_vid_win_id, const pjmedia_rect_size *) {}

inline pj_status_t pjsua_call_get_stream_stat(pjsua_call_id, int, pjsua_stream_stat *stat)
{
    if (stat)
        *stat = {};
    return PJ_SUCCESS;
}

inline void pjsua_vid_preview_param_default(pjsua_vid_preview_param *param)
{
    if (param)
        *param = {};
}

inline pj_status_t pjsua_call_get_med_tp_info(pjsua_call_id, int, void *) { return PJ_SUCCESS; }
inline pj_status_t pjsua_call_get_stream_info2(pjsua_call_id, int, void *) { return PJ_SUCCESS; }
inline int pjsua_get_state() { return 0; }

template <typename... Args>
inline pj_status_t pjsua_call_hangup_all_noincoming(Args&&...) { return PJ_SUCCESS; }
