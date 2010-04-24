#ifndef FRMCOMPARE_UI_H
#define FRMCOMPARE_UI_H

#include <QtGui>
#include "qwt_plot.h"
#include "mpiDateEdit.h"

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
    QTableView *correlations;
    QTableView *stats;
    QwtPlot *chart;
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
};

#endif // FRMCOMPARE_UI_H
