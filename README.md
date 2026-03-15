# Serverless VoIP (VoIP_Serverless)

A production-grade, Serverless Voice over IP (VoIP) application written in **C++20**. The project features a decentralized architecture, network discovery, NAT traversal, robust audio processing, custom streaming protocols, and a modern GUI built with **Qt6**.

## Overview

This project implements a Serverless VoIP application designed to work over both LAN and WAN without relying on a central server for call routing. It features a decentralized peer-to-peer (P2P) / tree-based topology where nodes can dynamically elect routers (super-nodes) to handle stream multiplexing.

### Key Features
- **Serverless & Decentralized Architecture**: No single point of failure. The network is organized dynamically using a tree topology with automatic router election and failover.
- **Robust Audio Pipeline**: Comprehensive audio processing using the **Opus** codec, along with Automatic Gain Control (AGC), Echo Cancellation, Noise Suppression, and an Active Speaker Detector. Audio I/O is handled robustly.
- **Network & NAT Traversal**: Seamless connectivity using ICE (Interactive Connectivity Establishment), STUN, and TURN protocols to bypass NATs and firewalls.
- **Modern User Interface**: A sleek desktop application built with Qt6 (Widgets, UiTools).
- **Custom Protocol & Security**: Efficient serialization of custom message types (RTP/RTCP wrappers) for media transport, with built-in congestion control.

## Architecture & Codebase Structure

The source code is modular, located under the `src/` directory:

- **`core/`**: Core utilities, including the `EventBus` for decoupling application module communication, and `Scheduler` mechanisms.
- **`network/`**: The heart of the serverless architecture. Contains the `IceAgent`, `Stun` traversal, `PeerManager`, `Tree` topology builder, `CongestionController`, and `FailoverManager`. Ensures nodes can discover each other, negotiate paths, and maintain a stable network.
- **`audio/`**: The media pipeline. It wraps the `Opus` codec, handles media mixing (`Mixer`), and implements intelligent audio enhancements like `AGC`, `Echo Cancellation`, and `Noise Suppression`.
- **`protocol/`**: Custom packet definitions, serialization logic, and RTP implementations.
- **`ui/`**: Qt6 based user frontend elements (Forms and UI logic).
- **`security/`**: Cryptography and connection security frameworks.
- **`net/`** & **`tools/`**: Additional network and utility components.

## Technical Stack

- **Language**: C++20
- **Build System**: CMake (Required minimum version 3.16)
- **GUI & Networking**: Qt6 (Core, Gui, Widgets, Network, UiTools, Multimedia)
- **Audio Codec**: Opus (Prebuilt Windows binaries included in `third_party/opus`)

## Building the Project

### Prerequisites
1. **CMake** 3.16 or higher.
2. A **C++20** compatible compiler (MSVC 2022, MinGW, GCC, Clang).
3. **Qt6** locally installed. (The build script will probe `third_party/Qt6` or default path `C:/Qt/6.10.2/`, or you can supply `Qt6_DIR` manually).
4. **Opus Library**: Windows builds automatically use the prebuilt binaries in `third_party/opus`. Linux/macOS users will need `libopus` installed locally.

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone <repository_url>
   cd VoIP_Serverless
   ```
2. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```
3. **Configure the project**:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
4. **Build the project**:
   ```bash
   cmake --build . --config Release
   ```

After a successful build, the main executable `voip_client` (or `voip_client.exe`) will be generated. On Windows, the build script automatically copies `opus.dll` and necessary UI resources to the target output directory so it is ready to run.

## Running the Client

Launch the compiled executable `voip_client`. On startup, it will prompt you for a unique client name to represent you in the local machine or network. The application initializes audio, binds to local network interfaces for discovery, and then begins participating in the decentralized VoIP network.
