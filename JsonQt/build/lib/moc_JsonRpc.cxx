/****************************************************************************
** Meta object code from reading C++ file 'JsonRpc.h'
**
** Created: Sat Apr 9 01:22:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/JsonRpc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonRpc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JsonQt__JsonRpc[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   44, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x05,
      58,   40,   16,   16, 0x05,
     118,   97,   16,   16, 0x05,
     171,  161,   16,   16, 0x05,
     233,  207,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     278,   17,   16,   16, 0x0a,

 // enums: name, flags, count, data
     299, 0x0,    7,   48,

 // enum data: key, value
     309, uint(JsonQt::JsonRpc::InvalidJson),
     321, uint(JsonQt::JsonRpc::InvalidJsonRpc),
     336, uint(JsonQt::JsonRpc::MethodNotFound),
     351, uint(JsonQt::JsonRpc::BadParameters),
     365, uint(JsonQt::JsonRpc::InternalError),
     379, uint(JsonQt::JsonRpc::ServerErrorLow),
     394, uint(JsonQt::JsonRpc::ServerErrorHigh),

       0        // eod
};

static const char qt_meta_stringdata_JsonQt__JsonRpc[] = {
    "JsonQt::JsonRpc\0\0json\0sendJson(QString)\0"
    "method,parameters\0"
    "notificationReceived(QString,QVariant)\0"
    "id,method,parameters\0"
    "requestReceived(QVariant,QString,QVariant)\0"
    "id,result\0responseReceived(QVariant,QVariant)\0"
    "id,errorCode,message,data\0"
    "errorReceived(QVariant,int,QString,QVariant)\0"
    "processJson(QString)\0ErrorCode\0"
    "InvalidJson\0InvalidJsonRpc\0MethodNotFound\0"
    "BadParameters\0InternalError\0ServerErrorLow\0"
    "ServerErrorHigh\0"
};

const QMetaObject JsonQt::JsonRpc::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonQt__JsonRpc,
      qt_meta_data_JsonQt__JsonRpc, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonQt::JsonRpc::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonQt::JsonRpc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonQt::JsonRpc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonQt__JsonRpc))
        return static_cast<void*>(const_cast< JsonRpc*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonQt::JsonRpc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: notificationReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 2: requestReceived((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3]))); break;
        case 3: responseReceived((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 4: errorReceived((*reinterpret_cast< const QVariant(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        case 5: processJson((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void JsonQt::JsonRpc::sendJson(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void JsonQt::JsonRpc::notificationReceived(const QString & _t1, const QVariant & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void JsonQt::JsonRpc::requestReceived(const QVariant & _t1, const QString & _t2, const QVariant & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void JsonQt::JsonRpc::responseReceived(const QVariant & _t1, const QVariant & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void JsonQt::JsonRpc::errorReceived(const QVariant & _t1, int _t2, const QString & _t3, const QVariant & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
