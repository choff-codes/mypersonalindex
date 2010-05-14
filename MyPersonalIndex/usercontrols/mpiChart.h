#ifndef MPICHART_H
#define MPICHART_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "qwt_plot.h"

class mpiChart : public QwtPlot
{
    Q_OBJECT

public:
    mpiChart(QWidget *parent = 0): QwtPlot(parent) {}

public slots:
    void exportChart();
};

#endif // MPICHART_H
