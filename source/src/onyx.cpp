#include "stdafx.h"               // keep if needed, or replace with Qt headers
#include "onyx.h"
#include "mainDlg.h"              // now Qt‑based
#include "const.h"
#include "settings.h"
#include "langpack.h"

#include <Strsafe.h>
#include <Psapi.h>
#include <Dbghelp.h>
#include <sddl.h>
#include <commctrl.h>
#include <shellapi.h>
#include <wininet.h>              // for direct HTTP (replaces CInternetSession)
#include <io.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Psapi")
#pragma comment(lib, "Dbghelp")
#pragma comment(lib, "wininet.lib")

// ================== RecursiveDelete ==================
void RecursiveDelete(const QString &path)
{
    if (path.isEmpty()) return;

    QDir dir(path);
    if (!dir.exists()) return;

    // Normalize path (remove trailing backslash)
    QString cleanPath = QDir::cleanPath(path);
    dir.setPath(cleanPath);

    // Recursively delete all files and subdirectories
    dir.removeRecursively();
}

// ================== Helper: wine version ==================
static QString wineVersion()
{
    static const char *(CDECL *pwine_get_version)(void) = nullptr;
    HMODULE hntdll = GetModuleHandle(L"ntdll.dll");
    if (hntdll) {
        pwine_get_version = (const char* (*)())GetProcAddress(hntdll, "wine_get_version");
        if (pwine_get_version)
            return QString::fromLatin1(pwine_get_version());
    }
    return "n/a";
}

// ================== Exception filter ==================
static LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo)
{
    bool sent = false;
    QDateTime tm = QDateTime::currentDateTime();
    const QString version = QStringLiteral(_GLOBAL_VERSION);

    QString blockFileName = accountSettings.pathLocal + QString("block_%1.dat").arg(version);
    bool blockDump = QFile::exists(blockFileName);

    QString filename, dataStr;
    QFile file;
    QByteArray data;      // using QByteArray for binary data

    // ---- Write text crash dump ----
    filename = QString("%1crash-dump_%2.txt")
                   .arg(accountSettings.pathLocal, version);
    file.setFileName(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QString dumpText;
        QTextStream stream(&dumpText);
        stream << "Time: " << tm.toString("yyyy-MM-dd hh:mm:ss") << " (" << tm.toSecsSinceEpoch() << ")\r\n";
        stream << "ExceptionCode: 0x" << QString::number(ExceptionInfo->ExceptionRecord->ExceptionCode, 16) << "\r\n";
        for (int i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++) {
            stream << "ExceptionInformation(" << i << "): 0x"
                   << QString::number(ExceptionInfo->ExceptionRecord->ExceptionInformation[i], 16) << "\r\n";
        }
        stream << "Wine version: " << wineVersion() << "\r\n";

        // Windows version info
#pragma warning(push)
#pragma warning(disable: 4996)
        DWORD dwVersion = GetVersion();
#pragma warning(pop)
        DWORD dwMajorVersion = LOBYTE(LOWORD(dwVersion));
        DWORD dwMinorVersion = HIBYTE(LOWORD(dwVersion));
        DWORD dwBuild = 0;
        if (dwVersion < 0x80000000) dwBuild = HIWORD(dwVersion);
        BOOL Wow64Process = FALSE;
        IsWow64Process(GetCurrentProcess(), &Wow64Process);
        const QString bitness = Wow64Process ? QStringLiteral("32") : QStringLiteral("64");
        const QString winVer = QStringLiteral("%1.%2 (%3) %4-bit")
                            .arg(QString::number(dwMajorVersion))
                            .arg(QString::number(dwMinorVersion))
                            .arg(QString::number(dwBuild))
                            .arg(bitness);
        stream << "Windows version: " << winVer << "\r\n";

        stream << "Name: " << QString::fromUtf8(urlencode(_GLOBAL_NAME)) << "\r\n"
               << "Version: " << version << "\r\n"
               << "State: " << pjsua_get_state() << "\r\n";
#ifdef _GLOBAL_VIDEO
        stream << "Video: yes\r\n";
#else
        stream << "Video: no\r\n";
#endif
        stream << "Log enabled: " << (accountSettings.enableLog ? 1 : 0) << "\r\n";

        if (accountSettings.enableLog && !accountSettings.logFile.isEmpty()) {
            stream << "\r\n";
            QFile logFile(accountSettings.logFile);
            if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                stream << logFile.readAll();
                logFile.close();
            }
        }
        stream.flush();
        dataStr = dumpText;
        file.write(dumpText.toUtf8());
        file.close();
    }

    // ---- HTTP crash report ----
    bool sendCrashReport = false;
    bool txtSent = false;
    if (!blockDump) {
        // Use WinInet directly to avoid needing MFC
        HINTERNET hSession = InternetOpen(L"Onyx-voip", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hSession) {
            HINTERNET hConnect = InternetConnect(hSession, L"onyx-voip.invalid",
                                                 INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL,
                                                 INTERNET_SERVICE_HTTP, 0, 0);
            if (hConnect) {
                HINTERNET hRequest = HttpOpenRequest(hConnect, L"POST", L"/crash-report?rev=2",
                                                     NULL, NULL, NULL,
                                                     INTERNET_FLAG_SECURE, 0);
                if (hRequest) {
                    QString headers = "Content-Type: application/x-www-form-urlencoded\r\n";
        QString formData = QStringLiteral("name=%1&version=%2")
                                          .arg(QString::fromUtf8(urlencode(_GLOBAL_NAME)), version);
#ifdef _GLOBAL_VIDEO
                    formData += "&video=1";
#endif
                    if (sendCrashReport) {
                        formData += "&dump=" + QString::fromUtf8(urlencode(dataStr.toUtf8()));
                    }

                    if (HttpSendRequest(hRequest, headers.toStdWString().c_str(), headers.length(),
                                        (LPVOID)formData.toUtf8().constData(), formData.toUtf8().size())) {
                        DWORD statusCode = 0;
                        DWORD size = sizeof(statusCode);
                        if (HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                                          &statusCode, &size, NULL) && statusCode == 200) {
                            txtSent = true;
                        }
                    }
                    InternetCloseHandle(hRequest);
                }
                InternetCloseHandle(hConnect);
            }
            InternetCloseHandle(hSession);
        }
    }

    // ---- MiniDump ----
    QString dumpFileName = QString("%1crash-dump_%2.dmp")
                               .arg(accountSettings.pathLocal, version);
    file.setFileName(dumpFileName);
    if (file.open(QIODevice::ReadWrite)) {
        MINIDUMP_EXCEPTION_INFORMATION MinidumpExceptionInfo;
        MinidumpExceptionInfo.ThreadId = GetCurrentThreadId();
        MinidumpExceptionInfo.ExceptionPointers = ExceptionInfo;
        MinidumpExceptionInfo.ClientPointers = FALSE;
        const HANDLE dumpHandle = reinterpret_cast<HANDLE>(static_cast<ULONG_PTR>(_get_osfhandle(file.handle())));
        if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                              dumpHandle, MiniDumpNormal,
                              &MinidumpExceptionInfo, NULL, NULL)) {
            // Upload dump if needed
            if (sendCrashReport && txtSent && !blockDump) {
                HINTERNET hSess = InternetOpen(L"Onyx-voip", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
                if (hSess) {
                    HINTERNET hConn = InternetConnect(hSess, L"onyx-voip.invalid",
                                                      INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL,
                                                      INTERNET_SERVICE_HTTP, 0, 0);
                    if (hConn) {
                        HINTERNET hReq = HttpOpenRequest(hConn, L"POST", L"/crash-report",
                                                         NULL, NULL, NULL,
                                                         INTERNET_FLAG_SECURE, 0);
                        if (hReq) {
                             QString hdrs = QStringLiteral("Content-Type: application/octet-stream\r\n"
                                                    "X-Name: %1\r\nX-Version: %2\r\n"
                                                    "%3")
                                               .arg(QString::fromUtf8(urlencode(_GLOBAL_NAME_VISIBLE)), version,
#ifdef _GLOBAL_VIDEO
                                               QStringLiteral("X-Video: 1\r\n")
#else
                                               QStringLiteral("")
#endif
                                               );
                            if (HttpSendRequest(hReq, hdrs.toStdWString().c_str(), hdrs.length(),
                                                NULL, 0)) {
                                char buf[1024];
                                int len;
                                file.seek(0);
                                while ((len = file.read(buf, sizeof(buf))) > 0) {
                                    DWORD written;
                                    InternetWriteFile(hReq, buf, len, &written);
                                }
                                HttpEndRequest(hReq, NULL, 0, 0);
                            }
                            InternetCloseHandle(hReq);
                        }
                        InternetCloseHandle(hConn);
                    }
                    InternetCloseHandle(hSess);
                }
            }
        }
        file.close();
    }

    // ---- User notification / restart ----
    bool restart = is_pjsua_running() && (tm.toSecsSinceEpoch() - startTime.toSecsSinceEpoch() > 10);
    QString message;

    if (blockDump) {
        message = QStringLiteral("%1 %2")
                      .arg(Translate("A crash happened. It is strongly recommended that you update your version to continue using the software safely."),
                           Translate("Would you like to update it now?"));
        if (QMessageBox::critical(nullptr, _GLOBAL_NAME_VISIBLE, message,
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            Onyx::OpenURL("https://example.com/onyx-voip/downloads");
            return EXCEPTION_EXECUTE_HANDLER;
        }
    } else {
        if (!restart) {
#ifdef _GLOBAL_VIDEO
            message = QStringLiteral("A crash happened. Check your video card driver or try to install the LITE version (without video). Tracking info: %1%2")
                          .arg(tm.toString("yyyyMMddhhmmss"),
                               sent ? QStringLiteral("Y") : QStringLiteral("N"));
#else
            message = QStringLiteral("A crash happened. Make sure your system is working properly and that you have enough free memory and hard disk space. Check your sound device driver, antivirus software. Try disabling additional softphone features. You can try uninstalling Onyx-voip \"with configuration\" and reinstalling it. Tracking info: %1%2")
                          .arg(tm.toString("yyyyMMddhhmmss"),
                               sent ? QStringLiteral("Y") : QStringLiteral("N"));
#endif
            QMessageBox::critical(nullptr, _GLOBAL_NAME_VISIBLE, message);
        }
    }
    if (restart) {
        // automatic restart after sip crash
        ShellExecute(NULL, NULL, accountSettings.exeFile.toStdWString().c_str(), NULL, NULL, SW_SHOWDEFAULT);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

// ================== Single instance enum ==================
struct OnyxEnumWindowsProcData {
    HINSTANCE hInst;
    HWND hWnd;
    int count;
};

static BOOL CALLBACK OnyxEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    OnyxEnumWindowsProcData *data = (OnyxEnumWindowsProcData *)lParam;
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    if (hInstance && hInstance == data->hInst && GetWindow(hWnd, GW_OWNER) == (HWND)0) {
        TCHAR className[256];
        if (GetClassName(hWnd, className, 256)) {
            if (_tcscmp(className, _T(_GLOBAL_NAME)) == 0) {
                DWORD dwProcessID;
                GetWindowThreadProcessId(hWnd, &dwProcessID);
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
                if (hProcess) {
                    TCHAR exeFilePath[MAX_PATH];
                    if (GetModuleFileNameEx(hProcess, NULL, exeFilePath, MAX_PATH)) {
                        if (_tcsicmp(exeFilePath, accountSettings.exeFile.toStdWString().c_str()) == 0) {
                            data->hWnd = hWnd;
                            data->count++;
                            return FALSE;
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        }
    }
    return TRUE;
}

// ================== Application constructor ==================
OnyxApp::OnyxApp(int &argc, char **argv)
    : QApplication(argc, argv)
{
    // MFC’s InitInstance equivalent will be called explicitly
}

// ================== InitInstance ==================
bool OnyxApp::InitInstance()
{
    theApp = this;
    QString strCommandLine = QString::fromWCharArray(GetCommandLine()); // or use QCoreApplication::arguments()
    accountSettings.Init();
    SetUnhandledExceptionFilter(ExceptionFilter);

    OnyxEnumWindowsProcData data;
    data.hInst = GetModuleHandle(nullptr);
    data.count = 0;
    data.hWnd = NULL;

    EnumWindows(OnyxEnumWindowsProc, (LPARAM)&data);
    HWND hWndRunning = data.hWnd;

    //*((char*)NULL) = 0; //produce a crash!!

    bool cmdReset = (strCommandLine == "/reset");
    bool cmdResetNoAsk = (strCommandLine == "/resetnoask");
    if (cmdReset || cmdResetNoAsk) {
        if (hWndRunning) {
            ::SendMessage(hWndRunning, WM_CLOSE, NULL, NULL);
        }
        if (cmdResetNoAsk ||
            QMessageBox::question(nullptr, Translate("Delete Settings"),
                                  Translate("Do you want to delete user data and program settings? This action cannot be undone."),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
            if (cmdResetNoAsk ||
                QMessageBox::question(nullptr, Translate("Confirm Delete"),
                                      Translate("Are you sure you want to delete?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
                RecursiveDelete(accountSettings.appDataRoaming);
                if (!QFile::exists(accountSettings.appDataLocal + "/Uninstall.exe")) {
                    RecursiveDelete(accountSettings.appDataLocal);
                }
            }
        }
        return false;
    }

    bool cmdExit = (strCommandLine == "/exit");
    if (cmdExit) {
        if (hWndRunning) {
            ::SendMessage(hWndRunning, WM_CLOSE, NULL, NULL);
        }
        return false;
    }

    if (hWndRunning) {
        if (strCommandLine != "/minimized") {
            bool activate = true;
            if (!strCommandLine.isEmpty()) {
                COPYDATASTRUCT cd;
                cd.dwData = 1;
                cd.lpData = (PVOID)strCommandLine.utf16();
                cd.cbData = (DWORD)(strCommandLine.size() + 1) * sizeof(wchar_t);
                activate = ::SendMessage(hWndRunning, WM_COPYDATA, NULL, (LPARAM)&cd);
            }
            if (activate) {
                ::ShowWindow(hWndRunning, SW_SHOW);
                ::SetForegroundWindow(hWndRunning);
            }
        }
        return false;
    } else {
        if (strCommandLine == "/answer" || strCommandLine == "/hangupall") {
            return false;
        }
    }

    // Init common controls (same as MFC)
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_LINK_CLASS | ICC_BAR_CLASSES |
                      ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS;
    InitCommonControlsEx(&InitCtrls);

    // OLE init is not needed in Qt (but keep if you use COM)
    // if (!AfxOleInit()) { ... } -> skip; Qt handles COM differently if needed.

    // Rich edit 2.0 (if you still use RichEdit controls)
    // LoadLibrary(L"Msftedit.dll"); etc. – you can do that.

    // Shell manager (for drag/drop etc.) not needed.

    // Register window class (the original registered a class for the main dialog)
    WNDCLASSW wc = {};
    if (::GetClassInfoW(GetModuleHandle(nullptr), L"#32770", &wc)) {
        wc.lpszClassName = _T(_GLOBAL_NAME);
        ::RegisterClassW(&wc);
    }

    CmainDlg *mainDlg = new CmainDlg;   // Qt‑based CmainDlg (QWidget)
    // MFC’s m_pMainWnd = mainDlg;   we can simply show it later
    ::mainDlg = mainDlg;
    mainDlg->show();

    if (!mainDlg) {
        QMessageBox::critical(nullptr, "Error", "Main dialog creation failed.");
        return false;
    }

    // Post‑creation step
    mainDlg->OnCreated();

    return true;
}

// ================== Global application pointer ==================
OnyxApp *theApp = nullptr;

int main(int argc, char *argv[])
{
    OnyxApp app(argc, argv);
    theApp = &app;
    if (!app.InitInstance()) {
        return 1;
    }
    return app.exec();
}
