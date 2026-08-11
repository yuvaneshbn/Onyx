# Install script for directory: C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/pjproject")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/g7221.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/g7221/g7221/common" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/basic_op.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/basic_op_i.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/count.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/defs.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/huff_def.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/huff_tab.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/tables.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/common/typedef.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/g7221/g7221/decode" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/decode/dct4_s.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/g7221/g7221/encode" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/g7221/include/g7221/encode/dct4_a.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/gsm/gsm.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/gsm" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/gsm.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/private.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/proto.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/toast.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/gsm/inc/unproto.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/ilbc/ilbc.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/ilbc" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/anaFilter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/constants.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/createCB.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/doCPLC.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/enhancer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/FrameClassify.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/gainquant.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/getCBvec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/helpfun.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/hpInput.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/hpOutput.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/iCBConstruct.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/iCBSearch.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/iLBC_decode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/iLBC_define.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/iLBC_encode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/LPCdecode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/LPCencode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/lsf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/packing.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/StateConstructW.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/StateSearchW.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/ilbc/syntFilter.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/resample/resample.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/resample" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/resample/include/resamplesubs.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/speex/speex.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/speex/speex" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_bits.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_callbacks.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_config_types.h.in"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_echo.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_header.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_jitter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_preprocess.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_resampler.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_stereo.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speex_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/include/speex/speexdsp_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/speex/../build/speex/speex/speex_config_types.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/srtp/srtp.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/srtp" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/include/srtp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/include/srtp_priv.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/include/stream_list_priv.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/aes.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/aes_gcm.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/aes_icm.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/aes_icm_ext.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/alloc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/auth.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/cipher.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/cipher_priv.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/cipher_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/crypto_kernel.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/crypto_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/datatypes.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/err.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/hmac.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/integers.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/key.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/null_auth.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/null_cipher.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/rdb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/rdbx.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/crypto/include/sha1.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/srtp/../build/srtp/srtp_config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/webrtc/webrtc.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/pj_config.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/typedefs.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/common_audio" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/fft4g.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/ring_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/wav_file.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/common_audio/signal_processing" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/complex_fft_tables.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/resample_by_2_internal.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/common_audio/signal_processing/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/include/real_fft.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/include/signal_processing_library.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/include/spl_inl.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/include/spl_inl_armv7.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/common_audio/signal_processing/include/spl_inl_mips.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/aec" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/aec_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/aec_core.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/aec_core_internal.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/aec_rdft.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/aec_resampler.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/echo_cancellation_internal.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/aec/include" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aec/include/echo_cancellation.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/aecm" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aecm/aecm_core.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aecm/aecm_defines.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/aecm/include" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/aecm/include/echo_control_mobile.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/logging" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/logging/aec_logging.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/logging/aec_logging_file_handling.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/ns" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/defines.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/ns_core.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/nsx_core.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/nsx_defines.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/windows_private.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/ns/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/include/noise_suppression.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/ns/include/noise_suppression_x.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/modules/audio_processing/utility" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/utility/delay_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/utility/delay_estimator_internal.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/modules/audio_processing/utility/delay_estimator_wrapper.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc/webrtc/system_wrappers/interface" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/system_wrappers/interface/compile_assert_c.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc/src/webrtc/system_wrappers/interface/cpu_features_wrapper.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/webrtc_aec3/webrtc_aec3.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/base" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/attributes.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/const_init.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/log_severity.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/macros.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/optimization.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/options.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/policy_checks.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/port.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/base/internal" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/atomic_hook.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/identity.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/inline_variable.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/invoke.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/raw_logging.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/base/internal/throw_delegate.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/memory" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/memory/memory.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/meta" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/meta/type_traits.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/strings" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/strings/string_view.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/types" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/types/bad_optional_access.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/types/optional.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/types/internal" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/types/internal/optional.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/absl/utility" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/absl/utility/utility.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/api" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/array_view.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/function_view.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/rtp_headers.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/rtp_packet_info.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/scoped_refptr.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/api/audio" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/audio_frame.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/audio_frame_processor.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/audio_mixer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/channel_layout.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/echo_canceller3_config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/echo_canceller3_config_json.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/echo_canceller3_factory.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/echo_control.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/audio/echo_detector_creator.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/api/units" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/units/time_delta.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/api/units/timestamp.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/channel_buffer.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/include" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/include/audio_util.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/resampler" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/resampler/push_sinc_resampler.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/resampler/sinc_resampler.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/resampler/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/resampler/include/push_resampler.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/resampler/include/resampler.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/signal_processing" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/signal_processing/dot_product_with_scale.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/signal_processing/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/signal_processing/include/signal_processing_library.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/signal_processing/include/spl_inl.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/signal_processing/include/spl_inl_armv7.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/signal_processing/include/spl_inl_mips.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/third_party/ooura/fft_size_128" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/third_party/ooura/fft_size_128/ooura_fft.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/third_party/ooura/fft_size_128/ooura_fft_tables_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/third_party/ooura/fft_size_128/ooura_fft_tables_neon_sse2.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/third_party/ooura/fft_size_256" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/third_party/ooura/fft_size_256/fft4g.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/common_audio/third_party/spl_sqrt_floor" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/audio_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/gain_controller2.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/high_pass_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/residual_echo_detector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/splitting_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/three_band_filter_bank.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/aec3" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/adaptive_fir_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/adaptive_fir_filter_erl.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/aec3_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/aec3_fft.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/aec_state.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/alignment_mixer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/api_call_jitter_metrics.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/block_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/block_delay_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/block_framer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/block_processor.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/block_processor_metrics.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/clockdrift_detector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/coarse_filter_update_gain.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/comfort_noise_generator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/decimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/delay_estimate.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/dominant_nearend_detector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/downsampled_render_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_audibility.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_canceller3.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_path_delay_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_path_variability.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_remover.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/echo_remover_metrics.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/erl_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/erle_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/fft_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/fft_data.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/filter_analyzer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/frame_blocker.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/fullband_erle_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/matched_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/matched_filter_lag_aggregator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/moving_average.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/nearend_detector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/refined_filter_update_gain.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/render_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/render_delay_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/render_delay_controller.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/render_delay_controller_metrics.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/render_signal_analyzer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/residual_echo_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/reverb_decay_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/reverb_frequency_response.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/reverb_model.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/reverb_model_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/signal_dependent_erle_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/spectrum_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/stationarity_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/subband_erle_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/subband_nearend_detector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/subtractor.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/subtractor_output.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/subtractor_output_analyzer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/suppression_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/suppression_gain.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/transparent_mode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/vector_math.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/aec3/mock" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/mock/mock_block_processor.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/mock/mock_echo_remover.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/mock/mock_render_delay_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/aec3/mock/mock_render_delay_controller.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/agc2" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/adaptive_agc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/adaptive_digital_gain_applier.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/adaptive_mode_level_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/agc2_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/agc2_testing_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/biquad_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/compute_interpolated_gain_curve.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/cpu_features.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/down_sampler.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/fixed_digital_level_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/gain_applier.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/interpolated_gain_curve.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/limiter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/limiter_db_gain_curve.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/noise_level_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/noise_spectrum_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/saturation_protector.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/saturation_protector_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/signal_classifier.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/vad_with_level.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/vector_float_frame.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/agc2/rnn_vad" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/auto_correlation.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/features_extraction.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/lp_residual.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/pitch_search.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/pitch_search_internal.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/ring_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/rnn.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/rnn_fc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/rnn_gru.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/sequence_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/spectral_features.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/spectral_features_internal.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/symmetric_matrix_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/test_utils.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/agc2/rnn_vad/vector_math.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/echo_detector" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/echo_detector/circular_buffer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/echo_detector/mean_variance_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/echo_detector/moving_max.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/echo_detector/normalized_covariance_estimator.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/include/audio_frame_view.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/include/audio_processing.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/include/audio_processing_statistics.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/include/config.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/logging" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/logging/apm_data_dumper.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/ns" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/fast_math.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/histograms.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/noise_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/noise_suppressor.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/ns_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/ns_config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/ns_fft.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/prior_signal_model.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/prior_signal_model_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/quantile_noise_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/signal_model.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/signal_model_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/speech_probability_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/suppression_params.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/ns/wiener_filter.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/modules/audio_processing/utility" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/utility/cascaded_biquad_filter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/utility/delay_estimator.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/utility/delay_estimator_internal.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/utility/delay_estimator_wrapper.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/modules/audio_processing/utility/pffft_wrapper.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/arraysize.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/atomic_ops.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/checks.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/compile_assert_c.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/constructor_magic.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/gtest_prod_util.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/logging.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/platform_thread_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/race_checker.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/random.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/ref_count.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/ref_counted_object.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/ref_counter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/string_encode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/string_to_number.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/string_utils.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/swap_queue.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/system_time.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/thread_annotations.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/time_utils.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/type_traits.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/experiments" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/experiments/field_trial_parser.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/memory" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/memory/aligned_malloc.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/numerics" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/numerics/safe_compare.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/numerics/safe_conversions.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/numerics/safe_conversions_impl.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/numerics/safe_minmax.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/strings" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/strings/string_builder.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/synchronization" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/mutex.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/mutex_abseil.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/mutex_critical_section.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/mutex_pthread.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/mutex_race_check.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/synchronization/yield.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/system" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/system/arch.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/system/file_wrapper.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/system/inline.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/system/rtc_export.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/rtc_base/units" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/rtc_base/units/unit_base.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/system_wrappers/include" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/system_wrappers/include/cpu_features_wrapper.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/system_wrappers/include/field_trial.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/system_wrappers/include/metrics.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/third_party/pffft/src" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/third_party/pffft/src/pffft.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/webrtc_aec3/third_party/rnnoise/src" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/third_party/rnnoise/src/rnn_activations.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/webrtc_aec3/src/third_party/rnnoise/src/rnn_vad_weights.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/pjproject/third_party" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/yuv/yuv.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/yuv" TYPE FILE OPTIONAL FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjproject/third_party/yuv/libyuv" TYPE FILE OPTIONAL FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/basic_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/compare.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/compare_row.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/convert.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/convert_argb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/convert_from.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/convert_from_argb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/cpu_id.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/macros_msa.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/mjpeg_decoder.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/planar_functions.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/rotate.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/rotate_argb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/rotate_row.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/row.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/scale.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/scale_argb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/scale_row.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/version.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/third_party/yuv/include/libyuv/video_common.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
