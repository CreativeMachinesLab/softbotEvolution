/****************************************************************************
** Meta object code from reading C++ file 'QOpenGL.h'
**
** Created: Mon Aug 25 22:50:46 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QTUtils/QOpenGL.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QOpenGL.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CQOpenGL[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   10,    9,    9, 0x05,
      56,   48,    9,    9, 0x05,
      89,   80,    9,    9, 0x05,
     102,    9,    9,    9, 0x05,
     128,  118,    9,    9, 0x05,
     158,  149,    9,    9, 0x05,
     183,  180,    9,    9, 0x05,
     202,  180,    9,    9, 0x05,
     221,  180,    9,    9, 0x05,
     257,  247,    9,    9, 0x05,
     293,  284,    9,    9, 0x05,
     319,  313,    9,    9, 0x05,
     353,  313,    9,    9, 0x05,
     403,  390,    9,    9, 0x05,
     438,  313,    9,    9, 0x25,
     468,  313,    9,    9, 0x05,
     496,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     512,    9,    9,    9, 0x0a,
     526,    9,    9,    9, 0x0a,
     554,  544,    9,    9, 0x0a,
     567,    9,    9,    9, 0x0a,
     580,    9,    9,    9, 0x0a,
     596,    9,    9,    9, 0x0a,
     611,    9,    9,    9, 0x0a,
     625,    9,    9,    9, 0x0a,
     640,    9,    9,    9, 0x0a,
     654,    9,    9,    9, 0x0a,
     682,  671,    9,    9, 0x0a,
     710,  700,    9,    9, 0x0a,
     734,  731,    9,    9, 0x0a,
     755,    9,    9,    9, 0x2a,
     773,    9,    9,    9, 0x0a,
     801,  790,    9,    9, 0x0a,
     827,  818,    9,    9, 0x0a,
     865,  847,    9,    9, 0x0a,
     900,  893,    9,    9, 0x2a,
     919,    9,    9,    9, 0x2a,
     934,    9,    9,    9, 0x0a,
     962,  953,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CQOpenGL[] = {
    "CQOpenGL\0\0pVec,pOff\0FindDims(Vec3D<>*,Vec3D<>*)\0"
    "pTarget\0FindCamTarget(Vec3D<>*)\0"
    "FastMode\0DrawGL(bool)\0DrawGLOverlay()\0"
    "NameIndex\0MousePressIndex(int)\0Entering\0"
    "FastModeChanged(bool)\0YN\0WantGLIndex(bool*)\0"
    "WantCoord3D(bool*)\0WantAutoSaveFrames(bool*)\0"
    "pFilePath\0GetFrameFilePath(QString*)\0"
    "Positive\0CtrlWheelRoll(bool)\0X,Y,Z\0"
    "MouseMoveHover(float,float,float)\0"
    "LMouseMovePressed(float,float,float)\0"
    "X,Y,Z,IsCtrl\0LMouseDown(float,float,float,bool)\0"
    "LMouseDown(float,float,float)\0"
    "LMouseUp(float,float,float)\0PressedEscape()\0"
    "ZoomExtents()\0ReqGLUpdateThis()\0"
    "ViewIndex\0SetView(int)\0SetViewTop()\0"
    "SetViewBottom()\0SetViewFront()\0"
    "SetViewBack()\0SetViewRight()\0SetViewLeft()\0"
    "SetViewCustom1()\0IsFastMode\0"
    "SetFastMode(bool)\0IsVisible\0"
    "SetAxesVisible(bool)\0ms\0StartAutoRedraw(int)\0"
    "StartAutoRedraw()\0StopAutoRedraw()\0"
    "pIsDrawing\0IsDrawing(bool*)\0entering\0"
    "EnterFastMode(bool)\0ReZoom,pTargetPos\0"
    "GLCenterView(bool,Vec3D<>*)\0ReZoom\0"
    "GLCenterView(bool)\0GLCenterView()\0"
    "GLSaveScreenShot()\0FilePath\0"
    "GlSaveScreenShot(QString)\0"
};

void CQOpenGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CQOpenGL *_t = static_cast<CQOpenGL *>(_o);
        switch (_id) {
        case 0: _t->FindDims((*reinterpret_cast< Vec3D<>*(*)>(_a[1])),(*reinterpret_cast< Vec3D<>*(*)>(_a[2]))); break;
        case 1: _t->FindCamTarget((*reinterpret_cast< Vec3D<>*(*)>(_a[1]))); break;
        case 2: _t->DrawGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->DrawGLOverlay(); break;
        case 4: _t->MousePressIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->FastModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->WantGLIndex((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 7: _t->WantCoord3D((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 8: _t->WantAutoSaveFrames((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 9: _t->GetFrameFilePath((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 10: _t->CtrlWheelRoll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->MouseMoveHover((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 12: _t->LMouseMovePressed((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 13: _t->LMouseDown((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 14: _t->LMouseDown((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 15: _t->LMouseUp((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 16: _t->PressedEscape(); break;
        case 17: _t->ZoomExtents(); break;
        case 18: _t->ReqGLUpdateThis(); break;
        case 19: _t->SetView((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->SetViewTop(); break;
        case 21: _t->SetViewBottom(); break;
        case 22: _t->SetViewFront(); break;
        case 23: _t->SetViewBack(); break;
        case 24: _t->SetViewRight(); break;
        case 25: _t->SetViewLeft(); break;
        case 26: _t->SetViewCustom1(); break;
        case 27: _t->SetFastMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->SetAxesVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->StartAutoRedraw((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->StartAutoRedraw(); break;
        case 31: _t->StopAutoRedraw(); break;
        case 32: _t->IsDrawing((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 33: _t->EnterFastMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->GLCenterView((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< Vec3D<>*(*)>(_a[2]))); break;
        case 35: _t->GLCenterView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->GLCenterView(); break;
        case 37: _t->GLSaveScreenShot(); break;
        case 38: _t->GlSaveScreenShot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CQOpenGL::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CQOpenGL::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_CQOpenGL,
      qt_meta_data_CQOpenGL, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CQOpenGL::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CQOpenGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CQOpenGL::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOpenGL))
        return static_cast<void*>(const_cast< CQOpenGL*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int CQOpenGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void CQOpenGL::FindDims(Vec3D<> * _t1, Vec3D<> * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQOpenGL::FindCamTarget(Vec3D<> * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CQOpenGL::DrawGL(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CQOpenGL::DrawGLOverlay()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CQOpenGL::MousePressIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CQOpenGL::FastModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CQOpenGL::WantGLIndex(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CQOpenGL::WantCoord3D(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CQOpenGL::WantAutoSaveFrames(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CQOpenGL::GetFrameFilePath(QString * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CQOpenGL::CtrlWheelRoll(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CQOpenGL::MouseMoveHover(float _t1, float _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CQOpenGL::LMouseMovePressed(float _t1, float _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CQOpenGL::LMouseDown(float _t1, float _t2, float _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 15
void CQOpenGL::LMouseUp(float _t1, float _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void CQOpenGL::PressedEscape()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}
QT_END_MOC_NAMESPACE
