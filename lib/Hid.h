#pragma once

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include <windows.h>
#include "HidApi.h"
#include "hidusage.h"

// Forward declaration of the C HID device structure (defined in hid.c)
struct hid_device_;

class Hid : public QObject
{
    Q_OBJECT

private:
    // Timer callback replaced by QTimer
    void timerEvent(QTimerEvent *event) override;

    static hid_device *m_hDevice;

    //	static bool m_stateOnline;
    static bool m_stateOffhook;
    static bool m_stateRing;
    static bool m_stateMute;
    static UCHAR m_reportIdOnline;
    static UCHAR m_reportIdOffhook;
    static UCHAR m_reportIdRing;
    static UCHAR m_reportIdMute;

    static void UpdateLED(USAGE Usage = 0, USAGE Usage2 = 0);

public:
    explicit Hid(QObject *parent = nullptr);
    virtual ~Hid();

    static void OpenDevice();
    static void CloseDevice(bool exit = false);
    static void SetOnline(bool state);
    static void SetOffhook(bool state);
    static void SetRing(bool state);
    static void SetOffhookRing(bool stateOffhook, bool stateRing);
    static void SetMute(bool state);

private:
    QTimer *m_pollTimer;          // replaces Windows SetTimer
    int m_timerId;                // keeps the original timer ID concept

    // Kept for compatibility with the original static callback approach.
    static Hid *s_instance;
};
