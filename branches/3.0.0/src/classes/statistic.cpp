#include "statistic.h"
#include <qmath.h>
#include <qnumeric.h>
#include "tradeDateCalendar.h"
#include "historicalNAV.h"

statistic::statistic(const historicalNAV &historicalNAV_):
    beginDate(historicalNAV_.beginDate()),
    endDate(historicalNAV_.endDate()),
    beginNAV(1),
    endNAV(1),
    beginTotalValue(0),
    endTotalValue(0),
    days(0),
    standardDeviation(0),
    maxChangePositive(0),
    maxChangePositiveDay(0),
    maxChangeNegative(0),
    maxChangeNegativeDay(0),
    minNAVValue(0),
    minNAVValueDay(0),
    maxNAVValue(0),
    maxNAVValueDay(0),
    minTotalValue(0),
    minTotalValueDay(0),
    maxTotalValue(0),
    maxTotalValueDay(0),
    expenseRatio(historicalNAV_.expenseRatio),
    costBasis(historicalNAV_.costBasis),
    taxLiability(historicalNAV_.taxLiability)
{
    days = historicalNAV_.count() - 1; // first is baseline nav, so discard
    if (days <= 0)
        return;

    beginNAV = historicalNAV_.nav(historicalNAV_.beginDate());
    beginTotalValue = historicalNAV_.totalValue(historicalNAV_.beginDate());
    endNAV = historicalNAV_.nav(historicalNAV_.endDate());
    endTotalValue = historicalNAV_.totalValue(historicalNAV_.endDate());
    minNAVValue = beginNAV;
    minNAVValueDay = historicalNAV_.beginDate();
    maxNAVValue = beginNAV;
    maxNAVValueDay = historicalNAV_.beginDate();
    minTotalValue = beginTotalValue;
    minTotalValueDay = historicalNAV_.beginDate();
    maxTotalValue = beginTotalValue;
    maxTotalValueDay = historicalNAV_.beginDate();

    double previousNAV = beginNAV;

    //http://www.johndcook.com/standard_deviation.html
    double oldM = 0;
    double oldS = 0;
    double newM = 0;
    double newS = 0;
    int count = 1;

    tradeDateCalendar calendar(historicalNAV_.beginDate());
    foreach(int date, ++calendar)
    {
        if (date > historicalNAV_.endDate())
            break;

        double newNav = historicalNAV_.nav(date);
        double change = newNav / previousNAV - 1;
        double totalValue = historicalNAV_.totalValue(date);

        if (totalValue > maxTotalValue)
        {
           maxTotalValue = totalValue;
           maxTotalValueDay = date;
        }

        if (totalValue < minTotalValue)
        {
           minTotalValue = totalValue;
           minTotalValueDay = date;
        }

        if (change > maxChangePositive || count == 1)
        {
            maxChangePositive = change;
            maxChangePositiveDay = date;
        }
        if (change < maxChangeNegative || count == 1)
        {
            maxChangeNegative = change;
            maxChangeNegativeDay = date;
        }
        if (newNav > maxNAVValue)
        {
            maxNAVValue = newNav;
            maxNAVValueDay = date;
        }
        if (newNav < minNAVValue)
        {
            minNAVValue = newNav;
            minNAVValueDay = date;
        }

        newM = oldM + (change - oldM) / count;
        newS = oldS + (change - oldM) * (change - newM);

        // set up for next iteration
        oldM = newM;
        oldS = newS;

        previousNAV = newNav;
        ++count;
    }

    // standard deviation
    if (--count != 1)
        standardDeviation = sqrt(newS / (count - 1));
}
