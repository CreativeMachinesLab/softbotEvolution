/********************************************************************************
** Form generated from reading UI file 'vWorkSpace.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VWORKSPACE_H
#define UI_VWORKSPACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorkspaceDialog
{
public:
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *PresetWScombo;
    QLabel *Preset_Label;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *LatDimSpin;
    QLabel *LatDim_Label;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QSpinBox *XV_Spin;
    QLabel *XV_Label;
    QVBoxLayout *verticalLayout_2;
    QSpinBox *YV_Spin;
    QLabel *YV_Label;
    QVBoxLayout *verticalLayout;
    QSpinBox *ZV_Spin;
    QLabel *ZV_Label;
    QLabel *XCustom_Label;
    QTabWidget *CustomSettings;
    QWidget *LatticeTab;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QDoubleSpinBox *XDimAdj_Spin;
    QLabel *XDimAdj_Label;
    QHBoxLayout *horizontalLayout_5;
    QDoubleSpinBox *YDimAdj_Spin;
    QLabel *YDimAdj_Label;
    QHBoxLayout *horizontalLayout_6;
    QDoubleSpinBox *ZDimAdj_Spin;
    QLabel *ZDimAdj_Label;
    QHBoxLayout *horizontalLayout_7;
    QDoubleSpinBox *XLineOff_Spin;
    QLabel *XLineOff_Label;
    QHBoxLayout *horizontalLayout_8;
    QDoubleSpinBox *YLineOff_Spin;
    QLabel *YLineOff_Label;
    QHBoxLayout *horizontalLayout_9;
    QDoubleSpinBox *XLayerOff_Spin;
    QLabel *XLayerOff_Label;
    QHBoxLayout *horizontalLayout_10;
    QDoubleSpinBox *YLayerOff_Spin;
    QLabel *YLayerOff_Label;
    QSpacerItem *verticalSpacer_2;
    QWidget *VoxelTab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_11;
    QComboBox *VoxelCombo;
    QLabel *Voxel_Label;
    QHBoxLayout *horizontalLayout_12;
    QDoubleSpinBox *XSqueeze_Spin;
    QLabel *XSqueeze_Label;
    QHBoxLayout *horizontalLayout_13;
    QDoubleSpinBox *YSqueeze_Spin;
    QLabel *YSqueeze_Label;
    QHBoxLayout *horizontalLayout_14;
    QDoubleSpinBox *ZSqueeze_Spin;
    QLabel *ZSqueeze_Label;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *WorkspaceDialog)
    {
        if (WorkspaceDialog->objectName().isEmpty())
            WorkspaceDialog->setObjectName(QString::fromUtf8("WorkspaceDialog"));
        WorkspaceDialog->resize(189, 381);
        verticalLayout_6 = new QVBoxLayout(WorkspaceDialog);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        PresetWScombo = new QComboBox(WorkspaceDialog);
        PresetWScombo->setObjectName(QString::fromUtf8("PresetWScombo"));

        horizontalLayout_3->addWidget(PresetWScombo);

        Preset_Label = new QLabel(WorkspaceDialog);
        Preset_Label->setObjectName(QString::fromUtf8("Preset_Label"));

        horizontalLayout_3->addWidget(Preset_Label);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        LatDimSpin = new QDoubleSpinBox(WorkspaceDialog);
        LatDimSpin->setObjectName(QString::fromUtf8("LatDimSpin"));
        LatDimSpin->setMinimum(0.01);
        LatDimSpin->setMaximum(1000);

        horizontalLayout_2->addWidget(LatDimSpin);

        LatDim_Label = new QLabel(WorkspaceDialog);
        LatDim_Label->setObjectName(QString::fromUtf8("LatDim_Label"));

        horizontalLayout_2->addWidget(LatDim_Label);


        verticalLayout_6->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        XV_Spin = new QSpinBox(WorkspaceDialog);
        XV_Spin->setObjectName(QString::fromUtf8("XV_Spin"));
        XV_Spin->setMaximum(999);

        verticalLayout_3->addWidget(XV_Spin);

        XV_Label = new QLabel(WorkspaceDialog);
        XV_Label->setObjectName(QString::fromUtf8("XV_Label"));
        XV_Label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(XV_Label);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        YV_Spin = new QSpinBox(WorkspaceDialog);
        YV_Spin->setObjectName(QString::fromUtf8("YV_Spin"));
        YV_Spin->setMaximum(999);

        verticalLayout_2->addWidget(YV_Spin);

        YV_Label = new QLabel(WorkspaceDialog);
        YV_Label->setObjectName(QString::fromUtf8("YV_Label"));
        YV_Label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(YV_Label);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ZV_Spin = new QSpinBox(WorkspaceDialog);
        ZV_Spin->setObjectName(QString::fromUtf8("ZV_Spin"));
        ZV_Spin->setMaximum(999);

        verticalLayout->addWidget(ZV_Spin);

        ZV_Label = new QLabel(WorkspaceDialog);
        ZV_Label->setObjectName(QString::fromUtf8("ZV_Label"));
        ZV_Label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(ZV_Label);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_6->addLayout(horizontalLayout);

        XCustom_Label = new QLabel(WorkspaceDialog);
        XCustom_Label->setObjectName(QString::fromUtf8("XCustom_Label"));

        verticalLayout_6->addWidget(XCustom_Label);

        CustomSettings = new QTabWidget(WorkspaceDialog);
        CustomSettings->setObjectName(QString::fromUtf8("CustomSettings"));
        LatticeTab = new QWidget();
        LatticeTab->setObjectName(QString::fromUtf8("LatticeTab"));
        verticalLayout_4 = new QVBoxLayout(LatticeTab);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        XDimAdj_Spin = new QDoubleSpinBox(LatticeTab);
        XDimAdj_Spin->setObjectName(QString::fromUtf8("XDimAdj_Spin"));
        XDimAdj_Spin->setDecimals(4);
        XDimAdj_Spin->setMinimum(0.0001);
        XDimAdj_Spin->setMaximum(1000);
        XDimAdj_Spin->setSingleStep(0.01);

        horizontalLayout_4->addWidget(XDimAdj_Spin);

        XDimAdj_Label = new QLabel(LatticeTab);
        XDimAdj_Label->setObjectName(QString::fromUtf8("XDimAdj_Label"));

        horizontalLayout_4->addWidget(XDimAdj_Label);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        YDimAdj_Spin = new QDoubleSpinBox(LatticeTab);
        YDimAdj_Spin->setObjectName(QString::fromUtf8("YDimAdj_Spin"));
        YDimAdj_Spin->setDecimals(4);
        YDimAdj_Spin->setMinimum(0.0001);
        YDimAdj_Spin->setMaximum(1000);
        YDimAdj_Spin->setSingleStep(0.01);

        horizontalLayout_5->addWidget(YDimAdj_Spin);

        YDimAdj_Label = new QLabel(LatticeTab);
        YDimAdj_Label->setObjectName(QString::fromUtf8("YDimAdj_Label"));

        horizontalLayout_5->addWidget(YDimAdj_Label);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        ZDimAdj_Spin = new QDoubleSpinBox(LatticeTab);
        ZDimAdj_Spin->setObjectName(QString::fromUtf8("ZDimAdj_Spin"));
        ZDimAdj_Spin->setDecimals(4);
        ZDimAdj_Spin->setMinimum(0.0001);
        ZDimAdj_Spin->setMaximum(1000);
        ZDimAdj_Spin->setSingleStep(0.01);

        horizontalLayout_6->addWidget(ZDimAdj_Spin);

        ZDimAdj_Label = new QLabel(LatticeTab);
        ZDimAdj_Label->setObjectName(QString::fromUtf8("ZDimAdj_Label"));

        horizontalLayout_6->addWidget(ZDimAdj_Label);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        XLineOff_Spin = new QDoubleSpinBox(LatticeTab);
        XLineOff_Spin->setObjectName(QString::fromUtf8("XLineOff_Spin"));
        XLineOff_Spin->setDecimals(4);
        XLineOff_Spin->setMaximum(1);
        XLineOff_Spin->setSingleStep(0.01);

        horizontalLayout_7->addWidget(XLineOff_Spin);

        XLineOff_Label = new QLabel(LatticeTab);
        XLineOff_Label->setObjectName(QString::fromUtf8("XLineOff_Label"));

        horizontalLayout_7->addWidget(XLineOff_Label);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        YLineOff_Spin = new QDoubleSpinBox(LatticeTab);
        YLineOff_Spin->setObjectName(QString::fromUtf8("YLineOff_Spin"));
        YLineOff_Spin->setDecimals(4);
        YLineOff_Spin->setMaximum(1);
        YLineOff_Spin->setSingleStep(0.01);

        horizontalLayout_8->addWidget(YLineOff_Spin);

        YLineOff_Label = new QLabel(LatticeTab);
        YLineOff_Label->setObjectName(QString::fromUtf8("YLineOff_Label"));

        horizontalLayout_8->addWidget(YLineOff_Label);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        XLayerOff_Spin = new QDoubleSpinBox(LatticeTab);
        XLayerOff_Spin->setObjectName(QString::fromUtf8("XLayerOff_Spin"));
        XLayerOff_Spin->setDecimals(4);
        XLayerOff_Spin->setMaximum(1);
        XLayerOff_Spin->setSingleStep(0.01);

        horizontalLayout_9->addWidget(XLayerOff_Spin);

        XLayerOff_Label = new QLabel(LatticeTab);
        XLayerOff_Label->setObjectName(QString::fromUtf8("XLayerOff_Label"));

        horizontalLayout_9->addWidget(XLayerOff_Label);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        YLayerOff_Spin = new QDoubleSpinBox(LatticeTab);
        YLayerOff_Spin->setObjectName(QString::fromUtf8("YLayerOff_Spin"));
        YLayerOff_Spin->setDecimals(4);
        YLayerOff_Spin->setMaximum(1);
        YLayerOff_Spin->setSingleStep(0.01);

        horizontalLayout_10->addWidget(YLayerOff_Spin);

        YLayerOff_Label = new QLabel(LatticeTab);
        YLayerOff_Label->setObjectName(QString::fromUtf8("YLayerOff_Label"));

        horizontalLayout_10->addWidget(YLayerOff_Label);


        verticalLayout_4->addLayout(horizontalLayout_10);

        verticalSpacer_2 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        CustomSettings->addTab(LatticeTab, QString());
        VoxelTab = new QWidget();
        VoxelTab->setObjectName(QString::fromUtf8("VoxelTab"));
        verticalLayout_5 = new QVBoxLayout(VoxelTab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        VoxelCombo = new QComboBox(VoxelTab);
        VoxelCombo->setObjectName(QString::fromUtf8("VoxelCombo"));

        horizontalLayout_11->addWidget(VoxelCombo);

        Voxel_Label = new QLabel(VoxelTab);
        Voxel_Label->setObjectName(QString::fromUtf8("Voxel_Label"));

        horizontalLayout_11->addWidget(Voxel_Label);


        verticalLayout_5->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        XSqueeze_Spin = new QDoubleSpinBox(VoxelTab);
        XSqueeze_Spin->setObjectName(QString::fromUtf8("XSqueeze_Spin"));
        XSqueeze_Spin->setDecimals(4);
        XSqueeze_Spin->setMinimum(0.0001);
        XSqueeze_Spin->setMaximum(1000);
        XSqueeze_Spin->setSingleStep(0.01);

        horizontalLayout_12->addWidget(XSqueeze_Spin);

        XSqueeze_Label = new QLabel(VoxelTab);
        XSqueeze_Label->setObjectName(QString::fromUtf8("XSqueeze_Label"));

        horizontalLayout_12->addWidget(XSqueeze_Label);


        verticalLayout_5->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        YSqueeze_Spin = new QDoubleSpinBox(VoxelTab);
        YSqueeze_Spin->setObjectName(QString::fromUtf8("YSqueeze_Spin"));
        YSqueeze_Spin->setDecimals(4);
        YSqueeze_Spin->setMinimum(0.0001);
        YSqueeze_Spin->setMaximum(1000);
        YSqueeze_Spin->setSingleStep(0.01);

        horizontalLayout_13->addWidget(YSqueeze_Spin);

        YSqueeze_Label = new QLabel(VoxelTab);
        YSqueeze_Label->setObjectName(QString::fromUtf8("YSqueeze_Label"));

        horizontalLayout_13->addWidget(YSqueeze_Label);


        verticalLayout_5->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        ZSqueeze_Spin = new QDoubleSpinBox(VoxelTab);
        ZSqueeze_Spin->setObjectName(QString::fromUtf8("ZSqueeze_Spin"));
        ZSqueeze_Spin->setDecimals(4);
        ZSqueeze_Spin->setMinimum(0.0001);
        ZSqueeze_Spin->setMaximum(1000);
        ZSqueeze_Spin->setSingleStep(0.01);

        horizontalLayout_14->addWidget(ZSqueeze_Spin);

        ZSqueeze_Label = new QLabel(VoxelTab);
        ZSqueeze_Label->setObjectName(QString::fromUtf8("ZSqueeze_Label"));

        horizontalLayout_14->addWidget(ZSqueeze_Label);


        verticalLayout_5->addLayout(horizontalLayout_14);

        verticalSpacer = new QSpacerItem(20, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        CustomSettings->addTab(VoxelTab, QString());

        verticalLayout_6->addWidget(CustomSettings);


        retranslateUi(WorkspaceDialog);

        CustomSettings->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WorkspaceDialog);
    } // setupUi

    void retranslateUi(QWidget *WorkspaceDialog)
    {
        WorkspaceDialog->setWindowTitle(QApplication::translate("WorkspaceDialog", "Workspace", 0, QApplication::UnicodeUTF8));
        Preset_Label->setText(QApplication::translate("WorkspaceDialog", "Scheme", 0, QApplication::UnicodeUTF8));
        LatDim_Label->setText(QApplication::translate("WorkspaceDialog", "Lattice Dim (mm)", 0, QApplication::UnicodeUTF8));
        XV_Label->setText(QApplication::translate("WorkspaceDialog", "X Voxels", 0, QApplication::UnicodeUTF8));
        YV_Label->setText(QApplication::translate("WorkspaceDialog", "Y Voxels", 0, QApplication::UnicodeUTF8));
        ZV_Label->setText(QApplication::translate("WorkspaceDialog", "Z Voxels", 0, QApplication::UnicodeUTF8));
        XCustom_Label->setText(QApplication::translate("WorkspaceDialog", "Custom Settings:", 0, QApplication::UnicodeUTF8));
        XDimAdj_Label->setText(QApplication::translate("WorkspaceDialog", "X Dim Adjust", 0, QApplication::UnicodeUTF8));
        YDimAdj_Label->setText(QApplication::translate("WorkspaceDialog", "Y Dim Adjust", 0, QApplication::UnicodeUTF8));
        ZDimAdj_Label->setText(QApplication::translate("WorkspaceDialog", "Z Dim Adjust", 0, QApplication::UnicodeUTF8));
        XLineOff_Label->setText(QApplication::translate("WorkspaceDialog", "X Line Offset", 0, QApplication::UnicodeUTF8));
        YLineOff_Label->setText(QApplication::translate("WorkspaceDialog", "Y Line Offset", 0, QApplication::UnicodeUTF8));
        XLayerOff_Label->setText(QApplication::translate("WorkspaceDialog", "X Layer Offset", 0, QApplication::UnicodeUTF8));
        YLayerOff_Label->setText(QApplication::translate("WorkspaceDialog", "Y Layer Offset", 0, QApplication::UnicodeUTF8));
        CustomSettings->setTabText(CustomSettings->indexOf(LatticeTab), QApplication::translate("WorkspaceDialog", "Lattice", 0, QApplication::UnicodeUTF8));
        Voxel_Label->setText(QApplication::translate("WorkspaceDialog", "Voxel Shape", 0, QApplication::UnicodeUTF8));
        XSqueeze_Label->setText(QApplication::translate("WorkspaceDialog", "X Squeeze", 0, QApplication::UnicodeUTF8));
        YSqueeze_Label->setText(QApplication::translate("WorkspaceDialog", "YSqueeze", 0, QApplication::UnicodeUTF8));
        ZSqueeze_Label->setText(QApplication::translate("WorkspaceDialog", "Z Squeeze", 0, QApplication::UnicodeUTF8));
        CustomSettings->setTabText(CustomSettings->indexOf(VoxelTab), QApplication::translate("WorkspaceDialog", "Voxel Display", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WorkspaceDialog: public Ui_WorkspaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VWORKSPACE_H
