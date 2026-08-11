cmake_minimum_required(VERSION 3.16)
project(ffmpeg-wrapper LANGUAGES C)

message(STATUS "This directory contains the FFmpeg source tree.")
message(STATUS "FFmpeg is normally built with its own configure/make flow rather than a simple CMake target.")
message(STATUS "Use the Visual Studio Developer Command Prompt and run the following from this directory:")
message(STATUS "  ./configure --disable-everything --enable-avcodec --enable-avformat --enable-avutil --enable-swresample --enable-swscale --disable-doc --disable-programs --disable-network --disable-autodetect --disable-x86asm --enable-shared --disable-static")
message(STATUS "  make -j")

add_custom_target(ffmpeg-build
    COMMAND ${CMAKE_COMMAND} -E echo "FFmpeg build is not fully automated by this wrapper on Windows. Please run configure/make manually in the Developer Command Prompt."
    COMMENT "Provide FFmpeg build instructions"
)
