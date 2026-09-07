#include "HidHeadsetManager.h"
#include <QDebug>

namespace Onyx {

HidHeadsetManager::HidHeadsetManager(QObject* parent)
    : QObject(parent) {
    hid_init();
}

HidHeadsetManager::~HidHeadsetManager() {
    stopMonitoring();
    hid_exit();
}

QList<HidDeviceInfo> HidHeadsetManager::enumerateDevices() {
    QList<HidDeviceInfo> list;
    struct hid_device_info* devs = hid_enumerate(0x0, 0x0);
    struct hid_device_info* cur = devs;

    while (cur) {
        HidDeviceInfo info;
        info.path = QString::fromUtf8(cur->path);
        info.vendorId = cur->vendor_id;
        info.productId = cur->product_id;
        info.manufacturer = cur->manufacturer_string ? QString::fromWCharArray(cur->manufacturer_string) : "Generic";
        info.product = cur->product_string ? QString::fromWCharArray(cur->product_string) : "HID Device";
        info.productString = info.product;
        info.usagePage = cur->usage_page;
        info.usage = cur->usage;

        // Telephony (0x0B) or Consumer Audio (0x0C)
        if (cur->usage_page == 0x0B || cur->usage_page == 0x0C) {
            info.isTelephony = true;
        }

        list.append(info);
        cur = cur->next;
    }

    hid_free_enumeration(devs);
    return list;
}

bool HidHeadsetManager::startMonitoring() {
    if (m_monitoring) return true;

    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &HidHeadsetManager::pollDevices);
    m_pollTimer->start(50); // Poll every 50ms for low latency button events
    m_monitoring = true;

    // Trigger initial enumeration
    pollDevices();
    return true;
}

void HidHeadsetManager::stopMonitoring() {
    if (!m_monitoring) return;

    if (m_pollTimer) {
        m_pollTimer->stop();
        delete m_pollTimer;
        m_pollTimer = nullptr;
    }

    if (m_activeDevice) {
        hid_close(m_activeDevice);
        m_activeDevice = nullptr;
        m_activeDevicePath.clear();
        emit headsetDisconnected();
        emit deviceDisconnected();
    }

    m_monitoring = false;
}

void HidHeadsetManager::pollDevices() {
    if (!m_activeDevice) {
        // Try auto-reconnecting to any detected telephony headset
        auto devices = enumerateDevices();
        for (const auto& dev : devices) {
            if (dev.isTelephony) {
                m_activeDevice = hid_open_path(dev.path.toUtf8().constData());
                if (m_activeDevice) {
                    hid_set_nonblocking(m_activeDevice, 1);
                    m_activeDevicePath = dev.path;
                    emit headsetConnected(dev);
                    emit deviceConnected(dev);
                    emit deviceEventLogged(QString("Auto-connected to Headset: %1").arg(dev.product));
                    break;
                }
            }
        }
        return;
    }

    unsigned char buf[64];
    int bytesRead = hid_read(m_activeDevice, buf, sizeof(buf));
    if (bytesRead > 0) {
        processInputReport(buf, bytesRead);
    } else if (bytesRead < 0) {
        // Device disconnected
        hid_close(m_activeDevice);
        m_activeDevice = nullptr;
        m_activeDevicePath.clear();
        emit headsetDisconnected();
        emit deviceDisconnected();
        emit deviceEventLogged("Headset disconnected");
    }
}

void HidHeadsetManager::processInputReport(const unsigned char* data, int length) {
    if (length <= 0) return;

    // Standard Telephony / Consumer control report parsing
    for (int i = 0; i < length; ++i) {
        unsigned char b = data[i];
        if (b & 0x01) {
            emit hookSwitchPressed();
            emit deviceEventLogged("Headset Event: Hook Switch / Answer / Hangup Pressed");
        }
        if (b & 0x02) {
            emit muteButtonPressed();
            emit deviceEventLogged("Headset Event: Mic Mute Toggled");
        }
        if (b & 0x04) {
            emit volumeUpPressed();
            emit deviceEventLogged("Headset Event: Volume Up Pressed");
        }
        if (b & 0x08) {
            emit volumeDownPressed();
            emit deviceEventLogged("Headset Event: Volume Down Pressed");
        }
    }
}

} // namespace Onyx
