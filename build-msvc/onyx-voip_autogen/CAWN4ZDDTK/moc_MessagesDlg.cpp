/****************************************************************************
** Meta object code from reading C++ file 'MessagesDlg.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/MessagesDlg.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MessagesDlg.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11MessagesDlgE_t {};
} // unnamed namespace

template <> constexpr inline auto MessagesDlg::qt_create_metaobjectdata<qt_meta_tag_ZN11MessagesDlgE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MessagesDlg",
        "OnTcnSelchangeTab",
        "",
        "index",
        "OnTcnSelchangingTab",
        "OnBnClickedCallEnd",
        "OnBnClickedVideoCall",
        "OnBnClickedActions",
        "isConference",
        "OnBnClickedTransfer",
        "OnBnClickedConference",
        "OnBnClickedHold",
        "OnBnClickedEnd",
        "OnCloseAllTabs",
        "OnGoToLastTab",
        "OnBnClickedCloseAll",
        "OnBnClickedLastCall",
        "OnCopy",
        "OnSelectAll",
        "OnTransfer",
        "OnAttendedTransfer",
        "OnAttendedTransferRange",
        "OnConference",
        "OnMerge",
        "OnMergeAll",
        "OnSeparate",
        "OnSeparateAll",
        "OnDisconnect",
        "OnEnMsgfilterMessage",
        "OnEnLink",
        "QUrl",
        "url",
        "onTabCloseRequested",
        "onContextMenuRequested",
        "QPoint",
        "pos"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'OnTcnSelchangeTab'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'OnTcnSelchangingTab'
        QtMocHelpers::SlotData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'OnBnClickedCallEnd'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedVideoCall'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedActions'
        QtMocHelpers::SlotData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'OnBnClickedActions'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'OnBnClickedTransfer'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedConference'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedHold'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedEnd'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCloseAllTabs'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnGoToLastTab'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedCloseAll'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedLastCall'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnCopy'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnSelectAll'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnTransfer'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnAttendedTransfer'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnAttendedTransferRange'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'OnConference'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnMerge'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'OnMergeAll'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnSeparate'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnSeparateAll'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnDisconnect'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnEnMsgfilterMessage'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnEnLink'
        QtMocHelpers::SlotData<void(const QUrl &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
        // Slot 'onTabCloseRequested'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onContextMenuRequested'
        QtMocHelpers::SlotData<void(const QPoint &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 34, 35 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MessagesDlg, qt_meta_tag_ZN11MessagesDlgE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MessagesDlg::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11MessagesDlgE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11MessagesDlgE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11MessagesDlgE_t>.metaTypes,
    nullptr
} };

void MessagesDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MessagesDlg *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->OnTcnSelchangeTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->OnTcnSelchangingTab((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->OnBnClickedCallEnd(); break;
        case 3: _t->OnBnClickedVideoCall(); break;
        case 4: _t->OnBnClickedActions((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->OnBnClickedActions(); break;
        case 6: _t->OnBnClickedTransfer(); break;
        case 7: _t->OnBnClickedConference(); break;
        case 8: _t->OnBnClickedHold(); break;
        case 9: _t->OnBnClickedEnd(); break;
        case 10: _t->OnCloseAllTabs(); break;
        case 11: _t->OnGoToLastTab(); break;
        case 12: _t->OnBnClickedCloseAll(); break;
        case 13: _t->OnBnClickedLastCall(); break;
        case 14: _t->OnCopy(); break;
        case 15: _t->OnSelectAll(); break;
        case 16: _t->OnTransfer(); break;
        case 17: _t->OnAttendedTransfer(); break;
        case 18: _t->OnAttendedTransferRange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->OnConference(); break;
        case 20: _t->OnMerge((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->OnMergeAll(); break;
        case 22: _t->OnSeparate(); break;
        case 23: _t->OnSeparateAll(); break;
        case 24: _t->OnDisconnect(); break;
        case 25: _t->OnEnMsgfilterMessage(); break;
        case 26: _t->OnEnLink((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 27: _t->onTabCloseRequested((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->onContextMenuRequested((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MessagesDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessagesDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11MessagesDlgE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MessagesDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 29;
    }
    return _id;
}
QT_WARNING_POP
