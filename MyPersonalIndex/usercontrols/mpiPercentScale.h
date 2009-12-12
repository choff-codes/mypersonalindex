#ifndef MPIPERCENTSCALE_H
#define MPIPERCENTSCALE_H

#include <QtGui>
#include <qwt_scale_draw.h>
#include "functions.h"

class mpiPercentScale: public QwtScaleDraw
{
public:

    QwtText label(double v) const
    {
        return functions::doubleToPercentage(v);
    }
};

#endif // MPIPERCENTSCALE_H
