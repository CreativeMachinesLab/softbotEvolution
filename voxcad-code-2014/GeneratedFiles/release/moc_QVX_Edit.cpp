/****************************************************************************
** Meta object code from reading C++ file 'QVX_Edit.h'
**
** Created: Mon Aug 25 22:50:32 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/QVX_Edit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QVX_Edit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQDM_Edit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      38,   29,   10,   10, 0x05,
      66,   59,   10,   10, 0x05,
      89,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,  104,   10,   10, 0x0a,
     133,   10,   10,   10, 0x2a,
     149,  104,   10,   10, 0x0a,
     169,   10,   10,   10, 0x2a,
     187,  185,   10,   10, 0x0a,
     219,  206,   10,   10, 0x0a,
     244,  185,   10,   10, 0x0a,
     262,  185,   10,   10, 0x0a,
     286,   10,   10,   10, 0x0a,
     311,  302,   10,   10, 0x0a,
     331,   10,   10,   10, 0x0a,
     343,   10,   10,   10, 0x0a,
     358,   10,   10,   10, 0x0a,
     371,   10,   10,   10, 0x0a,
     385,   10,   10,   10, 0x0a,
     399,   10,   10,   10, 0x0a,
     412,   10,   10,   10, 0x0a,
     424,   10,   10,   10, 0x0a,
     439,   10,   10,   10, 0x0a,
     455,   10,   10,   10, 0x0a,
     474,   10,   10,   10, 0x0a,
     490,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CQDM_Edit[] = {
    "CQDM_Edit\0\0UpdateGLWindows()\0MatIndex\0"
    "GetCurMaterial(int*)\0CurSel\0"
    "GetCurGLSelected(int*)\0ModelChanged()\0"
    "FastMode\0DrawSceneView(bool)\0"
    "DrawSceneView()\0DrawSceneEdit(bool)\0"
    "DrawSceneEdit()\0P\0HoverMove(Vec3D<>)\0"
    "P,IsCtrlDown\0LMouseDown(Vec3D<>,bool)\0"
    "LMouseUp(Vec3D<>)\0LMouseDownMove(Vec3D<>)\0"
    "PressedEscape()\0Positive\0CtrlMouseRoll(bool)\0"
    "SetV2DTop()\0SetV2DBottom()\0SetV2DLeft()\0"
    "SetV2DRight()\0SetV2DFront()\0SetV2DBack()\0"
    "LayerBack()\0LayerForward()\0SetDrawPencil()\0"
    "SetDrawRectangle()\0SetDrawCircle()\0"
    "SetDrawBucket()\0"
};

void CQDM_Edit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQDM_Edit *_t = static_cast<CQDM_Edit *>(_o);
        switch (_id) {
        case 0: _t->UpdateGLWindows(); break;
        case 1: _t->GetCurMaterial((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 2: _t->GetCurGLSelected((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 3: _t->ModelChanged(); break;
        case 4: _t->DrawSceneView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->DrawSceneView(); break;
        case 6: _t->DrawSceneEdit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->DrawSceneEdit(); break;
        case 8: _t->HoverMove((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 9: _t->LMouseDown((*reinterpret_cast< Vec3D<>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->LMouseUp((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 11: _t->LMouseDownMove((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 12: _t->PressedEscape(); break;
        case 13: _t->CtrlMouseRoll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->SetV2DTop(); break;
        case 15: _t->SetV2DBottom(); break;
        case 16: _t->SetV2DLeft(); break;
        case 17: _t->SetV2DRight(); break;
        case 18: _t->SetV2DFront(); break;
        case 19: _t->SetV2DBack(); break;
        case 20: _t->LayerBack(); break;
        case 21: _t->LayerForward(); break;
        case 22: _t->SetDrawPencil(); break;
        case 23: _t->SetDrawRectangle(); break;
        case 24: _t->SetDrawCircle(); break;
        case 25: _t->SetDrawBucket(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQDM_Edit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQDM_Edit::staticMetaObject = {
    { &QVX_Object::staticMetaObject, qt_meta_stringdata_CQDM_Edit,
      qt_meta_data_CQDM_Edit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQDM_Edit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQDM_Edit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQDM_Edit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQDM_Edit))
        return static_cast<void*>(const_cast< CQDM_Edit*>(this));
    return QVX_Object::qt_metacast(_clname);
}

int CQDM_Edit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVX_Object::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void CQDM_Edit::UpdateGLWindows()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQDM_Edit::GetCurMaterial(int * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CQDM_Edit::GetCurGLSelected(int * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CQDM_Edit::ModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
