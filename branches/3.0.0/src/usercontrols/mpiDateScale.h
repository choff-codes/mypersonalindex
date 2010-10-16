#ifndef MPIDATESCALE_H
#define MPIDATESCALE_H

#include <QDate>
#include <qwt_scale_draw.h>

class mpiDateScale: public QwtScaleDraw
{
public:

    QwtText label(double v_) const
    {
        return QDate::fromJulianDay((int)v_).toString(Qt::SystemLocaleShortDate);
    }
};

#endif // MPIDATESCALE_H
