/****************************************************************************
** Meta object code from reading C++ file 'Dlg_EditPrim.h'
**
** Created: Mon Aug 25 22:50:38 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_EditPrim.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_EditPrim.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_EditPrim[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   32,   13,   13, 0x0a,
      79,   70,   13,   13, 0x0a,
      96,   70,   13,   13, 0x0a,
     120,  113,   13,   13, 0x0a,
     134,  113,   13,   13, 0x0a,
     151,  113,   13,   13, 0x0a,
     165,  113,   13,   13, 0x0a,
     182,  113,   13,   13, 0x0a,
     196,  113,   13,   13, 0x0a,
     213,  113,   13,   13, 0x0a,
     228,  113,   13,   13, 0x0a,
     246,  113,   13,   13, 0x0a,
     261,  113,   13,   13, 0x0a,
     279,  113,   13,   13, 0x0a,
     294,  113,   13,   13, 0x0a,
     312,  113,   13,   13, 0x0a,
     328,  113,   13,   13, 0x0a,
     347,   13,   13,   13, 0x0a,
     365,   13,   13,   13, 0x0a,
     386,   13,   13,   13, 0x0a,
     409,   13,   13,   13, 0x0a,
     428,   13,   13,   13, 0x0a,
     446,   13,   13,   13, 0x0a,
     460,   13,   13,   13, 0x0a,
     474,   13,   13,   13, 0x0a,
     488,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_EditPrim[] = {
    "Dlg_EditPrim\0\0RequestUpdateGL()\0"
    "pNewRegion\0UpdateRegion(CVX_FRegion*)\0"
    "NewState\0ChangedSnap(int)\0ChangedLock(int)\0"
    "NewVal\0ChangedX(int)\0ChangedX(double)\0"
    "ChangedY(int)\0ChangedY(double)\0"
    "ChangedZ(int)\0ChangedZ(double)\0"
    "ChangedDX(int)\0ChangedDX(double)\0"
    "ChangedDY(int)\0ChangedDY(double)\0"
    "ChangedDZ(int)\0ChangedDZ(double)\0"
    "ChangedRad(int)\0ChangedRad(double)\0"
    "ClickedBoxRadio()\0ClickedSphereRadio()\0"
    "ClickedCylinderRadio()\0ClickedMeshRadio()\0"
    "ClickedLoadMesh()\0ClickedRotX()\0"
    "ClickedRotY()\0ClickedRotZ()\0UpdateUI()\0"
};

void Dlg_EditPrim::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_EditPrim *_t = static_cast<Dlg_EditPrim *>(_o);
        switch (_id) {
        case 0: _t->RequestUpdateGL(); break;
        case 1: _t->UpdateRegion((*reinterpret_cast< CVX_FRegion*(*)>(_a[1]))); break;
        case 2: _t->ChangedSnap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ChangedLock((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ChangedX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangedX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->ChangedY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ChangedY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->ChangedZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->ChangedZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->ChangedDX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->ChangedDX((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->ChangedDY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->ChangedDY((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->ChangedDZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->ChangedDZ((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->ChangedRad((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->ChangedRad((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 18: _t->ClickedBoxRadio(); break;
        case 19: _t->ClickedSphereRadio(); break;
        case 20: _t->ClickedCylinderRadio(); break;
        case 21: _t->ClickedMeshRadio(); break;
        case 22: _t->ClickedLoadMesh(); break;
        case 23: _t->ClickedRotX(); break;
        case 24: _t->ClickedRotY(); break;
        case 25: _t->ClickedRotZ(); break;
        case 26: _t->UpdateUI(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_EditPrim::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_EditPrim::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_EditPrim,
      qt_meta_data_Dlg_EditPrim, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_EditPrim::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_EditPrim::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_EditPrim::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_EditPrim))
        return static_cast<void*>(const_cast< Dlg_EditPrim*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_EditPrim::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void Dlg_EditPrim::RequestUpdateGL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
