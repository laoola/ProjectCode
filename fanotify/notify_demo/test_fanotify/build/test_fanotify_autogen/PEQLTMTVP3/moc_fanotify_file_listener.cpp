/****************************************************************************
** Meta object code from reading C++ file 'fanotify_file_listener.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../filelisten/fanotify_file_listener.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fanotify_file_listener.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Fanotify_t {
    QByteArrayData data[7];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fanotify_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fanotify_t qt_meta_stringdata_Fanotify = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Fanotify"
QT_MOC_LITERAL(1, 9, 12), // "operationDir"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "dir"
QT_MOC_LITERAL(4, 27, 15), // "operationHappen"
QT_MOC_LITERAL(5, 43, 27), // "FileListener::OperationNode"
QT_MOC_LITERAL(6, 71, 4) // "node"

    },
    "Fanotify\0operationDir\0\0dir\0operationHappen\0"
    "FileListener::OperationNode\0node"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fanotify[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void Fanotify::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Fanotify *_t = static_cast<Fanotify *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->operationDir((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->operationHappen((*reinterpret_cast< FileListener::OperationNode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Fanotify::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fanotify::operationDir)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Fanotify::*_t)(FileListener::OperationNode );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Fanotify::operationHappen)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Fanotify::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Fanotify.data,
      qt_meta_data_Fanotify,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Fanotify::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fanotify::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Fanotify.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Fanotify::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Fanotify::operationDir(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Fanotify::operationHappen(FileListener::OperationNode _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
