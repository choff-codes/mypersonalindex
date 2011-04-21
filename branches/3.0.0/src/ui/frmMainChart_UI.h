#ifndef FRMMAINCHART_UI_H
#define FRMMAINCHART_UI_H

#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include "frmMainTableViewTree_UI.h"
#include "mpiChart.h"

class frmMainChart_UI: public frmMainTableViewTree_UI
{
public:
    mpiChart *chart;
    QwtPlotGrid *chartGridLines;
    QwtPlotZoomer *chartZoomer;
    QwtLegend *chartLegend;

    virtual ~frmMainChart_UI() {}

    virtual void setupUI(QWidget *parent_ = 0);

private:
    mpiTableView *table;
    QShortcut *tableCopy;
};

#endif // FRMMAINCHART_UI_H
