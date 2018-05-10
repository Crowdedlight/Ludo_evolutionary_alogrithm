/****************************************************************************
** Meta object code from reading C++ file 'game.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ludo/game.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_game_t {
    QByteArrayData data[21];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_game_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_game_t qt_meta_stringdata_game = {
    {
QT_MOC_LITERAL(0, 0, 4), // "game"
QT_MOC_LITERAL(1, 5, 13), // "player1_start"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 18), // "positions_and_dice"
QT_MOC_LITERAL(4, 39, 13), // "player2_start"
QT_MOC_LITERAL(5, 53, 13), // "player3_start"
QT_MOC_LITERAL(6, 67, 13), // "player4_start"
QT_MOC_LITERAL(7, 81, 11), // "player1_end"
QT_MOC_LITERAL(8, 93, 16), // "std::vector<int>"
QT_MOC_LITERAL(9, 110, 11), // "player2_end"
QT_MOC_LITERAL(10, 122, 11), // "player3_end"
QT_MOC_LITERAL(11, 134, 11), // "player4_end"
QT_MOC_LITERAL(12, 146, 15), // "update_graphics"
QT_MOC_LITERAL(13, 162, 9), // "set_color"
QT_MOC_LITERAL(14, 172, 15), // "set_dice_result"
QT_MOC_LITERAL(15, 188, 14), // "declare_winner"
QT_MOC_LITERAL(16, 203, 5), // "close"
QT_MOC_LITERAL(17, 209, 12), // "turnComplete"
QT_MOC_LITERAL(18, 222, 3), // "win"
QT_MOC_LITERAL(19, 226, 9), // "movePiece"
QT_MOC_LITERAL(20, 236, 14) // "relative_piece"

    },
    "game\0player1_start\0\0positions_and_dice\0"
    "player2_start\0player3_start\0player4_start\0"
    "player1_end\0std::vector<int>\0player2_end\0"
    "player3_end\0player4_end\0update_graphics\0"
    "set_color\0set_dice_result\0declare_winner\0"
    "close\0turnComplete\0win\0movePiece\0"
    "relative_piece"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_game[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,
       5,    1,   95,    2, 0x06 /* Public */,
       6,    1,   98,    2, 0x06 /* Public */,
       7,    1,  101,    2, 0x06 /* Public */,
       9,    1,  104,    2, 0x06 /* Public */,
      10,    1,  107,    2, 0x06 /* Public */,
      11,    1,  110,    2, 0x06 /* Public */,
      12,    1,  113,    2, 0x06 /* Public */,
      13,    1,  116,    2, 0x06 /* Public */,
      14,    1,  119,    2, 0x06 /* Public */,
      15,    1,  122,    2, 0x06 /* Public */,
      16,    0,  125,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  126,    2, 0x0a /* Public */,
      19,    1,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

void game::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        game *_t = static_cast<game *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->player1_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 1: _t->player2_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 2: _t->player3_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 3: _t->player4_start((*reinterpret_cast< positions_and_dice(*)>(_a[1]))); break;
        case 4: _t->player1_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 5: _t->player2_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 6: _t->player3_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 7: _t->player4_end((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 8: _t->update_graphics((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 9: _t->set_color((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->set_dice_result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->declare_winner((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->close(); break;
        case 13: _t->turnComplete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->movePiece((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (game::*_t)(positions_and_dice );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player1_start)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (game::*_t)(positions_and_dice );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player2_start)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (game::*_t)(positions_and_dice );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player3_start)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (game::*_t)(positions_and_dice );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player4_start)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (game::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player1_end)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (game::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player2_end)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (game::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player3_end)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (game::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::player4_end)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (game::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::update_graphics)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (game::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::set_color)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (game::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::set_dice_result)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (game::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::declare_winner)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (game::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&game::close)) {
                *result = 12;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject game::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_game.data,
      qt_meta_data_game,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *game::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *game::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_game.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void game::player1_start(positions_and_dice _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void game::player2_start(positions_and_dice _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void game::player3_start(positions_and_dice _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void game::player4_start(positions_and_dice _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void game::player1_end(std::vector<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void game::player2_end(std::vector<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void game::player3_end(std::vector<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void game::player4_end(std::vector<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void game::update_graphics(std::vector<int> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void game::set_color(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void game::set_dice_result(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void game::declare_winner(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void game::close()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
