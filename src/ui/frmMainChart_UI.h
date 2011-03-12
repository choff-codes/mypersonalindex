#ifndef FRMMAINCHART_UI_H
#define FRMMAINCHART_UI_H

#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSplitter>
#include <QComboBox>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include "frmMainToolbar_UI.h"
#include "mpiChart.h"

class frmMainChart_UI: public frmMainToolbar_UI
{
public:
    QWidget *widget;
    QVBoxLayout *layout;
    QSplitter *chartSplitter;
    QWidget *treeWidget;
    QVBoxLayout *treeLayout;
    QComboBox *treeCmb;
    QTreeWidget *tree;
    mpiChart *chart;
    QwtPlotGrid *chartGridLines;
    QwtPlotZoomer *chartZoomer;
    QwtLegend *chartLegend;

    virtual ~frmMainChart_UI() {}

    virtual void setupUI(QWidget *parent_ = 0);
};

#endif // FRMMAINCHART_UI_H
