/****************************************************************************
** Meta object code from reading C++ file 'oauth2.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "oauth2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oauth2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OAuth2[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      20,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OAuth2[] = {
    "OAuth2\0\0loginDone()\0accessTokenObtained()\0"
};

void OAuth2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OAuth2 *_t = static_cast<OAuth2 *>(_o);
        switch (_id) {
        case 0: _t->loginDone(); break;
        case 1: _t->accessTokenObtained(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OAuth2::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OAuth2::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OAuth2,
      qt_meta_data_OAuth2, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OAuth2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OAuth2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OAuth2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OAuth2))
        return static_cast<void*>(const_cast< OAuth2*>(this));
    return QObject::qt_metacast(_clname);
}

int OAuth2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void OAuth2::loginDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
