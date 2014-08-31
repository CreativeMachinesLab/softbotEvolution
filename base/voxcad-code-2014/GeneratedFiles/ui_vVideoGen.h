/********************************************************************************
** Form generated from reading UI file 'vVideoGen.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VVIDEOGEN_H
#define UI_VVIDEOGEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoDialog
{
public:
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *OutputFolderEdit;
    QPushButton *SelectFolderButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *WidthPixEdit;
    QLabel *label;
    QLineEdit *HeightPixEdit;
    QLabel *label_2;
    QGridLayout *gridLayout;
    QRadioButton *r320x240Radio;
    QRadioButton *r800x600Radio;
    QRadioButton *r1280x720Radio;
    QRadioButton *r640x480Radio;
    QRadioButton *r1024x768Radio;
    QRadioButton *r1920x1080Radio;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *vsDisplayTimeRadio;
    QRadioButton *vsSimTimeRadio;
    QRadioButton *vsEveryFrameRadio;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *OutputFpsEdit;
    QLabel *label_3;
    QLineEdit *OutputSpeedFactorEdit;
    QLabel *label_4;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *AutoStopEnabledCheck;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_5;
    QRadioButton *TimeStepsRadio;
    QRadioButton *SimTimeRadio;
    QRadioButton *TempCycleRadio;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *TimeStepsEdit;
    QLineEdit *SimTimeEdit;
    QLineEdit *TempCycleEdit;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QCheckBox *ResetSimCheck;
    QPushButton *BeginButton;
    QPushButton *CancelButton;

    void setupUi(QWidget *VideoDialog)
    {
        if (VideoDialog->objectName().isEmpty())
            VideoDialog->setObjectName(QString::fromUtf8("VideoDialog"));
        VideoDialog->resize(308, 444);
        verticalLayout_7 = new QVBoxLayout(VideoDialog);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox = new QGroupBox(VideoDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        OutputFolderEdit = new QLineEdit(groupBox);
        OutputFolderEdit->setObjectName(QString::fromUtf8("OutputFolderEdit"));
        OutputFolderEdit->setEnabled(false);

        horizontalLayout_2->addWidget(OutputFolderEdit);

        SelectFolderButton = new QPushButton(groupBox);
        SelectFolderButton->setObjectName(QString::fromUtf8("SelectFolderButton"));

        horizontalLayout_2->addWidget(SelectFolderButton);


        verticalLayout_7->addWidget(groupBox);

        groupBox_2 = new QGroupBox(VideoDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        WidthPixEdit = new QLineEdit(groupBox_2);
        WidthPixEdit->setObjectName(QString::fromUtf8("WidthPixEdit"));

        horizontalLayout->addWidget(WidthPixEdit);

        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        HeightPixEdit = new QLineEdit(groupBox_2);
        HeightPixEdit->setObjectName(QString::fromUtf8("HeightPixEdit"));

        horizontalLayout->addWidget(HeightPixEdit);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        r320x240Radio = new QRadioButton(groupBox_2);
        r320x240Radio->setObjectName(QString::fromUtf8("r320x240Radio"));

        gridLayout->addWidget(r320x240Radio, 0, 0, 1, 1);

        r800x600Radio = new QRadioButton(groupBox_2);
        r800x600Radio->setObjectName(QString::fromUtf8("r800x600Radio"));

        gridLayout->addWidget(r800x600Radio, 0, 1, 1, 1);

        r1280x720Radio = new QRadioButton(groupBox_2);
        r1280x720Radio->setObjectName(QString::fromUtf8("r1280x720Radio"));

        gridLayout->addWidget(r1280x720Radio, 0, 2, 1, 1);

        r640x480Radio = new QRadioButton(groupBox_2);
        r640x480Radio->setObjectName(QString::fromUtf8("r640x480Radio"));

        gridLayout->addWidget(r640x480Radio, 1, 0, 1, 1);

        r1024x768Radio = new QRadioButton(groupBox_2);
        r1024x768Radio->setObjectName(QString::fromUtf8("r1024x768Radio"));

        gridLayout->addWidget(r1024x768Radio, 1, 1, 1, 1);

        r1920x1080Radio = new QRadioButton(groupBox_2);
        r1920x1080Radio->setObjectName(QString::fromUtf8("r1920x1080Radio"));

        gridLayout->addWidget(r1920x1080Radio, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_7->addWidget(groupBox_2);

        groupBox_5 = new QGroupBox(VideoDialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_2 = new QVBoxLayout(groupBox_5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        vsDisplayTimeRadio = new QRadioButton(groupBox_5);
        vsDisplayTimeRadio->setObjectName(QString::fromUtf8("vsDisplayTimeRadio"));

        horizontalLayout_3->addWidget(vsDisplayTimeRadio);

        vsSimTimeRadio = new QRadioButton(groupBox_5);
        vsSimTimeRadio->setObjectName(QString::fromUtf8("vsSimTimeRadio"));

        horizontalLayout_3->addWidget(vsSimTimeRadio);

        vsEveryFrameRadio = new QRadioButton(groupBox_5);
        vsEveryFrameRadio->setObjectName(QString::fromUtf8("vsEveryFrameRadio"));

        horizontalLayout_3->addWidget(vsEveryFrameRadio);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        OutputFpsEdit = new QLineEdit(groupBox_5);
        OutputFpsEdit->setObjectName(QString::fromUtf8("OutputFpsEdit"));

        horizontalLayout_4->addWidget(OutputFpsEdit);

        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        OutputSpeedFactorEdit = new QLineEdit(groupBox_5);
        OutputSpeedFactorEdit->setObjectName(QString::fromUtf8("OutputSpeedFactorEdit"));

        horizontalLayout_4->addWidget(OutputSpeedFactorEdit);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_7->addWidget(groupBox_5);

        groupBox_3 = new QGroupBox(VideoDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        AutoStopEnabledCheck = new QCheckBox(groupBox_3);
        AutoStopEnabledCheck->setObjectName(QString::fromUtf8("AutoStopEnabledCheck"));

        verticalLayout_6->addWidget(AutoStopEnabledCheck);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        TimeStepsRadio = new QRadioButton(groupBox_3);
        TimeStepsRadio->setObjectName(QString::fromUtf8("TimeStepsRadio"));

        verticalLayout_5->addWidget(TimeStepsRadio);

        SimTimeRadio = new QRadioButton(groupBox_3);
        SimTimeRadio->setObjectName(QString::fromUtf8("SimTimeRadio"));

        verticalLayout_5->addWidget(SimTimeRadio);

        TempCycleRadio = new QRadioButton(groupBox_3);
        TempCycleRadio->setObjectName(QString::fromUtf8("TempCycleRadio"));

        verticalLayout_5->addWidget(TempCycleRadio);


        horizontalLayout_5->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        TimeStepsEdit = new QLineEdit(groupBox_3);
        TimeStepsEdit->setObjectName(QString::fromUtf8("TimeStepsEdit"));

        verticalLayout_4->addWidget(TimeStepsEdit);

        SimTimeEdit = new QLineEdit(groupBox_3);
        SimTimeEdit->setObjectName(QString::fromUtf8("SimTimeEdit"));

        verticalLayout_4->addWidget(SimTimeEdit);

        TempCycleEdit = new QLineEdit(groupBox_3);
        TempCycleEdit->setObjectName(QString::fromUtf8("TempCycleEdit"));

        verticalLayout_4->addWidget(TempCycleEdit);


        horizontalLayout_5->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_3->addWidget(label_5);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_3->addWidget(label_6);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_3->addWidget(label_7);


        horizontalLayout_5->addLayout(verticalLayout_3);


        verticalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout_7->addWidget(groupBox_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        ResetSimCheck = new QCheckBox(VideoDialog);
        ResetSimCheck->setObjectName(QString::fromUtf8("ResetSimCheck"));

        horizontalLayout_6->addWidget(ResetSimCheck);

        BeginButton = new QPushButton(VideoDialog);
        BeginButton->setObjectName(QString::fromUtf8("BeginButton"));

        horizontalLayout_6->addWidget(BeginButton);

        CancelButton = new QPushButton(VideoDialog);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));

        horizontalLayout_6->addWidget(CancelButton);


        verticalLayout_7->addLayout(horizontalLayout_6);


        retranslateUi(VideoDialog);

        QMetaObject::connectSlotsByName(VideoDialog);
    } // setupUi

    void retranslateUi(QWidget *VideoDialog)
    {
        VideoDialog->setWindowTitle(QApplication::translate("VideoDialog", "Video Capture Settings", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("VideoDialog", "Output Folder", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        OutputFolderEdit->setToolTip(QApplication::translate("VideoDialog", "Output folder for jpg frames. Use ffmpeg, virtualDub, etc. to convert to movie file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SelectFolderButton->setToolTip(QApplication::translate("VideoDialog", "Output folder for jpg frames. Use ffmpeg, virtualDub, etc. to convert to movie file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SelectFolderButton->setText(QApplication::translate("VideoDialog", "Select", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("VideoDialog", "Resolution", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        WidthPixEdit->setToolTip(QApplication::translate("VideoDialog", "Video width", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("VideoDialog", "Width (pixels)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        HeightPixEdit->setToolTip(QApplication::translate("VideoDialog", "Video height", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("VideoDialog", "Height (pixels)", 0, QApplication::UnicodeUTF8));
        r320x240Radio->setText(QApplication::translate("VideoDialog", "320x240", 0, QApplication::UnicodeUTF8));
        r800x600Radio->setText(QApplication::translate("VideoDialog", "800x600", 0, QApplication::UnicodeUTF8));
        r1280x720Radio->setText(QApplication::translate("VideoDialog", "1280x720", 0, QApplication::UnicodeUTF8));
        r640x480Radio->setText(QApplication::translate("VideoDialog", "640x480", 0, QApplication::UnicodeUTF8));
        r1024x768Radio->setText(QApplication::translate("VideoDialog", "1024x768", 0, QApplication::UnicodeUTF8));
        r1920x1080Radio->setText(QApplication::translate("VideoDialog", "1920x1080", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("VideoDialog", "Video speed", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        vsDisplayTimeRadio->setToolTip(QApplication::translate("VideoDialog", "Records video at the speed the simulation is displayed on your screen.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        vsDisplayTimeRadio->setText(QApplication::translate("VideoDialog", "Display time", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        vsSimTimeRadio->setToolTip(QApplication::translate("VideoDialog", "Records video at the speed of the simulation.\n"
"One second of simulation time is one second of\n"
"video. Use \"Output speed factor\" to vary this ratio.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        vsSimTimeRadio->setText(QApplication::translate("VideoDialog", "Simulation time", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        vsEveryFrameRadio->setToolTip(QApplication::translate("VideoDialog", "The video contains a frame from every single timestep of the simulation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        vsEveryFrameRadio->setText(QApplication::translate("VideoDialog", "Every frame", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        OutputFpsEdit->setToolTip(QApplication::translate("VideoDialog", "Desired framerate of output video in frames per second. [Usually 30 or so]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("VideoDialog", "Desired framerate of output video in frames per second. [Usually 30 or so]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("VideoDialog", "Output fps", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        OutputSpeedFactorEdit->setToolTip(QApplication::translate("VideoDialog", "Speed up or slow down output video. [0.001-1000.0]\n"
"2.0 is double speed, 0.5 is half speed.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("VideoDialog", "Speed up or slow down output video. [0.001-1000.0]\n"
"2.0 is double speed, 0.5 is half speed.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("VideoDialog", "Output\n"
"speed factor", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("VideoDialog", "AutoStop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        AutoStopEnabledCheck->setToolTip(QApplication::translate("VideoDialog", "Enables auto-stopping of the simulation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        AutoStopEnabledCheck->setText(QApplication::translate("VideoDialog", "Enabled", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TimeStepsRadio->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of timesteps.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        TimeStepsRadio->setText(QApplication::translate("VideoDialog", "Time steps", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SimTimeRadio->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed amount of simulation time in seconds.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SimTimeRadio->setText(QApplication::translate("VideoDialog", "Simulation time", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TempCycleRadio->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of temperature variation cycles.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        TempCycleRadio->setText(QApplication::translate("VideoDialog", "Temperature cycles", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        TimeStepsEdit->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of timesteps.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SimTimeEdit->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed amount of simulation time in seconds.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        TempCycleEdit->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of temperature variation cycles.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of timesteps.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("VideoDialog", "# steps", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed amount of simulation time in seconds.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("VideoDialog", "sec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("VideoDialog", "Stops the simulation after a fixed number of temperature variation cycles.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("VideoDialog", "# cyc", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ResetSimCheck->setToolTip(QApplication::translate("VideoDialog", "Resets the simulation upon \"Begin\" when enabled.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ResetSimCheck->setText(QApplication::translate("VideoDialog", "Reset Sim", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        BeginButton->setToolTip(QApplication::translate("VideoDialog", "Begin recording.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        BeginButton->setText(QApplication::translate("VideoDialog", "Begin", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        CancelButton->setToolTip(QApplication::translate("VideoDialog", "Cancels the record operation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        CancelButton->setText(QApplication::translate("VideoDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VideoDialog: public Ui_VideoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VVIDEOGEN_H
