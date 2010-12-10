#ifndef FRMMAIN_UI_H
#define FRMMAIN_UI_H

#include <QMainWindow>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>

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
    QActionGroup *viewGroup;
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
    QStackedWidget *cornerWidget;
    QProgressBar *progressBar;
    QWidget *portfolioDropDownWidget;
    QHBoxLayout *portfolioDropDownLayout;
    QLabel *portfolioDropDown;
    QComboBox *portfolioDropDownCmb;
    QStackedWidget *centralWidget;

    void setupUI(QMainWindow *mainWindow_);
};

#endif // FRMMAIN_UI_H
