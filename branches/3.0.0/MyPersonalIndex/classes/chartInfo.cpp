#include "chartInfo.h"

void chartInfo::setCurve(QwtPlotCurve *curve_)
{
    if (m_curve)
    {
        m_curve->detach();
        delete m_curve;
        m_xData.clear();
        m_yData.clear();
    }
    m_curve = curve_;
}

void chartInfo::attach(QwtPlot *chart_)
{
    if (!m_curve)
        return;

    m_curve->setRawSamples(&m_xData[0], &m_yData[0], count());
    m_curve->attach(chart_);
}
