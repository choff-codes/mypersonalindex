#ifndef FRMCOMPARE_UI_H
#define FRMCOMPARE_UI_H

#include <QtGui>
#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_layout.h"
#include "mpiDateEdit.h"
#include "mpiTableView.h"
#include "mpiDateScale.h"
#include "mpiPercentScale.h"

class frmCompare_UI
{
public:
    QGridLayout *mainLayout;
    QTreeWidget *tree;
    QTabWidget *tab;
    QWidget *tabCorrelation;
    QWidget *tabChart;
    QWidget *tabStat;
    QVBoxLayout *correlationLayout;
    QVBoxLayout *chartLayout;
    QVBoxLayout *statLayout;
    mpiTableView *correlations;
    QShortcut *correlationsCopyShortcut;
    mpiTableView *stats;
    QShortcut *statsCopyShortcut;
    QwtPlot *chart;
    QwtPlotGrid *chartGridLines;
    QwtPlotZoomer *chartZoomer;
    QDialogButtonBox *btnOk;
    QToolBar *mainToolbar;
    QAction *mainRefresh;
    QAction *mainIncludeDividends;
    QAction *mainExport;
    QLabel *mainStartDate;
    mpiDateEdit *mainStartDateDropDown;
    QLabel *mainEndDate;
    mpiDateEdit *mainEndDateDropDown;
    QTreeWidgetItem *treePortfolios;
    QTreeWidgetItem *treeAccounts;
    QTreeWidgetItem *treeAssetAllocations;
    QTreeWidgetItem *treeSecurities;
    QTreeWidgetItem *treeSymbols;

    void setupUI(QDialog* dialog);

    ~frmCompare_UI() { delete chartGridLines; delete chartZoomer; }
};

#endif // FRMCOMPARE_UI_H
