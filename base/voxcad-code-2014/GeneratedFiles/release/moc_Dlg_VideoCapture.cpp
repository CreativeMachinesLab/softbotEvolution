/****************************************************************************
** Meta object code from reading C++ file 'Dlg_VideoCapture.h'
**
** Created: Mon Aug 25 22:50:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VoxCad/Dlg_VideoCapture.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_VideoCapture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_VideoCapture[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      46,   17,   17,   17, 0x0a,
      68,   17,   17,   17, 0x0a,
      91,   17,   17,   17, 0x0a,
     114,   17,   17,   17, 0x0a,
     137,   17,   17,   17, 0x0a,
     160,   17,   17,   17, 0x0a,
     184,   17,   17,   17, 0x0a,
     208,   17,   17,   17, 0x0a,
     233,   17,   17,   17, 0x0a,
     256,   17,   17,   17, 0x0a,
     283,   17,   17,   17, 0x0a,
     309,   17,   17,   17, 0x0a,
     331,   17,   17,   17, 0x0a,
     364,  356,   17,   17, 0x0a,
     391,   17,   17,   17, 0x0a,
     415,   17,   17,   17, 0x0a,
     438,   17,   17,   17, 0x0a,
     461,   17,   17,   17, 0x0a,
     482,   17,   17,   17, 0x0a,
     506,   17,   17,   17, 0x0a,
     529,  356,   17,   17, 0x0a,
     556,   17,   17,   17, 0x0a,
     571,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_VideoCapture[] = {
    "Dlg_VideoCapture\0\0ClickedSelectFolderButton()\0"
    "WidthPixEditChanged()\0HeightPixEditChanged()\0"
    "Clickedr320x240Radio()\0Clickedr640x480Radio()\0"
    "Clickedr800x600Radio()\0Clickedr1024x768Radio()\0"
    "Clickedr1280x720Radio()\0"
    "Clickedr1920x1080Radio()\0"
    "OutputFpsEditChanged()\0"
    "OutputSpdFctrEditChanged()\0"
    "ClickedDisplayTimeRadio()\0"
    "ClickedSimTimeRadio()\0ClickedEveryFrameRadio()\0"
    "Checked\0ClickedAutoStopCheck(bool)\0"
    "ClickedTimeStepsRadio()\0TimeStepsEditChanged()\0"
    "ClickedSimTimeRadio2()\0SimTimeEditChanged()\0"
    "ClickedTempCycleRadio()\0TempCycleEditChanged()\0"
    "ClickedResetSimCheck(bool)\0ClickedBegin()\0"
    "ClickedCancel()\0"
};

void Dlg_VideoCapture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_VideoCapture *_t = static_cast<Dlg_VideoCapture *>(_o);
        switch (_id) {
        case 0: _t->ClickedSelectFolderButton(); break;
        case 1: _t->WidthPixEditChanged(); break;
        case 2: _t->HeightPixEditChanged(); break;
        case 3: _t->Clickedr320x240Radio(); break;
        case 4: _t->Clickedr640x480Radio(); break;
        case 5: _t->Clickedr800x600Radio(); break;
        case 6: _t->Clickedr1024x768Radio(); break;
        case 7: _t->Clickedr1280x720Radio(); break;
        case 8: _t->Clickedr1920x1080Radio(); break;
        case 9: _t->OutputFpsEditChanged(); break;
        case 10: _t->OutputSpdFctrEditChanged(); break;
        case 11: _t->ClickedDisplayTimeRadio(); break;
        case 12: _t->ClickedSimTimeRadio(); break;
        case 13: _t->ClickedEveryFrameRadio(); break;
        case 14: _t->ClickedAutoStopCheck((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->ClickedTimeStepsRadio(); break;
        case 16: _t->TimeStepsEditChanged(); break;
        case 17: _t->ClickedSimTimeRadio2(); break;
        case 18: _t->SimTimeEditChanged(); break;
        case 19: _t->ClickedTempCycleRadio(); break;
        case 20: _t->TempCycleEditChanged(); break;
        case 21: _t->ClickedResetSimCheck((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->ClickedBegin(); break;
        case 23: _t->ClickedCancel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Dlg_VideoCapture::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_VideoCapture::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dlg_VideoCapture,
      qt_meta_data_Dlg_VideoCapture, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_VideoCapture::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_VideoCapture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_VideoCapture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_VideoCapture))
        return static_cast<void*>(const_cast< Dlg_VideoCapture*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dlg_VideoCapture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
