
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was PjConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

# Add our Find modules to the search path for dependencies below
set(_pj_saved_module_path "${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

find_dependency(Threads)

# SSL/TLS backend (mutually exclusive)
set(_pj_ssl_backend "")
if(_pj_ssl_backend STREQUAL "openssl")
  find_package(OpenSSL COMPONENTS Crypto SSL MODULE QUIET)
  if(NOT OpenSSL_FOUND AND NOT OPENSSL_FOUND)
    find_dependency(OpenSSL COMPONENTS Crypto SSL)
  endif()
elseif(_pj_ssl_backend STREQUAL "gnutls")
  find_dependency(GnuTLS)
elseif(_pj_ssl_backend STREQUAL "mbedtls")
  find_dependency(MbedTLS CONFIG)
endif()
unset(_pj_ssl_backend)

# pjlib platform deps
if()
  find_dependency(UUID)
endif()

# pjnath deps
if(OFF)
  find_dependency(UPNP)
endif()

# Audio device backends
if()
  find_dependency(CoreAudio)
endif()
if()
  find_dependency(ALSA)
endif()

# Video device backends
if()
  find_dependency(OpenGL)
endif()
if(OFF)
  find_dependency(SDL2)
endif()
if()
  find_dependency(V4L2)
endif()

# AEC backends
if(OFF)
  find_dependency(SpeexDSP)
endif()

# Resampling backend
set(_pj_resample "libresample")
if(_pj_resample STREQUAL "libsamplerate")
  find_dependency(SampleRate)
endif()
unset(_pj_resample)

# FFMPEG
if(OFF)
  find_dependency(FFMPEG)
endif()

# Audio/video codecs
if(OFF)
  find_dependency(OpenCoreAMRNB)
endif()
if(OFF)
  find_dependency(OpenCoreAMRWB)
  find_dependency(VisualOnAMRWBEnc)
endif()
if(OFF)
  find_dependency(Silk)
endif()
if(OFF)
  find_dependency(OPUS)
endif()
if(OFF)
  find_dependency(BCG729)
endif()
if(OFF)
  find_dependency(Lyra)
endif()
if(OFF)
  find_dependency(VPX)
endif()
if(OFF)
  find_dependency(OpenH264)
endif()

set(CMAKE_MODULE_PATH "${_pj_saved_module_path}")
unset(_pj_saved_module_path)

# Load exported targets
include("${CMAKE_CURRENT_LIST_DIR}/PjTargets.cmake")

# Recreate Pj::Dep::* alias targets for bundled/system dependencies
include("${CMAKE_CURRENT_LIST_DIR}/PjDeps.cmake")
