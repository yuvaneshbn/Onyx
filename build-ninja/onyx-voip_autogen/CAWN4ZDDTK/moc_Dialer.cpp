/****************************************************************************
** Meta object code from reading C++ file 'Dialer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/Dialer.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dialer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13CBitmapButtonE_t {};
} // unnamed namespace

template <> constexpr inline auto CBitmapButton::qt_create_metaobjectdata<qt_meta_tag_ZN13CBitmapButtonE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CBitmapButton"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CBitmapButton, qt_meta_tag_ZN13CBitmapButtonE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CBitmapButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CBitmapButtonE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CBitmapButtonE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13CBitmapButtonE_t>.metaTypes,
    nullptr
} };

void CBitmapButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CBitmapButton *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *CBitmapButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CBitmapButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CBitmapButtonE_t>.strings))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int CBitmapButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN6DialerE_t {};
} // unnamed namespace

template <> constexpr inline auto Dialer::qt_create_metaobjectdata<qt_meta_tag_ZN6DialerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Dialer",
        "onOkClicked",
        "",
        "onCancelClicked",
        "onDNDClicked",
        "onFWDClicked",
        "onAAClicked",
        "onACClicked",
        "onConfClicked",
        "onRecClicked",
        "onVoicemailClicked",
        "onCallClicked",
        "onVideoCallClicked",
        "onDTMFClicked",
        "onMessageClicked",
        "onHoldClicked",
        "onTransferClicked",
        "onEndClicked",
        "onComboEditTextChanged",
        "text",
        "onComboIndexChanged",
        "index",
        "onPlusInputClicked",
        "onMinusInputClicked",
        "onPlusOutputClicked",
        "onMinusOutputClicked",
        "onMuteOutputClicked",
        "onMuteInputClicked",
        "onKey1Clicked",
        "onKey2Clicked",
        "onKey3Clicked",
        "onKey4Clicked",
        "onKey5Clicked",
        "onKey6Clicked",
        "onKey7Clicked",
        "onKey8Clicked",
        "onKey9Clicked",
        "onKeyStarClicked",
        "onKey0Clicked",
        "onKeyGrateClicked",
        "onKeyPlusClicked",
        "onClearClicked",
        "onRedialClicked",
        "onDeleteClicked",
        "OnBnClickedRedial",
        "OnBnClickedMuteInput",
        "OnBnClickedMuteOutput",
        "OnBnClickedPlusInput",
        "OnBnClickedMinusInput",
        "OnBnClickedPlusOutput",
        "OnBnClickedMinusOutput",
        "onSliderOutputMoved",
        "value",
        "onSliderInputMoved",
        "onShortcutClicked",
        "onTimerBlink",
        "OnBnClickedCall",
        "OnTimerVUMeter"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onOkClicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCancelClicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onDNDClicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onFWDClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onAAClicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onACClicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onConfClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onRecClicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onVoicemailClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onCallClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onVideoCallClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onDTMFClicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onMessageClicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onHoldClicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTransferClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onEndClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onComboEditTextChanged'
        QtMocHelpers::SlotData<void(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Slot 'onComboIndexChanged'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 },
        }}),
        // Slot 'onPlusInputClicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onMinusInputClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPlusOutputClicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onMinusOutputClicked'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onMuteOutputClicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onMuteInputClicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey1Clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey2Clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey3Clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey4Clicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey5Clicked'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey6Clicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey7Clicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey8Clicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey9Clicked'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKeyStarClicked'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKey0Clicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKeyGrateClicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKeyPlusClicked'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onClearClicked'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onRedialClicked'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onDeleteClicked'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedRedial'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMuteInput'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMuteOutput'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedPlusInput'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMinusInput'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedPlusOutput'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedMinusOutput'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onSliderOutputMoved'
        QtMocHelpers::SlotData<void(int)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onSliderInputMoved'
        QtMocHelpers::SlotData<void(int)>(53, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 52 },
        }}),
        // Slot 'onShortcutClicked'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTimerBlink'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnBnClickedCall'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'OnTimerVUMeter'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Dialer, qt_meta_tag_ZN6DialerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Dialer::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6DialerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6DialerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6DialerE_t>.metaTypes,
    nullptr
} };

void Dialer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Dialer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onOkClicked(); break;
        case 1: _t->onCancelClicked(); break;
        case 2: _t->onDNDClicked(); break;
        case 3: _t->onFWDClicked(); break;
        case 4: _t->onAAClicked(); break;
        case 5: _t->onACClicked(); break;
        case 6: _t->onConfClicked(); break;
        case 7: _t->onRecClicked(); break;
        case 8: _t->onVoicemailClicked(); break;
        case 9: _t->onCallClicked(); break;
        case 10: _t->onVideoCallClicked(); break;
        case 11: _t->onDTMFClicked(); break;
        case 12: _t->onMessageClicked(); break;
        case 13: _t->onHoldClicked(); break;
        case 14: _t->onTransferClicked(); break;
        case 15: _t->onEndClicked(); break;
        case 16: _t->onComboEditTextChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->onComboIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->onPlusInputClicked(); break;
        case 19: _t->onMinusInputClicked(); break;
        case 20: _t->onPlusOutputClicked(); break;
        case 21: _t->onMinusOutputClicked(); break;
        case 22: _t->onMuteOutputClicked(); break;
        case 23: _t->onMuteInputClicked(); break;
        case 24: _t->onKey1Clicked(); break;
        case 25: _t->onKey2Clicked(); break;
        case 26: _t->onKey3Clicked(); break;
        case 27: _t->onKey4Clicked(); break;
        case 28: _t->onKey5Clicked(); break;
        case 29: _t->onKey6Clicked(); break;
        case 30: _t->onKey7Clicked(); break;
        case 31: _t->onKey8Clicked(); break;
        case 32: _t->onKey9Clicked(); break;
        case 33: _t->onKeyStarClicked(); break;
        case 34: _t->onKey0Clicked(); break;
        case 35: _t->onKeyGrateClicked(); break;
        case 36: _t->onKeyPlusClicked(); break;
        case 37: _t->onClearClicked(); break;
        case 38: _t->onRedialClicked(); break;
        case 39: _t->onDeleteClicked(); break;
        case 40: _t->OnBnClickedRedial(); break;
        case 41: _t->OnBnClickedMuteInput(); break;
        case 42: _t->OnBnClickedMuteOutput(); break;
        case 43: _t->OnBnClickedPlusInput(); break;
        case 44: _t->OnBnClickedMinusInput(); break;
        case 45: _t->OnBnClickedPlusOutput(); break;
        case 46: _t->OnBnClickedMinusOutput(); break;
        case 47: _t->onSliderOutputMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 48: _t->onSliderInputMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 49: _t->onShortcutClicked(); break;
        case 50: _t->onTimerBlink(); break;
        case 51: _t->OnBnClickedCall(); break;
        case 52: _t->OnTimerVUMeter(); break;
        default: ;
        }
    }
}

const QMetaObject *Dialer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6DialerE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Dialer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 53)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 53;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 53)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 53;
    }
    return _id;
}
QT_WARNING_POP
