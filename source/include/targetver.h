#pragma once

#include "const.h"

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
// (For Qt cross-platform builds, these macros are only effective on Windows; they are harmless elsewhere.)

#if defined(Q_OS_WIN)
#   ifndef WINVER
#   define WINVER 0x0601           // Change this to the appropriate value to target other versions of Windows.
#   endif

#   ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0601           // Change this to the appropriate value to target other versions of Windows.
#   endif

#   ifndef _WIN32_WINDOWS
#   define _WIN32_WINDOWS 0x0601           // Change this to the appropriate value to target other versions of Windows.
#   endif

#   ifndef NTDDI_VERSION
#   define NTDDI_VERSION 0x06010000           // Change this to the appropriate value to target other versions of Windows.
#   endif

#   ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 7.0.
#   define _WIN32_IE 0x0700        // Change this to the appropriate value to target other versions of IE.
#   endif
#else
// Non-Windows platforms: define to zero or an invalid value so any usage can be easily detected.
#   define WINVER 0
#   define _WIN32_WINNT 0
#   define _WIN32_WINDOWS 0
#   define NTDDI_VERSION 0
#   define _WIN32_IE 0
#endif