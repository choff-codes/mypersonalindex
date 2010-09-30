#ifndef MPIPERCENTSCALE_H
#define MPIPERCENTSCALE_H

#include <qwt_scale_draw.h>
#include "functions.h"

class mpiPercentScale: public QwtScaleDraw
{
public:

    QwtText label(double v_) const
    {
        return functions::doubleToPercentage(v_);
    }
};

#endif // MPIPERCENTSCALE_H
