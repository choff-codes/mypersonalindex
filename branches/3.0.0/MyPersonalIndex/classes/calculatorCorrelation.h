#ifndef CALCULATORCORRELATION_H
#define CALCULATORCORRELATION_H

#include <qmath.h>
#include <qnumeric.h>
#include "tradeDateCalendar.h"
#include "historicalNAV.h"

class calculatorCorrelation
{
public:
    static double correlation(const historicalNAV &first_, const historicalNAV &second_);
};

#endif // CALCULATORCORRELATION_H
