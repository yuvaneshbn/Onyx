#pragma once

#include <QApplication>
#include <QString>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <windows.h>
#include <Dbghelp.h>
#include <Psapi.h>
#include <commctrl.h>

// Replacement for the MFC application class
class OnyxApp : public QApplication
{
    Q_OBJECT
public:
    OnyxApp(int &argc, char **argv);
    bool InitInstance();

    static OnyxApp* instance() { return static_cast<OnyxApp*>(QCoreApplication::instance()); }
};

extern OnyxApp *theApp;