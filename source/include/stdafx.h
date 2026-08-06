// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include "define.h"

// Qt equivalent of Windows versioning – not needed in cross‑platform code.
// (Original: #include "targetver.h")

// The following MFC/ATL specific defines are no longer required.
// #ifndef _SECURE_ATL
// #define _SECURE_ATL 1
// #endif
//
// #ifndef VC_EXTRALEAN
// #define VC_EXTRALEAN
// #endif
//
// #define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
//
// #define _AFX_ALL_WARNINGS

// Qt core and GUI headers (replaces <afxwin.h>, <afxext.h>, <afxwinappex.h>)
#include <QtWidgets>          // QApplication, QMainWindow, QDialog, all widgets
#include <QtGui>              // QPixmap, QPainter, etc.
#include <QtCore>             // QString, QFile, QThread, etc.
#include <QtNetwork>          // QTcpSocket, QUdpSocket, QNetworkAccessManager
#include <QSslSocket>         // SSL support (replaces schannel.h, sspi.h)

// Standard C++ headers that were originally included
#include <sstream>            // std::ostringstream, std::istringstream
#include <algorithm>          // std::sort, std::find, etc.

// The following Windows‑specific security and networking headers are no
// longer needed; Qt provides cross‑platform alternatives through its modules.
// #define SECURITY_WIN32
// #include <sspi.h>
// #include <schannel.h>
// #include <cryptuiapi.h>
// #include <atlfile.h>        (replaced by QFile)
// #include <atlenc.h>         (replaced by QByteArray::toBase64())
// #include <atlsocket.h>      (replaced by QTcpSocket/QUdpSocket)

// Original automation and internet control headers are handled by Qt's
// ActiveQt framework if needed, but typically not required.
// #include <afxdisp.h>
// #include <afxdtctl.h>
// #include <afxcmn.h>
// #include <afxdhtml.h>
