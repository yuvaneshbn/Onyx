cmake_minimum_required(VERSION 3.12)
project(vo-amrwbenc C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES
    "amrwbenc/src/*.c"
    "common/*.c"
    "wrapper.c"
)
list(FILTER SRC_FILES EXCLUDE REGEX "/asm/")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/common/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/amrwbenc/inc"
)

add_library(vo-amrwbenc SHARED
    ${SRC_FILES}
    vo-amrwbenc.def
)

target_include_directories(vo-amrwbenc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/common/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/amrwbenc/inc"
)

if(MSVC)
    target_compile_options(vo-amrwbenc PRIVATE "/wd4996")
endif()
