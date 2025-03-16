/****************************************************************************
** Meta object code from reading C++ file 'budgetpage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "budgetpage.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'budgetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
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
struct qt_meta_tag_ZN10BudgetPageE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10BudgetPageE = QtMocHelpers::stringData(
    "BudgetPage",
    "onBudgetChangeSlot",
    "",
    "budget",
    "onExpenseChangedSlot",
    "delta",
    "onBudgetPeriodTypeChangedSlot",
    "index",
    "onBudgetPeriodChangeSlot",
    "period",
    "calculateRemainingBudget",
    "createBudgetPeriodSelector",
    "createBudgetSelector",
    "createExpensesSubPage",
    "changeBudgetPage",
    "newExpense",
    "deleteExpense",
    "BudgetPageExpenses*",
    "toDelete",
    "updateBarGraph"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10BudgetPageE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   86,    2, 0x0a,    1 /* Public */,
       4,    1,   89,    2, 0x0a,    3 /* Public */,
       6,    1,   92,    2, 0x0a,    5 /* Public */,
       8,    2,   95,    2, 0x0a,    7 /* Public */,
      10,    0,  100,    2, 0x0a,   10 /* Public */,
      11,    0,  101,    2, 0x0a,   11 /* Public */,
      12,    0,  102,    2, 0x0a,   12 /* Public */,
      13,    0,  103,    2, 0x0a,   13 /* Public */,
      14,    0,  104,    2, 0x0a,   14 /* Public */,
      15,    0,  105,    2, 0x0a,   15 /* Public */,
      16,    1,  106,    2, 0x0a,   16 /* Public */,
      19,    0,  109,    2, 0x0a,   18 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Char,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject BudgetPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10BudgetPageE.offsetsAndSizes,
    qt_meta_data_ZN10BudgetPageE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10BudgetPageE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BudgetPage, std::true_type>,
        // method 'onBudgetChangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onExpenseChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onBudgetPeriodTypeChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onBudgetPeriodChangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        // method 'calculateRemainingBudget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createBudgetPeriodSelector'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createBudgetSelector'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createExpensesSubPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changeBudgetPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newExpense'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteExpense'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<BudgetPageExpenses *, std::false_type>,
        // method 'updateBarGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void BudgetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BudgetPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onBudgetChangeSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->onExpenseChangedSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->onBudgetPeriodTypeChangedSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->onBudgetPeriodChangeSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<char>>(_a[2]))); break;
        case 4: _t->calculateRemainingBudget(); break;
        case 5: _t->createBudgetPeriodSelector(); break;
        case 6: _t->createBudgetSelector(); break;
        case 7: _t->createExpensesSubPage(); break;
        case 8: _t->changeBudgetPage(); break;
        case 9: _t->newExpense(); break;
        case 10: _t->deleteExpense((*reinterpret_cast< std::add_pointer_t<BudgetPageExpenses*>>(_a[1]))); break;
        case 11: _t->updateBarGraph(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< BudgetPageExpenses* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *BudgetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BudgetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10BudgetPageE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BudgetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
