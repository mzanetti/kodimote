/****************************************************************************
** Meta object code from reading C++ file 'JsonRpcAdaptor.h'
**
** Created: Sat Apr 9 01:22:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/JsonRpcAdaptor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonRpcAdaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JsonQt__JsonRpcAdaptor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   24,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_JsonQt__JsonRpcAdaptor[] = {
    "JsonQt::JsonRpcAdaptor\0\0json\0"
    "sendJson(QString)\0processJson(QString)\0"
};

const QMetaObject JsonQt::JsonRpcAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonQt__JsonRpcAdaptor,
      qt_meta_data_JsonQt__JsonRpcAdaptor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonQt::JsonRpcAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonQt::JsonRpcAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonQt::JsonRpcAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonQt__JsonRpcAdaptor))
        return static_cast<void*>(const_cast< JsonRpcAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonQt::JsonRpcAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: processJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void JsonQt::JsonRpcAdaptor::sendJson(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
