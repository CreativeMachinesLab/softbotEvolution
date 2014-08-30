/****************************************************************************
** Meta object code from reading C++ file 'Dlg_Tensile.h'
**
** Created: Mon Aug 25 22:50:44 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_Tensile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_Tensile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_Tensile[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   12,   12,   12, 0x0a,
      46,   12,   12,   12, 0x0a,
      57,   12,   12,   12, 0x0a,
      76,   12,   12,   12, 0x0a,
      99,   12,   12,   12, 0x0a,
     122,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_Tensile[] = {
    "Dlg_Tensile\0\0DoneTensileTesting()\0"
    "StartTest()\0UpdateUI()\0ClickedFastRadio()\0"
    "ClickedBalancedRadio()\0ClickedAccurateRadio()\0"
    "ClickedManualRadio()\0"
};

void Dlg_Tensile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_Tensile *_t = static_cast<Dlg_Tensile *>(_o);
        switch (_id) {
        case 0: _t->DoneTensileTesting(); break;
        case 1: _t->StartTest(); break;
        case 2: _t->UpdateUI(); break;
        case 3: _t->ClickedFastRadio(); break;
        case 4: _t->ClickedBalancedRadio(); break;
        case 5: _t->ClickedAccurateRadio(); break;
        case 6: _t->ClickedManualRadio(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Dlg_Tensile::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_Tensile::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dlg_Tensile,
      qt_meta_data_Dlg_Tensile, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_Tensile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_Tensile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_Tensile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_Tensile))
        return static_cast<void*>(const_cast< Dlg_Tensile*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dlg_Tensile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Dlg_Tensile::DoneTensileTesting()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
