/****************************************************************************
** Meta object code from reading C++ file 'MouseEventPixmapItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/MouseEventPixmapItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MouseEventPixmapItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MouseEventRectItem_t {
    QByteArrayData data[9];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MouseEventRectItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MouseEventRectItem_t qt_meta_stringdata_MouseEventRectItem = {
    {
QT_MOC_LITERAL(0, 0, 18), // "MouseEventRectItem"
QT_MOC_LITERAL(1, 19, 7), // "clicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 25), // "QGraphicsSceneMouseEvent*"
QT_MOC_LITERAL(4, 54, 5), // "event"
QT_MOC_LITERAL(5, 60, 12), // "hoverEntered"
QT_MOC_LITERAL(6, 73, 25), // "QGraphicsSceneHoverEvent*"
QT_MOC_LITERAL(7, 99, 10), // "hoverMoved"
QT_MOC_LITERAL(8, 110, 9) // "hoverLeft"

    },
    "MouseEventRectItem\0clicked\0\0"
    "QGraphicsSceneMouseEvent*\0event\0"
    "hoverEntered\0QGraphicsSceneHoverEvent*\0"
    "hoverMoved\0hoverLeft"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MouseEventRectItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       7,    1,   40,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 6,    4,

       0        // eod
};

void MouseEventRectItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MouseEventRectItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->hoverEntered((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        case 2: _t->hoverMoved((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        case 3: _t->hoverLeft((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MouseEventRectItem::*)(QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventRectItem::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MouseEventRectItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventRectItem::hoverEntered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MouseEventRectItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventRectItem::hoverMoved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MouseEventRectItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventRectItem::hoverLeft)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MouseEventRectItem::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MouseEventRectItem.data,
    qt_meta_data_MouseEventRectItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MouseEventRectItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MouseEventRectItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MouseEventRectItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(this);
    return QObject::qt_metacast(_clname);
}

int MouseEventRectItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MouseEventRectItem::clicked(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MouseEventRectItem::hoverEntered(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MouseEventRectItem::hoverMoved(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MouseEventRectItem::hoverLeft(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_MouseEventPixmapItem_t {
    QByteArrayData data[9];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MouseEventPixmapItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MouseEventPixmapItem_t qt_meta_stringdata_MouseEventPixmapItem = {
    {
QT_MOC_LITERAL(0, 0, 20), // "MouseEventPixmapItem"
QT_MOC_LITERAL(1, 21, 7), // "clicked"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 25), // "QGraphicsSceneMouseEvent*"
QT_MOC_LITERAL(4, 56, 5), // "event"
QT_MOC_LITERAL(5, 62, 12), // "hoverEntered"
QT_MOC_LITERAL(6, 75, 25), // "QGraphicsSceneHoverEvent*"
QT_MOC_LITERAL(7, 101, 10), // "hoverMoved"
QT_MOC_LITERAL(8, 112, 9) // "hoverLeft"

    },
    "MouseEventPixmapItem\0clicked\0\0"
    "QGraphicsSceneMouseEvent*\0event\0"
    "hoverEntered\0QGraphicsSceneHoverEvent*\0"
    "hoverMoved\0hoverLeft"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MouseEventPixmapItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       7,    1,   40,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 6,    4,

       0        // eod
};

void MouseEventPixmapItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MouseEventPixmapItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->hoverEntered((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        case 2: _t->hoverMoved((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        case 3: _t->hoverLeft((*reinterpret_cast< QGraphicsSceneHoverEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MouseEventPixmapItem::*)(QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventPixmapItem::clicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MouseEventPixmapItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventPixmapItem::hoverEntered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MouseEventPixmapItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventPixmapItem::hoverMoved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MouseEventPixmapItem::*)(QGraphicsSceneHoverEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MouseEventPixmapItem::hoverLeft)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MouseEventPixmapItem::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MouseEventPixmapItem.data,
    qt_meta_data_MouseEventPixmapItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MouseEventPixmapItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MouseEventPixmapItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MouseEventPixmapItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(this);
    return QObject::qt_metacast(_clname);
}

int MouseEventPixmapItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MouseEventPixmapItem::clicked(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MouseEventPixmapItem::hoverEntered(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MouseEventPixmapItem::hoverMoved(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MouseEventPixmapItem::hoverLeft(QGraphicsSceneHoverEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_HoverChangedPixmapItem_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HoverChangedPixmapItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HoverChangedPixmapItem_t qt_meta_stringdata_HoverChangedPixmapItem = {
    {
QT_MOC_LITERAL(0, 0, 22) // "HoverChangedPixmapItem"

    },
    "HoverChangedPixmapItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HoverChangedPixmapItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void HoverChangedPixmapItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject HoverChangedPixmapItem::staticMetaObject = { {
    &MouseEventPixmapItem::staticMetaObject,
    qt_meta_stringdata_HoverChangedPixmapItem.data,
    qt_meta_data_HoverChangedPixmapItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HoverChangedPixmapItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HoverChangedPixmapItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HoverChangedPixmapItem.stringdata0))
        return static_cast<void*>(this);
    return MouseEventPixmapItem::qt_metacast(_clname);
}

int HoverChangedPixmapItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MouseEventPixmapItem::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_MoviePixmapItem_t {
    QByteArrayData data[10];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MoviePixmapItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MoviePixmapItem_t qt_meta_stringdata_MoviePixmapItem = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MoviePixmapItem"
QT_MOC_LITERAL(1, 16, 5), // "click"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 25), // "QGraphicsSceneMouseEvent*"
QT_MOC_LITERAL(4, 49, 5), // "event"
QT_MOC_LITERAL(5, 55, 11), // "loopStarted"
QT_MOC_LITERAL(6, 67, 8), // "finished"
QT_MOC_LITERAL(7, 76, 5), // "start"
QT_MOC_LITERAL(8, 82, 4), // "stop"
QT_MOC_LITERAL(9, 87, 5) // "reset"

    },
    "MoviePixmapItem\0click\0\0QGraphicsSceneMouseEvent*\0"
    "event\0loopStarted\0finished\0start\0stop\0"
    "reset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MoviePixmapItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,
       6,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   49,    2, 0x0a /* Public */,
       8,    0,   50,    2, 0x0a /* Public */,
       9,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MoviePixmapItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MoviePixmapItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->click((*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->loopStarted(); break;
        case 2: _t->finished(); break;
        case 3: _t->start(); break;
        case 4: _t->stop(); break;
        case 5: _t->reset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MoviePixmapItem::*)(QGraphicsSceneMouseEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoviePixmapItem::click)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MoviePixmapItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoviePixmapItem::loopStarted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MoviePixmapItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoviePixmapItem::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MoviePixmapItem::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MoviePixmapItem.data,
    qt_meta_data_MoviePixmapItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MoviePixmapItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MoviePixmapItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MoviePixmapItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(this);
    return QObject::qt_metacast(_clname);
}

int MoviePixmapItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MoviePixmapItem::click(QGraphicsSceneMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MoviePixmapItem::loopStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MoviePixmapItem::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
