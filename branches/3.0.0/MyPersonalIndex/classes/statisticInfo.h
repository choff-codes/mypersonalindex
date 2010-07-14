#ifndef STATISTICINFO_H
#define STATISTICINFO_H

#include <qmath.h>
#include <qnumeric.h>
#include "historicalNAV.h"
#include "tradeDateCalendar.h"

class statisticInfo
{
public:
    statisticInfo(const historicalNAV &historicalNAV_, double beginNAV_ = 1);

    const historicalNAV endInfo() const { return m_endInfo; }
    int startDate() const { return m_endInfo.firstDate(); }
    int endDate() const { return m_endInfo.lastDate(); }
    double startNAV() const { return m_startNAV; }
    double endNAV() const { return m_endNAV; }
    double startTotalValue() const { return m_startTotalValue; }
    double endTotalValue() const { return m_endTotalValue; }
    int days() const { return m_count; }
    double standardDeviation() const { return m_stdDev; }
    double maxChangePositive() const { return m_maxChangePositive; }
    int maxChangePositiveDay() const { return m_maxChangePositiveDay; }
    double maxChangeNegative() const { return m_maxChangeNegative; }
    int maxChangeNegativeDay() const { return m_maxChangeNegativeDay; }
    double minNAVValue() const { return m_minNAVValue; }
    int minNAVValueDay() const { return m_minNAVValueDay; }
    double maxNAVValue() const { return m_maxNAVValue; }
    int maxNAVValueDay() const { return m_maxNAVValueDay; }
    double minTotalValue() const { return m_minTotalValue; }
    int minTotalValueDay() const { return m_minTotalValueDay; }
    double maxTotalValue() const { return m_maxTotalValue; }
    int maxTotalValueDay() const { return m_maxTotalValueDay; }
    double expenseRatio() const { return m_endInfo.expenseRatio; }

private:
    const historicalNAV m_endInfo;
    double m_startNAV;
    double m_startTotalValue;
    double m_endNAV;
    double m_endTotalValue;
    int m_count;
    double m_stdDev;
    double m_maxChangePositive;
    int m_maxChangePositiveDay;
    double m_maxChangeNegative;
    int m_maxChangeNegativeDay;
    double m_minNAVValue;
    int m_minNAVValueDay;
    double m_maxNAVValue;
    int m_maxNAVValueDay;
    double m_minTotalValue;
    int m_minTotalValueDay;
    double m_maxTotalValue;
    int m_maxTotalValueDay;
};

#endif // STATISTICINFO_H
