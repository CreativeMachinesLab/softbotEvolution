/****************************************************************************
** Meta object code from reading C++ file 'Dlg_3DBrush.h'
**
** Created: Mon Aug 25 22:50:36 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_3DBrush.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_3DBrush.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_3DBrush[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      31,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   12,   12,   12, 0x0a,
      57,   12,   12,   12, 0x0a,
      68,   12,   12,   12, 0x0a,
      80,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_3DBrush[] = {
    "Dlg_3DBrush\0\0RequestUpdateGL()\0"
    "DoneAdding()\0ApplyBrush()\0UpdateUI()\0"
    "DrawBrush()\0ClickedDone()\0"
};

void Dlg_3DBrush::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_3DBrush *_t = static_cast<Dlg_3DBrush *>(_o);
        switch (_id) {
        case 0: _t->RequestUpdateGL(); break;
        case 1: _t->DoneAdding(); break;
        case 2: _t->ApplyBrush(); break;
        case 3: _t->UpdateUI(); break;
        case 4: _t->DrawBrush(); break;
        case 5: _t->ClickedDone(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Dlg_3DBrush::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_3DBrush::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_3DBrush,
      qt_meta_data_Dlg_3DBrush, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_3DBrush::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_3DBrush::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_3DBrush::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_3DBrush))
        return static_cast<void*>(const_cast< Dlg_3DBrush*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_3DBrush::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Dlg_3DBrush::RequestUpdateGL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Dlg_3DBrush::DoneAdding()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
