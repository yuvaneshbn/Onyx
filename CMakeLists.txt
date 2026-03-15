cmake_minimum_required(VERSION 3.16)
project(VoIP_Serverless LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Help CMake locate a local Qt6 SDK.
# 1) Honor a user-provided Qt6_DIR.
# 2) Otherwise probe common locations (third_party drop-in or C:\Qt\6.10.2 toolchains) and set Qt6_DIR.
if(NOT Qt6_DIR)
    set(_qt_candidates
        "${CMAKE_SOURCE_DIR}/third_party/Qt6/lib/cmake/Qt6"
        "C:/Qt/6.10.2/msvc2022_64/lib/cmake/Qt6"
        "C:/Qt/6.10.2/mingw_64/lib/cmake/Qt6"
    )
    foreach(_qt_path IN LISTS _qt_candidates)
        if(EXISTS "${_qt_path}/Qt6Config.cmake")
            set(Qt6_DIR "${_qt_path}" CACHE PATH "Path to Qt6Config.cmake" FORCE)
            break()
        endif()
    endforeach()
endif()

if(Qt6_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${Qt6_DIR}")
endif()

message(STATUS "Qt search paths (CMAKE_PREFIX_PATH): ${CMAKE_PREFIX_PATH}")
message(STATUS "Qt6_DIR: ${Qt6_DIR}")

find_package(Qt6 COMPONENTS Core Gui Widgets Network UiTools Multimedia REQUIRED PATHS ${Qt6_DIR} NO_DEFAULT_PATH)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party
)

# Collect source files
file(GLOB_RECURSE CORE_SRCS "src/core/*.cpp")
file(GLOB_RECURSE PROTOCOL_SRCS "src/protocol/*.cpp")
file(GLOB_RECURSE UTILS_SRCS "src/utils/*.cpp")
file(GLOB_RECURSE AUDIO_SRCS "src/audio/*.cpp")
file(GLOB_RECURSE NETCLIENT_SRCS "src/net/*.cpp")
file(GLOB_RECURSE NETWORK_SRCS "src/network/*.cpp")
file(GLOB_RECURSE SECURITY_SRCS "src/security/*.cpp")
file(GLOB_RECURSE TOOLS_SRCS "src/tools/*.cpp")

# UI Files (Keep them in their original path, but compile them into our project)
file(GLOB_RECURSE UI_SRCS "src/ui/*.cpp")
file(GLOB_RECURSE UI_FORMS "src/ui/*.ui")
file(GLOB_RECURSE UI_HEADERS "src/ui/*.h")

# Opus (prebuilt binaries in third_party/opus)
set(OPUS_ROOT ${CMAKE_SOURCE_DIR}/third_party/opus)

if(WIN32)
    add_library(opus SHARED IMPORTED)
    set_target_properties(opus PROPERTIES
        IMPORTED_LOCATION         ${OPUS_ROOT}/opus.dll
        IMPORTED_IMPLIB           ${OPUS_ROOT}/opus.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/third_party
    )
else()
    find_path(OPUS_INCLUDE_DIR opus/opus.h)
    find_library(OPUS_LIBRARY NAMES opus)
    if(OPUS_INCLUDE_DIR AND OPUS_LIBRARY)
        add_library(opus UNKNOWN IMPORTED)
        set_target_properties(opus PROPERTIES
            IMPORTED_LOCATION         ${OPUS_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${OPUS_INCLUDE_DIR}
        )
    else()
        message(FATAL_ERROR "Opus library not found. Install libopus or drop binaries into third_party/opus.")
    endif()
endif()

# The main executable
add_executable(voip_client
    src/main.cpp
    ${CORE_SRCS}
    ${PROTOCOL_SRCS}
    ${UTILS_SRCS}
    ${AUDIO_SRCS}
    ${NETWORK_SRCS}
    ${NETCLIENT_SRCS}
    ${SECURITY_SRCS}
    ${TOOLS_SRCS}
    ${UI_SRCS}
    ${UI_FORMS}
    ${UI_HEADERS}
)

# Link Qt and other libraries
target_link_libraries(voip_client PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::UiTools
    Qt6::Multimedia
    opus
)

if(WIN32)
    add_custom_command(TARGET voip_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${OPUS_ROOT}/opus.dll
                $<TARGET_FILE_DIR:voip_client>)
endif()

# Deploy runtime .ui resources for QUiLoader
add_custom_command(TARGET voip_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:voip_client>/ui
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${UI_FORMS} $<TARGET_FILE_DIR:voip_client>/ui
)

# Optional: Wall and other strict flags
if(MSVC)
    target_compile_options(voip_client PRIVATE /W4 /WX)
else()
    target_compile_options(voip_client PRIVATE -Wall -Wextra -Werror)
endif()
