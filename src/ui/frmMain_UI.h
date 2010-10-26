#ifndef FRMMAIN_UI_H
#define FRMMAIN_UI_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>

class frmMain_UI
{
public:
    QMenu *file;
    QAction *fileNew;
    QAction *fileOpen;
    QMenu *fileRecent;
    QAction *fileSave;
    QAction *fileSaveAs;
    QAction *fileExit;
    QMenu *view;
    QAction *viewSummary;
    QAction *viewTrades;
    QAction *viewAccounts;
    QAction *viewAssetAllocation;
    QAction *viewSecurities;
    QAction *viewStatistics;
    QAction *viewCharts;
    QAction *viewCorrelations;
    QAction *viewPerformance;
    QMenu *portfolio;
    QAction *portfolioAdd;
    QAction *portfolioEdit;
    QAction *portfolioDelete;
    QMenu *import;
    QAction *importYahoo;
    QAction *importPrices;
    QAction *importPortfolio;
    QAction *importFile;
    QMenu *help;
    QAction *helpAbout;
    QMenuBar *menubar;
    QWidget *portfolioDropDownWidget;
    QHBoxLayout *portfolioDropDownLayout;
    QLabel *portfolioDropDown;
    QComboBox *portfolioDropDownCmb;

    void setupUI(QMainWindow *mainWindow);
};

#endif // FRMMAIN_UI_H
