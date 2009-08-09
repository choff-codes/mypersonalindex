#ifndef FRMMAIN_UI_H
#define FRMMAIN_UI_H

#include <QtGui>
#include "usercontrols/mpiToolButton.h"

class frmMain_UI
{
public:
    static const QString LAST_UPDATED_TEXT;
    static const QString INDEX_START_TEXT;
    static const QString STATUS_TEXT;
    static const QString DATE;

    QWidget *centralwidget;
    QHBoxLayout *centralgrid;

    QToolBar *mainToolbar;
    QAction *mainUpdatePrices;
    QAction *mainCompare;
    QAction *mainEdit;
    QAction *mainDelete;
    QAction *mainAdd_Portfolio;
    QAction *mainOptions;
    QAction *mainAbout;
    QLabel *mainPortfolioLabel;
    QComboBox *mainPortfolioCombo;

    QMenu *DateMenu;
    QCalendarWidget *DateCalendar;
    QWidgetAction *DateAction;

    QVBoxLayout *holdingsgrid;
    QToolBar *holdingsToolbar;
    QAction *holdingsAdd;
    QAction *holdingsEdit;
    QAction *holdingsDelete;
    QAction *holdingsShowHidden;
    QAction *holdingsExport;
    mpiToolButton *holdingsDateDropDown;

    QLabel *holdingsSortLabel;
    QComboBox *holdingsSortCombo;
    QTableView *holdings;

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
