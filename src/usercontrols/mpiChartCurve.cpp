#include "mpiChartCurve.h"
#include <qwt_plot_curve.h>

mpiChartCurve::mpiChartCurve(QwtPlot *chart_):
    m_chart(chart_),
    m_curve(new QwtPlotCurve())
{
}

mpiChartCurve::~mpiChartCurve()
{
    QVector<double> x, y;
    m_curve->detach();
    // hack for now?  qwt doesn't seem to redraw properly until a curve is attached after a detachment, so attach dummy
    m_curve->setRawData(x.constData(), y.constData(), 0);
    m_curve->attach(m_chart);
    m_curve->detach();
    delete m_curve;
}

void mpiChartCurve::attach()
{
    m_curve->setRawData(m_xData.constData(),m_yData.constData(), count());
    m_curve->attach(m_chart);
}
