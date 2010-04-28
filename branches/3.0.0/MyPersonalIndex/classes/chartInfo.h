#ifndef CHARTINFO_H
#define CHARTINFO_H

#include <QtCore>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class chartInfo
{
public:
    chartInfo(): m_curve(0) {}

    void setCurve(QwtPlotCurve *curve);
    void attach(QwtPlot *chart);
    const QwtPlotCurve* curve() const { return m_curve; }

    void append(double x, double y) { m_xData.append(x); m_yData.append(y); }
    int count() const { return m_xData.count(); }
    double firstX() const { return count() == 0 ? 0 : m_xData.first(); }
    double lastX() const { return count() == 0 ? 0 : m_xData.last(); };

private:
    QwtPlotCurve *m_curve;
    QVector<double> m_xData;
    QVector<double> m_yData;
};

#endif // CHARTINFO_H
