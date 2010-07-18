#ifndef MPICHARTCURVE_H
#define MPICHARTCURVE_H

#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class mpiChartCurve
{
public:
    mpiChartCurve(): m_curve(0) {}
    ~mpiChartCurve() { setCurve(0); }

    void setCurve(QwtPlotCurve *curve_);
    void attach(QwtPlot *chart_);
    const QwtPlotCurve* curve() const { return m_curve; }

    void append(double x_, double y_) { m_xData.append(x_); m_yData.append(y_); }
    int count() const { return m_xData.count(); }
    double firstX() const { return count() == 0 ? 0 : m_xData.first(); }
    double lastX() const { return count() == 0 ? 0 : m_xData.last(); };

private:
    QwtPlotCurve *m_curve;
    QVector<double> m_xData;
    QVector<double> m_yData;
};

#endif // MPICHARTCURVE_H
