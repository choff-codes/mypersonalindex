#ifndef FRMCOMPARE_UI_H
#define FRMCOMPARE_UI_H

#include <QtGui>
#include "qwt_plot.h"

class frmCompare_UI
{
public:
    QGridLayout *mainLayout;
    QTreeWidget *lstSelected;
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

    void setupUI(QDialog* dialog);
};

#endif // FRMCOMPARE_UI_H
