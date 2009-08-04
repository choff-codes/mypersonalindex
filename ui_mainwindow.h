/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Mon Aug 3 22:12:50 2009
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
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
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
    QLabel *label_2;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QComboBox *comboBox;
    QLabel *label_4;
    QLabel *label;
    QProgressBar *progressBar;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget;
    QWidget *tab_2;
    QWidget *tab_3;
    QWidget *tab_4;
    QWidget *tab_5;
    QWidget *tab_6;
    QWidget *tab_7;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
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
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(210, 460, 46, 14));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setMargin(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(100, 0));
        comboBox->setMaximumSize(QSize(100, 16777215));
        comboBox->setFrame(true);

        gridLayout->addWidget(comboBox, 1, 1, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 2, 0, 1, 2);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 3, 0, 1, 2);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 16));
        progressBar->setValue(24);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(false);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);

        gridLayout->addWidget(progressBar, 3, 2, 1, 1);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setMargin(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 11)
            tableWidget->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        tabWidget->addTab(tab_7, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

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

        tabWidget->setCurrentIndex(0);


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
        label_2->setText(QApplication::translate("MainWindow", " Index Start Date: 1/2/2009 ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Portfolio: ", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Matt's", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Test", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QString());
        label->setText(QApplication::translate("MainWindow", " Last Updated: 7/31/2009 ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Active", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Ticker", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Closing Price", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Shares", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Avg Price Per Share", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Cost Basis", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Total Value", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Gain/Loss", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "Gain/Loss %", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "Account", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "Asset Allocation", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Holdings", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Statistics", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Chart", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Performance", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Correlations", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("MainWindow", "Accounts", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("MainWindow", "Asset Allocation", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
