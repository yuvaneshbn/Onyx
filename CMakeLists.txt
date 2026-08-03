cmake_minimum_required(VERSION 3.24)

project(OnyxVoip VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(ONYX_ENABLE_PJSIP "Enable PJSIP when prebuilt pjproject libraries are available." ON)
set(ONYX_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty" CACHE PATH "Root folder for vendored third-party packages.")
set(ONYX_PJPROJECT_DIR "${ONYX_THIRDPARTY_DIR}/pjproject-2.17/pjproject-2.17" CACHE PATH "Path to the pjproject source tree.")
set(ONYX_PJSIP_LIB_DIR "${ONYX_PJPROJECT_DIR}/lib" CACHE PATH "Folder containing built pjproject .lib files.")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network Xml)
qt_standard_project_setup(REQUIRES 6.5)

file(GLOB_RECURSE ONYX_APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp"
)

list(REMOVE_ITEM ONYX_APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/ui/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/ggets.cpp"
)

file(GLOB_RECURSE ONYX_APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h"
)

qt_add_executable(onyx-voip WIN32)

target_sources(onyx-voip PRIVATE
    ${ONYX_APP_SOURCES}
    ${ONYX_APP_HEADERS}
    source/resources.qrc
)

target_include_directories(onyx-voip PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/models"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/network"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/jsoncpp/json"
)

target_compile_definitions(onyx-voip PRIVATE
    WIN32
    _WINDOWS
    UNICODE
    _UNICODE
    NOMINMAX
)

target_link_libraries(onyx-voip
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Xml
        Ws2_32
        Iphlpapi
        Winmm
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
    file(GLOB ONYX_PJSIP_LIBS CONFIGURE_DEPENDS "${ONYX_PJSIP_LIB_DIR}/*.lib")

    if(ONYX_PJSIP_LIBS)
        target_compile_definitions(onyx-voip PRIVATE ONYX_HAS_PJSIP=1)
        target_include_directories(onyx-voip PRIVATE
            "${ONYX_PJPROJECT_DIR}/pjlib/include"
            "${ONYX_PJPROJECT_DIR}/pjlib-util/include"
            "${ONYX_PJPROJECT_DIR}/pjmedia/include"
            "${ONYX_PJPROJECT_DIR}/pjnath/include"
            "${ONYX_PJPROJECT_DIR}/pjsip/include"
        )
        target_link_libraries(onyx-voip PRIVATE
            ${ONYX_PJSIP_LIBS}
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
