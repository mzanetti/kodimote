/****************************************************************************
** Meta object code from reading C++ file 'JsonRpc.cpp'
**
** Created: Sat Apr 9 01:22:34 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonRpc.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JsonRpc[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      24,    8,    8,    8, 0x08,
      42,    8,    8,    8, 0x08,
      64,    8,    8,    8, 0x08,
      97,    8,    8,    8, 0x08,
     125,    8,    8,    8, 0x08,
     145,    8,    8,    8, 0x08,
     166,    8,    8,    8, 0x08,
     200,    8,    8,    8, 0x08,
     232,    8,    8,    8, 0x08,
     259,    8,    8,    8, 0x08,
     281,    8,    8,    8, 0x08,
     300,    8,    8,    8, 0x08,
     333,    8,    8,    8, 0x08,
     360,    8,    8,    8, 0x08,
     382,    8,    8,    8, 0x08,
     405,    8,    8,    8, 0x08,
     425,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JsonRpc[] = {
    "JsonRpc\0\0initTestCase()\0testInvalidJson()\0"
    "testEmptyJsonObject()\0"
    "testSimpleNotificationReceived()\0"
    "testSimpleRequestReceived()\0"
    "testErrorReceived()\0testJsonRpcVersion()\0"
    "testComplexNotificationReceived()\0"
    "testSendingSimpleNotification()\0"
    "testSendingSimpleRequest()\0"
    "testSendingResponse()\0testSendingError()\0"
    "testSendingComplexNotification()\0"
    "testNotificationBothEnds()\0"
    "testRequestBothEnds()\0testResponseBothEnds()\0"
    "testErrorBothEnds()\0testJsonErrorBothEnds()\0"
};

const QMetaObject JsonRpc::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonRpc,
      qt_meta_data_JsonRpc, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonRpc::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonRpc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonRpc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonRpc))
        return static_cast<void*>(const_cast< JsonRpc*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonRpc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: initTestCase(); break;
        case 1: testInvalidJson(); break;
        case 2: testEmptyJsonObject(); break;
        case 3: testSimpleNotificationReceived(); break;
        case 4: testSimpleRequestReceived(); break;
        case 5: testErrorReceived(); break;
        case 6: testJsonRpcVersion(); break;
        case 7: testComplexNotificationReceived(); break;
        case 8: testSendingSimpleNotification(); break;
        case 9: testSendingSimpleRequest(); break;
        case 10: testSendingResponse(); break;
        case 11: testSendingError(); break;
        case 12: testSendingComplexNotification(); break;
        case 13: testNotificationBothEnds(); break;
        case 14: testRequestBothEnds(); break;
        case 15: testResponseBothEnds(); break;
        case 16: testErrorBothEnds(); break;
        case 17: testJsonErrorBothEnds(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
