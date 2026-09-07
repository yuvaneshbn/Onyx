cmake_minimum_required(VERSION 3.16)
project(Onyx LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

# Find Qt 6
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/msvc2022_64" ${CMAKE_PREFIX_PATH})
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Svg)

# Third-party directories
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

# Include directories
include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${THIRDPARTY_DIR}/opus-1.6.1/include"
    "${THIRDPARTY_DIR}/jsoncpp-1.9.6/include"
    "${THIRDPARTY_DIR}/hidapi-hidapi-0.14.0/hidapi"
    "${THIRDPARTY_DIR}/vo-amrwbenc"
    "${THIRDPARTY_DIR}/opencore-amr-0.1.6/amrwb"
    "${THIRDPARTY_DIR}/opencore-amr-0.1.6/amrnb"
    "${THIRDPARTY_DIR}/libvpx-1.14.1"
    "${THIRDPARTY_DIR}/libvpx-1.14.1/build-c"
    "${THIRDPARTY_DIR}/openssl-4.0.1/include"
    "${THIRDPARTY_DIR}/ffmpeg-8.1.2"
    "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/pjlib/include"
    "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/pjlib-util/include"
    "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/pjmedia/include"
    "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/pjsip/include"
    "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/pjnath/include"
)

set(SOURCES
    src/main.cpp
    src/core/JsonProtocol.cpp
    src/core/P2PDiscovery.cpp
    src/audio/OpusAudioPipeline.cpp
    src/audio/AmrCodecManager.cpp
    src/audio/AudioManager.cpp
    src/video/VpxVideoPipeline.cpp
    src/video/FFmpegProcessor.cpp
    src/video/VideoManager.cpp
    src/voip/PjSipManager.cpp
    src/filetransfer/CryptoEngine.cpp
    src/filetransfer/SecureFileTransferManager.cpp
    src/hardware/HidHeadsetManager.cpp
    src/ui/DarkTheme.cpp
    src/ui/VideoRenderWidget.cpp
    src/ui/VideoGridWidget.cpp
    src/ui/LoginDialog.cpp
    src/ui/SettingsDialog.cpp
    src/ui/MainWindow.cpp
    resources/resources.qrc

    # JsonCpp 1.9.6 (compiled directly to avoid MSVC Debug/Release runtime mismatch LNK2038)
    thirdparty/jsoncpp-1.9.6/src/lib_json/json_reader.cpp
    thirdparty/jsoncpp-1.9.6/src/lib_json/json_value.cpp
    thirdparty/jsoncpp-1.9.6/src/lib_json/json_writer.cpp
)

set(HEADERS
    src/core/NetworkConfig.h
    src/core/JsonProtocol.h
    src/core/P2PDiscovery.h
    src/audio/OpusAudioPipeline.h
    src/audio/AmrCodecManager.h
    src/audio/AudioManager.h
    src/video/VpxVideoPipeline.h
    src/video/FFmpegProcessor.h
    src/video/VideoManager.h
    src/voip/PjSipManager.h
    src/filetransfer/CryptoEngine.h
    src/filetransfer/SecureFileTransferManager.h
    src/hardware/HidHeadsetManager.h
    src/ui/DarkTheme.h
    src/ui/VideoRenderWidget.h
    src/ui/VideoGridWidget.h
    src/ui/LoginDialog.h
    src/ui/SettingsDialog.h
    src/ui/MainWindow.h
)

add_executable(Onyx WIN32 ${SOURCES} ${HEADERS})

# Libraries to link
set(PJ_LIB_DIR "${THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17/lib")
set(FF_LIB_DIR "${THIRDPARTY_DIR}/ffmpeg-8.1.2")

target_link_libraries(Onyx PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Svg

    # Opus 1.6.1
    "${THIRDPARTY_DIR}/opus-1.6.1/build-ninja-x64/opus.lib"

    # HIDAPI 0.14.0
    "${THIRDPARTY_DIR}/hidapi-hidapi-0.14.0/build-vs/src/windows/Release/hidapi.lib"

    # VO-AMRWBENC
    "${THIRDPARTY_DIR}/vo-amrwbenc/build-vs/Release/vo-amrwbenc.lib"

    # OpenCore-AMR 0.1.6
    "${THIRDPARTY_DIR}/opencore-amr-0.1.6/build-ninja-x64/opencore-amrwb.lib"
    "${THIRDPARTY_DIR}/opencore-amr-0.1.6/build-ninja-x64/opencore-amrnb.lib"

    # LibVPX 1.14.1
    "${THIRDPARTY_DIR}/libvpx-1.14.1/build-ninja-x64/vpx.lib"

    # OpenSSL 4.0.1
    "${THIRDPARTY_DIR}/openssl-4.0.1/libcrypto.lib"
    "${THIRDPARTY_DIR}/openssl-4.0.1/libssl.lib"

    # FFmpeg 8.1.2
    "${FF_LIB_DIR}/libavformat/avformat.lib"
    "${FF_LIB_DIR}/libavcodec/avcodec.lib"
    "${FF_LIB_DIR}/libswscale/swscale.lib"
    "${FF_LIB_DIR}/libswresample/swresample.lib"
    "${FF_LIB_DIR}/libavutil/avutil.lib"

    # PJProject 2.17
    "${PJ_LIB_DIR}/pjsua-lib.lib"
    "${PJ_LIB_DIR}/pjsip.lib"
    "${PJ_LIB_DIR}/pjsip-ua.lib"
    "${PJ_LIB_DIR}/pjsip-simple.lib"
    "${PJ_LIB_DIR}/pjmedia.lib"
    "${PJ_LIB_DIR}/pjmedia-audiodev.lib"
    "${PJ_LIB_DIR}/pjmedia-videodev.lib"
    "${PJ_LIB_DIR}/pjmedia-codec.lib"
    "${PJ_LIB_DIR}/pjlib.lib"
    "${PJ_LIB_DIR}/pjlib-util.lib"
    "${PJ_LIB_DIR}/pjnath.lib"
    "${PJ_LIB_DIR}/srtp.lib"
    "${PJ_LIB_DIR}/webrtc_aec3.lib"
    "${PJ_LIB_DIR}/resample.lib"
    "${PJ_LIB_DIR}/speex.lib"
    "${PJ_LIB_DIR}/gsm.lib"
    "${PJ_LIB_DIR}/g7221.lib"
    "${PJ_LIB_DIR}/ilbc.lib"
    "${PJ_LIB_DIR}/yuv.lib"

    # System libraries
    ws2_32
    crypt32
    bcrypt
    ncrypt
    secur32
    iphlpapi
    ole32
    oleaut32
    user32
    advapi32
    mfplat
    mfuuid
    strmiids
    winmm
)

# Copy dependent DLLs next to the target executable
add_custom_command(TARGET Onyx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${THIRDPARTY_DIR}/hidapi-hidapi-0.14.0/build-vs/src/windows/Release/hidapi.dll"
        $<TARGET_FILE_DIR:Onyx>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${THIRDPARTY_DIR}/vo-amrwbenc/build-vs/Release/vo-amrwbenc.dll"
        $<TARGET_FILE_DIR:Onyx>
)
