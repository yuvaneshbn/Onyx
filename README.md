# Onyx

**High-Performance Decentralized LAN Voice, Video & Secure File Transfer Client**

[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-blue.svg)](#)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C.svg)](#)
[![Framework](https://img.shields.io/badge/Framework-Qt%206.11-41CD52.svg)](#)
[![Security](https://img.shields.io/badge/Encryption-AES--256--GCM-green.svg)](#)
[![Audio](https://img.shields.io/badge/Audio-Opus%20%7C%20AMR--WB-orange.svg)](#)
[![Video](https://img.shields.io/badge/Video-VP8%20%7C%20VP9-red.svg)](#)

Onyx is an enterprise-grade, peer-to-peer desktop communication platform designed for local area networks (LANs). It delivers ultra-low-latency voice and video conferencing, rich messaging, and high-speed encrypted file transfers without relying on external servers, cloud infrastructure, or internet connectivity.

---

## Key Features

### 1. Zero-Configuration Peer Discovery
- **UDP Multicast**: Automatically discovers peers on the local subnet via multicast group 239.255.42.99:45454.
- **Presence & Heartbeats**: Real-time broadcast of username, status message, IP address, and media capabilities with automatic timeout detection.
- **Dynamic Profile Updates**: Instant synchronization of user avatars, status messages, and call activity.

### 2. Multi-Party Voice Engine
- **Floating-Point Audio Mixing**: Low-latency multi-stream audio mixer utilizing 32-bit floating-point accumulation, dynamic automatic gain control (AGC), and soft-clipping protection.
- **Adaptive Codecs**:
  - **Opus (48 kHz)**: Fullband, high-fidelity voice transmission with dynamic packet loss concealment (PLC) and variable bitrates (16 kbps - 128 kbps).
  - **AMR-WB (16 kHz)**: Wideband telephony speech encoding and decoding via o-amrwbenc and opencore-amrwb.
  - **AMR-NB (8 kHz)**: Narrowband legacy speech codec via opencore-amrnb.
- **Voice Activity Detection (VAD)**: Energy-based VAD with configurable sensitivity thresholds to broadcast speaking state indicators and conserve network throughput.

### 3. Real-Time Video Conferencing
- **VP8 & VP9 Codecs**: Hardware-efficient video compression powered by libvpx-1.14.1.
- **Responsive Video Grid**: Dynamic participant tile layouts accommodating 1 to 16+ participants with green active-speaker glow indicators and fallback avatar initials.
- **High-Performance Color Conversion**: Fast color space conversion between RGB32 and YUV420P using FFmpeg (libswscale).
- **Packet Fragmentation & MTU Safety**: Frame chunking engine with sequence reassembly designed for standard 1500-byte Ethernet MTUs.
- **Session Recording**: Integrated live video recording to MP4 containers via FFmpeg (libavformat/libavcodec).

### 4. End-to-End Encryption (E2EE)
- **AES-256-GCM**: Authenticated encryption for all media streams, text chats, and file transfers using OpenSSL.
- **PBKDF2 Key Derivation**: 256-bit cryptographic keys derived from a shared room passkey via PBKDF2-HMAC-SHA256 (10,000 rounds).
- **Replay & Tampering Protection**: Unique 96-bit random initialization vectors (IV) and 128-bit authentication tags attached to every network datagram.
- **Visual Verification**: SHA-256 fingerprint badges displayed in user profiles and settings for out-of-band key verification.

### 5. High-Speed Encrypted File Transfer
- **Dedicated TCP Pipeline**: High-throughput file transfers over port 45455 using 64 KB chunking.
- **Data Integrity**: Pre-computed and verified SHA-256 checksums ensuring bit-exact delivery.
- **Transfer Manager**: Real-time progress monitoring, transfer speed calculation (MB/s), and completion tracking.

### 6. Hardware Telephony Headset Support
- **HID Integration**: Direct hardware control of USB and Bluetooth headsets via hidapi.
- **Call Controls**: Physical headset mute button detection, hook switch (call answer/hangup), and volume controls synchronized with the application.

### 7. Modern Slate Dark Interface
- **Navigation Rail**: Quick navigation between Chats, Calls, Files, and Settings with vector SVG icons.
- **Sidebar**: Search filtering, active conference indicator card with call duration timer, and live contact cards.
- **Floating Call Control Bar**: Floating, non-intrusive in-call toolbar with quick toggles for mic, camera, screen sharing, recording, and call termination.
- **Comprehensive Settings**: Audio gain boost (+0 dB to +20 dB), microphone VU test meter, video presets (1080p, 720p, 480p, 360p), frame rates, notifications, and security keys.
- **Startup Profile Prompt**: User profile configuration dialog on startup with persistent settings.

---

## Architecture & Network Protocols

`
                          +--------------------------------------------------+
                          |                   Onyx Client                    |
                          +--------------------------------------------------+
                               |              |              |              |
          +--------------------+              |              |              +--------------------+
          |                                   |              |                                   |
          v                                   v              v                                   v
+-------------------+               +-------------------+  +-------------------+       +-------------------+
|  Peer Discovery   |               |   Group Audio     |  |   Group Video     |       |   File Transfer   |
|   UDP Multicast   |               |    UDP Audio      |  |    UDP Video      |       |     TCP Stream    |
| 239.255.42.99:    |               |    Port 45457     |  |    Port 45458     |       |     Port 45455    |
|      45454        |               |   (Opus/AMR-WB)   |  |     (VP8/VP9)     |       |     (AES-GCM)     |
+-------------------+               +-------------------+  +-------------------+       +-------------------+
`

### Port Allocation Reference

| Port | Protocol | Purpose | Description |
| :--- | :--- | :--- | :--- |
| **45454** | UDP | Peer Discovery | Subnet multicast announcements and heartbeats (239.255.42.99) |
| **45455** | TCP | File Transfer | High-speed encrypted peer-to-peer file transfers |
| **45456** | UDP | Signaling & Chat | Encrypted instant messaging and conference signaling |
| **45457** | UDP | Group Voice | Mixed multi-stream Opus/AMR-WB audio packets |
| **45458** | UDP | Group Video | Chunked VP8/VP9 video frame packets |
| **5060** | UDP/TCP | SIP Signaling | PJSIP session initiation and direct peer routing |

---

## Core Dependencies

Onyx integrates the following libraries:

| Library | Version | Role in Onyx |
| :--- | :--- | :--- |
| **Qt 6** | 6.11.1 | Core application runtime, Widgets GUI, Network stack, and SVG rendering |
| **OpenSSL** | 4.0.1 | Cryptographic engine, AES-256-GCM encryption, PBKDF2 key derivation |
| **Opus** | 1.6.1 | Low-latency, high-definition audio compression and decoding |
| **libvpx** | 1.14.1 | VP8 and VP9 real-time video encoding and decoding |
| **FFmpeg** | 8.1.2 | High-performance color conversion (libswscale) and video recording (libavcodec/libavformat) |
| **pjproject** | 2.17 | SIP signaling, VoIP call orchestration, and RTP media routing |
| **vo-amrwbenc** | Latest | AMR Wideband (AMR-WB) speech encoding |
| **opencore-amr**| 0.1.6 | AMR-WB and AMR-NB speech decoding |
| **hidapi** | 0.14.0 | Telephony USB and Bluetooth HID headset button monitoring |
| **jsoncpp** | 1.9.6 | JSON message serialization and signaling protocol parser |

---

## Prerequisites

- **Operating System**: Windows 10 / 11 (64-bit)
- **Compiler**: Microsoft Visual Studio 2022 or 2026 Build Tools (MSVC v143 or 145 x64)
- **Build System**: CMake 3.16+ and Ninja
- **Framework**: Qt 6.x with MSVC 64-bit binaries

---

## Building from Source

### 1. Set Up Environment
Open a Developer Command Prompt or PowerShell with MSVC environment initialized:
`powershell
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
`

### 2. Configure with CMake
`powershell
cd /d "C:\Users\YUVANESH\Desktop\Voice"
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.11.1/msvc2022_64"
`

### 3. Build the Executable
`powershell
ninja
`

### 4. Deploy Qt Runtime
Deploy the necessary Qt libraries, SVG icon engine, and platform plugins:
`powershell
C:\Qt\6.11.1\msvc2022_64\bin\windeployqt.exe --no-translations Onyx.exe
`

---

## Running Onyx

Launch the compiled executable from the build directory:
`powershell
.\Onyx.exe
`

### Initial Launch
1. On startup, enter your **Display Name**, select a **Status Message**, and choose an **Avatar Color**.
2. Click **Join Onyx LAN** to enter the main window.
3. Onyx will immediately discover other instances running on your local network.

---

## Security Model

All communications in Onyx are protected by End-to-End Encryption:
- **Zero Central Server**: Encryption keys never leave the local devices.
- **Symmetric Cipher**: AES-256-GCM provides both confidentiality and data authenticity.
- **Key Derivation**: Keys are derived from a user-configurable Room Passkey using PBKDF2-HMAC-SHA256 with 10,000 iterations.
- **Cryptographic Nonce**: Every UDP packet carries an unpredictable 96-bit random IV and a 128-bit authentication tag to prevent replay attacks and tampering.
- **File Integrity**: Files are streamed in AES-GCM encrypted blocks and verified against an end-to-end SHA-256 checksum upon receipt.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
