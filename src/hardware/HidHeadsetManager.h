#ifndef HID_HEADSET_MANAGER_H
#define HID_HEADSET_MANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QStringList>
#include <hidapi.h>

namespace Onyx {

struct HidDeviceInfo {
    QString path;
    quint16 vendorId = 0;
    quint16 productId = 0;
    QString manufacturer;
    QString product;
    QString productString;
    quint16 usagePage = 0;
    quint16 usage = 0;
    bool isTelephony = false;
};

class HidHeadsetManager : public QObject {
    Q_OBJECT
public:
    explicit HidHeadsetManager(QObject* parent = nullptr);
    ~HidHeadsetManager() override;

    bool startMonitoring();
    void stopMonitoring();

    QList<HidDeviceInfo> enumerateDevices();

signals:
    void headsetConnected(const HidDeviceInfo& device);
    void deviceConnected(const HidDeviceInfo& device);
    void headsetDisconnected();
    void deviceDisconnected();
    void hookSwitchPressed();     // Answer / End Call button on headset
    void muteButtonPressed();     // Physical mute button on headset
    void volumeUpPressed();       // Volume up button
    void volumeDownPressed();     // Volume down button
    void deviceEventLogged(const QString& event);

private slots:
    void pollDevices();

private:
    void processInputReport(const unsigned char* data, int length);

    hid_device* m_activeDevice = nullptr;
    QString m_activeDevicePath;
    QTimer* m_pollTimer = nullptr;
    bool m_monitoring = false;
};

} // namespace Onyx

#endif // HID_HEADSET_MANAGER_H
