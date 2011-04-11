/****************************************************************************
** Meta object code from reading C++ file 'JsonRpcAdaptor.cpp'
**
** Created: Sat Apr 9 01:22:37 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonRpcAdaptor.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestObject[] = {

 // content:
       5,       // revision
       0,       // classname
       4,   14, // classinfo
       8,   22, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // classinfo: key, value
      20,   11,
      50,   36,
      73,   64,
      99,   92,

 // signals: signature, parameters, type, tag, flags
     123,  119,  118,  118, 0x05,
     150,  118,  118,  118, 0x05,
     174,  170,  118,  118, 0x05,
     231,  229,  118,  118, 0x05,

 // slots: signature, parameters, type, tag, flags
     267,  119,  118,  118, 0x0a,
     296,  118,  288,  118, 0x0a,
     329,  310,  118,  118, 0x0a,
     386,  378,  118,  118, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TestObject[] = {
    "TestObject\0TestName\0JsonQt-RPC-name\0"
    "urn:data:test\0JsonQt-RPC-id\0""9000.001\0"
    "JsonQt-RPC-version\0Ponies\0JsonQt-RPC-summary\0"
    "\0foo\0functionOneCalled(QString)\0"
    "functionTwoCalled()\0,,,\0"
    "functionThreeCalled(int,bool,QVariantList,QVariantMap)\0"
    ",\0functionFourCalled(QString,QString)\0"
    "functionOne(QString)\0QString\0functionTwo()\0"
    "one,two,three,four\0"
    "functionThree(int,bool,QVariantList,QVariantMap)\0"
    "foo,bar\0functionFour(QString,QString)\0"
};

const QMetaObject TestObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestObject,
      qt_meta_data_TestObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestObject))
        return static_cast<void*>(const_cast< TestObject*>(this));
    return QObject::qt_metacast(_clname);
}

int TestObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: functionOneCalled((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: functionTwoCalled(); break;
        case 2: functionThreeCalled((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QVariantList(*)>(_a[3])),(*reinterpret_cast< QVariantMap(*)>(_a[4]))); break;
        case 3: functionFourCalled((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: functionOne((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: { QString _r = functionTwo();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: functionThree((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QVariantList(*)>(_a[3])),(*reinterpret_cast< QVariantMap(*)>(_a[4]))); break;
        case 7: functionFour((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TestObject::functionOneCalled(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TestObject::functionTwoCalled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TestObject::functionThreeCalled(int _t1, bool _t2, QVariantList _t3, QVariantMap _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TestObject::functionFourCalled(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
static const uint qt_meta_data_JsonRpcAdaptor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      31,   15,   15,   15, 0x08,
      49,   15,   15,   15, 0x08,
      68,   15,   15,   15, 0x08,
      86,   15,   15,   15, 0x08,
     106,   15,   15,   15, 0x08,
     128,   15,   15,   15, 0x08,
     151,   15,   15,   15, 0x08,
     176,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JsonRpcAdaptor[] = {
    "JsonRpcAdaptor\0\0initTestCase()\0"
    "testFunctionOne()\0testFunctionFour()\0"
    "testFunctionTwo()\0testFunctionThree()\0"
    "testInvalidFunction()\0testBadArgumentCount()\0"
    "testWrongArgumentTypes()\0testIntropection()\0"
};

const QMetaObject JsonRpcAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonRpcAdaptor,
      qt_meta_data_JsonRpcAdaptor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonRpcAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonRpcAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonRpcAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonRpcAdaptor))
        return static_cast<void*>(const_cast< JsonRpcAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonRpcAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: initTestCase(); break;
        case 1: testFunctionOne(); break;
        case 2: testFunctionFour(); break;
        case 3: testFunctionTwo(); break;
        case 4: testFunctionThree(); break;
        case 5: testInvalidFunction(); break;
        case 6: testBadArgumentCount(); break;
        case 7: testWrongArgumentTypes(); break;
        case 8: testIntropection(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
