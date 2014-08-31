/****************************************************************************
** Meta object code from reading C++ file 'Dlg_BCs.h'
**
** Created: Mon Aug 25 22:50:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_BCs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_BCs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_BCs[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      38,   27,    8,    8, 0x05,
      59,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,    8,    8,    8, 0x0a,
      81,    8,    8,    8, 0x0a,
      92,    8,    8,    8, 0x0a,
     102,    8,    8,    8, 0x0a,
     119,  112,    8,    8, 0x0a,
     137,    8,    8,    8, 0x0a,
     152,   27,    8,    8, 0x0a,
     175,    8,    8,    8, 0x0a,
     191,    8,    8,    8, 0x0a,
     214,  208,    8,    8, 0x0a,
     234,  208,    8,    8, 0x0a,
     254,  208,    8,    8, 0x0a,
     274,  208,    8,    8, 0x0a,
     295,  208,    8,    8, 0x0a,
     316,  208,    8,    8, 0x0a,
     337,    8,    8,    8, 0x0a,
     352,    8,    8,    8, 0x0a,
     367,    8,    8,    8, 0x0a,
     382,    8,    8,    8, 0x0a,
     396,    8,    8,    8, 0x0a,
     410,    8,    8,    8, 0x0a,
     424,    8,    8,    8, 0x0a,
     441,    8,    8,    8, 0x0a,
     458,    8,    8,    8, 0x0a,
     475,    8,    8,    8, 0x0a,
     490,    8,    8,    8, 0x0a,
     505,    8,    8,    8, 0x0a,
     528,  520,    8,    8, 0x0a,
     560,  550,    8,    8, 0x0a,
     577,    8,    8,    8, 0x0a,
     597,    8,    8,    8, 0x0a,
     619,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_BCs[] = {
    "Dlg_BCs\0\0RequestUpdateGL()\0NewGLIndex\0"
    "RequestGLSelect(int)\0DoneEditing()\0"
    "AddBC()\0DelCurBC()\0SaveBCs()\0LoadBCs()\0"
    "NewRow\0BCrowChanged(int)\0BCRowClicked()\0"
    "ApplyExtSelection(int)\0ClickedFixAll()\0"
    "ClickedFixNone()\0State\0ChangedXFixed(bool)\0"
    "ChangedYFixed(bool)\0ChangedZFixed(bool)\0"
    "ChangedTXFixed(bool)\0ChangedTYFixed(bool)\0"
    "ChangedTZFixed(bool)\0EditedXForce()\0"
    "EditedYForce()\0EditedZForce()\0"
    "EditedXDisp()\0EditedYDisp()\0EditedZDisp()\0"
    "EditedTXTorque()\0EditedTYTorque()\0"
    "EditedTZTorque()\0EditedTXDisp()\0"
    "EditedTYDisp()\0EditedTZDisp()\0NewText\0"
    "ChangedTDisp(QString)\0NewPreset\0"
    "ApplyPreset(int)\0DoneButtonClicked()\0"
    "EditLocationClicked()\0UpdateBCLists()\0"
};

void Dlg_BCs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_BCs *_t = static_cast<Dlg_BCs *>(_o);
        switch (_id) {
        case 0: _t->RequestUpdateGL(); break;
        case 1: _t->RequestGLSelect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->DoneEditing(); break;
        case 3: _t->AddBC(); break;
        case 4: _t->DelCurBC(); break;
        case 5: _t->SaveBCs(); break;
        case 6: _t->LoadBCs(); break;
        case 7: _t->BCrowChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->BCRowClicked(); break;
        case 9: _t->ApplyExtSelection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->ClickedFixAll(); break;
        case 11: _t->ClickedFixNone(); break;
        case 12: _t->ChangedXFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->ChangedYFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->ChangedZFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->ChangedTXFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->ChangedTYFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->ChangedTZFixed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->EditedXForce(); break;
        case 19: _t->EditedYForce(); break;
        case 20: _t->EditedZForce(); break;
        case 21: _t->EditedXDisp(); break;
        case 22: _t->EditedYDisp(); break;
        case 23: _t->EditedZDisp(); break;
        case 24: _t->EditedTXTorque(); break;
        case 25: _t->EditedTYTorque(); break;
        case 26: _t->EditedTZTorque(); break;
        case 27: _t->EditedTXDisp(); break;
        case 28: _t->EditedTYDisp(); break;
        case 29: _t->EditedTZDisp(); break;
        case 30: _t->ChangedTDisp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->ApplyPreset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->DoneButtonClicked(); break;
        case 33: _t->EditLocationClicked(); break;
        case 34: _t->UpdateBCLists(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_BCs::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_BCs::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_BCs,
      qt_meta_data_Dlg_BCs, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_BCs::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_BCs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_BCs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_BCs))
        return static_cast<void*>(const_cast< Dlg_BCs*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_BCs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void Dlg_BCs::RequestUpdateGL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Dlg_BCs::RequestGLSelect(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Dlg_BCs::DoneEditing()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
