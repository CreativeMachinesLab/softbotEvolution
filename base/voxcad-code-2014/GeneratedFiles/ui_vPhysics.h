/********************************************************************************
** Form generated from reading UI file 'vPhysics.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VPHYSICS_H
#define UI_VPHYSICS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PhysicsDialog
{
public:
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *PauseButton;
    QPushButton *ResetButton;
    QPushButton *RecordButton;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *UseEquilibriumCheck;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_11;
    QComboBox *StopSelectCombo;
    QLineEdit *StopValueEdit;
    QLabel *StopValueLabel;
    QHBoxLayout *horizontalLayout_3;
    QSlider *dtSlider;
    QLabel *label;
    QLineEdit *dtEdit;
    QHBoxLayout *horizontalLayout_9;
    QSlider *GNDDampSlider;
    QLabel *label_9;
    QLineEdit *GNDDampEdit;
    QCheckBox *UseSelfColCheck;
    QHBoxLayout *horizontalLayout_7;
    QSlider *ColDampSlider;
    QLabel *label_7;
    QLineEdit *ColDampEdit;
    QTabWidget *PhysTabWidget;
    QWidget *View;
    QVBoxLayout *verticalLayout_11;
    QGroupBox *DisplayBox;
    QHBoxLayout *horizontalLayout_12;
    QRadioButton *DispDisableRadio;
    QRadioButton *DispVoxelsRadio;
    QRadioButton *DispConnRadio;
    QGroupBox *ViewOptionsGroup;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_9;
    QRadioButton *ViewDiscreteRadio;
    QRadioButton *ViewDeformedRadio;
    QRadioButton *ViewSmoothRadio;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *ForcesCheck;
    QCheckBox *LocalCoordCheck;
    QGroupBox *ColorGroup;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_10;
    QRadioButton *TypeRadio;
    QRadioButton *KineticERadio;
    QRadioButton *DisplacementRadio;
    QRadioButton *StateRadio;
    QVBoxLayout *verticalLayout_8;
    QRadioButton *StrainERadio;
    QRadioButton *StressRadio;
    QRadioButton *StrainRadio;
    QRadioButton *PressureRadio;
    QCheckBox *CoMCheck;
    QSpacerItem *verticalSpacer_2;
    QWidget *Environment;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *UseTempCheck;
    QHBoxLayout *horizontalLayout_4;
    QSlider *TempSlider;
    QLabel *label_5;
    QLineEdit *TempEdit;
    QCheckBox *VaryTempCheck;
    QHBoxLayout *horizontalLayout_8;
    QSlider *TempPerSlider;
    QLabel *label_8;
    QLineEdit *TempPerEdit;
    QCheckBox *UseGravCheck;
    QHBoxLayout *horizontalLayout_5;
    QSlider *GravSlider;
    QLabel *label_6;
    QLineEdit *GravEdit;
    QCheckBox *UseFloorCheck;
    QSpacerItem *verticalSpacer;
    QWidget *Trace;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QComboBox *VariableCombo;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QComboBox *DirectionCombo;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer;
    QCheckBox *LogEachCheck;
    QPushButton *SaveDataButton;
    QWidget *Output;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *OutText;
    QWidget *Other;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout;
    QSlider *BondDampSlider;
    QLabel *label_4;
    QLineEdit *BondDampEdit;
    QCheckBox *UseMaxVelLimitCheck;
    QHBoxLayout *horizontalLayout_13;
    QSlider *MaxVelLimitSlider;
    QLabel *label_10;
    QCheckBox *UseVolumeEffectsCheck;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *PhysicsDialog)
    {
        if (PhysicsDialog->objectName().isEmpty())
            PhysicsDialog->setObjectName(QString::fromUtf8("PhysicsDialog"));
        PhysicsDialog->resize(263, 708);
        verticalLayout_6 = new QVBoxLayout(PhysicsDialog);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        PauseButton = new QPushButton(PhysicsDialog);
        PauseButton->setObjectName(QString::fromUtf8("PauseButton"));
        PauseButton->setMinimumSize(QSize(0, 24));

        horizontalLayout_6->addWidget(PauseButton);

        ResetButton = new QPushButton(PhysicsDialog);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));
        ResetButton->setMinimumSize(QSize(0, 24));

        horizontalLayout_6->addWidget(ResetButton);

        RecordButton = new QPushButton(PhysicsDialog);
        RecordButton->setObjectName(QString::fromUtf8("RecordButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RecordButton->sizePolicy().hasHeightForWidth());
        RecordButton->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/Icons/Record.png"), QSize(), QIcon::Normal, QIcon::Off);
        RecordButton->setIcon(icon);
        RecordButton->setCheckable(true);

        horizontalLayout_6->addWidget(RecordButton);


        verticalLayout_6->addLayout(horizontalLayout_6);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        UseEquilibriumCheck = new QCheckBox(PhysicsDialog);
        UseEquilibriumCheck->setObjectName(QString::fromUtf8("UseEquilibriumCheck"));

        horizontalLayout_16->addWidget(UseEquilibriumCheck);


        verticalLayout_6->addLayout(horizontalLayout_16);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_11 = new QLabel(PhysicsDialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);
        label_11->setMinimumSize(QSize(0, 0));
        label_11->setMaximumSize(QSize(80, 16777215));
        label_11->setWordWrap(true);

        horizontalLayout_15->addWidget(label_11);

        StopSelectCombo = new QComboBox(PhysicsDialog);
        StopSelectCombo->setObjectName(QString::fromUtf8("StopSelectCombo"));

        horizontalLayout_15->addWidget(StopSelectCombo);

        StopValueEdit = new QLineEdit(PhysicsDialog);
        StopValueEdit->setObjectName(QString::fromUtf8("StopValueEdit"));
        StopValueEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(StopValueEdit->sizePolicy().hasHeightForWidth());
        StopValueEdit->setSizePolicy(sizePolicy);
        StopValueEdit->setMinimumSize(QSize(41, 0));
        StopValueEdit->setMaximumSize(QSize(41, 16777215));
        StopValueEdit->setAcceptDrops(false);

        horizontalLayout_15->addWidget(StopValueEdit);

        StopValueLabel = new QLabel(PhysicsDialog);
        StopValueLabel->setObjectName(QString::fromUtf8("StopValueLabel"));
        sizePolicy1.setHeightForWidth(StopValueLabel->sizePolicy().hasHeightForWidth());
        StopValueLabel->setSizePolicy(sizePolicy1);
        StopValueLabel->setMinimumSize(QSize(0, 0));
        StopValueLabel->setMaximumSize(QSize(80, 16777215));
        StopValueLabel->setWordWrap(true);

        horizontalLayout_15->addWidget(StopValueLabel);


        verticalLayout_6->addLayout(horizontalLayout_15);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        dtSlider = new QSlider(PhysicsDialog);
        dtSlider->setObjectName(QString::fromUtf8("dtSlider"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dtSlider->sizePolicy().hasHeightForWidth());
        dtSlider->setSizePolicy(sizePolicy2);
        dtSlider->setMinimumSize(QSize(120, 0));
        dtSlider->setMaximum(100);
        dtSlider->setSingleStep(1);
        dtSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(dtSlider);

        label = new QLabel(PhysicsDialog);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(70, 0));
        label->setMaximumSize(QSize(80, 16777215));
        label->setWordWrap(true);

        horizontalLayout_3->addWidget(label);

        dtEdit = new QLineEdit(PhysicsDialog);
        dtEdit->setObjectName(QString::fromUtf8("dtEdit"));
        dtEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(dtEdit->sizePolicy().hasHeightForWidth());
        dtEdit->setSizePolicy(sizePolicy);
        dtEdit->setMinimumSize(QSize(41, 0));
        dtEdit->setMaximumSize(QSize(41, 16777215));
        dtEdit->setAcceptDrops(false);

        horizontalLayout_3->addWidget(dtEdit);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        GNDDampSlider = new QSlider(PhysicsDialog);
        GNDDampSlider->setObjectName(QString::fromUtf8("GNDDampSlider"));
        sizePolicy2.setHeightForWidth(GNDDampSlider->sizePolicy().hasHeightForWidth());
        GNDDampSlider->setSizePolicy(sizePolicy2);
        GNDDampSlider->setMinimumSize(QSize(120, 0));
        GNDDampSlider->setMaximum(100);
        GNDDampSlider->setSingleStep(1);
        GNDDampSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(GNDDampSlider);

        label_9 = new QLabel(PhysicsDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(70, 0));
        label_9->setMaximumSize(QSize(80, 16777215));
        label_9->setWordWrap(true);
        label_9->setIndent(-2);

        horizontalLayout_9->addWidget(label_9);

        GNDDampEdit = new QLineEdit(PhysicsDialog);
        GNDDampEdit->setObjectName(QString::fromUtf8("GNDDampEdit"));
        GNDDampEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(GNDDampEdit->sizePolicy().hasHeightForWidth());
        GNDDampEdit->setSizePolicy(sizePolicy);
        GNDDampEdit->setMinimumSize(QSize(41, 0));
        GNDDampEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout_9->addWidget(GNDDampEdit);


        verticalLayout_6->addLayout(horizontalLayout_9);

        UseSelfColCheck = new QCheckBox(PhysicsDialog);
        UseSelfColCheck->setObjectName(QString::fromUtf8("UseSelfColCheck"));

        verticalLayout_6->addWidget(UseSelfColCheck);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        ColDampSlider = new QSlider(PhysicsDialog);
        ColDampSlider->setObjectName(QString::fromUtf8("ColDampSlider"));
        sizePolicy2.setHeightForWidth(ColDampSlider->sizePolicy().hasHeightForWidth());
        ColDampSlider->setSizePolicy(sizePolicy2);
        ColDampSlider->setMinimumSize(QSize(120, 0));
        ColDampSlider->setMaximum(100);
        ColDampSlider->setSingleStep(1);
        ColDampSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(ColDampSlider);

        label_7 = new QLabel(PhysicsDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(70, 0));
        label_7->setMaximumSize(QSize(70, 16777215));
        label_7->setWordWrap(true);
        label_7->setIndent(-2);

        horizontalLayout_7->addWidget(label_7);

        ColDampEdit = new QLineEdit(PhysicsDialog);
        ColDampEdit->setObjectName(QString::fromUtf8("ColDampEdit"));
        ColDampEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(ColDampEdit->sizePolicy().hasHeightForWidth());
        ColDampEdit->setSizePolicy(sizePolicy);
        ColDampEdit->setMinimumSize(QSize(41, 0));
        ColDampEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout_7->addWidget(ColDampEdit);


        verticalLayout_6->addLayout(horizontalLayout_7);

        PhysTabWidget = new QTabWidget(PhysicsDialog);
        PhysTabWidget->setObjectName(QString::fromUtf8("PhysTabWidget"));
        PhysTabWidget->setTabPosition(QTabWidget::North);
        View = new QWidget();
        View->setObjectName(QString::fromUtf8("View"));
        verticalLayout_11 = new QVBoxLayout(View);
        verticalLayout_11->setSpacing(2);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(-1, 3, -1, 3);
        DisplayBox = new QGroupBox(View);
        DisplayBox->setObjectName(QString::fromUtf8("DisplayBox"));
        horizontalLayout_12 = new QHBoxLayout(DisplayBox);
        horizontalLayout_12->setSpacing(2);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(-1, 3, -1, 3);
        DispDisableRadio = new QRadioButton(DisplayBox);
        DispDisableRadio->setObjectName(QString::fromUtf8("DispDisableRadio"));

        horizontalLayout_12->addWidget(DispDisableRadio);

        DispVoxelsRadio = new QRadioButton(DisplayBox);
        DispVoxelsRadio->setObjectName(QString::fromUtf8("DispVoxelsRadio"));

        horizontalLayout_12->addWidget(DispVoxelsRadio);

        DispConnRadio = new QRadioButton(DisplayBox);
        DispConnRadio->setObjectName(QString::fromUtf8("DispConnRadio"));

        horizontalLayout_12->addWidget(DispConnRadio);


        verticalLayout_11->addWidget(DisplayBox);

        ViewOptionsGroup = new QGroupBox(View);
        ViewOptionsGroup->setObjectName(QString::fromUtf8("ViewOptionsGroup"));
        horizontalLayout_14 = new QHBoxLayout(ViewOptionsGroup);
        horizontalLayout_14->setSpacing(2);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(-1, 3, -1, 3);
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        ViewDiscreteRadio = new QRadioButton(ViewOptionsGroup);
        ViewDiscreteRadio->setObjectName(QString::fromUtf8("ViewDiscreteRadio"));

        verticalLayout_9->addWidget(ViewDiscreteRadio);

        ViewDeformedRadio = new QRadioButton(ViewOptionsGroup);
        ViewDeformedRadio->setObjectName(QString::fromUtf8("ViewDeformedRadio"));

        verticalLayout_9->addWidget(ViewDeformedRadio);

        ViewSmoothRadio = new QRadioButton(ViewOptionsGroup);
        ViewSmoothRadio->setObjectName(QString::fromUtf8("ViewSmoothRadio"));

        verticalLayout_9->addWidget(ViewSmoothRadio);


        horizontalLayout_14->addLayout(verticalLayout_9);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        ForcesCheck = new QCheckBox(ViewOptionsGroup);
        ForcesCheck->setObjectName(QString::fromUtf8("ForcesCheck"));

        verticalLayout_7->addWidget(ForcesCheck);

        LocalCoordCheck = new QCheckBox(ViewOptionsGroup);
        LocalCoordCheck->setObjectName(QString::fromUtf8("LocalCoordCheck"));

        verticalLayout_7->addWidget(LocalCoordCheck);


        horizontalLayout_14->addLayout(verticalLayout_7);


        verticalLayout_11->addWidget(ViewOptionsGroup);

        ColorGroup = new QGroupBox(View);
        ColorGroup->setObjectName(QString::fromUtf8("ColorGroup"));
        horizontalLayout_11 = new QHBoxLayout(ColorGroup);
        horizontalLayout_11->setSpacing(2);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(-1, 3, -1, 3);
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        TypeRadio = new QRadioButton(ColorGroup);
        TypeRadio->setObjectName(QString::fromUtf8("TypeRadio"));

        verticalLayout_10->addWidget(TypeRadio);

        KineticERadio = new QRadioButton(ColorGroup);
        KineticERadio->setObjectName(QString::fromUtf8("KineticERadio"));

        verticalLayout_10->addWidget(KineticERadio);

        DisplacementRadio = new QRadioButton(ColorGroup);
        DisplacementRadio->setObjectName(QString::fromUtf8("DisplacementRadio"));

        verticalLayout_10->addWidget(DisplacementRadio);

        StateRadio = new QRadioButton(ColorGroup);
        StateRadio->setObjectName(QString::fromUtf8("StateRadio"));

        verticalLayout_10->addWidget(StateRadio);


        horizontalLayout_11->addLayout(verticalLayout_10);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(1);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        StrainERadio = new QRadioButton(ColorGroup);
        StrainERadio->setObjectName(QString::fromUtf8("StrainERadio"));

        verticalLayout_8->addWidget(StrainERadio);

        StressRadio = new QRadioButton(ColorGroup);
        StressRadio->setObjectName(QString::fromUtf8("StressRadio"));

        verticalLayout_8->addWidget(StressRadio);

        StrainRadio = new QRadioButton(ColorGroup);
        StrainRadio->setObjectName(QString::fromUtf8("StrainRadio"));

        verticalLayout_8->addWidget(StrainRadio);

        PressureRadio = new QRadioButton(ColorGroup);
        PressureRadio->setObjectName(QString::fromUtf8("PressureRadio"));

        verticalLayout_8->addWidget(PressureRadio);


        horizontalLayout_11->addLayout(verticalLayout_8);


        verticalLayout_11->addWidget(ColorGroup);

        CoMCheck = new QCheckBox(View);
        CoMCheck->setObjectName(QString::fromUtf8("CoMCheck"));

        verticalLayout_11->addWidget(CoMCheck);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_11->addItem(verticalSpacer_2);

        PhysTabWidget->addTab(View, QString());
        Environment = new QWidget();
        Environment->setObjectName(QString::fromUtf8("Environment"));
        verticalLayout_5 = new QVBoxLayout(Environment);
        verticalLayout_5->setSpacing(2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 3, -1, 3);
        UseTempCheck = new QCheckBox(Environment);
        UseTempCheck->setObjectName(QString::fromUtf8("UseTempCheck"));

        verticalLayout_5->addWidget(UseTempCheck);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        TempSlider = new QSlider(Environment);
        TempSlider->setObjectName(QString::fromUtf8("TempSlider"));
        TempSlider->setMaximum(100);
        TempSlider->setSingleStep(1);
        TempSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(TempSlider);

        label_5 = new QLabel(Environment);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        TempEdit = new QLineEdit(Environment);
        TempEdit->setObjectName(QString::fromUtf8("TempEdit"));
        TempEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(TempEdit->sizePolicy().hasHeightForWidth());
        TempEdit->setSizePolicy(sizePolicy);
        TempEdit->setMinimumSize(QSize(41, 0));
        TempEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout_4->addWidget(TempEdit);


        verticalLayout_5->addLayout(horizontalLayout_4);

        VaryTempCheck = new QCheckBox(Environment);
        VaryTempCheck->setObjectName(QString::fromUtf8("VaryTempCheck"));

        verticalLayout_5->addWidget(VaryTempCheck);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        TempPerSlider = new QSlider(Environment);
        TempPerSlider->setObjectName(QString::fromUtf8("TempPerSlider"));
        TempPerSlider->setMaximum(100);
        TempPerSlider->setSingleStep(1);
        TempPerSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(TempPerSlider);

        label_8 = new QLabel(Environment);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        TempPerEdit = new QLineEdit(Environment);
        TempPerEdit->setObjectName(QString::fromUtf8("TempPerEdit"));
        TempPerEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(TempPerEdit->sizePolicy().hasHeightForWidth());
        TempPerEdit->setSizePolicy(sizePolicy);
        TempPerEdit->setMinimumSize(QSize(41, 0));
        TempPerEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout_8->addWidget(TempPerEdit);


        verticalLayout_5->addLayout(horizontalLayout_8);

        UseGravCheck = new QCheckBox(Environment);
        UseGravCheck->setObjectName(QString::fromUtf8("UseGravCheck"));

        verticalLayout_5->addWidget(UseGravCheck);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        GravSlider = new QSlider(Environment);
        GravSlider->setObjectName(QString::fromUtf8("GravSlider"));
        GravSlider->setMaximum(100);
        GravSlider->setSingleStep(1);
        GravSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(GravSlider);

        label_6 = new QLabel(Environment);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_5->addWidget(label_6);

        GravEdit = new QLineEdit(Environment);
        GravEdit->setObjectName(QString::fromUtf8("GravEdit"));
        GravEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(GravEdit->sizePolicy().hasHeightForWidth());
        GravEdit->setSizePolicy(sizePolicy);
        GravEdit->setMinimumSize(QSize(41, 0));
        GravEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout_5->addWidget(GravEdit);


        verticalLayout_5->addLayout(horizontalLayout_5);

        UseFloorCheck = new QCheckBox(Environment);
        UseFloorCheck->setObjectName(QString::fromUtf8("UseFloorCheck"));

        verticalLayout_5->addWidget(UseFloorCheck);

        verticalSpacer = new QSpacerItem(20, 191, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        PhysTabWidget->addTab(Environment, QString());
        Trace = new QWidget();
        Trace->setObjectName(QString::fromUtf8("Trace"));
        verticalLayout_3 = new QVBoxLayout(Trace);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        VariableCombo = new QComboBox(Trace);
        VariableCombo->setObjectName(QString::fromUtf8("VariableCombo"));

        verticalLayout_2->addWidget(VariableCombo);

        label_2 = new QLabel(Trace);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        DirectionCombo = new QComboBox(Trace);
        DirectionCombo->setObjectName(QString::fromUtf8("DirectionCombo"));

        verticalLayout->addWidget(DirectionCombo);

        label_3 = new QLabel(Trace);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer);

        LogEachCheck = new QCheckBox(Trace);
        LogEachCheck->setObjectName(QString::fromUtf8("LogEachCheck"));

        horizontalLayout_10->addWidget(LogEachCheck);

        SaveDataButton = new QPushButton(Trace);
        SaveDataButton->setObjectName(QString::fromUtf8("SaveDataButton"));

        horizontalLayout_10->addWidget(SaveDataButton);


        verticalLayout_3->addLayout(horizontalLayout_10);

        PhysTabWidget->addTab(Trace, QString());
        Output = new QWidget();
        Output->setObjectName(QString::fromUtf8("Output"));
        verticalLayout_4 = new QVBoxLayout(Output);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        OutText = new QTextEdit(Output);
        OutText->setObjectName(QString::fromUtf8("OutText"));

        verticalLayout_4->addWidget(OutText);

        PhysTabWidget->addTab(Output, QString());
        Other = new QWidget();
        Other->setObjectName(QString::fromUtf8("Other"));
        verticalLayout_12 = new QVBoxLayout(Other);
        verticalLayout_12->setSpacing(1);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(6, 3, 6, 3);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        BondDampSlider = new QSlider(Other);
        BondDampSlider->setObjectName(QString::fromUtf8("BondDampSlider"));
        sizePolicy2.setHeightForWidth(BondDampSlider->sizePolicy().hasHeightForWidth());
        BondDampSlider->setSizePolicy(sizePolicy2);
        BondDampSlider->setMinimumSize(QSize(80, 0));
        BondDampSlider->setMaximum(100);
        BondDampSlider->setSingleStep(1);
        BondDampSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(BondDampSlider);

        label_4 = new QLabel(Other);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(50, 0));
        label_4->setMaximumSize(QSize(50, 16777215));
        label_4->setWordWrap(true);
        label_4->setIndent(-2);

        horizontalLayout->addWidget(label_4);

        BondDampEdit = new QLineEdit(Other);
        BondDampEdit->setObjectName(QString::fromUtf8("BondDampEdit"));
        BondDampEdit->setEnabled(true);
        sizePolicy.setHeightForWidth(BondDampEdit->sizePolicy().hasHeightForWidth());
        BondDampEdit->setSizePolicy(sizePolicy);
        BondDampEdit->setMinimumSize(QSize(41, 0));
        BondDampEdit->setMaximumSize(QSize(41, 16777215));

        horizontalLayout->addWidget(BondDampEdit);


        verticalLayout_12->addLayout(horizontalLayout);

        UseMaxVelLimitCheck = new QCheckBox(Other);
        UseMaxVelLimitCheck->setObjectName(QString::fromUtf8("UseMaxVelLimitCheck"));

        verticalLayout_12->addWidget(UseMaxVelLimitCheck);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        MaxVelLimitSlider = new QSlider(Other);
        MaxVelLimitSlider->setObjectName(QString::fromUtf8("MaxVelLimitSlider"));
        sizePolicy2.setHeightForWidth(MaxVelLimitSlider->sizePolicy().hasHeightForWidth());
        MaxVelLimitSlider->setSizePolicy(sizePolicy2);
        MaxVelLimitSlider->setMinimumSize(QSize(120, 0));
        MaxVelLimitSlider->setMaximum(100);
        MaxVelLimitSlider->setSingleStep(1);
        MaxVelLimitSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_13->addWidget(MaxVelLimitSlider);

        label_10 = new QLabel(Other);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(70, 0));
        label_10->setMaximumSize(QSize(70, 16777215));
        label_10->setWordWrap(true);
        label_10->setIndent(-2);

        horizontalLayout_13->addWidget(label_10);


        verticalLayout_12->addLayout(horizontalLayout_13);

        UseVolumeEffectsCheck = new QCheckBox(Other);
        UseVolumeEffectsCheck->setObjectName(QString::fromUtf8("UseVolumeEffectsCheck"));

        verticalLayout_12->addWidget(UseVolumeEffectsCheck);

        verticalSpacer_3 = new QSpacerItem(20, 363, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_3);

        PhysTabWidget->addTab(Other, QString());

        verticalLayout_6->addWidget(PhysTabWidget);


        retranslateUi(PhysicsDialog);

        PhysTabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(PhysicsDialog);
    } // setupUi

    void retranslateUi(QWidget *PhysicsDialog)
    {
        PhysicsDialog->setWindowTitle(QApplication::translate("PhysicsDialog", "Form", 0, QApplication::UnicodeUTF8));
        PauseButton->setText(QApplication::translate("PhysicsDialog", "Pause", 0, QApplication::UnicodeUTF8));
        ResetButton->setText(QApplication::translate("PhysicsDialog", "Reset", 0, QApplication::UnicodeUTF8));
        RecordButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        UseEquilibriumCheck->setToolTip(QApplication::translate("PhysicsDialog", "Accelerates the simulation to find the equilibrium position quickly", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseEquilibriumCheck->setText(QApplication::translate("PhysicsDialog", "Find Equilibrium", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_11->setToolTip(QApplication::translate("PhysicsDialog", "Percent of optimal timestep to run at. [0-1.0+]\n"
"Greater values speed up the simulation, but >1.0\n"
"can be unstable.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_11->setText(QApplication::translate("PhysicsDialog", "Stop condition", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        StopSelectCombo->setToolTip(QApplication::translate("PhysicsDialog", "Sets a condition for auto-stopping the simulation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        StopValueEdit->setToolTip(QApplication::translate("PhysicsDialog", "The value of selected auto-stopping condition to trigger a stop.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        StopValueLabel->setToolTip(QApplication::translate("PhysicsDialog", "Percent of optimal timestep to run at. [0-1.0+]\n"
"Greater values speed up the simulation, but >1.0\n"
"can be unstable.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        StopValueLabel->setText(QApplication::translate("PhysicsDialog", "Sec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dtSlider->setToolTip(QApplication::translate("PhysicsDialog", "Percent of optimal timestep to run at. [0-1.0+]\n"
"Greater values speed up the simulation, but >1.0\n"
"can be unstable.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("PhysicsDialog", "Percent of optimal timestep to run at. [0-1.0+]\n"
"Greater values speed up the simulation, but >1.0\n"
"can be unstable.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("PhysicsDialog", "timestep (%  optimal dt)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dtEdit->setToolTip(QApplication::translate("PhysicsDialog", "Percent of optimal timestep to run at. [0-1.0+]\n"
"Greater values speed up the simulation, but >1.0\n"
"can be unstable.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        GNDDampSlider->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio relative to ground [0-0.1+].\n"
"0 represents a vacuum, larger values\n"
"represent a more viscous fluid environment.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_9->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio relative to ground [0-0.1+].\n"
"0 represents a vacuum, larger values\n"
"represent a more viscous fluid environment.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_9->setText(QApplication::translate("PhysicsDialog", "Ground damp ratio (z)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        GNDDampEdit->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio relative to ground [0-0.1+].\n"
"0 represents a vacuum, larger values\n"
"represent a more viscous fluid environment.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        UseSelfColCheck->setToolTip(QApplication::translate("PhysicsDialog", "Prevents self penetration when enabled.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseSelfColCheck->setText(QApplication::translate("PhysicsDialog", "Enable Self Collision", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ColDampSlider->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio of collisions [0-2+].\n"
"Low values make elastic (bouncy) collisions, higher values\n"
"make inelastic (dead) collisions. 1.0 is critically damped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio of collisions [0-2+].\n"
"Low values make elastic (bouncy) collisions, higher values\n"
"make inelastic (dead) collisions. 1.0 is critically damped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("PhysicsDialog", "Collision damp ratio (z)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ColDampEdit->setToolTip(QApplication::translate("PhysicsDialog", "Damping ratio of collisions [0-2+].\n"
"Low values make elastic (bouncy) collisions, higher values\n"
"make inelastic (dead) collisions. 1.0 is critically damped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        PhysTabWidget->setToolTip(QApplication::translate("PhysicsDialog", "Options", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        DisplayBox->setTitle(QApplication::translate("PhysicsDialog", "Display", 0, QApplication::UnicodeUTF8));
        DispDisableRadio->setText(QApplication::translate("PhysicsDialog", "Disable", 0, QApplication::UnicodeUTF8));
        DispVoxelsRadio->setText(QApplication::translate("PhysicsDialog", "Voxels", 0, QApplication::UnicodeUTF8));
        DispConnRadio->setText(QApplication::translate("PhysicsDialog", "Connections", 0, QApplication::UnicodeUTF8));
        ViewOptionsGroup->setTitle(QApplication::translate("PhysicsDialog", "View Options", 0, QApplication::UnicodeUTF8));
        ViewDiscreteRadio->setText(QApplication::translate("PhysicsDialog", "Discrete", 0, QApplication::UnicodeUTF8));
        ViewDeformedRadio->setText(QApplication::translate("PhysicsDialog", "Deformed", 0, QApplication::UnicodeUTF8));
        ViewSmoothRadio->setText(QApplication::translate("PhysicsDialog", "Smooth", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ForcesCheck->setToolTip(QApplication::translate("PhysicsDialog", "Internal forces (currently disabled)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ForcesCheck->setText(QApplication::translate("PhysicsDialog", "Forces", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        LocalCoordCheck->setToolTip(QApplication::translate("PhysicsDialog", "Origin and axes of each voxel.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        LocalCoordCheck->setText(QApplication::translate("PhysicsDialog", "Local Coords", 0, QApplication::UnicodeUTF8));
        ColorGroup->setTitle(QApplication::translate("PhysicsDialog", "Color", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TypeRadio->setToolTip(QApplication::translate("PhysicsDialog", "Material color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        TypeRadio->setText(QApplication::translate("PhysicsDialog", "Type", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        KineticERadio->setToolTip(QApplication::translate("PhysicsDialog", "Internal material strain", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        KineticERadio->setText(QApplication::translate("PhysicsDialog", "Kinetic Energy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DisplacementRadio->setToolTip(QApplication::translate("PhysicsDialog", "Distance from starting point of each voxel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        DisplacementRadio->setText(QApplication::translate("PhysicsDialog", "Displacement", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        StateRadio->setToolTip(QApplication::translate("PhysicsDialog", "White is unyielded, yellow is yielded, red is broken.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        StateRadio->setText(QApplication::translate("PhysicsDialog", "State", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        StrainERadio->setToolTip(QApplication::translate("PhysicsDialog", "Internal material strain", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        StrainERadio->setText(QApplication::translate("PhysicsDialog", "Strain Energy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        StressRadio->setToolTip(QApplication::translate("PhysicsDialog", "Internal material stress", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        StressRadio->setText(QApplication::translate("PhysicsDialog", "Engineering\n"
"Stress", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        StrainRadio->setToolTip(QApplication::translate("PhysicsDialog", "Internal material strain", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        StrainRadio->setText(QApplication::translate("PhysicsDialog", "Engineering\n"
"Strain", 0, QApplication::UnicodeUTF8));
        PressureRadio->setText(QApplication::translate("PhysicsDialog", "Pressure", 0, QApplication::UnicodeUTF8));
        CoMCheck->setText(QApplication::translate("PhysicsDialog", "Lock view to center of mass", 0, QApplication::UnicodeUTF8));
        PhysTabWidget->setTabText(PhysTabWidget->indexOf(View), QApplication::translate("PhysicsDialog", "View", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        UseTempCheck->setToolTip(QApplication::translate("PhysicsDialog", "Enables temperature changes.\n"
"Affects the size of each voxel according to its CTE.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseTempCheck->setText(QApplication::translate("PhysicsDialog", "Enable Temperature", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TempSlider->setToolTip(QApplication::translate("PhysicsDialog", "Current environment temperature.\n"
"Base (no expansion) is 25\302\260.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("PhysicsDialog", "Temp (\302\260C)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        VaryTempCheck->setToolTip(QApplication::translate("PhysicsDialog", "Periodically varies the environment temperature when enabled.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        VaryTempCheck->setText(QApplication::translate("PhysicsDialog", "Vary Temperature", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TempPerSlider->setToolTip(QApplication::translate("PhysicsDialog", "Period of temperature variation in environment.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_8->setText(QApplication::translate("PhysicsDialog", "Period (sec)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        UseGravCheck->setToolTip(QApplication::translate("PhysicsDialog", "Enables gravitational acceleration.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseGravCheck->setText(QApplication::translate("PhysicsDialog", "Enable Gravity", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        GravSlider->setToolTip(QApplication::translate("PhysicsDialog", "Gravitational acceleration (9.81 for earth).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("PhysicsDialog", "Grav (m/s\302\262)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        UseFloorCheck->setToolTip(QApplication::translate("PhysicsDialog", "Enables a flat floor.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseFloorCheck->setText(QApplication::translate("PhysicsDialog", "Enable Floor", 0, QApplication::UnicodeUTF8));
        PhysTabWidget->setTabText(PhysTabWidget->indexOf(Environment), QApplication::translate("PhysicsDialog", "Environment", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PhysicsDialog", "Plot Variable", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PhysicsDialog", "Plot Direction", 0, QApplication::UnicodeUTF8));
        LogEachCheck->setText(QApplication::translate("PhysicsDialog", "Log Every", 0, QApplication::UnicodeUTF8));
        SaveDataButton->setText(QApplication::translate("PhysicsDialog", "Save Data", 0, QApplication::UnicodeUTF8));
        PhysTabWidget->setTabText(PhysTabWidget->indexOf(Trace), QApplication::translate("PhysicsDialog", "Trace", 0, QApplication::UnicodeUTF8));
        PhysTabWidget->setTabText(PhysTabWidget->indexOf(Output), QApplication::translate("PhysicsDialog", "Output", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        BondDampSlider->setToolTip(QApplication::translate("PhysicsDialog", "Bulk material damping. [0-1.0]\n"
"Small values are more jiggly. 0.0 represents no damping\n"
"and is unstable, 1.0 is critical damping.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("PhysicsDialog", "Bulk material damping. [0-1.0]\n"
"Small values are more jiggly. 0.0 represents no damping\n"
"and is unstable, 1.0 is critical damping.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("PhysicsDialog", "Bulk damp ratio (z)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        BondDampEdit->setToolTip(QApplication::translate("PhysicsDialog", "Bulk material damping. [0-1.0]\n"
"Small values are more jiggly. 0.0 represents no damping\n"
"and is unstable, 1.0 is critical damping.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        UseMaxVelLimitCheck->setToolTip(QApplication::translate("PhysicsDialog", "Enforces a maximum velocity when enabled (rarely used).\n"
"Useful for highly unstable scenarios.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        UseMaxVelLimitCheck->setText(QApplication::translate("PhysicsDialog", "Enable Velocity Limit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        MaxVelLimitSlider->setToolTip(QApplication::translate("PhysicsDialog", "Maximum voxel velocity.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("PhysicsDialog", "Maximum Voxel Velocity", 0, QApplication::UnicodeUTF8));
        UseVolumeEffectsCheck->setText(QApplication::translate("PhysicsDialog", "Enable Volume Effects", 0, QApplication::UnicodeUTF8));
        PhysTabWidget->setTabText(PhysTabWidget->indexOf(Other), QApplication::translate("PhysicsDialog", "Other", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PhysicsDialog: public Ui_PhysicsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VPHYSICS_H
