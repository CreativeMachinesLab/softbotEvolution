/****************************************************************************
** Meta object code from reading C++ file 'Dlg_Workspace.h'
**
** Created: Mon Aug 25 22:50:46 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_Workspace.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_Workspace.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_Workspace[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      33,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   14,   14,   14, 0x0a,
      59,   14,   14,   14, 0x0a,
      85,   73,   14,   14, 0x0a,
     114,  104,   14,   14, 0x0a,
     136,   14,   14,   14, 0x0a,
     151,   14,   14,   14, 0x0a,
     166,   14,   14,   14, 0x0a,
     192,  181,   14,   14, 0x0a,
     226,  215,   14,   14, 0x0a,
     260,  249,   14,   14, 0x0a,
     291,  283,   14,   14, 0x0a,
     319,  311,   14,   14, 0x0a,
     347,  339,   14,   14, 0x0a,
     375,  367,   14,   14, 0x0a,
     407,  395,   14,   14, 0x0a,
     440,  428,   14,   14, 0x0a,
     476,  464,   14,   14, 0x0a,
     512,  500,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_Workspace[] = {
    "Dlg_Workspace\0\0RequestUpdateGL()\0"
    "WSDimChanged()\0UpdateUI()\0IniUpdateUI()\0"
    "NewWSScheme\0ChangedScheme(int)\0NewLatDim\0"
    "ChangedLatDim(double)\0ChangedVXDim()\0"
    "ChangedVYDim()\0ChangedVZDim()\0NewXDimAdj\0"
    "ChangedXDimAdj(double)\0NewYDimAdj\0"
    "ChangedYDimAdj(double)\0NewZDimAdj\0"
    "ChangedZDimAdj(double)\0NewXLiO\0"
    "ChangedXLiO(double)\0NewYLiO\0"
    "ChangedYLiO(double)\0NewXLaO\0"
    "ChangedXLaO(double)\0NewYLaO\0"
    "ChangedYLaO(double)\0NewVoxShape\0"
    "ChangedVoxShape(int)\0NewXSqueeze\0"
    "ChangedXSqueeze(double)\0NewYSqueeze\0"
    "ChangedYSqueeze(double)\0NewZSqueeze\0"
    "ChangedZSqueeze(double)\0"
};

void Dlg_Workspace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_Workspace *_t = static_cast<Dlg_Workspace *>(_o);
        switch (_id) {
        case 0: _t->RequestUpdateGL(); break;
        case 1: _t->WSDimChanged(); break;
        case 2: _t->UpdateUI(); break;
        case 3: _t->IniUpdateUI(); break;
        case 4: _t->ChangedScheme((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangedLatDim((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->ChangedVXDim(); break;
        case 7: _t->ChangedVYDim(); break;
        case 8: _t->ChangedVZDim(); break;
        case 9: _t->ChangedXDimAdj((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->ChangedYDimAdj((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->ChangedZDimAdj((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->ChangedXLiO((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->ChangedYLiO((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->ChangedXLaO((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: _t->ChangedYLaO((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->ChangedVoxShape((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->ChangedXSqueeze((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 18: _t->ChangedYSqueeze((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: _t->ChangedZSqueeze((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_Workspace::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_Workspace::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_Workspace,
      qt_meta_data_Dlg_Workspace, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_Workspace::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_Workspace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_Workspace::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_Workspace))
        return static_cast<void*>(const_cast< Dlg_Workspace*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_Workspace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void Dlg_Workspace::RequestUpdateGL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Dlg_Workspace::WSDimChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
