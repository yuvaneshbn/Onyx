#define THIS_FILENAME "Hid.cpp"

#include "Hid.h"

#include <QDebug>
#include <QList>
#include <cstring>

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "Dialer.h"
#include "settings.h"
#include "mainDlg.h"

// Original HIDAPI implementation (unchanged)
#include "hid.c"

//#include <SetupAPI.h>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "hid")

#define HID_USAGE_TELEPHONY_HOOK_SWITCH                 ((USAGE) 0x20)
#define HID_USAGE_TELEPHONY_MUTE                 ((USAGE) 0x2F)
#define HID_USAGE_TELEPHONY_REDIAL                 ((USAGE) 0x24)
#define HID_USAGE_TELEPHONY_FLASH                 ((USAGE) 0x21)
#define HID_USAGE_TELEPHONY_LINE_BUSY                 ((USAGE) 0x97)

hid_device *Hid::m_hDevice = NULL;
bool Hid::m_stateOffhook = false;
bool Hid::m_stateRing = false;
bool Hid::m_stateMute = false;

bool m_Offhook = false;
bool m_Redial = false;

UCHAR Hid::m_reportIdOnline = 0;
UCHAR Hid::m_reportIdOffhook = 0;
UCHAR Hid::m_reportIdRing = 0;
UCHAR Hid::m_reportIdMute = 0;

static bool offHookIsAbsolute = false;
static bool muteIsAbsolute = false;
static bool redialIsAbsolute = false;

static bool rejectCall = false;

// Singleton instance (needed for QTimer)
Hid *Hid::s_instance = nullptr;

Hid::Hid(QObject *parent)
    : QObject(parent)
    , m_pollTimer(nullptr)
    , m_timerId(0)
{
    s_instance = this;
}

Hid::~Hid()
{
    if (m_pollTimer) {
        m_pollTimer->stop();
        delete m_pollTimer;
    }
    s_instance = nullptr;
}

// Timer callback replaced by QTimer event
void Hid::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_timerId)
        return;

    if (!m_hDevice) {
        return;
    }

    int ReportLength = m_hDevice->input_report_length;
    unsigned char* Report = (unsigned char*)m_hDevice->read_buf;
    ReportLength = hid_read(m_hDevice, Report, ReportLength);

    if (ReportLength > 0) {
        UCHAR ReportId = Report[0];
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "IN --- Received report %d, length %d bytes ---", ReportId, ReportLength));
        }
        USAGE UsagePage = 0x0b;
        USAGE UsageList[32];
        ULONG UsageLength = 32;
        NTSTATUS status = HidP_GetUsages(
            HidP_Input,
            UsagePage,
            0,
            UsageList,
            &UsageLength,
            m_hDevice->pp_data,
            (PCHAR)Report,
            ReportLength
        );
        if (status != HIDP_STATUS_SUCCESS) {
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "HidP_GetUsages failed with code 0x%04hx", status));
            }
        }
        else {
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "IN get %d Usage(s):", UsageLength));
            }
            bool offHook = false;
            bool lineBusy = false;
            bool mute = false;
            bool redial = false;
            for (unsigned int j = 0; j < UsageLength; j++) {
                if (is_pjsua_running()) {
                    PJ_LOG(3, (THIS_FILENAME, "IN %02hhx", UsageList[j]));
                }
                switch (UsageList[j]) {
                case HID_USAGE_TELEPHONY_HOOK_SWITCH:
                    offHook = true;
                    break;
                case HID_USAGE_TELEPHONY_MUTE:
                    mute = true;
                    break;
                case HID_USAGE_TELEPHONY_REDIAL:
                    redial = true;
                    break;
                case HID_USAGE_TELEPHONY_FLASH:
                    if (is_pjsua_running()) {
                        PJ_LOG(3, (THIS_FILENAME, "IN Event: Flash (not implemented)"));
                    }
                    break;
                case HID_USAGE_TELEPHONY_LINE_BUSY:
                    lineBusy = true;
                    break;
                }
            }
            //-- REDIAL
            if (!redialIsAbsolute) {
                if (!redial) {
                    redial = m_Redial;
                }
                else {
                    redial = !m_Redial;
                }
            }
            if (redial) {
                if (is_pjsua_running()) {
                    PJ_LOG(3, (THIS_FILENAME, "IN Event: Redial"));
                }
                mainDlg->pageDialer->OnBnClickedRedial();
            }
            //-- MUTE
            if (!muteIsAbsolute) {
                if (!mute) {
                    mute = m_stateMute;
                }
                else {
                    mute = !m_stateMute;
                }
            }
            if (m_stateMute != mute) {
                mainDlg->pageDialer->MuteInput(mute);
                m_stateMute = mute;
            }
            //-- OFF-HOOK
            if (!offHookIsAbsolute) {
                if (!offHook) {
                    offHook = m_Offhook;
                }
                else {
                    offHook = !m_Offhook;
                }
            }
            if (offHook) {
                if (!m_Offhook) {
                    if (!m_stateOffhook) {
                        if (m_stateRing) {
                            if (is_pjsua_running()) {
                                PJ_LOG(3, (THIS_FILENAME, "IN Event: Answer incoming call"));
                            }
                            mainDlg->CommandCallAnswer();
                        }
                        else {
                            if (is_pjsua_running()) {
                                PJ_LOG(3, (THIS_FILENAME, "IN Event: Outgoing call"));
                            }
                            mainDlg->pageDialer->Action(ACTION_CALL);
                        }
                    }
                }
                else if (m_stateRing) {
                    if (rejectCall) {
                        if (is_pjsua_running()) {
                            PJ_LOG(3, (THIS_FILENAME, "IN Event: Reject call during an active call"));
                        }
                        mainDlg->CommandCallReject();
                    }
                    else {
                        rejectCall = true;
                        return;
                    }
                }
            }
            else {
                if (m_Offhook) {
                    if (m_stateOffhook) {
                        if (is_pjsua_running()) {
                            PJ_LOG(3, (THIS_FILENAME, "IN Event: End call"));
                        }
                        call_hangup_all_noincoming(true);
                    }
                }
                else if (m_stateRing) {
                    if (rejectCall) {
                        if (is_pjsua_running()) {
                            PJ_LOG(3, (THIS_FILENAME, "IN Event: Reject incoming call"));
                        }
                        mainDlg->CommandCallReject();
                    }
                    else {
                        rejectCall = true;
                        return;
                    }
                }
            }
            m_Offhook = offHook;
            rejectCall = false;
        }
    }
    if (ReportLength < 0) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "hid_read() returned error"));
        }
        CloseDevice();
    }
}

void Hid::UpdateLED(USAGE Usage, USAGE Usage2)
{
    if (!m_hDevice) {
        return;
    }
    if (is_pjsua_running()) {
        PJ_LOG(3, (THIS_FILENAME, "OUT --- Update LEDs state, Usages: 0x%02hx 0x%02hx ---", Usage, Usage2));
    }
    ULONG ReportLength = m_hDevice->caps.OutputReportByteLength;
    if (is_pjsua_running()) {
        PJ_LOG(3, (THIS_FILENAME, "OUT Report length %d", ReportLength));
    }
    if (ReportLength <= 0) {
        return;
    }
    unsigned char *Report = new (std::nothrow) unsigned char[ReportLength];
    if (Report == nullptr) {
        return;
    }
    USAGE UsagePage = HID_USAGE_PAGE_LED;
    HIDP_BUTTON_CAPS ButtonCaps[32];
    USHORT ButtonCapsLength = 32;

    QList<UCHAR> ReportList;   // replaces MFC CList<UCHAR, UCHAR>
    if (!Usage || Usage == HID_USAGE_LED_ON_LINE || Usage2 == HID_USAGE_LED_ON_LINE) {
        // if (m_reportIdOnline && !ReportList.contains(m_reportIdOnline))
        //     ReportList.append(m_reportIdOnline);
    }
    if (!Usage || Usage == HID_USAGE_LED_OFF_HOOK || Usage2 == HID_USAGE_LED_OFF_HOOK) {
        if (m_reportIdOnline && !ReportList.contains(m_reportIdOnline))
            ReportList.append(m_reportIdOnline);
        if (m_reportIdOffhook && !ReportList.contains(m_reportIdOffhook))
            ReportList.append(m_reportIdOffhook);
    }
    if (!Usage || Usage == HID_USAGE_LED_RING || Usage2 == HID_USAGE_LED_RING) {
        if (m_reportIdRing && !ReportList.contains(m_reportIdRing))
            ReportList.append(m_reportIdRing);
    }
    if (!Usage || Usage == HID_USAGE_LED_MUTE || Usage2 == HID_USAGE_LED_MUTE) {
        if (m_reportIdMute && !ReportList.contains(m_reportIdMute))
            ReportList.append(m_reportIdMute);
    }

    for (UCHAR ReportID : ReportList) {
        ZeroMemory(Report, ReportLength);
        Report[0] = ReportID;
        USAGE UsageList[8];
        ULONG UsageLength = 0;
        if (ReportID == m_reportIdOnline) {
            if (m_stateOffhook) {
                UsageList[UsageLength] = HID_USAGE_LED_ON_LINE;
                UsageLength++;
            }
        }
        if (ReportID == m_reportIdOffhook) {
            if (m_stateOffhook) {
                UsageList[UsageLength] = HID_USAGE_LED_OFF_HOOK;
                UsageLength++;
            }
        }
        if (ReportID == m_reportIdRing) {
            if (m_stateRing) {
                UsageList[UsageLength] = HID_USAGE_LED_RING;
                UsageLength++;
            }
        }
        if (ReportID == m_reportIdMute) {
            if (m_stateMute) {
                UsageList[UsageLength] = HID_USAGE_LED_MUTE;
                UsageLength++;
            }
        }
        HidP_SetUsages(
            HidP_Output,
            UsagePage,
            0,
            UsageList,
            &UsageLength,
            m_hDevice->pp_data,
            (PCHAR)Report,
            ReportLength
        );
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Report:"));
            for (int j = 0; j < (int)ReportLength; j++) {
                PJ_LOG(3, (THIS_FILENAME, "0x%02hx", Report[j]));
            }
            PJ_LOG(3, (THIS_FILENAME, "OUT set %d Usage(s), report id %d", UsageLength, ReportID));
        }
        ULONG BytesWritten = hid_write(m_hDevice, Report, ReportLength);
        if (BytesWritten < 0) {
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "hid_write() returned error %ls", hid_error(m_hDevice)));
            }
            return;
        }
    }
    delete[] Report;
}

void Hid::OpenDevice()
{
    if (m_hDevice) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Device already opened"));
        }
        return;
    }
    struct hid_device_info *devs, *cur_dev, *sel_dev = NULL;
    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Device Found %04hx:%04hx (usage: %04hx:%04hx)", cur_dev->vendor_id, cur_dev->product_id, cur_dev->usage_page, cur_dev->usage));
            PJ_LOG(3, (THIS_FILENAME, "  Manufacturer: %ls", cur_dev->manufacturer_string));
            PJ_LOG(3, (THIS_FILENAME, "  Product:      %ls", cur_dev->product_string));
        }
        if (!sel_dev && cur_dev->usage_page == 0x0b) {
            sel_dev = cur_dev;
        }
        if (cur_dev->usage_page == 0x0b && (cur_dev->usage == 0x05 || cur_dev->usage == 0x04)) {
            sel_dev = cur_dev;
        }
        cur_dev = cur_dev->next;
    }
    if (!sel_dev) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Telephony Device not found"));
        }
    }
    else {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Device Selected %04hx:%04hx (usage: %04hx:%04hx)", sel_dev->vendor_id, sel_dev->product_id, sel_dev->usage_page, sel_dev->usage));
        }
        m_hDevice = hid_open_path(sel_dev->path);
        if (!m_hDevice) {
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "hid_open_path() returned error"));
            }
        }
        else {
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "Device Opened %02hhx %02hhx /iof-rl %d %d %d /nlc %d /ni-bvd %d %d %d /no-bvd %d %d %d /nf-bvd %d %d %d",
                    m_hDevice->caps.UsagePage,
                    m_hDevice->caps.Usage,
                    m_hDevice->caps.InputReportByteLength,
                    m_hDevice->caps.OutputReportByteLength,
                    m_hDevice->caps.FeatureReportByteLength,
                    m_hDevice->caps.NumberLinkCollectionNodes,
                    m_hDevice->caps.NumberInputButtonCaps,
                    m_hDevice->caps.NumberInputValueCaps,
                    m_hDevice->caps.NumberInputDataIndices,
                    m_hDevice->caps.NumberOutputButtonCaps,
                    m_hDevice->caps.NumberOutputValueCaps,
                    m_hDevice->caps.NumberOutputDataIndices,
                    m_hDevice->caps.NumberFeatureButtonCaps,
                    m_hDevice->caps.NumberFeatureValueCaps,
                    m_hDevice->caps.NumberFeatureDataIndices
                    ));
            }
            hid_set_nonblocking(m_hDevice, 1);

            HIDP_BUTTON_CAPS ButtonCaps[256];
            USHORT ButtonCapsLength;

            ButtonCapsLength = 256;
            HidP_GetButtonCaps(
                HidP_Input,
                ButtonCaps,
                &ButtonCapsLength,
                m_hDevice->pp_data
            );
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "Input ButtonCaps %d:", ButtonCapsLength));
            }
            for (int i = 0; i < ButtonCapsLength; i++) {
                if (is_pjsua_running()) {
                    PJ_LOG(3, (THIS_FILENAME, "%02hhx %d %d %d %d %d %d %02hhx %02hhx %02hhx",
                        ButtonCaps[i].UsagePage,
                        ButtonCaps[i].ReportID,
                        ButtonCaps[i].IsAlias,
                        ButtonCaps[i].IsRange,
                        ButtonCaps[i].IsStringRange,
                        ButtonCaps[i].IsDesignatorRange,
                        ButtonCaps[i].IsAbsolute,
                        ButtonCaps[i].Range.UsageMin,
                        ButtonCaps[i].Range.UsageMax,
                        ButtonCaps[i].NotRange.Usage
                        ));
                }
                if (ButtonCaps[i].UsagePage == HID_USAGE_PAGE_TELEPHONY) {
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_TELEPHONY_HOOK_SWITCH) {
                        offHookIsAbsolute = ButtonCaps[i].IsAbsolute;
                    }
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_TELEPHONY_MUTE) {
                        muteIsAbsolute = ButtonCaps[i].IsAbsolute;
                    }
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_TELEPHONY_REDIAL) {
                        redialIsAbsolute = ButtonCaps[i].IsAbsolute;
                    }
                }
            }

            m_reportIdOnline = 0;
            m_reportIdOffhook = 0;
            m_reportIdRing = 0;

            ButtonCapsLength = 256;
            HidP_GetButtonCaps(
                HidP_Output,
                ButtonCaps,
                &ButtonCapsLength,
                m_hDevice->pp_data
            );
            if (is_pjsua_running()) {
                PJ_LOG(3, (THIS_FILENAME, "Output ButtonCaps %d:", ButtonCapsLength));
            }
            for (int i = 0; i < ButtonCapsLength; i++) {
                if (is_pjsua_running()) {
                    PJ_LOG(3, (THIS_FILENAME, "%02hhx %d %d %d %d %d %d %02hhx %02hhx %02hhx",
                        ButtonCaps[i].UsagePage,
                        ButtonCaps[i].ReportID,
                        ButtonCaps[i].IsAlias,
                        ButtonCaps[i].IsRange,
                        ButtonCaps[i].IsStringRange,
                        ButtonCaps[i].IsDesignatorRange,
                        ButtonCaps[i].IsAbsolute,
                        ButtonCaps[i].Range.UsageMin,
                        ButtonCaps[i].Range.UsageMax,
                        ButtonCaps[i].NotRange.Usage
                        ));
                }
                if (ButtonCaps[i].UsagePage == HID_USAGE_PAGE_LED) {
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_LED_ON_LINE) {
                        m_reportIdOnline = ButtonCaps[i].ReportID;
                    }
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_LED_OFF_HOOK) {
                        m_reportIdOffhook = ButtonCaps[i].ReportID;
                    }
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_LED_RING) {
                        m_reportIdRing = ButtonCaps[i].ReportID;
                    }
                    if (ButtonCaps[i].NotRange.Usage == HID_USAGE_LED_MUTE) {
                        m_reportIdMute = ButtonCaps[i].ReportID;
                    }
                }
            }
            if (!HidD_FlushQueue(m_hDevice->device_handle)) {
                if (is_pjsua_running()) {
                    PJ_LOG(3, (THIS_FILENAME, "FlushQueue failed"));
                }
            }
            m_Redial = false;
            m_Offhook = m_stateOffhook;

            UpdateLED();

            // Replace SetTimer with QTimer
            if (s_instance) {
                if (!s_instance->m_pollTimer) {
                    s_instance->m_pollTimer = new QTimer(s_instance);
                    QObject::connect(s_instance->m_pollTimer, &QTimer::timeout,
                                     s_instance, []() {
                                         if (Hid::s_instance) {
                                             Hid::s_instance->timerEvent(nullptr);
                                         }
                                     });
                }
                s_instance->m_pollTimer->start(200);
                s_instance->m_timerId = 1; // dummy
            }
        }
    }
    hid_free_enumeration(devs);
}

void Hid::CloseDevice(bool exit)
{
    if (!m_hDevice) {
        return;
    }

    // Kill timer
    if (s_instance && s_instance->m_pollTimer) {
        s_instance->m_pollTimer->stop();
        delete s_instance->m_pollTimer;
        s_instance->m_pollTimer = nullptr;
    }

    if (exit) {
        m_stateOffhook = false;
        m_stateRing = false;
        UpdateLED();
    }
    if (is_pjsua_running()) {
        PJ_LOG(3, (THIS_FILENAME, "Device Closed"));
    }

    hid_close(m_hDevice);
    hid_exit();
    m_hDevice = NULL;
}

void Hid::SetOnline(bool state)
{
    // (original was empty aside from logging; kept as is)
}

void Hid::SetOffhook(bool state)
{
    if (m_stateOffhook != state) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Set off-hook: %s", state ? "Yes" : "No"));
        }
        m_stateOffhook = state;
        UpdateLED(HID_USAGE_LED_OFF_HOOK);
    }
}

void Hid::SetRing(bool state)
{
    if (m_stateRing != state) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Set ring: %s", state ? "Yes" : "No"));
        }
        m_stateRing = state;
        UpdateLED(HID_USAGE_LED_RING);
    }
}

void Hid::SetOffhookRing(bool stateOffhook, bool stateRing)
{
    if (m_stateOffhook != stateOffhook && m_stateRing != stateRing) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Set off-hook: %s", stateOffhook ? "Yes" : "No"));
            PJ_LOG(3, (THIS_FILENAME, "Set ring: %s", stateRing ? "Yes" : "No"));
        }
        m_stateOffhook = stateOffhook;
        m_stateRing = stateRing;
        UpdateLED(HID_USAGE_LED_OFF_HOOK, HID_USAGE_LED_RING);
    }
    else {
        SetOffhook(stateOffhook);
        SetRing(stateRing);
    }
}

void Hid::SetMute(bool state)
{
    if (m_stateMute != state) {
        if (is_pjsua_running()) {
            PJ_LOG(3, (THIS_FILENAME, "Set mute: %s", state ? "Yes" : "No"));
        }
        m_stateMute = state;
        UpdateLED(HID_USAGE_LED_MUTE);
    }
}
