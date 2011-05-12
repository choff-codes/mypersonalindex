#ifndef MPICHARTCURVE_H
#define MPICHARTCURVE_H

#include <QVector>

class QwtPlot;
class QwtPlotCurve;
class mpiChartCurve
{
public:
    mpiChartCurve(QwtPlot *chart_);

    ~mpiChartCurve();

    void attach();
    // call before deleting the curve, but only if the parent plot is not being destroyed
    void detach();
    QwtPlotCurve* curve() const { return m_curve; }

    void append(double x_, double y_) { m_xData.append(x_); m_yData.append(y_); }
    int count() const { return m_xData.count(); }

private:
    QwtPlot *m_chart;
    QwtPlotCurve *m_curve;
    QVector<double> m_xData;
    QVector<double> m_yData;
};

#endif // MPICHARTCURVE_H
