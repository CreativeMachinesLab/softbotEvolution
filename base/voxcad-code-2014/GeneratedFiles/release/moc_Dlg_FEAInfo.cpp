/****************************************************************************
** Meta object code from reading C++ file 'Dlg_FEAInfo.h'
**
** Created: Mon Aug 25 22:50:39 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_FEAInfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_FEAInfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_FEAInfo[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      40,   31,   12,   12, 0x05,
      63,   58,   12,   12, 0x05,
     101,   90,   12,   12, 0x05,
     132,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     158,  148,   12,   12, 0x0a,
     182,  175,   12,   12, 0x0a,
     212,  205,   12,   12, 0x0a,
     239,  232,   12,   12, 0x0a,
     255,   12,   12,   12, 0x0a,
     267,   12,   12,   12, 0x0a,
     279,   12,   12,   12, 0x0a,
     291,   12,   12,   12, 0x0a,
     305,   12,   12,   12, 0x0a,
     316,   12,   12,   12, 0x0a,
     329,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_FEAInfo[] = {
    "Dlg_FEAInfo\0\0RequestUpdateGL()\0CurIndex\0"
    "GetCurIndex(int*)\0Info\0"
    "GetFEAInfoString(QString*)\0Index,Info\0"
    "GetFEAInfoString(int,QString*)\0"
    "DoneAnalyzing()\0NewPreset\0ApplyPreset(int)\0"
    "NewDef\0ChangedDeflection(int)\0NewSec\0"
    "ChangedSection(int)\0NewIso\0ChangedIso(int)\0"
    "SetDirToX()\0SetDirToY()\0SetDirToZ()\0"
    "SetDirToMax()\0UpdateUI()\0UpdateText()\0"
    "DoneButtonPressed()\0"
};

void Dlg_FEAInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_FEAInfo *_t = static_cast<Dlg_FEAInfo *>(_o);
        switch (_id) {
        case 0: _t->RequestUpdateGL(); break;
        case 1: _t->GetCurIndex((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 2: _t->GetFEAInfoString((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        case 3: _t->GetFEAInfoString((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2]))); break;
        case 4: _t->DoneAnalyzing(); break;
        case 5: _t->ApplyPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->ChangedDeflection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ChangedSection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->ChangedIso((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->SetDirToX(); break;
        case 10: _t->SetDirToY(); break;
        case 11: _t->SetDirToZ(); break;
        case 12: _t->SetDirToMax(); break;
        case 13: _t->UpdateUI(); break;
        case 14: _t->UpdateText(); break;
        case 15: _t->DoneButtonPressed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_FEAInfo::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_FEAInfo::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_FEAInfo,
      qt_meta_data_Dlg_FEAInfo, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_FEAInfo::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_FEAInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_FEAInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_FEAInfo))
        return static_cast<void*>(const_cast< Dlg_FEAInfo*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_FEAInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void Dlg_FEAInfo::RequestUpdateGL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Dlg_FEAInfo::GetCurIndex(int * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Dlg_FEAInfo::GetFEAInfoString(QString * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Dlg_FEAInfo::GetFEAInfoString(int _t1, QString * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Dlg_FEAInfo::DoneAnalyzing()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
