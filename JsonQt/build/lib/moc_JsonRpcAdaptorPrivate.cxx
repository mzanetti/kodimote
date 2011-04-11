/****************************************************************************
** Meta object code from reading C++ file 'JsonRpcAdaptorPrivate.h'
**
** Created: Sat Apr 9 01:22:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/JsonRpcAdaptorPrivate.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonRpcAdaptorPrivate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JsonQt__JsonRpcAdaptorPrivate[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   31,   30,   30, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   31,   30,   30, 0x0a,
      96,   75,   30,   30, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JsonQt__JsonRpcAdaptorPrivate[] = {
    "JsonQt::JsonRpcAdaptorPrivate\0\0json\0"
    "sendJson(QString)\0processJson(QString)\0"
    "id,method,parameters\0"
    "requestReceived(QVariant,QString,QVariant)\0"
};

const QMetaObject JsonQt::JsonRpcAdaptorPrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonQt__JsonRpcAdaptorPrivate,
      qt_meta_data_JsonQt__JsonRpcAdaptorPrivate, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonQt::JsonRpcAdaptorPrivate::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonQt::JsonRpcAdaptorPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonQt::JsonRpcAdaptorPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonQt__JsonRpcAdaptorPrivate))
        return static_cast<void*>(const_cast< JsonRpcAdaptorPrivate*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonQt::JsonRpcAdaptorPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: processJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: requestReceived((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void JsonQt::JsonRpcAdaptorPrivate::sendJson(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
