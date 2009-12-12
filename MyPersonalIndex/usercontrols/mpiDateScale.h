#ifndef MPIDATESCALE_H
#define MPIDATESCALE_H

#include <QtGui>
#include <qwt_scale_draw.h>
#include "globals.h"

class mpiDateScale: public QwtScaleDraw
{
public:

    QwtText label(double v) const
    {
        return QDate::fromJulianDay((int)v).toString(globals::shortDateFormat);
    }
};

#endif // MPIDATESCALE_H
