#ifndef MPIPLOTZOOMER_H
#define MPIPLOTZOOMER_H

#include <qwt_plot_zoomer.h>

class mpiPlotZoomer: public QwtPlotZoomer
{
public:
    mpiPlotZoomer(QwtPlotCanvas *canvas_):
        QwtPlotZoomer(canvas_, false)
    {
        setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerText(const QwtDoublePoint &pos_) const;
};

#endif // MPIPLOTZOOMER_H
