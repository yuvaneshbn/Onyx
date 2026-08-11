# Install script for directory: C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/third_party/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib-util/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjnath/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip-apps/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib/pjlib.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pjlib.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pj" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/activesock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/addr_resolv.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/argparse.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/array.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/assert.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/atomic_queue.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/atomic_slist.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/ctype.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/doxygen.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/except.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/fifobuf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/file_access.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/file_io.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/guid.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/hash.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/ioqueue.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/ip_helper.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/limits.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/list.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/list_i.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/lock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/log.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/math.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/os.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/pool.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/pool_alt.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/pool_buf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/pool_i.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/rand.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/rbtree.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/sock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/sock_qos.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/sock_select.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/ssl_sock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/string.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/string_i.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/timer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/unicode.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/unittest.h"
    )
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pj/compat" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/assert.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_armcc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_clang.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_codew.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_gcc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_gcce.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_msvc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/cc_mwcc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/ctype.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/high_precision.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/limits.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_alpha.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_armv4.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_i386.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_m68k.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_powerpc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_sparc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/m_x86_64.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/malloc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_darwinos.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_linux.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_palmos.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_rtems.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_sunos.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_symbian.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_win32.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_win32_wince.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_winphone8.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/os_winuwp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/rand.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/setjmp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/size_t.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/socket.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/stdarg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/stdfileio.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/string.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/compat/time.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pj" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj/config_site.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pj/compat" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib/include/pj/compat/m_auto.h"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib/include/pj/compat/os_auto.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pjlib++.hpp")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pj++" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/file.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/hash.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/list.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/lock.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/os.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/pool.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/proactor.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/scanner.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/sock.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/string.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/timer.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/tree.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib/include/pj++/types.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjlib-util/pjlib-util.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjlib-util" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/base64.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/cli.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/cli_console.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/cli_imp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/cli_telnet.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/crc32.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/dns.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/dns_server.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/getopt.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/hmac_md5.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/hmac_sha1.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/http_client.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/json.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/md5.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/pcap.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/resolver.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/scanner.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/scanner_cis_bitwise.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/scanner_cis_uint.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/sha1.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/srv_resolver.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/string.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/stun_simple.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/websock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjlib-util/include/pjlib-util/xml.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjnath/pjnath.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjnath" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/ice_session.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/ice_strans.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/nat_detect.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_auth.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_msg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_session.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_sock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/stun_transaction.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/turn_session.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/turn_sock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjnath/include/pjnath/upnp.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/pjmedia.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/ai_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/alaw_ulaw.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/audiodev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/av_sync.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/avi.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/avi_stream.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/bidirectional.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/circbuf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/clock.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/codec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/conference.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/config_auto.h.cm"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/config_auto.h.in"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/converter.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/delaybuf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/doxygen.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/echo.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/echo_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/endpoint.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/event.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/format.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/frame.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/g711.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/jbuf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/master_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/mem_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/null_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/plc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/resample.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/rtcp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/rtcp_fb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/rtcp_xr.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/rtp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/sdp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/sdp_neg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/session.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/signatures.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/silencedet.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/sound.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/sound_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/splitcomb.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/stereo.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/stream.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/stream_common.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/symbian_sound_aps.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/tonegen.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport_adapter_sample.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport_ice.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport_loop.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport_srtp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/transport_udp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/txt_stream.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_codec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_codec_util.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_conf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_stream.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/vid_tee.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/videodev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/wav_playlist.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/wav_port.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/wave.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia/wsola.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/include/pjmedia/config_auto.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/pjmedia-codec.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia-codec" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/amr_helper.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/amr_sdp_match.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/and_aud_mediacodec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/and_vid_mediacodec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/audio_codecs.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/bcg729.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/config_auto.h.cm"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/config_auto.h.in"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/ffmpeg_vid_codecs.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/g722.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/g7221.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/g7221_sdp_match.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/gsm.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/h263_packetizer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/h264_packetizer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/ilbc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/ipp_codecs.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/l16.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/lyra.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/opencore_amr.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/openh264.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/opus.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/passthrough.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/silk.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/speex.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/vid_toolbox.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/vpx.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-codec/vpx_packetizer.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia-codec" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/include/pjmedia-codec/config_auto.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/pjmedia-audiodev.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia_audiodev.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia-audiodev" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/alsa.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/audiodev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/audiodev_imp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/audiotest.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-audiodev/errno.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjmedia/pjmedia-videodev.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia_videodev.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjmedia-videodev" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/avi_dev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/opengl_dev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/videodev.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjmedia/include/pjmedia-videodev/videodev_imp.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/pjsip.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsip" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/print_util.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_auth.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_auth_aka.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_auth_msg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_auth_parser.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_autoconf.h.cm"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_autoconf.h.in"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_config.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_dialog.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_endpoint.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_event.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_module.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_msg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_multipart.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_parser.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_private.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_resolve.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_tel_uri.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transaction.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transport.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transport_loop.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transport_tcp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transport_tls.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_transport_udp.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_ua_layer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_uri.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip/sip_util.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsip" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/include/pjsip/sip_autoconf.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/pjsip-simple.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip_simple.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsip-simple" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/dialog_info.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/dlg_event.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/errno.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/evsub.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/evsub_msg.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/iscomposing.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/mwi.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/pidf.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/presence.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/publish.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/rpid.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/types.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-simple/xpidf.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/pjsip-ua.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip_ua.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsip-ua" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_100rel.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_inv.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_regc.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_replaces.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_siprec.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_timer.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsip-ua/sip_xfer.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/pjsua-lib.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua.h")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsua-lib" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua-lib/pjsua.h"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua-lib/pjsua_internal.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE STATIC_LIBRARY FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip/pjsua2.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2.hpp")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pjsua2" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/account.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/call.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/config.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/doxygen.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/endpoint.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/json.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/media.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/persistent.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/presence.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/siptypes.hpp"
    "C:/Users/YUVANESH/Downloads/Onyx/thirdparty/pjproject-2.17/pjproject-2.17/pjsip/include/pjsua2/types.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjRuntime" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/pjsip-apps/pjsua.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj/PjTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj/PjTargets.cmake"
         "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/CMakeFiles/Export/0990442f4830e493e4bdac0893d2b5b3/PjTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj/PjTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj/PjTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/CMakeFiles/Export/0990442f4830e493e4bdac0893d2b5b3/PjTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/CMakeFiles/Export/0990442f4830e493e4bdac0893d2b5b3/PjTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "PjDevelopment" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Pj" TYPE FILE FILES
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/PjConfig.cmake"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/PjConfigVersion.cmake"
    "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/PjDeps.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/libpjproject.pc")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/YUVANESH/Downloads/Onyx/build-pjproject-msvc/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
