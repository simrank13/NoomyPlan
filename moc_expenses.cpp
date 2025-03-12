/****************************************************************************
** Meta object code from reading C++ file 'expenses.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "expenses.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'expenses.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Expenses_t {
    const uint offsetsAndSize[26];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Expenses_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Expenses_t qt_meta_stringdata_Expenses = {
    {
QT_MOC_LITERAL(0, 8), // "Expenses"
QT_MOC_LITERAL(9, 20), // "expenseChangedSignal"
QT_MOC_LITERAL(30, 0), // ""
QT_MOC_LITERAL(31, 10), // "oldExpense"
QT_MOC_LITERAL(42, 10), // "newExpense"
QT_MOC_LITERAL(53, 24), // "onExpenseNameChangedSlot"
QT_MOC_LITERAL(78, 7), // "newName"
QT_MOC_LITERAL(86, 31), // "onExpenseDescriptionChangedSlot"
QT_MOC_LITERAL(118, 14), // "newDescription"
QT_MOC_LITERAL(133, 20), // "expenseSBChangedSlot"
QT_MOC_LITERAL(154, 6), // "change"
QT_MOC_LITERAL(161, 11), // "changedType"
QT_MOC_LITERAL(173, 13) // "updateExpense"

    },
    "Expenses\0expenseChangedSignal\0\0"
    "oldExpense\0newExpense\0onExpenseNameChangedSlot\0"
    "newName\0onExpenseDescriptionChangedSlot\0"
    "newDescription\0expenseSBChangedSlot\0"
    "change\0changedType\0updateExpense"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Expenses[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   49,    2, 0x0a,    4 /* Public */,
       7,    1,   52,    2, 0x0a,    6 /* Public */,
       9,    2,   55,    2, 0x0a,    8 /* Public */,
      12,    0,   60,    2, 0x0a,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Double, QMetaType::Char,   10,   11,
    QMetaType::Void,

       0        // eod
};

void Expenses::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Expenses *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->expenseChangedSignal((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 1: _t->onExpenseNameChangedSlot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->onExpenseDescriptionChangedSlot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->expenseSBChangedSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<char>>(_a[2]))); break;
        case 4: _t->updateExpense(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Expenses::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Expenses::expenseChangedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Expenses::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Expenses.offsetsAndSize,
    qt_meta_data_Expenses,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Expenses_t
, QtPrivate::TypeAndForceComplete<Expenses, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<char, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Expenses::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Expenses::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Expenses.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Expenses::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Expenses::expenseChangedSignal(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
