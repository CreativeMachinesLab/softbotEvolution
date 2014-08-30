/********************************************************************************
** Form generated from reading UI file 'vPrimEdit.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VPRIMEDIT_H
#define UI_VPRIMEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrimEditDlg
{
public:
    QVBoxLayout *verticalLayout_9;
    QGroupBox *ShapeBox;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_7;
    QRadioButton *BoxRadio;
    QRadioButton *CylinderRadio;
    QRadioButton *SphereRadio;
    QVBoxLayout *verticalLayout_8;
    QRadioButton *MeshRadio;
    QPushButton *LoadMeshButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *PositionLabel;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *SnapCheckBox;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QSlider *XSlider;
    QSlider *YSlider;
    QSlider *ZSlider;
    QVBoxLayout *verticalLayout_5;
    QDoubleSpinBox *XSpin;
    QDoubleSpinBox *YSpin;
    QDoubleSpinBox *ZSpin;
    QVBoxLayout *verticalLayout_4;
    QLabel *XLabel;
    QLabel *YLabel;
    QLabel *ZLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *SizeLabel;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *LockCheckBox;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QSlider *DXSlider;
    QSlider *DYSlider;
    QSlider *DZSlider;
    QSlider *RSlider;
    QVBoxLayout *verticalLayout_2;
    QDoubleSpinBox *DXSpin;
    QDoubleSpinBox *DYSpin;
    QDoubleSpinBox *DZSpin;
    QDoubleSpinBox *RSpin;
    QVBoxLayout *verticalLayout;
    QLabel *DXLabel;
    QLabel *DYLabel;
    QLabel *DZLabel;
    QLabel *RLabel;
    QHBoxLayout *horizontalLayout_6;
    QLabel *RotateLabel;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QSpinBox *DegreeSpinBox;
    QLabel *AngleLabel;
    QHBoxLayout *horizontalLayout_8;
    QLabel *ApplyRotLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *RotXButton;
    QPushButton *RotYButton;
    QPushButton *RotZButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *PrimEditDlg)
    {
        if (PrimEditDlg->objectName().isEmpty())
            PrimEditDlg->setObjectName(QString::fromUtf8("PrimEditDlg"));
        PrimEditDlg->resize(165, 551);
        verticalLayout_9 = new QVBoxLayout(PrimEditDlg);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        ShapeBox = new QGroupBox(PrimEditDlg);
        ShapeBox->setObjectName(QString::fromUtf8("ShapeBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ShapeBox->sizePolicy().hasHeightForWidth());
        ShapeBox->setSizePolicy(sizePolicy);
        ShapeBox->setMaximumSize(QSize(16777215, 75));
        horizontalLayout_5 = new QHBoxLayout(ShapeBox);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(3, 0, 3, 3);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        BoxRadio = new QRadioButton(ShapeBox);
        BoxRadio->setObjectName(QString::fromUtf8("BoxRadio"));

        verticalLayout_7->addWidget(BoxRadio);

        CylinderRadio = new QRadioButton(ShapeBox);
        CylinderRadio->setObjectName(QString::fromUtf8("CylinderRadio"));

        verticalLayout_7->addWidget(CylinderRadio);

        SphereRadio = new QRadioButton(ShapeBox);
        SphereRadio->setObjectName(QString::fromUtf8("SphereRadio"));

        verticalLayout_7->addWidget(SphereRadio);


        horizontalLayout_5->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        MeshRadio = new QRadioButton(ShapeBox);
        MeshRadio->setObjectName(QString::fromUtf8("MeshRadio"));

        verticalLayout_8->addWidget(MeshRadio);

        LoadMeshButton = new QPushButton(ShapeBox);
        LoadMeshButton->setObjectName(QString::fromUtf8("LoadMeshButton"));
        LoadMeshButton->setMaximumSize(QSize(50, 16777215));

        verticalLayout_8->addWidget(LoadMeshButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);


        horizontalLayout_5->addLayout(verticalLayout_8);


        verticalLayout_9->addWidget(ShapeBox);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        PositionLabel = new QLabel(PrimEditDlg);
        PositionLabel->setObjectName(QString::fromUtf8("PositionLabel"));

        horizontalLayout_4->addWidget(PositionLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        SnapCheckBox = new QCheckBox(PrimEditDlg);
        SnapCheckBox->setObjectName(QString::fromUtf8("SnapCheckBox"));

        horizontalLayout_4->addWidget(SnapCheckBox);


        verticalLayout_9->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        XSlider = new QSlider(PrimEditDlg);
        XSlider->setObjectName(QString::fromUtf8("XSlider"));
        XSlider->setMaximum(100);
        XSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(XSlider);

        YSlider = new QSlider(PrimEditDlg);
        YSlider->setObjectName(QString::fromUtf8("YSlider"));
        YSlider->setMaximum(100);
        YSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(YSlider);

        ZSlider = new QSlider(PrimEditDlg);
        ZSlider->setObjectName(QString::fromUtf8("ZSlider"));
        ZSlider->setMaximum(100);
        ZSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(ZSlider);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        XSpin = new QDoubleSpinBox(PrimEditDlg);
        XSpin->setObjectName(QString::fromUtf8("XSpin"));
        XSpin->setMaximum(1);
        XSpin->setSingleStep(0.01);

        verticalLayout_5->addWidget(XSpin);

        YSpin = new QDoubleSpinBox(PrimEditDlg);
        YSpin->setObjectName(QString::fromUtf8("YSpin"));
        YSpin->setMaximum(1);
        YSpin->setSingleStep(0.01);

        verticalLayout_5->addWidget(YSpin);

        ZSpin = new QDoubleSpinBox(PrimEditDlg);
        ZSpin->setObjectName(QString::fromUtf8("ZSpin"));
        ZSpin->setMaximum(1);
        ZSpin->setSingleStep(0.01);

        verticalLayout_5->addWidget(ZSpin);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        XLabel = new QLabel(PrimEditDlg);
        XLabel->setObjectName(QString::fromUtf8("XLabel"));

        verticalLayout_4->addWidget(XLabel);

        YLabel = new QLabel(PrimEditDlg);
        YLabel->setObjectName(QString::fromUtf8("YLabel"));

        verticalLayout_4->addWidget(YLabel);

        ZLabel = new QLabel(PrimEditDlg);
        ZLabel->setObjectName(QString::fromUtf8("ZLabel"));

        verticalLayout_4->addWidget(ZLabel);


        horizontalLayout_2->addLayout(verticalLayout_4);


        verticalLayout_9->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        SizeLabel = new QLabel(PrimEditDlg);
        SizeLabel->setObjectName(QString::fromUtf8("SizeLabel"));

        horizontalLayout_3->addWidget(SizeLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        LockCheckBox = new QCheckBox(PrimEditDlg);
        LockCheckBox->setObjectName(QString::fromUtf8("LockCheckBox"));

        horizontalLayout_3->addWidget(LockCheckBox);


        verticalLayout_9->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        DXSlider = new QSlider(PrimEditDlg);
        DXSlider->setObjectName(QString::fromUtf8("DXSlider"));
        DXSlider->setMinimum(1);
        DXSlider->setMaximum(100);
        DXSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(DXSlider);

        DYSlider = new QSlider(PrimEditDlg);
        DYSlider->setObjectName(QString::fromUtf8("DYSlider"));
        DYSlider->setMinimum(1);
        DYSlider->setMaximum(100);
        DYSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(DYSlider);

        DZSlider = new QSlider(PrimEditDlg);
        DZSlider->setObjectName(QString::fromUtf8("DZSlider"));
        DZSlider->setMinimum(1);
        DZSlider->setMaximum(100);
        DZSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(DZSlider);

        RSlider = new QSlider(PrimEditDlg);
        RSlider->setObjectName(QString::fromUtf8("RSlider"));
        RSlider->setMinimum(1);
        RSlider->setMaximum(100);
        RSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(RSlider);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        DXSpin = new QDoubleSpinBox(PrimEditDlg);
        DXSpin->setObjectName(QString::fromUtf8("DXSpin"));
        DXSpin->setMaximum(1);
        DXSpin->setSingleStep(0.01);

        verticalLayout_2->addWidget(DXSpin);

        DYSpin = new QDoubleSpinBox(PrimEditDlg);
        DYSpin->setObjectName(QString::fromUtf8("DYSpin"));
        DYSpin->setMaximum(1);
        DYSpin->setSingleStep(0.01);

        verticalLayout_2->addWidget(DYSpin);

        DZSpin = new QDoubleSpinBox(PrimEditDlg);
        DZSpin->setObjectName(QString::fromUtf8("DZSpin"));
        DZSpin->setMaximum(1);
        DZSpin->setSingleStep(0.01);

        verticalLayout_2->addWidget(DZSpin);

        RSpin = new QDoubleSpinBox(PrimEditDlg);
        RSpin->setObjectName(QString::fromUtf8("RSpin"));
        RSpin->setMaximum(1);
        RSpin->setSingleStep(0.01);

        verticalLayout_2->addWidget(RSpin);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        DXLabel = new QLabel(PrimEditDlg);
        DXLabel->setObjectName(QString::fromUtf8("DXLabel"));

        verticalLayout->addWidget(DXLabel);

        DYLabel = new QLabel(PrimEditDlg);
        DYLabel->setObjectName(QString::fromUtf8("DYLabel"));

        verticalLayout->addWidget(DYLabel);

        DZLabel = new QLabel(PrimEditDlg);
        DZLabel->setObjectName(QString::fromUtf8("DZLabel"));

        verticalLayout->addWidget(DZLabel);

        RLabel = new QLabel(PrimEditDlg);
        RLabel->setObjectName(QString::fromUtf8("RLabel"));

        verticalLayout->addWidget(RLabel);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_9->addLayout(horizontalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        RotateLabel = new QLabel(PrimEditDlg);
        RotateLabel->setObjectName(QString::fromUtf8("RotateLabel"));

        horizontalLayout_6->addWidget(RotateLabel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_9->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        DegreeSpinBox = new QSpinBox(PrimEditDlg);
        DegreeSpinBox->setObjectName(QString::fromUtf8("DegreeSpinBox"));
        DegreeSpinBox->setMinimum(-359);
        DegreeSpinBox->setMaximum(359);
        DegreeSpinBox->setSingleStep(5);

        horizontalLayout_7->addWidget(DegreeSpinBox);

        AngleLabel = new QLabel(PrimEditDlg);
        AngleLabel->setObjectName(QString::fromUtf8("AngleLabel"));

        horizontalLayout_7->addWidget(AngleLabel);


        verticalLayout_9->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        ApplyRotLabel = new QLabel(PrimEditDlg);
        ApplyRotLabel->setObjectName(QString::fromUtf8("ApplyRotLabel"));

        horizontalLayout_8->addWidget(ApplyRotLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);

        RotXButton = new QPushButton(PrimEditDlg);
        RotXButton->setObjectName(QString::fromUtf8("RotXButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RotXButton->sizePolicy().hasHeightForWidth());
        RotXButton->setSizePolicy(sizePolicy1);
        RotXButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_8->addWidget(RotXButton);

        RotYButton = new QPushButton(PrimEditDlg);
        RotYButton->setObjectName(QString::fromUtf8("RotYButton"));
        RotYButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_8->addWidget(RotYButton);

        RotZButton = new QPushButton(PrimEditDlg);
        RotZButton->setObjectName(QString::fromUtf8("RotZButton"));
        RotZButton->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_8->addWidget(RotZButton);


        verticalLayout_9->addLayout(horizontalLayout_8);

        verticalSpacer = new QSpacerItem(20, 136, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);


        retranslateUi(PrimEditDlg);

        QMetaObject::connectSlotsByName(PrimEditDlg);
    } // setupUi

    void retranslateUi(QWidget *PrimEditDlg)
    {
        PrimEditDlg->setWindowTitle(QApplication::translate("PrimEditDlg", "Form", 0, QApplication::UnicodeUTF8));
        ShapeBox->setTitle(QApplication::translate("PrimEditDlg", "Shape", 0, QApplication::UnicodeUTF8));
        BoxRadio->setText(QApplication::translate("PrimEditDlg", "Box", 0, QApplication::UnicodeUTF8));
        CylinderRadio->setText(QApplication::translate("PrimEditDlg", "Cylinder", 0, QApplication::UnicodeUTF8));
        SphereRadio->setText(QApplication::translate("PrimEditDlg", "Sphere", 0, QApplication::UnicodeUTF8));
        MeshRadio->setText(QApplication::translate("PrimEditDlg", "Mesh", 0, QApplication::UnicodeUTF8));
        LoadMeshButton->setText(QApplication::translate("PrimEditDlg", "Load", 0, QApplication::UnicodeUTF8));
        PositionLabel->setText(QApplication::translate("PrimEditDlg", "Position", 0, QApplication::UnicodeUTF8));
        SnapCheckBox->setText(QApplication::translate("PrimEditDlg", "Snap", 0, QApplication::UnicodeUTF8));
        XLabel->setText(QApplication::translate("PrimEditDlg", "X (%)", 0, QApplication::UnicodeUTF8));
        YLabel->setText(QApplication::translate("PrimEditDlg", "Y (%)", 0, QApplication::UnicodeUTF8));
        ZLabel->setText(QApplication::translate("PrimEditDlg", "Z (%)", 0, QApplication::UnicodeUTF8));
        SizeLabel->setText(QApplication::translate("PrimEditDlg", "Size", 0, QApplication::UnicodeUTF8));
        LockCheckBox->setText(QApplication::translate("PrimEditDlg", "Lock Aspect", 0, QApplication::UnicodeUTF8));
        DXLabel->setText(QApplication::translate("PrimEditDlg", "dX (%)", 0, QApplication::UnicodeUTF8));
        DYLabel->setText(QApplication::translate("PrimEditDlg", "dY (%)", 0, QApplication::UnicodeUTF8));
        DZLabel->setText(QApplication::translate("PrimEditDlg", "dZ (%)", 0, QApplication::UnicodeUTF8));
        RLabel->setText(QApplication::translate("PrimEditDlg", "R (%)", 0, QApplication::UnicodeUTF8));
        RotateLabel->setText(QApplication::translate("PrimEditDlg", "Rotate", 0, QApplication::UnicodeUTF8));
        AngleLabel->setText(QApplication::translate("PrimEditDlg", "Deg", 0, QApplication::UnicodeUTF8));
        ApplyRotLabel->setText(QApplication::translate("PrimEditDlg", "Apply:", 0, QApplication::UnicodeUTF8));
        RotXButton->setText(QApplication::translate("PrimEditDlg", "X", 0, QApplication::UnicodeUTF8));
        RotYButton->setText(QApplication::translate("PrimEditDlg", "Y", 0, QApplication::UnicodeUTF8));
        RotZButton->setText(QApplication::translate("PrimEditDlg", "Z", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PrimEditDlg: public Ui_PrimEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VPRIMEDIT_H
