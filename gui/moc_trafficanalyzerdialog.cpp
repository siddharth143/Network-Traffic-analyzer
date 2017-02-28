/****************************************************************************
** Meta object code from reading C++ file 'trafficanalyzerdialog.h'
**
** Created: Fri May 27 17:32:02 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "trafficanalyzerdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trafficanalyzerdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TADialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TADialog[] = {
    "TADialog\0\0,\0onCellClicked(int,int)\0"
};

const QMetaObject TADialog::staticMetaObject = {
    { &MainWindow::staticMetaObject, qt_meta_stringdata_TADialog,
      qt_meta_data_TADialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TADialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TADialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TADialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TADialog))
        return static_cast<void*>(const_cast< TADialog*>(this));
    if (!strcmp(_clname, "PosixThread"))
        return static_cast< PosixThread*>(const_cast< TADialog*>(this));
    return MainWindow::qt_metacast(_clname);
}

int TADialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onCellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
