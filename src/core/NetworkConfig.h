#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H

#include <QString>

namespace Onyx {

inline const QString APP_NAME = "Onyx";
inline const QString APP_VERSION = "2.0.0";

// UDP Multicast Peer Discovery
inline const QString MULTICAST_GROUP = "239.255.42.99";
inline constexpr quint16 DISCOVERY_PORT = 45454;
inline constexpr int BEACON_INTERVAL_MS = 3000;
inline constexpr int PEER_TIMEOUT_MS = 9000;

// LAN Group Chat & Signaling Port
inline constexpr quint16 CHAT_SIGNALING_PORT = 45456;

// Direct / Group Audio & Video Streaming Ports (UDP)
inline constexpr quint16 GROUP_AUDIO_PORT = 45457;
inline constexpr quint16 GROUP_VIDEO_PORT = 45458;

// TCP High-Speed Encrypted File Transfer
inline constexpr quint16 FILE_TRANSFER_PORT = 45455;
inline constexpr qint64 FILE_CHUNK_SIZE = 64 * 1024; // 64 KB chunks

// VoIP / SIP Signaling & Media RTP
inline constexpr quint16 SIP_PORT = 5060;
inline constexpr quint16 AUDIO_RTP_PORT_START = 40000;
inline constexpr quint16 VIDEO_RTP_PORT_START = 40100;

// Audio Configuration
inline constexpr int AUDIO_SAMPLE_RATE = 48000;
inline constexpr int AUDIO_CHANNELS = 1;
inline constexpr int AUDIO_FRAME_SIZE_MS = 20; // 20 ms frames
inline constexpr int OPUS_SAMPLES_PER_FRAME = (AUDIO_SAMPLE_RATE * AUDIO_FRAME_SIZE_MS) / 1000; // 960 samples

// Video Configuration & Presets
inline constexpr int DEFAULT_VIDEO_WIDTH = 1280;
inline constexpr int DEFAULT_VIDEO_HEIGHT = 720;
inline constexpr int DEFAULT_VIDEO_FPS = 30;

// Security & E2EE Constants
inline constexpr int AES_KEY_SIZE = 32;       // 256-bit AES
inline constexpr int GCM_IV_SIZE = 12;        // 96-bit IV
inline constexpr int GCM_TAG_SIZE = 16;       // 128-bit authentication tag
inline const QString DEFAULT_ROOM_SECRET = "Onyx-LAN-Secure-Passkey-2026";

} // namespace Onyx

#endif // NETWORK_CONFIG_H
