/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Aug 2 22:20:54 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *mainUpdatePrices;
    QAction *mainCompare;
    QAction *mainEdit;
    QAction *mainDelete;
    QAction *mainAdd_Portfolio;
    QAction *mainOptions;
    QAction *mainAbout;
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QProgressBar *progressBar;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        MainWindow->setIconSize(QSize(16, 16));
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        mainUpdatePrices = new QAction(MainWindow);
        mainUpdatePrices->setObjectName(QString::fromUtf8("mainUpdatePrices"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainUpdatePrices->setIcon(icon);
        mainCompare = new QAction(MainWindow);
        mainCompare->setObjectName(QString::fromUtf8("mainCompare"));
        mainCompare->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/chart.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainCompare->setIcon(icon1);
        mainEdit = new QAction(MainWindow);
        mainEdit->setObjectName(QString::fromUtf8("mainEdit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainEdit->setIcon(icon2);
        mainDelete = new QAction(MainWindow);
        mainDelete->setObjectName(QString::fromUtf8("mainDelete"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainDelete->setIcon(icon3);
        mainAdd_Portfolio = new QAction(MainWindow);
        mainAdd_Portfolio->setObjectName(QString::fromUtf8("mainAdd_Portfolio"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainAdd_Portfolio->setIcon(icon4);
        mainOptions = new QAction(MainWindow);
        mainOptions->setObjectName(QString::fromUtf8("mainOptions"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/options.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainOptions->setIcon(icon5);
        mainAbout = new QAction(MainWindow);
        mainAbout->setObjectName(QString::fromUtf8("mainAbout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icons/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainAbout->setIcon(icon6);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 460, 151, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(210, 460, 46, 14));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(360, 460, 118, 16));
        progressBar->setMaximumSize(QSize(16777215, 16));
        progressBar->setValue(24);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(170, 430, 171, 16));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(150, 160, 46, 14));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(220, 160, 100, 22));
        comboBox->setMinimumSize(QSize(100, 0));
        comboBox->setMaximumSize(QSize(100, 16777215));
        comboBox->setFrame(true);
        MainWindow->setCentralWidget(centralwidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        toolBar->addAction(mainUpdatePrices);
        toolBar->addSeparator();
        toolBar->addAction(mainCompare);
        toolBar->addSeparator();
        toolBar->addAction(mainEdit);
        toolBar->addAction(mainDelete);
        toolBar->addSeparator();
        toolBar->addAction(mainAdd_Portfolio);
        toolBar->addSeparator();
        toolBar->addAction(mainOptions);
        toolBar->addAction(mainAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "My Personal Index", 0, QApplication::UnicodeUTF8));
        mainUpdatePrices->setText(QApplication::translate("MainWindow", "&Update Prices", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainUpdatePrices->setToolTip(QApplication::translate("MainWindow", "Update Prices", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainUpdatePrices->setShortcut(QApplication::translate("MainWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
        mainCompare->setText(QApplication::translate("MainWindow", "Compare...", 0, QApplication::UnicodeUTF8));
        mainCompare->setIconText(QApplication::translate("MainWindow", "Compare..", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainCompare->setToolTip(QApplication::translate("MainWindow", "Compare...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainCompare->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        mainEdit->setText(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainEdit->setToolTip(QApplication::translate("MainWindow", "Ediy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainDelete->setToolTip(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainAdd_Portfolio->setText(QApplication::translate("MainWindow", "Add Portfolio", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainAdd_Portfolio->setToolTip(QApplication::translate("MainWindow", "Add Portfolio", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainOptions->setText(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainOptions->setToolTip(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mainAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mainAbout->setToolTip(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", " Last Updated: 7/31/2009 ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", " Index Start Date: 1/2/2009 ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Portfolio: ", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Matt's", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8)
        );
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
