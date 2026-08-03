# Onyx-voip

Onyx-voip is a decentralized, low-latency peer-to-peer voice platform for LAN use. The current Qt6 shell is organized around small, isolated modules so the UI, discovery, transfer, and SIP/media layers stay separated.

## What is in this repo

- `source/ui/` contains the Qt6 application entrypoint and the QML shell.
- `source/network/` contains the LAN discovery, file transfer, and SIP bridge code.
- `source/core/` contains lightweight configuration helpers.
- `source/models/` contains the contact model used by the UI.
- `thirdparty/` contains vendored source trees and downloaded archives for the external libraries this project depends on.
- `scripts/fetch_thirdparty.ps1` refreshes the zip and tarball downloads into `thirdparty/_downloads` and extracts the matching source folders.

## Third-party packages

The repo currently vendors or stages these packages without vcpkg:

- `pjproject-2.17`
- `openssl-4.0.1`
- `opus-1.6.1`
- `ffmpeg-8.1.2`
- `libvpx-1.14.1`
- `hidapi-hidapi-0.14.0`
- `jsoncpp-1.9.6`
- `opencore-amr-0.1.6`
- `vo-amrwbenc`

Notes:

- `pjproject` is present as source, but Windows `.lib` outputs still need to be built separately if you want the SIP bridge enabled.
- `OpenSSL`, `FFmpeg`, and `Intel IPP` are not built by the top-level Qt6 project in this repo.
- `Wtsapi32`, `Ws2_32`, `Iphlpapi`, `Ole32`, `OleAut32`, and `Winmm` come from the Windows SDK / Visual Studio toolchain.

## Building

Requirements:

- CMake 3.24 or newer
- Qt 6.5 or newer
- MSVC 2022 or a compatible Visual Studio generator
- No vcpkg

Configure:

```powershell
& cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 && cmake -S . -B build-msvc -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.11.1/msvc2022_64'
```

Build:

```powershell
& cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 && cmake --build build-msvc'
```

## Running

Run the generated executable from `build-msvc\onyx-voip.exe` after adding the Qt `bin` folder to `PATH`, or launch it from Qt Creator after configuring the project with a Qt 6 kit.

Example terminal launch:

```powershell
$env:PATH = "C:\Qt\6.11.1\msvc2022_64\bin;$env:PATH"
$env:QT_QPA_PLATFORM = "offscreen"
.\build-msvc\onyx-voip.exe
```

The QML shell gives you:

- a status panel for the engine and LAN discovery
- a simple call box for SIP URIs
- a live contact list backed by the XML directory file
- a direct file transfer panel

If `pjproject` `.lib` files are not present, the shell still runs in LAN-only mode and the SIP bridge stays disabled.

## Qt Creator

1. Open the top-level `CMakeLists.txt`.
2. Select a Qt 6 MSVC kit.
3. Let Qt Creator configure the project.
4. Re-run CMake after you add or rebuild vendored libraries so the optional PJSIP link step can pick them up.

## Third-party refresh

Run this from PowerShell:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\scripts\fetch_thirdparty.ps1
```

That script downloads the zip-based dependencies plus the `opencore-amr` tarball, then extracts them into `thirdparty`.

## Project direction

The codebase is moving toward a clean split between:

- UI on the Qt thread
- LAN discovery on UDP multicast
- file transfer on a separate TCP path
- SIP/media behind an optional `SipController`

That keeps the real-time media path and the user interface from blocking each other.
