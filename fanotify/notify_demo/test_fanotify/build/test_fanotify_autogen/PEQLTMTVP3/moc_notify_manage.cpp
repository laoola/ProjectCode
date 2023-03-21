/****************************************************************************
** Meta object code from reading C++ file 'notify_manage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../filelisten/notify_manage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'notify_manage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NotifyManage_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NotifyManage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NotifyManage_t qt_meta_stringdata_NotifyManage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "NotifyManage"
QT_MOC_LITERAL(1, 13, 11), // "fileChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "operationHappen"
QT_MOC_LITERAL(4, 42, 27), // "FileListener::OperationNode"
QT_MOC_LITERAL(5, 70, 4), // "node"
QT_MOC_LITERAL(6, 75, 11) // "processNode"

    },
    "NotifyManage\0fileChanged\0\0operationHappen\0"
    "FileListener::OperationNode\0node\0"
    "processNode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NotifyManage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   30,    2, 0x08 /* Private */,
       6,    0,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void NotifyManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NotifyManage *_t = static_cast<NotifyManage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileChanged(); break;
        case 1: _t->operationHappen((*reinterpret_cast< FileListener::OperationNode(*)>(_a[1]))); break;
        case 2: _t->processNode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (NotifyManage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NotifyManage::fileChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NotifyManage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NotifyManage.data,
      qt_meta_data_NotifyManage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *NotifyManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NotifyManage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NotifyManage.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NotifyManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NotifyManage::fileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
