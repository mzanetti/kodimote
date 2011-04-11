/****************************************************************************
** Meta object code from reading C++ file 'JsonToVariant.cpp'
**
** Created: Sat Apr 9 01:22:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JsonToVariant.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JsonToVariant[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      30,   14,   14,   14, 0x08,
      66,   14,   14,   14, 0x08,
      78,   14,   14,   14, 0x08,
      90,   14,   14,   14, 0x08,
     103,   14,   14,   14, 0x08,
     113,   14,   14,   14, 0x08,
     126,   14,   14,   14, 0x08,
     140,   14,   14,   14, 0x08,
     155,   14,   14,   14, 0x08,
     167,   14,   14,   14, 0x08,
     180,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_JsonToVariant[] = {
    "JsonToVariant\0\0initTestCase()\0"
    "testStringWithEscapedForwardSlash()\0"
    "testMulti()\0testEmpty()\0testString()\0"
    "testInt()\0testBigNum()\0testLongNum()\0"
    "testFraction()\0testArray()\0testObject()\0"
    "testEmptyObject()\0"
};

const QMetaObject JsonToVariant::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_JsonToVariant,
      qt_meta_data_JsonToVariant, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JsonToVariant::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JsonToVariant::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JsonToVariant::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JsonToVariant))
        return static_cast<void*>(const_cast< JsonToVariant*>(this));
    return QObject::qt_metacast(_clname);
}

int JsonToVariant::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: initTestCase(); break;
        case 1: testStringWithEscapedForwardSlash(); break;
        case 2: testMulti(); break;
        case 3: testEmpty(); break;
        case 4: testString(); break;
        case 5: testInt(); break;
        case 6: testBigNum(); break;
        case 7: testLongNum(); break;
        case 8: testFraction(); break;
        case 9: testArray(); break;
        case 10: testObject(); break;
        case 11: testEmptyObject(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
