/****************************************************************************
** Meta object code from reading C++ file 'mainDlg.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/mainDlg.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN8CmainDlgE_t {};
} // unnamed namespace

template <> constexpr inline auto CmainDlg::qt_create_metaobjectdata<qt_meta_tag_ZN8CmainDlgE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CmainDlg",
        "onUpdateWindowText",
        "",
        "wParam",
        "text",
        "onTrayNotify",
        "QSystemTrayIcon::ActivationReason",
        "reason",
        "onCreateRingingDlg",
        "pjsua_call_id",
        "call_id",
        "onRefreshLevels",
        "lParam",
        "onRegState2",
        "code",
        "QString*",
        "headerError",
        "onCallState",
        "pjsua_call_info*",
        "call_info",
        "call_user_data*",
        "user_data",
        "onIncomingCall",
        "onMWIInfo",
        "hasMail",
        "onCallMediaState",
        "onCallTransferStatus",
        "str",
        "onPager",
        "number",
        "message",
        "onPagerStatus",
        "onBuddyState",
        "pjsua_buddy_id",
        "buddy_id",
        "onCopyData",
        "data",
        "CreationComplete",
        "OnNetworkChange",
        "OnRestart",
        "PostRestart",
        "OnPowerBroadcast",
        "WPARAM",
        "LPARAM",
        "OnSysCommand",
        "UINT",
        "nID",
        "OnBnClickedOk",
        "OnBnClickedMenu",
        "OnContextMenu",
        "QWidget*",
        "w",
        "QPoint",
        "pos",
        "OnQueryEndSession",
        "BOOL",
        "OnClose",
        "OnCtlColor",
        "HBRUSH",
        "CDC*",
        "pDC",
        "CWnd*",
        "pWnd",
        "nCtlColor",
        "OnDeviceChange",
        "nEventType",
        "DWORD_PTR",
        "dwData",
        "OnSessionChange",
        "nSessionState",
        "nId",
        "OnMove",
        "x",
        "y",
        "OnSize",
        "type",
        "h",
        "onShellHookMessage",
        "onCallAnswer",
        "LRESULT",
        "onCallHangup",
        "onTabIconUpdate",
        "onPlayerPlay",
        "sound",
        "custom",
        "onPlayerStop",
        "onCommandLine",
        "params",
        "OnAccount",
        "OnMenuAccountAdd",
        "OnMenuAccountEdit",
        "OnMenuAccountChange",
        "OnMenuAccountLocalEdit",
        "OnMenuCustomRange",
        "OnMenuSettings",
        "OnMenuShortcuts",
        "OnMenuAlwaysOnTop",
        "OnMenuLog",
        "OnMenuExit",
        "OnTimer",
        "timerId",
        "OnTcnSelchangeTab",
        "index",
        "OnTcnSelchangingTab",
        "OnMenuWebsite",
        "OnMenuHelp",
        "OnMenuAddl",
        "OnMuteInput",
        "OnMuteOutput",
        "OnCheckUpdates",
        "CheckUpdates",
        "OnUpdateCheckerLoaded",
        "URLGetAsyncData*",
        "response",
        "onUsersDirectoryLoaded",
        "onShortcutsURLLoaded",
        "onCustomLoaded",
        "OnUpdatePane",
        "CCmdUI*",
        "pCmdUI",
        "startTimer",
        "id",
        "interval",
        "std::function<void()>",
        "callback",
        "killTimer",
        "timerEvent",
        "QTimerEvent*",
        "event"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onUpdateWindowText'
        QtMocHelpers::SlotData<void(int, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 },
        }}),
        // Slot 'onUpdateWindowText'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onUpdateWindowText'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'onTrayNotify'
        QtMocHelpers::SlotData<void(QSystemTrayIcon::ActivationReason)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'onCreateRingingDlg'
        QtMocHelpers::SlotData<void(pjsua_call_id)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'onRefreshLevels'
        QtMocHelpers::SlotData<void(int, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onRefreshLevels'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onRefreshLevels'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'onRegState2'
        QtMocHelpers::SlotData<void(int, QString *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 }, { 0x80000000 | 15, 16 },
        }}),
        // Slot 'onCallState'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 }, { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onIncomingCall'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 }, { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onMWIInfo'
        QtMocHelpers::SlotData<void(bool)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 24 },
        }}),
        // Slot 'onCallMediaState'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 }, { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onCallTransferStatus'
        QtMocHelpers::SlotData<void(pjsua_call_info *, const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 }, { QMetaType::QString, 27 },
        }}),
        // Slot 'onPager'
        QtMocHelpers::SlotData<void(QString *, QString *)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 29 }, { 0x80000000 | 15, 30 },
        }}),
        // Slot 'onPagerStatus'
        QtMocHelpers::SlotData<void(QString *, QString *)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 29 }, { 0x80000000 | 15, 30 },
        }}),
        // Slot 'onBuddyState'
        QtMocHelpers::SlotData<void(pjsua_buddy_id)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 33, 34 },
        }}),
        // Slot 'onCopyData'
        QtMocHelpers::SlotData<void(const QString &)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 36 },
        }}),
        // Slot 'CreationComplete'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnNetworkChange'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnRestart'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'PostRestart'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnPowerBroadcast'
        QtMocHelpers::SlotData<void(WPARAM, LPARAM)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 42, 3 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'OnPowerBroadcast'
        QtMocHelpers::SlotData<void(WPARAM)>(41, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 42, 3 },
        }}),
        // Slot 'OnSysCommand'
        QtMocHelpers::SlotData<void(UINT, LPARAM)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 45, 46 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'OnSysCommand'
        QtMocHelpers::SlotData<void(UINT)>(44, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 45, 46 },
        }}),
        // Slot 'OnBnClickedOk'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMenu'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnContextMenu'
        QtMocHelpers::SlotData<void(QWidget *, const QPoint &)>(49, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 50, 51 }, { 0x80000000 | 52, 53 },
        }}),
        // Slot 'OnQueryEndSession'
        QtMocHelpers::SlotData<BOOL()>(54, 2, QMC::AccessPublic, 0x80000000 | 55),
        // Slot 'OnClose'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCtlColor'
        QtMocHelpers::SlotData<HBRUSH(CDC *, CWnd *, UINT)>(57, 2, QMC::AccessPublic, 0x80000000 | 58, {{
            { 0x80000000 | 59, 60 }, { 0x80000000 | 61, 62 }, { 0x80000000 | 45, 63 },
        }}),
        // Slot 'OnDeviceChange'
        QtMocHelpers::SlotData<BOOL(UINT, DWORD_PTR)>(64, 2, QMC::AccessPublic, 0x80000000 | 55, {{
            { 0x80000000 | 45, 65 }, { 0x80000000 | 66, 67 },
        }}),
        // Slot 'OnSessionChange'
        QtMocHelpers::SlotData<void(UINT, UINT)>(68, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 45, 69 }, { 0x80000000 | 45, 70 },
        }}),
        // Slot 'OnMove'
        QtMocHelpers::SlotData<void(int, int)>(71, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 72 }, { QMetaType::Int, 73 },
        }}),
        // Slot 'OnSize'
        QtMocHelpers::SlotData<void(UINT, int, int)>(74, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 45, 75 }, { QMetaType::Int, 51 }, { QMetaType::Int, 76 },
        }}),
        // Slot 'onShellHookMessage'
        QtMocHelpers::SlotData<void()>(77, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<LRESULT(int, int)>(78, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<LRESULT(int)>(78, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 79, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onCallHangup'
        QtMocHelpers::SlotData<LRESULT(int)>(80, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onTabIconUpdate'
        QtMocHelpers::SlotData<LRESULT(int)>(81, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int, const QString &)>(82, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 83 }, { QMetaType::QString, 84 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int)>(82, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 83 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void(int, int)>(85, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void(int)>(85, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void()>(85, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'onCommandLine'
        QtMocHelpers::SlotData<void(const QString &)>(86, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 87 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void(WPARAM, LPARAM)>(88, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 42, 3 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void(WPARAM)>(88, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 42, 3 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void()>(88, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'OnMenuAccountAdd'
        QtMocHelpers::SlotData<void()>(89, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAccountEdit'
        QtMocHelpers::SlotData<void(int)>(90, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuAccountChange'
        QtMocHelpers::SlotData<void(int)>(91, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuAccountLocalEdit'
        QtMocHelpers::SlotData<void()>(92, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuCustomRange'
        QtMocHelpers::SlotData<void(int)>(93, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuSettings'
        QtMocHelpers::SlotData<void()>(94, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuShortcuts'
        QtMocHelpers::SlotData<void()>(95, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAlwaysOnTop'
        QtMocHelpers::SlotData<void()>(96, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuLog'
        QtMocHelpers::SlotData<void()>(97, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuExit'
        QtMocHelpers::SlotData<void()>(98, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnTimer'
        QtMocHelpers::SlotData<void(int)>(99, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 100 },
        }}),
        // Slot 'OnTcnSelchangeTab'
        QtMocHelpers::SlotData<void(int)>(101, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 102 },
        }}),
        // Slot 'OnTcnSelchangingTab'
        QtMocHelpers::SlotData<void(int)>(103, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 102 },
        }}),
        // Slot 'OnMenuWebsite'
        QtMocHelpers::SlotData<void()>(104, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuHelp'
        QtMocHelpers::SlotData<void()>(105, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAddl'
        QtMocHelpers::SlotData<void()>(106, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteInput'
        QtMocHelpers::SlotData<void()>(107, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteOutput'
        QtMocHelpers::SlotData<void()>(108, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCheckUpdates'
        QtMocHelpers::SlotData<void()>(109, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'CheckUpdates'
        QtMocHelpers::SlotData<void()>(110, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnUpdateCheckerLoaded'
        QtMocHelpers::SlotData<LRESULT(URLGetAsyncData *)>(111, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { 0x80000000 | 112, 113 },
        }}),
        // Slot 'onUsersDirectoryLoaded'
        QtMocHelpers::SlotData<LRESULT(URLGetAsyncData *)>(114, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { 0x80000000 | 112, 113 },
        }}),
        // Slot 'onShortcutsURLLoaded'
        QtMocHelpers::SlotData<void(URLGetAsyncData *)>(115, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 112, 113 },
        }}),
        // Slot 'onCustomLoaded'
        QtMocHelpers::SlotData<LRESULT(WPARAM, LPARAM)>(116, 2, QMC::AccessPublic, 0x80000000 | 79, {{
            { 0x80000000 | 42, 3 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'OnUpdatePane'
        QtMocHelpers::SlotData<void(CCmdUI *)>(117, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 118, 119 },
        }}),
        // Slot 'startTimer'
        QtMocHelpers::SlotData<void(int, int, std::function<void()>)>(120, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 121 }, { QMetaType::Int, 122 }, { 0x80000000 | 123, 124 },
        }}),
        // Slot 'startTimer'
        QtMocHelpers::SlotData<void(int, int)>(120, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 121 }, { QMetaType::Int, 122 },
        }}),
        // Slot 'killTimer'
        QtMocHelpers::SlotData<void(int)>(125, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 121 },
        }}),
        // Slot 'timerEvent'
        QtMocHelpers::SlotData<void(QTimerEvent *)>(126, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 127, 128 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CmainDlg, qt_meta_tag_ZN8CmainDlgE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CmainDlg::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8CmainDlgE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8CmainDlgE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8CmainDlgE_t>.metaTypes,
    nullptr
} };

void CmainDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CmainDlg *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onUpdateWindowText((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->onUpdateWindowText((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onUpdateWindowText(); break;
        case 3: _t->onTrayNotify((*reinterpret_cast<std::add_pointer_t<QSystemTrayIcon::ActivationReason>>(_a[1]))); break;
        case 4: _t->onCreateRingingDlg((*reinterpret_cast<std::add_pointer_t<pjsua_call_id>>(_a[1]))); break;
        case 5: _t->onRefreshLevels((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->onRefreshLevels((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onRefreshLevels(); break;
        case 8: _t->onRegState2((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString*>>(_a[2]))); break;
        case 9: _t->onCallState((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 10: _t->onIncomingCall((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 11: _t->onMWIInfo((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 12: _t->onCallMediaState((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 13: _t->onCallTransferStatus((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->onPager((*reinterpret_cast<std::add_pointer_t<QString*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString*>>(_a[2]))); break;
        case 15: _t->onPagerStatus((*reinterpret_cast<std::add_pointer_t<QString*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString*>>(_a[2]))); break;
        case 16: _t->onBuddyState((*reinterpret_cast<std::add_pointer_t<pjsua_buddy_id>>(_a[1]))); break;
        case 17: _t->onCopyData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->CreationComplete(); break;
        case 19: _t->OnNetworkChange(); break;
        case 20: _t->OnRestart(); break;
        case 21: _t->PostRestart(); break;
        case 22: _t->OnPowerBroadcast((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2]))); break;
        case 23: _t->OnPowerBroadcast((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1]))); break;
        case 24: _t->OnSysCommand((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2]))); break;
        case 25: _t->OnSysCommand((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1]))); break;
        case 26: _t->OnBnClickedOk(); break;
        case 27: _t->OnBnClickedMenu(); break;
        case 28: _t->OnContextMenu((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[2]))); break;
        case 29: { BOOL _r = _t->OnQueryEndSession();
            if (_a[0]) *reinterpret_cast<BOOL*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->OnClose(); break;
        case 31: { HBRUSH _r = _t->OnCtlColor((*reinterpret_cast<std::add_pointer_t<CDC*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<CWnd*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<UINT>>(_a[3])));
            if (_a[0]) *reinterpret_cast<HBRUSH*>(_a[0]) = std::move(_r); }  break;
        case 32: { BOOL _r = _t->OnDeviceChange((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<DWORD_PTR>>(_a[2])));
            if (_a[0]) *reinterpret_cast<BOOL*>(_a[0]) = std::move(_r); }  break;
        case 33: _t->OnSessionChange((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<UINT>>(_a[2]))); break;
        case 34: _t->OnMove((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 35: _t->OnSize((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 36: _t->onShellHookMessage(); break;
        case 37: { LRESULT _r = _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 38: { LRESULT _r = _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 39: { LRESULT _r = _t->onCallHangup((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 40: { LRESULT _r = _t->onTabIconUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 41: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 42: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 43: _t->onPlayerStop((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 44: _t->onPlayerStop((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 45: _t->onPlayerStop(); break;
        case 46: _t->onCommandLine((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 47: _t->OnAccount((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2]))); break;
        case 48: _t->OnAccount((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1]))); break;
        case 49: _t->OnAccount(); break;
        case 50: _t->OnMenuAccountAdd(); break;
        case 51: _t->OnMenuAccountEdit((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 52: _t->OnMenuAccountChange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 53: _t->OnMenuAccountLocalEdit(); break;
        case 54: _t->OnMenuCustomRange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 55: _t->OnMenuSettings(); break;
        case 56: _t->OnMenuShortcuts(); break;
        case 57: _t->OnMenuAlwaysOnTop(); break;
        case 58: _t->OnMenuLog(); break;
        case 59: _t->OnMenuExit(); break;
        case 60: _t->OnTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 61: _t->OnTcnSelchangeTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 62: _t->OnTcnSelchangingTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 63: _t->OnMenuWebsite(); break;
        case 64: _t->OnMenuHelp(); break;
        case 65: _t->OnMenuAddl(); break;
        case 66: _t->OnMuteInput(); break;
        case 67: _t->OnMuteOutput(); break;
        case 68: _t->OnCheckUpdates(); break;
        case 69: _t->CheckUpdates(); break;
        case 70: { LRESULT _r = _t->OnUpdateCheckerLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 71: { LRESULT _r = _t->onUsersDirectoryLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 72: _t->onShortcutsURLLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1]))); break;
        case 73: { LRESULT _r = _t->onCustomLoaded((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 74: _t->OnUpdatePane((*reinterpret_cast<std::add_pointer_t<CCmdUI*>>(_a[1]))); break;
        case 75: _t->startTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<std::function<void()>>>(_a[3]))); break;
        case 76: _t->startTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 77: _t->killTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 78: _t->timerEvent((*reinterpret_cast<std::add_pointer_t<QTimerEvent*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 28:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *CmainDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CmainDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8CmainDlgE_t>.strings))
        return static_cast<void*>(this);
    return BaseDialog::qt_metacast(_clname);
}

int CmainDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 79)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 79;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 79)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 79;
    }
    return _id;
}
QT_WARNING_POP
