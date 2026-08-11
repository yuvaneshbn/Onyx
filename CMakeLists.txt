cmake_minimum_required(VERSION 3.24)

if(NOT DEFINED CMAKE_SYSTEM_VERSION)
    set(CMAKE_SYSTEM_VERSION "10.0.26100.0" CACHE STRING "Windows SDK version to target")
endif()

if(NOT DEFINED CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0.26100.0" CACHE STRING "Windows SDK version to target")
endif()

project(OnyxVoip VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(ONYX_ENABLE_PJSIP "Enable PJSIP integration when static libraries are staged." ON)

set(ONYX_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty" CACHE PATH "Path to thirdparty packages.")
set(ONYX_PJPROJECT_DIR "${ONYX_THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17" CACHE PATH "Path to pjproject tree.")
set(ONYX_PJSIP_LIB_DIR "${ONYX_PJPROJECT_DIR}/lib" CACHE PATH "Folder containing prebuilt PJSIP .lib archives.")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network Xml Quick Qml)
qt_standard_project_setup(REQUIRES 6.5)

file(GLOB ONYX_SOURCE_CPP CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/models/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/network/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/src/*.cpp"
)

list(FILTER ONYX_SOURCE_CPP EXCLUDE REGEX ".*/ggets\\.cpp$")

set(ONYX_SOURCE_C
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/hid.c"
)

file(GLOB ONYX_SOURCE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/models/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/network/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/include/*.h"
)

set(ONYX_SOURCES
    ${ONYX_SOURCE_CPP}
    ${ONYX_SOURCE_C}
    ${ONYX_SOURCE_HEADERS}
)

qt_add_executable(onyx-voip WIN32
    ${ONYX_SOURCES}
    source/resources.qrc
)

target_include_directories(onyx-voip PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/models"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/network"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp/json"
)

set_source_files_properties(${ONYX_SOURCE_C} PROPERTIES LANGUAGE C)

target_compile_definitions(onyx-voip PRIVATE
    WIN32
    _WINDOWS
    UNICODE
    _UNICODE
    NOMINMAX
    _CRT_SECURE_NO_WARNINGS
    _WINSOCK_DEPRECATED_NO_WARNINGS
)

target_link_libraries(onyx-voip PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Xml
    Qt6::Quick
    Qt6::Qml
    Ws2_32
    Iphlpapi
    Winmm
    Wtsapi32
    Ole32
    OleAut32
    Shell32
    Comctl32
    Psapi
    Dbghelp
    Wininet
    Setupapi
    Advapi32
    Msi
    Userenv
)

if(ONYX_ENABLE_PJSIP AND EXISTS "${ONYX_PJPROJECT_DIR}/pjsip/include/pjsua-lib/pjsua.h")
    # Use GLOB_RECURSE so we find libs in subdirectories (e.g. build-pjproject-msvc/pjlib, /pjmedia, /pjsip, /third_party)
    file(GLOB_RECURSE ONYX_PJSIP_LIBS CONFIGURE_DEPENDS "${ONYX_PJSIP_LIB_DIR}/*.lib")

    if(ONYX_PJSIP_LIBS)
        set(ONYX_PJSIP_LIB_ORDER
            pjsua2
            pjsua-lib
            pjsip-ua
            pjsip-simple
            pjsip
            pjnath
            pjmedia-videodev
            pjmedia
            pjmedia-codec
            pjmedia-audiodev
            pjlib-util
            pjlib
            resample
            speex
            webrtc_aec3
            webrtc
            yuv
            g7221
            gsm
            ilbc
            srtp
        )

        # Build an ordered list by matching basenames in the recursively-found lib list.
        # We cannot use a flat "${ONYX_PJSIP_LIB_DIR}/${libname}.lib" path because
        # build-pjproject-msvc stores each lib in a per-component subdirectory.
        set(ONYX_PJSIP_LIBS_ORDERED "")
        foreach(libname IN LISTS ONYX_PJSIP_LIB_ORDER)
            # Search the full recursive list for this basename
            foreach(lib IN LISTS ONYX_PJSIP_LIBS)
                get_filename_component(_stem ${lib} NAME_WE)
                if(_stem STREQUAL libname)
                    list(APPEND ONYX_PJSIP_LIBS_ORDERED "${lib}")
                    break()
                endif()
            endforeach()
        endforeach()

        # Append any remaining libs that are not in the explicit ordering list
        foreach(lib IN LISTS ONYX_PJSIP_LIBS)
            get_filename_component(libname ${lib} NAME_WE)
            list(FIND ONYX_PJSIP_LIB_ORDER "${libname}" idx)
            if(idx EQUAL -1)
                list(APPEND ONYX_PJSIP_LIBS_ORDERED "${lib}")
            endif()
        endforeach()

        target_compile_definitions(onyx-voip PRIVATE
            ONYX_HAS_PJSIP=1
            PJMEDIA_HAS_VIDEO=1
            PJMEDIA_VIDEO_DEV_HAS_AVI=1
        )
        target_include_directories(onyx-voip PRIVATE
            # Source tree headers
            "${ONYX_PJPROJECT_DIR}/pjlib/include"
            "${ONYX_PJPROJECT_DIR}/pjlib-util/include"
            "${ONYX_PJPROJECT_DIR}/pjmedia/include"
            "${ONYX_PJPROJECT_DIR}/pjnath/include"
            "${ONYX_PJPROJECT_DIR}/pjsip/include"
            # Generated headers from the cmake build (os_auto.h, config_auto.h, etc.)
            "${ONYX_PJSIP_LIB_DIR}/pjlib/include"
            "${ONYX_PJSIP_LIB_DIR}/pjmedia/include"
        )
        # Suppress LNK4098: MSVCRT conflict between debug MDd CRT (from Qt/app) and
        # pjproject static libs.  /NODEFAULTLIB:MSVCRT tells the linker to ignore the
        # release multithreaded DLL CRT default-lib record embedded in the .lib files.
        if(MSVC)
            target_link_options(onyx-voip PRIVATE /NODEFAULTLIB:MSVCRT)
        endif()

        target_link_libraries(onyx-voip PRIVATE
            ${ONYX_PJSIP_LIBS_ORDERED}
        )
        message(STATUS "Onyx-voip: pjproject integration enabled from ${ONYX_PJSIP_LIB_DIR}")
    else()
        target_include_directories(onyx-voip PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/compat"
        )
        message(STATUS "Onyx-voip: pjproject headers found, but no .lib files exist in ${ONYX_PJSIP_LIB_DIR}. Building in LAN-only mode.")
    endif()
else()
    target_include_directories(onyx-voip PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/compat"
    )
    message(STATUS "Onyx-voip: pjproject not configured; building in LAN-only mode.")
endif()

install(TARGETS onyx-voip
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

qt_generate_deploy_app_script(
    TARGET onyx-voip
    OUTPUT_SCRIPT onyx_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${onyx_deploy_script})
