#ifndef CALCULATORCORRELATION_H
#define CALCULATORCORRELATION_H

class historicalNAV;
class calculatorCorrelation
{
public:
    static double correlation(const historicalNAV &first_, const historicalNAV &second_);
};

#endif // CALCULATORCORRELATION_H
