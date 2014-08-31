/********************************************************************************
** Form generated from reading UI file 'vPalette.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VPALETTE_H
#define UI_VPALETTE_H

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
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaletteDlg
{
public:
    QVBoxLayout *verticalLayout_14;
    QListView *MaterialList;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *AddMatButton;
    QPushButton *DelMatButton;
    QPushButton *LoadPalButton;
    QPushButton *SavePalButton;
    QSpacerItem *horizontalSpacer;
    QCheckBox *VisibleCheck;
    QGroupBox *MatTypeGroup;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *MTBasicRadio;
    QRadioButton *MTDitherRadio;
    QRadioButton *MTStructureRadio;
    QTabWidget *tabProperties;
    QWidget *AppearanceTab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QSlider *RedSlider;
    QSlider *GreenSlider;
    QSlider *BlueSlider;
    QSlider *AlphaSlider;
    QVBoxLayout *verticalLayout_2;
    QSpinBox *RedSpin;
    QSpinBox *GreenSpin;
    QSpinBox *BlueSpin;
    QSpinBox *AlphaSpin;
    QVBoxLayout *verticalLayout_3;
    QLabel *RedLabel;
    QLabel *GreenLabel;
    QLabel *BlueLabel;
    QLabel *AlphaLabel;
    QSpacerItem *verticalSpacer;
    QWidget *ModelTab;
    QVBoxLayout *verticalLayout_23;
    QHBoxLayout *horizontalLayout_14;
    QComboBox *MatModelCombo;
    QLabel *MatModelLabel;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout_22;
    QLineEdit *ElasticEdit;
    QLineEdit *PlasticEdit;
    QLineEdit *YieldEdit;
    QVBoxLayout *verticalLayout_21;
    QLabel *StiffLabel;
    QLabel *PlasticLabel;
    QLabel *YieldLabel;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_10;
    QRadioButton *StressFailRadio;
    QRadioButton *StrainFailRadio;
    QVBoxLayout *verticalLayout_19;
    QLineEdit *FailEdit;
    QLineEdit *FailStrainEdit;
    QVBoxLayout *verticalLayout_20;
    QLabel *FailLabel;
    QLabel *FailStrainLabel;
    QPushButton *LoadSSButton;
    QWidget *PhysicalTab;
    QHBoxLayout *horizontalLayout_16;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_6;
    QLineEdit *PoissoEdit;
    QLineEdit *DensityEdit;
    QLineEdit *CTEEdit;
    QLineEdit *TempPhaseEdit;
    QLineEdit *UsEdit;
    QLineEdit *UdEdit;
    QVBoxLayout *verticalLayout_7;
    QLabel *PoissonLabel;
    QLabel *DensityLabel;
    QLabel *CTELabel;
    QLabel *TempPhaseLabel;
    QLabel *UsLabel;
    QLabel *UdLabel;
    QSpacerItem *verticalSpacer_2;
    QWidget *DitherTab;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_8;
    QLabel *Mat1SelLabel;
    QComboBox *Mat1SelCombo;
    QHBoxLayout *horizontalLayout_7;
    QLabel *Mat2SelLabel;
    QComboBox *Mat2SelCombo;
    QHBoxLayout *horizontalLayout_5;
    QSlider *MatPercSlider;
    QSpinBox *MatPercSpin;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_2;
    QLabel *PercLabel;
    QSpacerItem *verticalSpacer_3;
    QWidget *StructureTab;
    QVBoxLayout *verticalLayout_24;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_18;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *ImportButton;
    QPushButton *ExportButton;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_16;
    QLabel *DimLabel;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_12;
    QSpinBox *XDimSpin;
    QSpinBox *YDimSpin;
    QSpinBox *ZDimSpin;
    QVBoxLayout *verticalLayout_11;
    QLabel *XDimLabel;
    QLabel *YDimLabel;
    QLabel *ZDimLabel;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_17;
    QLabel *OffLabel;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_13;
    QSpinBox *XOffSpin;
    QSpinBox *YOffSpin;
    QSpinBox *ZOffSpin;
    QVBoxLayout *verticalLayout_15;
    QLabel *XOffLabel;
    QLabel *YOffLabel;
    QLabel *ZOffLabel;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_17;
    QRadioButton *RxRadio;
    QRadioButton *RyRadio;
    QRadioButton *RzRadio;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_18;
    QRadioButton *R0Radio;
    QRadioButton *R90Radio;
    QRadioButton *R180Radio;
    QRadioButton *R270Radio;
    QPushButton *EditStructureButton;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *PaletteDlg)
    {
        if (PaletteDlg->objectName().isEmpty())
            PaletteDlg->setObjectName(QString::fromUtf8("PaletteDlg"));
        PaletteDlg->resize(220, 621);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PaletteDlg->sizePolicy().hasHeightForWidth());
        PaletteDlg->setSizePolicy(sizePolicy);
        PaletteDlg->setMinimumSize(QSize(0, 0));
        PaletteDlg->setMaximumSize(QSize(5000, 5000));
        verticalLayout_14 = new QVBoxLayout(PaletteDlg);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        MaterialList = new QListView(PaletteDlg);
        MaterialList->setObjectName(QString::fromUtf8("MaterialList"));
        MaterialList->setIconSize(QSize(16, 16));

        verticalLayout_14->addWidget(MaterialList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        AddMatButton = new QPushButton(PaletteDlg);
        AddMatButton->setObjectName(QString::fromUtf8("AddMatButton"));
        AddMatButton->setMaximumSize(QSize(25, 16777215));
        AddMatButton->setBaseSize(QSize(0, 0));

        horizontalLayout_2->addWidget(AddMatButton);

        DelMatButton = new QPushButton(PaletteDlg);
        DelMatButton->setObjectName(QString::fromUtf8("DelMatButton"));
        DelMatButton->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_2->addWidget(DelMatButton);

        LoadPalButton = new QPushButton(PaletteDlg);
        LoadPalButton->setObjectName(QString::fromUtf8("LoadPalButton"));
        LoadPalButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(LoadPalButton);

        SavePalButton = new QPushButton(PaletteDlg);
        SavePalButton->setObjectName(QString::fromUtf8("SavePalButton"));
        SavePalButton->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(SavePalButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_14->addLayout(horizontalLayout_2);

        VisibleCheck = new QCheckBox(PaletteDlg);
        VisibleCheck->setObjectName(QString::fromUtf8("VisibleCheck"));

        verticalLayout_14->addWidget(VisibleCheck);

        MatTypeGroup = new QGroupBox(PaletteDlg);
        MatTypeGroup->setObjectName(QString::fromUtf8("MatTypeGroup"));
        verticalLayout_4 = new QVBoxLayout(MatTypeGroup);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        MTBasicRadio = new QRadioButton(MatTypeGroup);
        MTBasicRadio->setObjectName(QString::fromUtf8("MTBasicRadio"));

        horizontalLayout_4->addWidget(MTBasicRadio);

        MTDitherRadio = new QRadioButton(MatTypeGroup);
        MTDitherRadio->setObjectName(QString::fromUtf8("MTDitherRadio"));

        horizontalLayout_4->addWidget(MTDitherRadio);

        MTStructureRadio = new QRadioButton(MatTypeGroup);
        MTStructureRadio->setObjectName(QString::fromUtf8("MTStructureRadio"));

        horizontalLayout_4->addWidget(MTStructureRadio);


        verticalLayout_4->addLayout(horizontalLayout_4);


        verticalLayout_14->addWidget(MatTypeGroup);

        tabProperties = new QTabWidget(PaletteDlg);
        tabProperties->setObjectName(QString::fromUtf8("tabProperties"));
        tabProperties->setTabPosition(QTabWidget::North);
        AppearanceTab = new QWidget();
        AppearanceTab->setObjectName(QString::fromUtf8("AppearanceTab"));
        verticalLayout_5 = new QVBoxLayout(AppearanceTab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        RedSlider = new QSlider(AppearanceTab);
        RedSlider->setObjectName(QString::fromUtf8("RedSlider"));
        RedSlider->setMaximum(255);
        RedSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(RedSlider);

        GreenSlider = new QSlider(AppearanceTab);
        GreenSlider->setObjectName(QString::fromUtf8("GreenSlider"));
        GreenSlider->setMaximum(255);
        GreenSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(GreenSlider);

        BlueSlider = new QSlider(AppearanceTab);
        BlueSlider->setObjectName(QString::fromUtf8("BlueSlider"));
        BlueSlider->setMaximum(255);
        BlueSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(BlueSlider);

        AlphaSlider = new QSlider(AppearanceTab);
        AlphaSlider->setObjectName(QString::fromUtf8("AlphaSlider"));
        AlphaSlider->setMaximum(255);
        AlphaSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(AlphaSlider);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        RedSpin = new QSpinBox(AppearanceTab);
        RedSpin->setObjectName(QString::fromUtf8("RedSpin"));
        RedSpin->setMaximum(255);
        RedSpin->setSingleStep(1);

        verticalLayout_2->addWidget(RedSpin);

        GreenSpin = new QSpinBox(AppearanceTab);
        GreenSpin->setObjectName(QString::fromUtf8("GreenSpin"));
        GreenSpin->setMaximum(255);
        GreenSpin->setSingleStep(1);

        verticalLayout_2->addWidget(GreenSpin);

        BlueSpin = new QSpinBox(AppearanceTab);
        BlueSpin->setObjectName(QString::fromUtf8("BlueSpin"));
        BlueSpin->setMaximum(255);
        BlueSpin->setSingleStep(1);

        verticalLayout_2->addWidget(BlueSpin);

        AlphaSpin = new QSpinBox(AppearanceTab);
        AlphaSpin->setObjectName(QString::fromUtf8("AlphaSpin"));
        AlphaSpin->setMaximum(255);
        AlphaSpin->setSingleStep(1);

        verticalLayout_2->addWidget(AlphaSpin);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        RedLabel = new QLabel(AppearanceTab);
        RedLabel->setObjectName(QString::fromUtf8("RedLabel"));

        verticalLayout_3->addWidget(RedLabel);

        GreenLabel = new QLabel(AppearanceTab);
        GreenLabel->setObjectName(QString::fromUtf8("GreenLabel"));

        verticalLayout_3->addWidget(GreenLabel);

        BlueLabel = new QLabel(AppearanceTab);
        BlueLabel->setObjectName(QString::fromUtf8("BlueLabel"));

        verticalLayout_3->addWidget(BlueLabel);

        AlphaLabel = new QLabel(AppearanceTab);
        AlphaLabel->setObjectName(QString::fromUtf8("AlphaLabel"));

        verticalLayout_3->addWidget(AlphaLabel);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 51, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        tabProperties->addTab(AppearanceTab, QString());
        ModelTab = new QWidget();
        ModelTab->setObjectName(QString::fromUtf8("ModelTab"));
        verticalLayout_23 = new QVBoxLayout(ModelTab);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        MatModelCombo = new QComboBox(ModelTab);
        MatModelCombo->setObjectName(QString::fromUtf8("MatModelCombo"));

        horizontalLayout_14->addWidget(MatModelCombo);

        MatModelLabel = new QLabel(ModelTab);
        MatModelLabel->setObjectName(QString::fromUtf8("MatModelLabel"));

        horizontalLayout_14->addWidget(MatModelLabel);


        verticalLayout_23->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        ElasticEdit = new QLineEdit(ModelTab);
        ElasticEdit->setObjectName(QString::fromUtf8("ElasticEdit"));

        verticalLayout_22->addWidget(ElasticEdit);

        PlasticEdit = new QLineEdit(ModelTab);
        PlasticEdit->setObjectName(QString::fromUtf8("PlasticEdit"));

        verticalLayout_22->addWidget(PlasticEdit);

        YieldEdit = new QLineEdit(ModelTab);
        YieldEdit->setObjectName(QString::fromUtf8("YieldEdit"));

        verticalLayout_22->addWidget(YieldEdit);


        horizontalLayout_15->addLayout(verticalLayout_22);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        StiffLabel = new QLabel(ModelTab);
        StiffLabel->setObjectName(QString::fromUtf8("StiffLabel"));

        verticalLayout_21->addWidget(StiffLabel);

        PlasticLabel = new QLabel(ModelTab);
        PlasticLabel->setObjectName(QString::fromUtf8("PlasticLabel"));

        verticalLayout_21->addWidget(PlasticLabel);

        YieldLabel = new QLabel(ModelTab);
        YieldLabel->setObjectName(QString::fromUtf8("YieldLabel"));

        verticalLayout_21->addWidget(YieldLabel);


        horizontalLayout_15->addLayout(verticalLayout_21);


        verticalLayout_23->addLayout(horizontalLayout_15);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(10);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        StressFailRadio = new QRadioButton(ModelTab);
        StressFailRadio->setObjectName(QString::fromUtf8("StressFailRadio"));
        StressFailRadio->setMaximumSize(QSize(13, 16777215));

        verticalLayout_10->addWidget(StressFailRadio);

        StrainFailRadio = new QRadioButton(ModelTab);
        StrainFailRadio->setObjectName(QString::fromUtf8("StrainFailRadio"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(StrainFailRadio->sizePolicy().hasHeightForWidth());
        StrainFailRadio->setSizePolicy(sizePolicy1);
        StrainFailRadio->setMaximumSize(QSize(13, 16777215));

        verticalLayout_10->addWidget(StrainFailRadio);


        horizontalLayout_13->addLayout(verticalLayout_10);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        FailEdit = new QLineEdit(ModelTab);
        FailEdit->setObjectName(QString::fromUtf8("FailEdit"));

        verticalLayout_19->addWidget(FailEdit);

        FailStrainEdit = new QLineEdit(ModelTab);
        FailStrainEdit->setObjectName(QString::fromUtf8("FailStrainEdit"));

        verticalLayout_19->addWidget(FailStrainEdit);


        horizontalLayout_13->addLayout(verticalLayout_19);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        FailLabel = new QLabel(ModelTab);
        FailLabel->setObjectName(QString::fromUtf8("FailLabel"));

        verticalLayout_20->addWidget(FailLabel);

        FailStrainLabel = new QLabel(ModelTab);
        FailStrainLabel->setObjectName(QString::fromUtf8("FailStrainLabel"));

        verticalLayout_20->addWidget(FailStrainLabel);


        horizontalLayout_13->addLayout(verticalLayout_20);


        verticalLayout_23->addLayout(horizontalLayout_13);

        LoadSSButton = new QPushButton(ModelTab);
        LoadSSButton->setObjectName(QString::fromUtf8("LoadSSButton"));

        verticalLayout_23->addWidget(LoadSSButton);

        tabProperties->addTab(ModelTab, QString());
        PhysicalTab = new QWidget();
        PhysicalTab->setObjectName(QString::fromUtf8("PhysicalTab"));
        horizontalLayout_16 = new QHBoxLayout(PhysicalTab);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        PoissoEdit = new QLineEdit(PhysicalTab);
        PoissoEdit->setObjectName(QString::fromUtf8("PoissoEdit"));

        verticalLayout_6->addWidget(PoissoEdit);

        DensityEdit = new QLineEdit(PhysicalTab);
        DensityEdit->setObjectName(QString::fromUtf8("DensityEdit"));

        verticalLayout_6->addWidget(DensityEdit);

        CTEEdit = new QLineEdit(PhysicalTab);
        CTEEdit->setObjectName(QString::fromUtf8("CTEEdit"));

        verticalLayout_6->addWidget(CTEEdit);

        TempPhaseEdit = new QLineEdit(PhysicalTab);
        TempPhaseEdit->setObjectName(QString::fromUtf8("TempPhaseEdit"));

        verticalLayout_6->addWidget(TempPhaseEdit);

        UsEdit = new QLineEdit(PhysicalTab);
        UsEdit->setObjectName(QString::fromUtf8("UsEdit"));

        verticalLayout_6->addWidget(UsEdit);

        UdEdit = new QLineEdit(PhysicalTab);
        UdEdit->setObjectName(QString::fromUtf8("UdEdit"));

        verticalLayout_6->addWidget(UdEdit);


        horizontalLayout_3->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        PoissonLabel = new QLabel(PhysicalTab);
        PoissonLabel->setObjectName(QString::fromUtf8("PoissonLabel"));

        verticalLayout_7->addWidget(PoissonLabel);

        DensityLabel = new QLabel(PhysicalTab);
        DensityLabel->setObjectName(QString::fromUtf8("DensityLabel"));

        verticalLayout_7->addWidget(DensityLabel);

        CTELabel = new QLabel(PhysicalTab);
        CTELabel->setObjectName(QString::fromUtf8("CTELabel"));

        verticalLayout_7->addWidget(CTELabel);

        TempPhaseLabel = new QLabel(PhysicalTab);
        TempPhaseLabel->setObjectName(QString::fromUtf8("TempPhaseLabel"));

        verticalLayout_7->addWidget(TempPhaseLabel);

        UsLabel = new QLabel(PhysicalTab);
        UsLabel->setObjectName(QString::fromUtf8("UsLabel"));

        verticalLayout_7->addWidget(UsLabel);

        UdLabel = new QLabel(PhysicalTab);
        UdLabel->setObjectName(QString::fromUtf8("UdLabel"));

        verticalLayout_7->addWidget(UdLabel);


        horizontalLayout_3->addLayout(verticalLayout_7);


        verticalLayout_8->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);


        horizontalLayout_16->addLayout(verticalLayout_8);

        tabProperties->addTab(PhysicalTab, QString());
        DitherTab = new QWidget();
        DitherTab->setObjectName(QString::fromUtf8("DitherTab"));
        verticalLayout_9 = new QVBoxLayout(DitherTab);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        Mat1SelLabel = new QLabel(DitherTab);
        Mat1SelLabel->setObjectName(QString::fromUtf8("Mat1SelLabel"));

        horizontalLayout_8->addWidget(Mat1SelLabel);

        Mat1SelCombo = new QComboBox(DitherTab);
        Mat1SelCombo->setObjectName(QString::fromUtf8("Mat1SelCombo"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Mat1SelCombo->sizePolicy().hasHeightForWidth());
        Mat1SelCombo->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(Mat1SelCombo);


        verticalLayout_9->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        Mat2SelLabel = new QLabel(DitherTab);
        Mat2SelLabel->setObjectName(QString::fromUtf8("Mat2SelLabel"));

        horizontalLayout_7->addWidget(Mat2SelLabel);

        Mat2SelCombo = new QComboBox(DitherTab);
        Mat2SelCombo->setObjectName(QString::fromUtf8("Mat2SelCombo"));
        sizePolicy2.setHeightForWidth(Mat2SelCombo->sizePolicy().hasHeightForWidth());
        Mat2SelCombo->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(Mat2SelCombo);


        verticalLayout_9->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        MatPercSlider = new QSlider(DitherTab);
        MatPercSlider->setObjectName(QString::fromUtf8("MatPercSlider"));
        MatPercSlider->setMaximum(100);
        MatPercSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(MatPercSlider);

        MatPercSpin = new QSpinBox(DitherTab);
        MatPercSpin->setObjectName(QString::fromUtf8("MatPercSpin"));
        MatPercSpin->setMaximum(100);

        horizontalLayout_5->addWidget(MatPercSpin);


        verticalLayout_9->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        PercLabel = new QLabel(DitherTab);
        PercLabel->setObjectName(QString::fromUtf8("PercLabel"));

        horizontalLayout_6->addWidget(PercLabel);


        verticalLayout_9->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 55, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_3);

        tabProperties->addTab(DitherTab, QString());
        StructureTab = new QWidget();
        StructureTab->setObjectName(QString::fromUtf8("StructureTab"));
        verticalLayout_24 = new QVBoxLayout(StructureTab);
        verticalLayout_24->setSpacing(0);
        verticalLayout_24->setContentsMargins(0, 0, 0, 0);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        scrollArea = new QScrollArea(StructureTab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 194, 270));
        verticalLayout_18 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        ImportButton = new QPushButton(scrollAreaWidgetContents);
        ImportButton->setObjectName(QString::fromUtf8("ImportButton"));

        horizontalLayout_9->addWidget(ImportButton);

        ExportButton = new QPushButton(scrollAreaWidgetContents);
        ExportButton->setObjectName(QString::fromUtf8("ExportButton"));

        horizontalLayout_9->addWidget(ExportButton);


        verticalLayout_18->addLayout(horizontalLayout_9);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        DimLabel = new QLabel(scrollAreaWidgetContents);
        DimLabel->setObjectName(QString::fromUtf8("DimLabel"));

        verticalLayout_16->addWidget(DimLabel);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        XDimSpin = new QSpinBox(scrollAreaWidgetContents);
        XDimSpin->setObjectName(QString::fromUtf8("XDimSpin"));
        XDimSpin->setMinimum(1);

        verticalLayout_12->addWidget(XDimSpin);

        YDimSpin = new QSpinBox(scrollAreaWidgetContents);
        YDimSpin->setObjectName(QString::fromUtf8("YDimSpin"));
        YDimSpin->setMinimum(1);

        verticalLayout_12->addWidget(YDimSpin);

        ZDimSpin = new QSpinBox(scrollAreaWidgetContents);
        ZDimSpin->setObjectName(QString::fromUtf8("ZDimSpin"));
        ZDimSpin->setMinimum(1);

        verticalLayout_12->addWidget(ZDimSpin);


        horizontalLayout_11->addLayout(verticalLayout_12);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        XDimLabel = new QLabel(scrollAreaWidgetContents);
        XDimLabel->setObjectName(QString::fromUtf8("XDimLabel"));

        verticalLayout_11->addWidget(XDimLabel);

        YDimLabel = new QLabel(scrollAreaWidgetContents);
        YDimLabel->setObjectName(QString::fromUtf8("YDimLabel"));

        verticalLayout_11->addWidget(YDimLabel);

        ZDimLabel = new QLabel(scrollAreaWidgetContents);
        ZDimLabel->setObjectName(QString::fromUtf8("ZDimLabel"));
        ZDimLabel->setFrameShape(QFrame::NoFrame);

        verticalLayout_11->addWidget(ZDimLabel);


        horizontalLayout_11->addLayout(verticalLayout_11);


        verticalLayout_16->addLayout(horizontalLayout_11);


        horizontalLayout_12->addLayout(verticalLayout_16);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_3);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        OffLabel = new QLabel(scrollAreaWidgetContents);
        OffLabel->setObjectName(QString::fromUtf8("OffLabel"));

        verticalLayout_17->addWidget(OffLabel);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        XOffSpin = new QSpinBox(scrollAreaWidgetContents);
        XOffSpin->setObjectName(QString::fromUtf8("XOffSpin"));

        verticalLayout_13->addWidget(XOffSpin);

        YOffSpin = new QSpinBox(scrollAreaWidgetContents);
        YOffSpin->setObjectName(QString::fromUtf8("YOffSpin"));

        verticalLayout_13->addWidget(YOffSpin);

        ZOffSpin = new QSpinBox(scrollAreaWidgetContents);
        ZOffSpin->setObjectName(QString::fromUtf8("ZOffSpin"));

        verticalLayout_13->addWidget(ZOffSpin);


        horizontalLayout_10->addLayout(verticalLayout_13);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        XOffLabel = new QLabel(scrollAreaWidgetContents);
        XOffLabel->setObjectName(QString::fromUtf8("XOffLabel"));

        verticalLayout_15->addWidget(XOffLabel);

        YOffLabel = new QLabel(scrollAreaWidgetContents);
        YOffLabel->setObjectName(QString::fromUtf8("YOffLabel"));

        verticalLayout_15->addWidget(YOffLabel);

        ZOffLabel = new QLabel(scrollAreaWidgetContents);
        ZOffLabel->setObjectName(QString::fromUtf8("ZOffLabel"));
        ZOffLabel->setFrameShape(QFrame::NoFrame);

        verticalLayout_15->addWidget(ZOffLabel);


        horizontalLayout_10->addLayout(verticalLayout_15);


        verticalLayout_17->addLayout(horizontalLayout_10);


        horizontalLayout_12->addLayout(verticalLayout_17);


        verticalLayout_18->addLayout(horizontalLayout_12);

        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_17 = new QHBoxLayout(groupBox);
        horizontalLayout_17->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        RxRadio = new QRadioButton(groupBox);
        RxRadio->setObjectName(QString::fromUtf8("RxRadio"));

        horizontalLayout_17->addWidget(RxRadio);

        RyRadio = new QRadioButton(groupBox);
        RyRadio->setObjectName(QString::fromUtf8("RyRadio"));

        horizontalLayout_17->addWidget(RyRadio);

        RzRadio = new QRadioButton(groupBox);
        RzRadio->setObjectName(QString::fromUtf8("RzRadio"));

        horizontalLayout_17->addWidget(RzRadio);


        verticalLayout_18->addWidget(groupBox);

        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_18 = new QHBoxLayout(groupBox_2);
        horizontalLayout_18->setSpacing(3);
        horizontalLayout_18->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        R0Radio = new QRadioButton(groupBox_2);
        R0Radio->setObjectName(QString::fromUtf8("R0Radio"));

        horizontalLayout_18->addWidget(R0Radio);

        R90Radio = new QRadioButton(groupBox_2);
        R90Radio->setObjectName(QString::fromUtf8("R90Radio"));

        horizontalLayout_18->addWidget(R90Radio);

        R180Radio = new QRadioButton(groupBox_2);
        R180Radio->setObjectName(QString::fromUtf8("R180Radio"));

        horizontalLayout_18->addWidget(R180Radio);

        R270Radio = new QRadioButton(groupBox_2);
        R270Radio->setObjectName(QString::fromUtf8("R270Radio"));

        horizontalLayout_18->addWidget(R270Radio);


        verticalLayout_18->addWidget(groupBox_2);

        EditStructureButton = new QPushButton(scrollAreaWidgetContents);
        EditStructureButton->setObjectName(QString::fromUtf8("EditStructureButton"));
        EditStructureButton->setCheckable(false);

        verticalLayout_18->addWidget(EditStructureButton);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_18->addItem(verticalSpacer_4);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_24->addWidget(scrollArea);

        tabProperties->addTab(StructureTab, QString());

        verticalLayout_14->addWidget(tabProperties);


        retranslateUi(PaletteDlg);
        QObject::connect(RedSlider, SIGNAL(valueChanged(int)), RedSpin, SLOT(setValue(int)));
        QObject::connect(RedSpin, SIGNAL(valueChanged(int)), RedSlider, SLOT(setValue(int)));
        QObject::connect(GreenSlider, SIGNAL(valueChanged(int)), GreenSpin, SLOT(setValue(int)));
        QObject::connect(GreenSpin, SIGNAL(valueChanged(int)), GreenSlider, SLOT(setValue(int)));
        QObject::connect(BlueSlider, SIGNAL(valueChanged(int)), BlueSpin, SLOT(setValue(int)));
        QObject::connect(BlueSpin, SIGNAL(valueChanged(int)), BlueSlider, SLOT(setValue(int)));
        QObject::connect(AlphaSpin, SIGNAL(valueChanged(int)), AlphaSlider, SLOT(setValue(int)));
        QObject::connect(AlphaSlider, SIGNAL(valueChanged(int)), AlphaSpin, SLOT(setValue(int)));
        QObject::connect(MatPercSpin, SIGNAL(valueChanged(int)), MatPercSlider, SLOT(setValue(int)));
        QObject::connect(MatPercSlider, SIGNAL(valueChanged(int)), MatPercSpin, SLOT(setValue(int)));

        tabProperties->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(PaletteDlg);
    } // setupUi

    void retranslateUi(QWidget *PaletteDlg)
    {
        PaletteDlg->setWindowTitle(QApplication::translate("PaletteDlg", "Palette Editor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        AddMatButton->setToolTip(QApplication::translate("PaletteDlg", "Add Material", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        AddMatButton->setText(QApplication::translate("PaletteDlg", "+", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        DelMatButton->setToolTip(QApplication::translate("PaletteDlg", "Delete Material", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        DelMatButton->setText(QApplication::translate("PaletteDlg", "-", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        LoadPalButton->setToolTip(QApplication::translate("PaletteDlg", "Load Palette", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        LoadPalButton->setText(QApplication::translate("PaletteDlg", "Load", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SavePalButton->setToolTip(QApplication::translate("PaletteDlg", "Save Palette", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SavePalButton->setText(QApplication::translate("PaletteDlg", "Save", 0, QApplication::UnicodeUTF8));
        VisibleCheck->setText(QApplication::translate("PaletteDlg", "Visible", 0, QApplication::UnicodeUTF8));
        MatTypeGroup->setTitle(QApplication::translate("PaletteDlg", "Material Type", 0, QApplication::UnicodeUTF8));
        MTBasicRadio->setText(QApplication::translate("PaletteDlg", "Basic", 0, QApplication::UnicodeUTF8));
        MTDitherRadio->setText(QApplication::translate("PaletteDlg", "Dither", 0, QApplication::UnicodeUTF8));
        MTStructureRadio->setText(QApplication::translate("PaletteDlg", "Structure", 0, QApplication::UnicodeUTF8));
        RedLabel->setText(QApplication::translate("PaletteDlg", "Red", 0, QApplication::UnicodeUTF8));
        GreenLabel->setText(QApplication::translate("PaletteDlg", "Green", 0, QApplication::UnicodeUTF8));
        BlueLabel->setText(QApplication::translate("PaletteDlg", "Blue", 0, QApplication::UnicodeUTF8));
        AlphaLabel->setText(QApplication::translate("PaletteDlg", "Alpha", 0, QApplication::UnicodeUTF8));
        tabProperties->setTabText(tabProperties->indexOf(AppearanceTab), QApplication::translate("PaletteDlg", "Appearance", 0, QApplication::UnicodeUTF8));
        MatModelLabel->setText(QApplication::translate("PaletteDlg", "Material Model", 0, QApplication::UnicodeUTF8));
        PlasticEdit->setText(QString());
        StiffLabel->setText(QApplication::translate("PaletteDlg", "Elastic Modulus (MPa)", 0, QApplication::UnicodeUTF8));
        PlasticLabel->setText(QApplication::translate("PaletteDlg", "Plastic Modulus (MPa)", 0, QApplication::UnicodeUTF8));
        YieldLabel->setText(QApplication::translate("PaletteDlg", "Yield Stress (MPa)", 0, QApplication::UnicodeUTF8));
        StressFailRadio->setText(QString());
        StrainFailRadio->setText(QString());
        FailLabel->setText(QApplication::translate("PaletteDlg", "Failure Stress (MPa)", 0, QApplication::UnicodeUTF8));
        FailStrainLabel->setText(QApplication::translate("PaletteDlg", "Failure Strain (m/m)", 0, QApplication::UnicodeUTF8));
        LoadSSButton->setText(QApplication::translate("PaletteDlg", "Load Stress/Strain Data", 0, QApplication::UnicodeUTF8));
        tabProperties->setTabText(tabProperties->indexOf(ModelTab), QApplication::translate("PaletteDlg", "Model", 0, QApplication::UnicodeUTF8));
        PoissonLabel->setText(QApplication::translate("PaletteDlg", "Poissons Ratio", 0, QApplication::UnicodeUTF8));
        DensityLabel->setText(QApplication::translate("PaletteDlg", "Density (Kg/m^3)", 0, QApplication::UnicodeUTF8));
        CTELabel->setText(QApplication::translate("PaletteDlg", "CTE (1/deg C)", 0, QApplication::UnicodeUTF8));
        TempPhaseLabel->setText(QApplication::translate("PaletteDlg", "Temp Phase (rad)", 0, QApplication::UnicodeUTF8));
        UsLabel->setText(QApplication::translate("PaletteDlg", "Static Fric Coeff", 0, QApplication::UnicodeUTF8));
        UdLabel->setText(QApplication::translate("PaletteDlg", "Dynamic Fric Coeff", 0, QApplication::UnicodeUTF8));
        tabProperties->setTabText(tabProperties->indexOf(PhysicalTab), QApplication::translate("PaletteDlg", "Physical", 0, QApplication::UnicodeUTF8));
        Mat1SelLabel->setText(QApplication::translate("PaletteDlg", "Material 1:", 0, QApplication::UnicodeUTF8));
        Mat2SelLabel->setText(QApplication::translate("PaletteDlg", "Material 2:", 0, QApplication::UnicodeUTF8));
        PercLabel->setText(QApplication::translate("PaletteDlg", "% Mat1", 0, QApplication::UnicodeUTF8));
        tabProperties->setTabText(tabProperties->indexOf(DitherTab), QApplication::translate("PaletteDlg", "Dither", 0, QApplication::UnicodeUTF8));
        ImportButton->setText(QApplication::translate("PaletteDlg", "Import", 0, QApplication::UnicodeUTF8));
        ExportButton->setText(QApplication::translate("PaletteDlg", "Export", 0, QApplication::UnicodeUTF8));
        DimLabel->setText(QApplication::translate("PaletteDlg", "Dim (voxels)", 0, QApplication::UnicodeUTF8));
        XDimLabel->setText(QApplication::translate("PaletteDlg", "X", 0, QApplication::UnicodeUTF8));
        YDimLabel->setText(QApplication::translate("PaletteDlg", "Y", 0, QApplication::UnicodeUTF8));
        ZDimLabel->setText(QApplication::translate("PaletteDlg", "Z", 0, QApplication::UnicodeUTF8));
        OffLabel->setText(QApplication::translate("PaletteDlg", "Offset (voxels)", 0, QApplication::UnicodeUTF8));
        XOffLabel->setText(QApplication::translate("PaletteDlg", "X", 0, QApplication::UnicodeUTF8));
        YOffLabel->setText(QApplication::translate("PaletteDlg", "Y", 0, QApplication::UnicodeUTF8));
        ZOffLabel->setText(QApplication::translate("PaletteDlg", "Z", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PaletteDlg", "Rotate Axis", 0, QApplication::UnicodeUTF8));
        RxRadio->setText(QApplication::translate("PaletteDlg", "X", 0, QApplication::UnicodeUTF8));
        RyRadio->setText(QApplication::translate("PaletteDlg", "Y", 0, QApplication::UnicodeUTF8));
        RzRadio->setText(QApplication::translate("PaletteDlg", "Z", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("PaletteDlg", "Rotate degrees", 0, QApplication::UnicodeUTF8));
        R0Radio->setText(QApplication::translate("PaletteDlg", "0", 0, QApplication::UnicodeUTF8));
        R90Radio->setText(QApplication::translate("PaletteDlg", "90", 0, QApplication::UnicodeUTF8));
        R180Radio->setText(QApplication::translate("PaletteDlg", "180", 0, QApplication::UnicodeUTF8));
        R270Radio->setText(QApplication::translate("PaletteDlg", "270", 0, QApplication::UnicodeUTF8));
        EditStructureButton->setText(QApplication::translate("PaletteDlg", "Edit Structure", 0, QApplication::UnicodeUTF8));
        tabProperties->setTabText(tabProperties->indexOf(StructureTab), QApplication::translate("PaletteDlg", "Structure", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PaletteDlg: public Ui_PaletteDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VPALETTE_H
