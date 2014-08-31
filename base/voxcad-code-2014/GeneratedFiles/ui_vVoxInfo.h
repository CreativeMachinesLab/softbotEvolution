/********************************************************************************
** Form generated from reading UI file 'vVoxInfo.ui'
**
** Created: Mon Aug 25 22:49:53 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VVOXINFO_H
#define UI_VVOXINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VoxInfoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *InfoScrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QLabel *InfoLabel;

    void setupUi(QWidget *VoxInfoDialog)
    {
        if (VoxInfoDialog->objectName().isEmpty())
            VoxInfoDialog->setObjectName(QString::fromUtf8("VoxInfoDialog"));
        VoxInfoDialog->resize(252, 201);
        verticalLayout = new QVBoxLayout(VoxInfoDialog);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        InfoScrollArea = new QScrollArea(VoxInfoDialog);
        InfoScrollArea->setObjectName(QString::fromUtf8("InfoScrollArea"));
        InfoScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        InfoScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 250, 199));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        InfoLabel = new QLabel(scrollAreaWidgetContents);
        InfoLabel->setObjectName(QString::fromUtf8("InfoLabel"));
        InfoLabel->setLineWidth(0);
        InfoLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        InfoLabel->setWordWrap(true);
        InfoLabel->setTextInteractionFlags(Qt::NoTextInteraction);

        verticalLayout_2->addWidget(InfoLabel);

        InfoScrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(InfoScrollArea);


        retranslateUi(VoxInfoDialog);

        QMetaObject::connectSlotsByName(VoxInfoDialog);
    } // setupUi

    void retranslateUi(QWidget *VoxInfoDialog)
    {
        VoxInfoDialog->setWindowTitle(QApplication::translate("VoxInfoDialog", "Voxel Info", 0, QApplication::UnicodeUTF8));
        InfoLabel->setText(QApplication::translate("VoxInfoDialog", "Voxel: Info", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VoxInfoDialog: public Ui_VoxInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VVOXINFO_H
