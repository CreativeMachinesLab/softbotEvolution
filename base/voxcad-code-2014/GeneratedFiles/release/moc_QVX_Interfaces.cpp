/****************************************************************************
** Meta object code from reading C++ file 'QVX_Interfaces.h'
**
** Created: Mon Aug 25 22:50:33 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/QVX_Interfaces.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QVX_Interfaces.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QVX_Object[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   12,   11,   11, 0x0a,
      48,   11,   11,   11, 0x0a,
      89,   54,   11,   11, 0x0a,
     132,  113,   11,   11, 0x2a,
     159,  147,   11,   11, 0x2a,
     169,   11,   11,   11, 0x2a,
     176,   11,   11,   11, 0x0a,
     200,  187,   11,   11, 0x0a,
     221,   11,   11,   11, 0x2a,
     234,   11,   11,   11, 0x0a,
     249,   11,   11,   11, 0x0a,
     271,   11,   11,   11, 0x0a,
     283,   11,   11,   11, 0x0a,
     295,   11,   11,   11, 0x0a,
     312,  187,  307,   11, 0x0a,
     327,   11,  307,   11, 0x2a,
     334,   11,   11,   11, 0x0a,
     342,   11,  307,   11, 0x0a,
     352,   11,  307,   11, 0x0a,
     370,  362,   11,   11, 0x0a,
     411,  397,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QVX_Object[] = {
    "QVX_Object\0\0pVec,pOff\0GetDim(Vec3D<>*,Vec3D<>*)\0"
    "New()\0Compression,NewLoc,pNewFilenameOut\0"
    "Save(int,bool,QString*)\0Compression,NewLoc\0"
    "Save(int,bool)\0Compression\0Save(int)\0"
    "Save()\0SaveZLib()\0pFilenameOut\0"
    "SaveAsZLib(QString*)\0SaveAsZLib()\0"
    "SaveAsBase64()\0SaveAsAsciiReadable()\0"
    "ExportSTL()\0ExportXYZ()\0ExportKV6()\0"
    "bool\0Open(QString*)\0Open()\0Close()\0"
    "OpenPal()\0SavePal()\0pString\0"
    "GetVXCInfoString(QString*)\0Index,pString\0"
    "GetVoxInfoString(int,QString*)\0"
};

void QVX_Object::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QVX_Object *_t = static_cast<QVX_Object *>(_o);
        switch (_id) {
        case 0: _t->GetDim((*reinterpret_cast< Vec3D<>*(*)>(_a[1])),(*reinterpret_cast< Vec3D<>*(*)>(_a[2]))); break;
        case 1: _t->New(); break;
        case 2: _t->Save((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString*(*)>(_a[3]))); break;
        case 3: _t->Save((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->Save((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->Save(); break;
        case 6: _t->SaveZLib(); break;
        case 7: _t->SaveAsZLib((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 8: _t->SaveAsZLib(); break;
        case 9: _t->SaveAsBase64(); break;
        case 10: _t->SaveAsAsciiReadable(); break;
        case 11: _t->ExportSTL(); break;
        case 12: _t->ExportXYZ(); break;
        case 13: _t->ExportKV6(); break;
        case 14: { bool _r = _t->Open((*reinterpret_cast< QString*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->Open();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: _t->Close(); break;
        case 17: { bool _r = _t->OpenPal();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->SavePal();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: _t->GetVXCInfoString((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 20: _t->GetVoxInfoString((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QVX_Object::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QVX_Object::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVX_Object,
      qt_meta_data_QVX_Object, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QVX_Object::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QVX_Object::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QVX_Object::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVX_Object))
        return static_cast<void*>(const_cast< QVX_Object*>(this));
    if (!strcmp(_clname, "CVX_Object"))
        return static_cast< CVX_Object*>(const_cast< QVX_Object*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVX_Object::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
static const uint qt_meta_data_QVX_FEA[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      34,   27,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,    8,    8,    8, 0x0a,
      84,   78,    8,    8, 0x0a,
     113,    8,    8,    8, 0x2a,
     134,    8,    8,    8, 0x0a,
     151,  146,    8,    8, 0x0a,
     174,    8,    8,    8, 0x0a,
     200,    8,    8,    8, 0x0a,
     219,    8,    8,    8, 0x0a,
     239,    8,    8,    8, 0x0a,
     269,  261,    8,    8, 0x0a,
     313,  296,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QVX_FEA[] = {
    "QVX_FEA\0\0SolveResult(bool)\0CurSel\0"
    "GetCurGLSelected(int*)\0RequestSolveSystem()\0"
    "Param\0ExecuteSolveSystem(QString*)\0"
    "ExecuteSolveSystem()\0DrawScene()\0View\0"
    "SetViewDisplaced(bool)\0SetViewModeDisplacement()\0"
    "SetViewModeForce()\0SetViewModeStrain()\0"
    "SetViewModeReaction()\0pString\0"
    "GetFEAInfoString(QString*)\0VoxIndex,pString\0"
    "GetFEAInfoString(int,QString*)\0"
};

void QVX_FEA::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QVX_FEA *_t = static_cast<QVX_FEA *>(_o);
        switch (_id) {
        case 0: _t->SolveResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->GetCurGLSelected((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 2: _t->RequestSolveSystem(); break;
        case 3: _t->ExecuteSolveSystem((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 4: _t->ExecuteSolveSystem(); break;
        case 5: _t->DrawScene(); break;
        case 6: _t->SetViewDisplaced((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->SetViewModeDisplacement(); break;
        case 8: _t->SetViewModeForce(); break;
        case 9: _t->SetViewModeStrain(); break;
        case 10: _t->SetViewModeReaction(); break;
        case 11: _t->GetFEAInfoString((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 12: _t->GetFEAInfoString((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QVX_FEA::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QVX_FEA::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVX_FEA,
      qt_meta_data_QVX_FEA, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QVX_FEA::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QVX_FEA::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QVX_FEA::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVX_FEA))
        return static_cast<void*>(const_cast< QVX_FEA*>(this));
    if (!strcmp(_clname, "CVX_FEA"))
        return static_cast< CVX_FEA*>(const_cast< QVX_FEA*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVX_FEA::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void QVX_FEA::SolveResult(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QVX_FEA::GetCurGLSelected(int * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_QVX_Environment[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   16,   30,   16, 0x0a,
      45,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QVX_Environment[] = {
    "QVX_Environment\0\0BCsChanged()\0bool\0"
    "OpenBCs()\0SaveBCs()\0"
};

void QVX_Environment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QVX_Environment *_t = static_cast<QVX_Environment *>(_o);
        switch (_id) {
        case 0: _t->BCsChanged(); break;
        case 1: { bool _r = _t->OpenBCs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: _t->SaveBCs(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QVX_Environment::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QVX_Environment::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVX_Environment,
      qt_meta_data_QVX_Environment, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QVX_Environment::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QVX_Environment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QVX_Environment::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVX_Environment))
        return static_cast<void*>(const_cast< QVX_Environment*>(this));
    if (!strcmp(_clname, "CVX_Environment"))
        return static_cast< CVX_Environment*>(const_cast< QVX_Environment*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVX_Environment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QVX_Environment::BCsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_QVX_Sim[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      33,   22,    8,    8, 0x05,
      60,   22,    8,    8, 0x05,
      96,   81,    8,    8, 0x05,
     124,  119,    8,    8, 0x05,
     148,    8,    8,    8, 0x05,
     168,    8,    8,    8, 0x05,
     196,    8,    8,    8, 0x05,
     210,    8,    8,    8, 0x05,
     224,    8,    8,    8, 0x05,
     250,    8,    8,    8, 0x05,
     277,    8,    8,    8, 0x05,
     302,  300,    8,    8, 0x05,
     326,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     355,  342,    8,    8, 0x0a,
     373,    8,    8,    8, 0x2a,
     401,  388,  383,    8, 0x0a,
     419,    8,  383,    8, 0x2a,
     429,    8,    8,    8, 0x0a,
     449,    8,    8,    8, 0x0a,
     479,  467,    8,    8, 0x0a,
     497,    8,    8,    8, 0x2a,
     507,    8,    8,    8, 0x0a,
     524,    8,    8,    8, 0x0a,
     533,    8,    8,    8, 0x0a,
     548,  544,    8,    8, 0x0a,
     582,  580,    8,    8, 0x0a,
     602,  580,    8,    8, 0x0a,
     620,  580,    8,    8, 0x0a,
     649,  644,    8,    8, 0x0a,
     669,  666,    8,    8, 0x0a,
     706,  696,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QVX_Sim[] = {
    "QVX_Sim\0\0BCsChanged()\0pIsVisible\0"
    "IsStatusTextVisible(bool*)\0"
    "IsPlotVisible(bool*)\0pStatsRequired\0"
    "GetPlotRqdStats(char*)\0time\0"
    "ReqAddPlotPoint(double)\0UpdateText(QString)\0"
    "SimEndedInternally(QString)\0ReqGLUpdate()\0"
    "ReqUiUpdate()\0ReqGLDrawingStatus(bool*)\0"
    "StartExternalGLUpdate(int)\0"
    "StopExternalGLUpdate()\0,\0"
    "ResizeGLWindow(int,int)\0ResetGLWindow()\0"
    "pFilenameOut\0SaveVXA(QString*)\0SaveVXA()\0"
    "bool\0pFileNameOut\0OpenVXA(QString*)\0"
    "OpenVXA()\0ExportDeformedSTL()\0"
    "RequestBeginSim()\0pSimMessage\0"
    "SimLoop(QString*)\0SimLoop()\0"
    "SimPauseToggle()\0EndSim()\0ResetSim()\0"
    "Msg\0CatchInternalSimEnding(QString)\0"
    "P\0LMouseDown(Vec3D<>)\0LMouseUp(Vec3D<>)\0"
    "LMouseDownMove(Vec3D<>)\0pCoM\0"
    "GetCoM(Vec3D<>*)\0YN\0WantFramesAutoSaved(bool*)\0"
    "pFilePath\0AutoSavePath(QString*)\0"
};

void QVX_Sim::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QVX_Sim *_t = static_cast<QVX_Sim *>(_o);
        switch (_id) {
        case 0: _t->BCsChanged(); break;
        case 1: _t->IsStatusTextVisible((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 2: _t->IsPlotVisible((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 3: _t->GetPlotRqdStats((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 4: _t->ReqAddPlotPoint((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->UpdateText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->SimEndedInternally((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->ReqGLUpdate(); break;
        case 8: _t->ReqUiUpdate(); break;
        case 9: _t->ReqGLDrawingStatus((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 10: _t->StartExternalGLUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->StopExternalGLUpdate(); break;
        case 12: _t->ResizeGLWindow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->ResetGLWindow(); break;
        case 14: _t->SaveVXA((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 15: _t->SaveVXA(); break;
        case 16: { bool _r = _t->OpenVXA((*reinterpret_cast< QString*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = _t->OpenVXA();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: _t->ExportDeformedSTL(); break;
        case 19: _t->RequestBeginSim(); break;
        case 20: _t->SimLoop((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 21: _t->SimLoop(); break;
        case 22: _t->SimPauseToggle(); break;
        case 23: _t->EndSim(); break;
        case 24: _t->ResetSim(); break;
        case 25: _t->CatchInternalSimEnding((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->LMouseDown((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 27: _t->LMouseUp((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 28: _t->LMouseDownMove((*reinterpret_cast< Vec3D<>(*)>(_a[1]))); break;
        case 29: _t->GetCoM((*reinterpret_cast< Vec3D<>*(*)>(_a[1]))); break;
        case 30: _t->WantFramesAutoSaved((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 31: _t->AutoSavePath((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QVX_Sim::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QVX_Sim::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QVX_Sim,
      qt_meta_data_QVX_Sim, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QVX_Sim::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QVX_Sim::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QVX_Sim::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QVX_Sim))
        return static_cast<void*>(const_cast< QVX_Sim*>(this));
    if (!strcmp(_clname, "CVX_Sim"))
        return static_cast< CVX_Sim*>(const_cast< QVX_Sim*>(this));
    return QWidget::qt_metacast(_clname);
}

int QVX_Sim::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void QVX_Sim::BCsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QVX_Sim::IsStatusTextVisible(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QVX_Sim::IsPlotVisible(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QVX_Sim::GetPlotRqdStats(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QVX_Sim::ReqAddPlotPoint(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QVX_Sim::UpdateText(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QVX_Sim::SimEndedInternally(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QVX_Sim::ReqGLUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void QVX_Sim::ReqUiUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void QVX_Sim::ReqGLDrawingStatus(bool * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QVX_Sim::StartExternalGLUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QVX_Sim::StopExternalGLUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void QVX_Sim::ResizeGLWindow(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void QVX_Sim::ResetGLWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}
QT_END_MOC_NAMESPACE
