#ifndef FRMMAIN_UI_H
#define FRMMAIN_UI_H

#include <QtGui>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include "qwt_plot_layout.h"
#include "qwt_plot_canvas.h"
#include "qwt_text_engine.h"
#include "mpiDateEdit.h"
#include "mpiTableView.h"
#include "mpiChart.h"
#include "mpiDateScale.h"
#include "mpiPercentScale.h"

class frmMain_UI
{
public:
    static const QString LAST_UPDATED_TEXT;
    static const QString INDEX_START_TEXT;
    static const QString STATUS_TEXT;
    static const QString DATE;
    static const QString START_DATE;
    static const QString END_DATE;
    static const QString WINDOW_TITLE;
    static const QString BUSY;

    QWidget *centralwidget;
    QHBoxLayout *centralgrid;

    QToolBar *mainToolbar;
    QAction *mainUpdatePrices;
    QAction *mainCompare;
    QAction *mainEdit;
    QAction *mainImport;
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
    mpiDateEdit *holdingsDateDropDown;
    QLabel *holdingsSortLabel;
    QComboBox *holdingsSortCombo;
    mpiTableView *holdings;
    QShortcut *holdingsCopyShortcut;

    QVBoxLayout *statGrid;
    QToolBar *statToolbar;
    QAction *statEdit;
    QAction *statExport;
    QLabel *statStartDate;
    mpiDateEdit *statStartDateDropDown;
    QLabel *statEndDate;
    mpiDateEdit *statEndDateDropDown;
    mpiTableView *stat;
    QShortcut *statCopyShortcut;

    QVBoxLayout *chartGrid;
    QToolBar *chartToolbar;
    QAction *chartExport;
    QLabel *chartStartDate;
    mpiDateEdit *chartStartDateDropDown;
    QLabel *chartEndDate;
    mpiDateEdit *chartEndDateDropDown;
    mpiChart *chart;
    QwtPlotGrid *chartGridLines;
    QwtPlotZoomer *chartZoomer;

    QVBoxLayout *performanceGrid;
    QToolBar *performanceToolbar;
    QAction *performanceSortDesc;
    QAction *performanceExport;
    mpiTableView *performance;
    QShortcut *performanceCopyShortcut;

    QVBoxLayout *correlationsGrid;
    QToolBar *correlationsToolbar;
    QAction *correlationsShowHidden;
    QAction *correlationsExport;
    QLabel *correlationsStartDate;
    mpiDateEdit *correlationsStartDateDropDown;
    QLabel *correlationsEndDate;
    mpiDateEdit *correlationsEndDateDropDown;
    mpiTableView *correlations;
    QShortcut *correlationsCopyShortcut;

    QVBoxLayout *accountsGrid;
    QToolBar *accountsToolbar;
    QAction *accountsAdd;
    QAction *accountsEdit;
    QAction *accountsDelete;
    QAction *accountsShowBlank;
    QAction *accountsReorderColumns;
    QAction *accountsExport;
    QLabel *accountsDate;
    mpiDateEdit *accountsDateDropDown;
    QLabel *accountsSortLabel;
    QComboBox *accountsSortCombo;
    mpiTableView *accounts;
    QShortcut *accountsCopyShortcut;

    QVBoxLayout *aaGrid;
    QToolBar *aaToolbar;
    QAction *aaAdd;
    QAction *aaEdit;
    QAction *aaDelete;
    QAction *aaShowBlank;
    QAction *aaReorderColumns;
    QAction *aaExport;
    QLabel *aaDate;
    mpiDateEdit *aaDateDropDown;
    QLabel *aaSortLabel;
    QComboBox *aaSortCombo;
    mpiTableView *aa;
    QShortcut *aaCopyShortcut;

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

    void setupUI(QMainWindow *mainWindow);

    ~frmMain_UI() { delete chartGridLines; delete chartZoomer; }
};

#endif // FRMMAIN_UI_H
