#ifndef MPICHART_H
#define MPICHART_H

#include <QObject>
#include "qwt_plot.h"


class mpiChart : public QwtPlot
{
    Q_OBJECT

public:
    mpiChart(QWidget *parent_ = 0):
        QwtPlot(parent_)
    {}

public slots:
    void exportChart();
};

#endif // MPICHART_H
