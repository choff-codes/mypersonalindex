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
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>
#include <QTabBar>
#include <QToolButton>

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
    QAction *viewAccounts;
    QAction *viewAssetAllocation;
    QAction *viewSecurities;
    QAction *viewTrades;
    QAction *viewStatistics;
    QAction *viewCharts;
    QAction *viewCorrelations;
    QAction *viewPerformance;
    QMenu *portfolio;
    QAction *portfolioAdd;
    QAction *portfolioEdit;
    QAction *portfolioDelete;
    QAction *portfolioImport;
    QAction *portfolioImportFile;
    QMenu *price;
    QAction *priceDownload;
    QAction *priceImport;
    QAction *priceClear;
    QMenu *help;
    QAction *helpAbout;
    QMenuBar *menubar;
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QHBoxLayout *layoutTop;
    QComboBox *portfolioTabsViewCmb;
    QTabBar *portfolioTabs;
    QToolButton *portfolioTabsEdit;
    QToolButton *portfolioTabsDelete;
    QToolButton *portfolioTabsAdd;
    QFrame *portfolioTabsSep;
    QStackedWidget *progressWidget;
    QToolButton *progressUpdateBtn;
    QProgressBar *progressUpdateBar;
    QStackedWidget *viewWidget;

    void setupUI(QMainWindow *mainWindow_);
};

#endif // FRMMAIN_UI_H
