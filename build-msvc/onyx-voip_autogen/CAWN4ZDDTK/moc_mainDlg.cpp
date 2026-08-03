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
        "onRegState2",
        "code",
        "headerError",
        "onCallState",
        "pjsua_call_info*",
        "call_info",
        "call_user_data*",
        "user_data",
        "onIncomingCall",
        "onMWIInfo",
        "WPARAM",
        "LPARAM",
        "lParam",
        "onCallMediaState",
        "onCallTransferStatus",
        "str",
        "onPager",
        "QString*",
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
        "OnSysCommand",
        "nID",
        "OnBnClickedOk",
        "OnBnClickedMenu",
        "OnContextMenu",
        "QPoint",
        "point",
        "OnDeviceChange",
        "OnSessionChange",
        "onShellHookMessage",
        "onCallAnswer",
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
        "OnUpdatePane"
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
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onRegState2'
        QtMocHelpers::SlotData<void(int, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::QString, 14 },
        }}),
        // Slot 'onCallState'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onIncomingCall'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onMWIInfo'
        QtMocHelpers::SlotData<void(WPARAM, LPARAM)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 3 }, { 0x80000000 | 23, 24 },
        }}),
        // Slot 'onCallMediaState'
        QtMocHelpers::SlotData<void(pjsua_call_info *, call_user_data *)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onCallTransferStatus'
        QtMocHelpers::SlotData<void(pjsua_call_info *, const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { QMetaType::QString, 27 },
        }}),
        // Slot 'onPager'
        QtMocHelpers::SlotData<void(QString *, QString *)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 29, 30 }, { 0x80000000 | 29, 31 },
        }}),
        // Slot 'onPagerStatus'
        QtMocHelpers::SlotData<void(QString *, QString *)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 29, 30 }, { 0x80000000 | 29, 31 },
        }}),
        // Slot 'onBuddyState'
        QtMocHelpers::SlotData<void(pjsua_buddy_id)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 34, 35 },
        }}),
        // Slot 'onCopyData'
        QtMocHelpers::SlotData<void(const QString &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 37 },
        }}),
        // Slot 'CreationComplete'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnNetworkChange'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnRestart'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'PostRestart'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnPowerBroadcast'
        QtMocHelpers::SlotData<void(quint32)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 2 },
        }}),
        // Slot 'OnSysCommand'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'OnBnClickedOk'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMenu'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 48, 49 },
        }}),
        // Slot 'OnDeviceChange'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnSessionChange'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onShellHookMessage'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<void(pjsua_call_id, int)>(53, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 }, { QMetaType::Int, 24 },
        }}),
        // Slot 'onCallAnswer'
        QtMocHelpers::SlotData<void(pjsua_call_id)>(53, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'onCallHangup'
        QtMocHelpers::SlotData<void(pjsua_call_id)>(54, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'onTabIconUpdate'
        QtMocHelpers::SlotData<void(pjsua_call_id)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int, const QString &)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 57 }, { QMetaType::QString, 58 },
        }}),
        // Slot 'onPlayerPlay'
        QtMocHelpers::SlotData<void(int)>(56, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::Int, 57 },
        }}),
        // Slot 'onPlayerStop'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCommandLine'
        QtMocHelpers::SlotData<void(const QString &)>(60, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 61 },
        }}),
        // Slot 'OnAccount'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAccountAdd'
        QtMocHelpers::SlotData<void()>(63, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAccountEdit'
        QtMocHelpers::SlotData<void(int)>(64, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'OnMenuAccountChange'
        QtMocHelpers::SlotData<void(int)>(65, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'OnMenuAccountLocalEdit'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuCustomRange'
        QtMocHelpers::SlotData<void(int)>(67, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
        // Slot 'OnMenuSettings'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuShortcuts'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAlwaysOnTop'
        QtMocHelpers::SlotData<void()>(70, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuLog'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuExit'
        QtMocHelpers::SlotData<void()>(72, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnTimer'
        QtMocHelpers::SlotData<void(int)>(73, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 74 },
        }}),
        // Slot 'OnTcnSelchangeTab'
        QtMocHelpers::SlotData<void(int)>(75, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 76 },
        }}),
        // Slot 'OnTcnSelchangingTab'
        QtMocHelpers::SlotData<void(int)>(77, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 76 },
        }}),
        // Slot 'OnMenuWebsite'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuHelp'
        QtMocHelpers::SlotData<void()>(79, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMenuAddl'
        QtMocHelpers::SlotData<void()>(80, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteInput'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMuteOutput'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCheckUpdates'
        QtMocHelpers::SlotData<void()>(83, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'CheckUpdates'
        QtMocHelpers::SlotData<void()>(84, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnUpdateCheckerLoaded'
        QtMocHelpers::SlotData<void(URLGetAsyncData *)>(85, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 86, 87 },
        }}),
        // Slot 'onUsersDirectoryLoaded'
        QtMocHelpers::SlotData<void(URLGetAsyncData *)>(88, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 86, 87 },
        }}),
        // Slot 'onShortcutsURLLoaded'
        QtMocHelpers::SlotData<void(URLGetAsyncData *)>(89, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 86, 87 },
        }}),
        // Slot 'onCustomLoaded'
        QtMocHelpers::SlotData<void()>(90, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnUpdatePane'
        QtMocHelpers::SlotData<void()>(91, 2, QMC::AccessPublic, QMetaType::Void),
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
        case 5: _t->onRefreshLevels(); break;
        case 6: _t->onRegState2((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->onCallState((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 8: _t->onIncomingCall((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 9: _t->onMWIInfo((*reinterpret_cast<std::add_pointer_t<WPARAM>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<LPARAM>>(_a[2]))); break;
        case 10: _t->onCallMediaState((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<call_user_data*>>(_a[2]))); break;
        case 11: _t->onCallTransferStatus((*reinterpret_cast<std::add_pointer_t<pjsua_call_info*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->onPager((*reinterpret_cast<std::add_pointer_t<QString*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString*>>(_a[2]))); break;
        case 13: _t->onPagerStatus((*reinterpret_cast<std::add_pointer_t<QString*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString*>>(_a[2]))); break;
        case 14: _t->onBuddyState((*reinterpret_cast<std::add_pointer_t<pjsua_buddy_id>>(_a[1]))); break;
        case 15: _t->onCopyData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->CreationComplete(); break;
        case 17: _t->OnNetworkChange(); break;
        case 18: _t->OnRestart(); break;
        case 19: _t->PostRestart(); break;
        case 20: _t->OnPowerBroadcast((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1]))); break;
        case 21: _t->OnSysCommand((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->OnBnClickedOk(); break;
        case 23: _t->OnBnClickedMenu(); break;
        case 24: _t->OnContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 25: _t->OnDeviceChange(); break;
        case 26: _t->OnSessionChange(); break;
        case 27: _t->onShellHookMessage(); break;
        case 28: _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<pjsua_call_id>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 29: _t->onCallAnswer((*reinterpret_cast<std::add_pointer_t<pjsua_call_id>>(_a[1]))); break;
        case 30: _t->onCallHangup((*reinterpret_cast<std::add_pointer_t<pjsua_call_id>>(_a[1]))); break;
        case 31: _t->onTabIconUpdate((*reinterpret_cast<std::add_pointer_t<pjsua_call_id>>(_a[1]))); break;
        case 32: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 33: _t->onPlayerPlay((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->onPlayerStop(); break;
        case 35: _t->onCommandLine((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 36: _t->OnAccount(); break;
        case 37: _t->OnMenuAccountAdd(); break;
        case 38: _t->OnMenuAccountEdit((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->OnMenuAccountChange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 40: _t->OnMenuAccountLocalEdit(); break;
        case 41: _t->OnMenuCustomRange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 42: _t->OnMenuSettings(); break;
        case 43: _t->OnMenuShortcuts(); break;
        case 44: _t->OnMenuAlwaysOnTop(); break;
        case 45: _t->OnMenuLog(); break;
        case 46: _t->OnMenuExit(); break;
        case 47: _t->OnTimer((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 48: _t->OnTcnSelchangeTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 49: _t->OnTcnSelchangingTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 50: _t->OnMenuWebsite(); break;
        case 51: _t->OnMenuHelp(); break;
        case 52: _t->OnMenuAddl(); break;
        case 53: _t->OnMuteInput(); break;
        case 54: _t->OnMuteOutput(); break;
        case 55: _t->OnCheckUpdates(); break;
        case 56: _t->CheckUpdates(); break;
        case 57: _t->OnUpdateCheckerLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1]))); break;
        case 58: _t->onUsersDirectoryLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1]))); break;
        case 59: _t->onShortcutsURLLoaded((*reinterpret_cast<std::add_pointer_t<URLGetAsyncData*>>(_a[1]))); break;
        case 60: _t->onCustomLoaded(); break;
        case 61: _t->OnUpdatePane(); break;
        default: ;
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
        if (_id < 62)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 62;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 62)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 62;
    }
    return _id;
}
QT_WARNING_POP
