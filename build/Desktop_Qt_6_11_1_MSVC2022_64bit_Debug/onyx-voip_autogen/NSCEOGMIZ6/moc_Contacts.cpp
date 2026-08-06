/****************************************************************************
** Meta object code from reading C++ file 'Contacts.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../source/include/Contacts.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Contacts.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8ContactsE_t {};
} // unnamed namespace

template <> constexpr inline auto Contacts::qt_create_metaobjectdata<qt_meta_tag_ZN8ContactsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Contacts",
        "onHeaderSectionResized",
        "",
        "logicalIndex",
        "oldSize",
        "newSize",
        "onFilterTextChanged",
        "text",
        "onItemDoubleClicked",
        "row",
        "column",
        "onOkClicked",
        "onCancelClicked",
        "onContextMenuRequested",
        "QPoint",
        "pos",
        "onMenuCallPickup",
        "onMenuCall",
        "onMenuCallPhone",
        "onMenuCallVideo",
        "onMenuCallMobile",
        "onMenuChat",
        "onMenuAdd",
        "onMenuEdit",
        "onMenuCopy",
        "onMenuDelete",
        "onMenuImport",
        "onMenuExport",
        "onTimerContactsBlink",
        "contactDelete",
        "contactDeleteRaw",
        "Contact*",
        "contact",
        "getSelectedRow",
        "getSelectedRows",
        "QList<int>",
        "presenceUnsubscribeOne",
        "pContact"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onHeaderSectionResized'
        QtMocHelpers::SlotData<void(int, int, int)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Int, 5 },
        }}),
        // Slot 'onFilterTextChanged'
        QtMocHelpers::SlotData<void(const QString &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Slot 'onItemDoubleClicked'
        QtMocHelpers::SlotData<void(int, int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Slot 'onOkClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCancelClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onContextMenuRequested'
        QtMocHelpers::SlotData<void(const QPoint &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'onMenuCallPickup'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuCall'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuCallPhone'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuCallVideo'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuCallMobile'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuChat'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuAdd'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuEdit'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuCopy'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuDelete'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuImport'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMenuExport'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTimerContactsBlink'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'contactDelete'
        QtMocHelpers::SlotData<void(int)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'contactDeleteRaw'
        QtMocHelpers::SlotData<void(Contact *)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 31, 32 },
        }}),
        // Slot 'getSelectedRow'
        QtMocHelpers::SlotData<int() const>(33, 2, QMC::AccessPrivate, QMetaType::Int),
        // Slot 'getSelectedRows'
        QtMocHelpers::SlotData<QList<int>() const>(34, 2, QMC::AccessPrivate, 0x80000000 | 35),
        // Slot 'presenceUnsubscribeOne'
        QtMocHelpers::SlotData<void(Contact *)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 31, 37 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Contacts, qt_meta_tag_ZN8ContactsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Contacts::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ContactsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ContactsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8ContactsE_t>.metaTypes,
    nullptr
} };

void Contacts::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Contacts *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onHeaderSectionResized((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 1: _t->onFilterTextChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->onItemDoubleClicked((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->onOkClicked(); break;
        case 4: _t->onCancelClicked(); break;
        case 5: _t->onContextMenuRequested((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 6: _t->onMenuCallPickup(); break;
        case 7: _t->onMenuCall(); break;
        case 8: _t->onMenuCallPhone(); break;
        case 9: _t->onMenuCallVideo(); break;
        case 10: _t->onMenuCallMobile(); break;
        case 11: _t->onMenuChat(); break;
        case 12: _t->onMenuAdd(); break;
        case 13: _t->onMenuEdit(); break;
        case 14: _t->onMenuCopy(); break;
        case 15: _t->onMenuDelete(); break;
        case 16: _t->onMenuImport(); break;
        case 17: _t->onMenuExport(); break;
        case 18: _t->onTimerContactsBlink(); break;
        case 19: _t->contactDelete((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->contactDeleteRaw((*reinterpret_cast<std::add_pointer_t<Contact*>>(_a[1]))); break;
        case 21: { int _r = _t->getSelectedRow();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 22: { QList<int> _r = _t->getSelectedRows();
            if (_a[0]) *reinterpret_cast<QList<int>*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->presenceUnsubscribeOne((*reinterpret_cast<std::add_pointer_t<Contact*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *Contacts::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Contacts::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8ContactsE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Contacts::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}
QT_WARNING_POP
