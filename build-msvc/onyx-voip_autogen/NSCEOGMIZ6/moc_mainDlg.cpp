/****************************************************************************
** Meta object code from reading C++ file 'mainDlg.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/include/mainDlg.h"
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
        // Slot 'OnDeviceChange'
        QtMocHelpers::SlotData<BOOL(UINT, DWORD_PTR)>(57, 2, QMC::AccessPublic, 0x80000000 | 55, {{
            { 0x80000000 | 45, 58 }, { 0x80000000 | 59, 60 },
        }}),
        // Slot 'OnSessionChange'
        QtMocHelpers::SlotData<void(UINT, UINT)>(61, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 45, 62 }, { 0x80000000 | 45, 63 },
        }}),
        // Slot 'OnMove'
        QtMocHelpers::SlotData<void(int, int)>(64, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 65 }, { QMetaType::Int, 66 },
        }}),
        // Slot 'OnSize'
        QtMocHelpers::SlotData<void(UINT, int, int)>(67, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 45, 68 }, { QMetaType::Int, 51 }, { QMetaType::Int, 69 },
        }}),
        // Slot 'onShellHookMessage'
        QtMocHelpers::SlotData<void()>(70, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<LRESULT(int, int)>(71, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<LRESULT(int)>(71, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 72, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onCallHangup'
        QtMocHelpers::SlotData<LRESULT(int)>(73, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onTabIconUpdate'
        QtMocHelpers::SlotData<LRESULT(int)>(74, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int, const QString &)>(75, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 76 }, { QMetaType::QString, 77 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int)>(75, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 76 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void(int, int)>(78, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void(int)>(78, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'onCommandLine'
        QtMocHelpers::SlotData<void(const QString &)>(79, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 80 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void(WPARAM, LPARAM)>(81, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 42, 3 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void(WPARAM)>(81, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 42, 3 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'OnMenuAccountAdd'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAccountEdit'
        QtMocHelpers::SlotData<void(int)>(83, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuAccountChange'
        QtMocHelpers::SlotData<void(int)>(84, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuAccountLocalEdit'
        QtMocHelpers::SlotData<void()>(85, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuCustomRange'
        QtMocHelpers::SlotData<void(int)>(86, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 46 },
        }}),
        // Slot 'OnMenuSettings'
        QtMocHelpers::SlotData<void()>(87, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuShortcuts'
        QtMocHelpers::SlotData<void()>(88, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAlwaysOnTop'
        QtMocHelpers::SlotData<void()>(89, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuLog'
        QtMocHelpers::SlotData<void()>(90, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuExit'
        QtMocHelpers::SlotData<void()>(91, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnTimer'
        QtMocHelpers::SlotData<void(int)>(92, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 93 },
        }}),
        // Slot 'OnTcnSelchangeTab'
        QtMocHelpers::SlotData<void(int)>(94, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 95 },
        }}),
        // Slot 'OnTcnSelchangingTab'
        QtMocHelpers::SlotData<void(int)>(96, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 95 },
        }}),
        // Slot 'OnMenuWebsite'
        QtMocHelpers::SlotData<void()>(97, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuHelp'
        QtMocHelpers::SlotData<void()>(98, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAddl'
        QtMocHelpers::SlotData<void()>(99, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteInput'
        QtMocHelpers::SlotData<void()>(100, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteOutput'
        QtMocHelpers::SlotData<void()>(101, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCheckUpdates'
        QtMocHelpers::SlotData<void()>(102, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'CheckUpdates'
        QtMocHelpers::SlotData<void()>(103, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnUpdateCheckerLoaded'
        QtMocHelpers::SlotData<LRESULT(URLGetAsyncData *)>(104, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { 0x80000000 | 105, 106 },
        }}),
        // Slot 'onUsersDirectoryLoaded'
        QtMocHelpers::SlotData<LRESULT(URLGetAsyncData *)>(107, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { 0x80000000 | 105, 106 },
        }}),
        // Slot 'onShortcutsURLLoaded'
        QtMocHelpers::SlotData<void(URLGetAsyncData *)>(108, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 105, 106 },
        }}),
        // Slot 'onCustomLoaded'
        QtMocHelpers::SlotData<LRESULT(WPARAM, LPARAM)>(109, 2, QMC::AccessPublic, 0x80000000 | 72, {{
            { 0x80000000 | 42, 3 }, { 0x80000000 | 43, 12 },
        }}),
        // Slot 'startTimer'
        QtMocHelpers::SlotData<void(int, int, std::function<void()>)>(110, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 111 }, { QMetaType::Int, 112 }, { 0x80000000 | 113, 114 },
        }}),
        // Slot 'startTimer'
        QtMocHelpers::SlotData<void(int, int)>(110, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 111 }, { QMetaType::Int, 112 },
        }}),
        // Slot 'killTimer'
        QtMocHelpers::SlotData<void(int)>(115, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 111 },
        }}),
        // Slot 'timerEvent'
        QtMocHelpers::SlotData<void(QTimerEvent *)>(116, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 117, 118 },
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
        case 31: { BOOL _r = _t->OnDeviceChange((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<DWORD_PTR>>(_a[2])));
            if (_a[0]) *reinterpret_cast<BOOL*>(_a[0]) = std::move(_r); }  break;
        case 32: _t->OnSessionChange((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<UINT>>(_a[2]))); break;
        case 33: _t->OnMove((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 34: _t->OnSize((*reinterpret_cast<std::add_pointer_t<UINT>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 35: _t->onShellHookMessage(); break;
        case 36: { LRESULT _r = _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 37: { LRESULT _r = _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 38: { LRESULT _r = _t->onCallHangup((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 39: { LRESULT _r = _t->onTabIconUpdate((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 40: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 41: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 42: _t->onPlayerStop((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 43: _t->onPlayerStop((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 44: _t->onPlayerStop(); break;
        case 45: _t->onCommandLine((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 46: _t->OnAccount((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2]))); break;
        case 47: _t->OnAccount((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1]))); break;
        case 48: _t->OnAccount(); break;
        case 49: _t->OnMenuAccountAdd(); break;
        case 50: _t->OnMenuAccountEdit((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 51: _t->OnMenuAccountChange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 52: _t->OnMenuAccountLocalEdit(); break;
        case 53: _t->OnMenuCustomRange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 54: _t->OnMenuSettings(); break;
        case 55: _t->OnMenuShortcuts(); break;
        case 56: _t->OnMenuAlwaysOnTop(); break;
        case 57: _t->OnMenuLog(); break;
        case 58: _t->OnMenuExit(); break;
        case 59: _t->OnTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 60: _t->OnTcnSelchangeTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 61: _t->OnTcnSelchangingTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 62: _t->OnMenuWebsite(); break;
        case 63: _t->OnMenuHelp(); break;
        case 64: _t->OnMenuAddl(); break;
        case 65: _t->OnMuteInput(); break;
        case 66: _t->OnMuteOutput(); break;
        case 67: _t->OnCheckUpdates(); break;
        case 68: _t->CheckUpdates(); break;
        case 69: { LRESULT _r = _t->OnUpdateCheckerLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 70: { LRESULT _r = _t->onUsersDirectoryLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 71: _t->onShortcutsURLLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1]))); break;
        case 72: { LRESULT _r = _t->onCustomLoaded((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2])));
            if (_a[0]) *reinterpret_cast<LRESULT*>(_a[0]) = std::move(_r); }  break;
        case 73: _t->startTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<std::function<void()>>>(_a[3]))); break;
        case 74: _t->startTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 75: _t->killTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 76: _t->timerEvent((*reinterpret_cast<std::add_pointer_t<QTimerEvent*>>(_a[1]))); break;
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
        if (_id < 77)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 77;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 77)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 77;
    }
    return _id;
}
QT_WARNING_POP
