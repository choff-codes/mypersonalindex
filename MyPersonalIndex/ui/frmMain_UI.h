#ifndef FRMMAIN_UI_H
#define FRMMAIN_UI_H

#include <QtGui>

class frmMain_UI
{
public:
    static const QString LAST_UPDATED_TEXT;
    static const QString INDEX_START_TEXT;
    static const QString STATUS_TEXT;
    static const QString DATE;
    static const QString START_DATE;
    static const QString END_DATE;

    QWidget *centralwidget;
    QHBoxLayout *centralgrid;

    QToolBar *mainToolbar;
    QAction *mainUpdatePrices;
    QAction *mainCompare;
    QAction *mainEdit;
    QAction *mainDelete;
    QAction *mainAdd;
    QAction *mainOptions;
    QAction *mainAbout;
    QLabel *mainPortfolioLabel;
    QComboBox *mainPortfolioCombo;

    QVBoxLayout *holdingsGrid;
    QToolBar *holdingsToolbar;
    QAction *holdingsAdd;
    QAction *holdingsEdit;
    QAction *holdingsDelete;
    QAction *holdingsShowHidden;
    QAction *holdingsReorderColumns;
    QAction *holdingsExport;
    QLabel *holdingsDate;
    QDateEdit *holdingsDateDropDown;
    QLabel *holdingsSortLabel;
    QComboBox *holdingsSortCombo;
    QTableView *holdings;

    QVBoxLayout *statGrid;
    QToolBar *statToolbar;
    QAction *statAddEdit;
    QAction *statExport;
    QLabel *statStartDate;
    QDateEdit *statStartDateDropDown;
    QLabel *statEndDate;
    QDateEdit *statEndDateDropDown;
    QTableView *stat;

    QVBoxLayout *chartGrid;
    QToolBar *chartToolbar;
    QAction *chartExport;
    QLabel *chartStartDate;
    QDateEdit *chartStartDateDropDown;
    QLabel *chartEndDate;
    QDateEdit *chartEndDateDropDown;
    QTableView *chart;

    QVBoxLayout *performanceGrid;
    QToolBar *performanceToolbar;
    QAction *performanceSortDesc;
    QAction *performanceExport;
    QTableView *performance;

    QVBoxLayout *correlationsGrid;
    QToolBar *correlationsToolbar;
    QAction *correlationsCalculate;
    QAction *correlationsShowHidden;
    QAction *correlationsExport;
    QLabel *correlationsStartDate;
    QDateEdit *correlationsStartDateDropDown;
    QLabel *correlationsEndDate;
    QDateEdit *correlationsEndDateDropDown;
    QTableView *correlations;

    QVBoxLayout *accountsGrid;
    QToolBar *accountsToolbar;
    QAction *accountsEdit;
    QAction *accountsShowBlank;
    QAction *accountsExport;
    QLabel *accountsDate;
    QDateEdit *accountsDateDropDown;
    QLabel *accountsSortLabel;
    QComboBox *accountsSortCombo;
    QTableView *accounts;

    QVBoxLayout *aaGrid;
    QToolBar *aaToolbar;
    QAction *aaEdit;
    QAction *aaShowBlank;
    QAction *aaExport;
    QLabel *aaDate;
    QDateEdit *aaDateDropDown;
    QLabel *aaSortLabel;
    QComboBox *aaSortCombo;
    QTableView *aa;

    QTabWidget *tab;
    QWidget *tab_holdings;
    QWidget *tab_stat;
    QWidget *tab_chart;
    QWidget *tab_performance;
    QWidget *tab_correlations;
    QWidget *tab_accounts;
    QWidget *tab_aa;

    QStatusBar *status;
    QLabel *stbLastUpdated;
    QLabel *stbStartDate;
    QLabel *stbStatus;
    QProgressBar *stbProgress;

    void setupUI(QMainWindow *MainWindow);
};

#endif // FRMMAIN_UI_H
