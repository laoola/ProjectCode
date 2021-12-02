/****************************************************************************
** Meta object code from reading C++ file 'majorimageprocessingthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../majorimageprocessingthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'majorimageprocessingthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MajorImageProcessingThread_t {
    QByteArrayData data[6];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MajorImageProcessingThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MajorImageProcessingThread_t qt_meta_stringdata_MajorImageProcessingThread = {
    {
QT_MOC_LITERAL(0, 0, 26), // "MajorImageProcessingThread"
QT_MOC_LITERAL(1, 27, 24), // "SendMajorImageProcessing"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 5), // "image"
QT_MOC_LITERAL(4, 59, 6), // "result"
QT_MOC_LITERAL(5, 66, 18) // "SendImgaeFaceCheck"

    },
    "MajorImageProcessingThread\0"
    "SendMajorImageProcessing\0\0image\0result\0"
    "SendImgaeFaceCheck"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MajorImageProcessingThread[] = {

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
       1,    2,   24,    2, 0x06 /* Public */,
       5,    1,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QImage,    3,

       0        // eod
};

void MajorImageProcessingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MajorImageProcessingThread *_t = static_cast<MajorImageProcessingThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SendMajorImageProcessing((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->SendImgaeFaceCheck((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MajorImageProcessingThread::*_t)(QImage , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MajorImageProcessingThread::SendMajorImageProcessing)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MajorImageProcessingThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MajorImageProcessingThread::SendImgaeFaceCheck)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MajorImageProcessingThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MajorImageProcessingThread.data,
      qt_meta_data_MajorImageProcessingThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MajorImageProcessingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MajorImageProcessingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MajorImageProcessingThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MajorImageProcessingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MajorImageProcessingThread::SendMajorImageProcessing(QImage _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MajorImageProcessingThread::SendImgaeFaceCheck(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_FaceCheckThread_t {
    QByteArrayData data[5];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceCheckThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceCheckThread_t qt_meta_stringdata_FaceCheckThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FaceCheckThread"
QT_MOC_LITERAL(1, 16, 16), // "SendFaceCheckRlt"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "image"
QT_MOC_LITERAL(4, 40, 18) // "slot_setCheckImage"

    },
    "FaceCheckThread\0SendFaceCheckRlt\0\0"
    "image\0slot_setCheckImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceCheckThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QImage,    3,

       0        // eod
};

void FaceCheckThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FaceCheckThread *_t = static_cast<FaceCheckThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SendFaceCheckRlt((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->slot_setCheckImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FaceCheckThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceCheckThread::SendFaceCheckRlt)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject FaceCheckThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_FaceCheckThread.data,
      qt_meta_data_FaceCheckThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FaceCheckThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceCheckThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceCheckThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int FaceCheckThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void FaceCheckThread::SendFaceCheckRlt(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
