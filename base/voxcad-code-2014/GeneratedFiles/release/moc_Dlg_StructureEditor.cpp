/****************************************************************************
** Meta object code from reading C++ file 'Dlg_StructureEditor.h'
**
** Created: Mon Aug 25 22:50:43 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_StructureEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_StructureEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_StructureEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   35,   20,   20, 0x0a,
      56,   20,   20,   20, 0x0a,
      72,   20,   20,   20, 0x0a,
      85,   20,   20,   20, 0x0a,
     102,   20,   20,   20, 0x0a,
     121,   20,   20,   20, 0x0a,
     149,  143,   20,   20, 0x0a,
     176,  170,   20,   20, 0x0a,
     218,  205,   20,   20, 0x0a,
     253,  170,   20,   20, 0x0a,
     281,  170,   20,   20, 0x0a,
     315,   20,   20,   20, 0x0a,
     340,  331,   20,   20, 0x0a,
     360,   20,   20,   20, 0x0a,
     374,   20,   20,   20, 0x0a,
     385,   20,   20,   20, 0x0a,
     405,  400,   20,   20, 0x0a,
     423,   35,   20,   20, 0x0a,
     442,   35,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_StructureEditor[] = {
    "Dlg_StructureEditor\0\0DoneEditing()\0"
    "YN\0IsEditMode(bool*)\0ClickedPencil()\0"
    "ClickedBox()\0ClickedEllipse()\0"
    "ClickedLayerBack()\0ClickedLayerForward()\0"
    "State\0ClickedRefView(bool)\0X,Y,Z\0"
    "HoverMove(float,float,float)\0X,Y,Z,IsCtrl\0"
    "LMouseDown(float,float,float,bool)\0"
    "LMouseUp(float,float,float)\0"
    "LMouseDownMove(float,float,float)\0"
    "PressedEscape()\0Positive\0CtrlMouseRoll(bool)\0"
    "IniUpdateUI()\0UpdateUI()\0UpdateGLWins()\0"
    "pMat\0CurMaterial(int*)\0WantGLIndex(bool*)\0"
    "WantCoord3D(bool*)\0"
};

void Dlg_StructureEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_StructureEditor *_t = static_cast<Dlg_StructureEditor *>(_o);
        switch (_id) {
        case 0: _t->DoneEditing(); break;
        case 1: _t->IsEditMode((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 2: _t->ClickedPencil(); break;
        case 3: _t->ClickedBox(); break;
        case 4: _t->ClickedEllipse(); break;
        case 5: _t->ClickedLayerBack(); break;
        case 6: _t->ClickedLayerForward(); break;
        case 7: _t->ClickedRefView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->HoverMove((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 9: _t->LMouseDown((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 10: _t->LMouseUp((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 11: _t->LMouseDownMove((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 12: _t->PressedEscape(); break;
        case 13: _t->CtrlMouseRoll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->IniUpdateUI(); break;
        case 15: _t->UpdateUI(); break;
        case 16: _t->UpdateGLWins(); break;
        case 17: _t->CurMaterial((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 18: _t->WantGLIndex((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        case 19: _t->WantCoord3D((*reinterpret_cast< bool*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_StructureEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_StructureEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_StructureEditor,
      qt_meta_data_Dlg_StructureEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_StructureEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_StructureEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_StructureEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_StructureEditor))
        return static_cast<void*>(const_cast< Dlg_StructureEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_StructureEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Dlg_StructureEditor::DoneEditing()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
