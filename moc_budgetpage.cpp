/****************************************************************************
** Meta object code from reading C++ file 'budgetpage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "budgetpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'budgetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BudgetPage_t {
    const uint offsetsAndSize[22];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_BudgetPage_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_BudgetPage_t qt_meta_stringdata_BudgetPage = {
    {
QT_MOC_LITERAL(0, 10), // "BudgetPage"
QT_MOC_LITERAL(11, 18), // "onBudgetChangeSlot"
QT_MOC_LITERAL(30, 0), // ""
QT_MOC_LITERAL(31, 6), // "budget"
QT_MOC_LITERAL(38, 20), // "onExpenseChangedSlot"
QT_MOC_LITERAL(59, 10), // "oldExpense"
QT_MOC_LITERAL(70, 10), // "newExpense"
QT_MOC_LITERAL(81, 25), // "onBudgetPeriodChangedSlot"
QT_MOC_LITERAL(107, 5), // "index"
QT_MOC_LITERAL(113, 10), // "addExpense"
QT_MOC_LITERAL(124, 19) // "updateTotalExpenses"

    },
    "BudgetPage\0onBudgetChangeSlot\0\0budget\0"
    "onExpenseChangedSlot\0oldExpense\0"
    "newExpense\0onBudgetPeriodChangedSlot\0"
    "index\0addExpense\0updateTotalExpenses"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BudgetPage[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x0a,    1 /* Public */,
       4,    2,   47,    2, 0x0a,    3 /* Public */,
       7,    1,   52,    2, 0x0a,    6 /* Public */,
       9,    0,   55,    2, 0x0a,    8 /* Public */,
      10,    2,   56,    2, 0x0a,    9 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    5,    6,

       0        // eod
};

void BudgetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BudgetPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onBudgetChangeSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->onExpenseChangedSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 2: _t->onBudgetPeriodChangedSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->addExpense(); break;
        case 4: _t->updateTotalExpenses((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject BudgetPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_BudgetPage.offsetsAndSize,
    qt_meta_data_BudgetPage,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_BudgetPage_t
, QtPrivate::TypeAndForceComplete<BudgetPage, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>


>,
    nullptr
} };


const QMetaObject *BudgetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BudgetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BudgetPage.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BudgetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
