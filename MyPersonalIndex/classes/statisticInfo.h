#ifndef STATISTICINFO_H
#define STATISTICINFO_H

#include <QtCore>
#include "portfolio.h"
#include "calculations.h"

class statisticInfo
{
public:
    statisticInfo(const portfolio *currentPortfolio, const dailyInfoPortfolio *info, const int &startDate, const int &previousClose);

    const dailyInfoPortfolio* endInfo() const { return m_endInfo; }
    int startDate() const { return m_startDate; }
    int previousCloseDate() const { return m_previousClose; }
    int endDate() const { return m_endInfo->date; }
    double startNAV() const { return m_startNAV; }
    double endNAV() const { return m_endNAV; }
    double startTotalValue() const { return m_startTotalValue; }
    double endTotalValue() const { return m_endTotalValue; }
    int days() const { return m_count; }
    double standardDeviation() const { return m_stdDev; }
    int maxChangePositiveDay() { return m_maxChangePositiveDay; }
    double maxChangeNegative() { return m_maxChangeNegative; }
    int maxChangeNegativeDay() { return m_maxChangeNegativeDay; }
    double minNAVValue() { return m_minNAVValue; }
    int minNAVValueDay() { return m_minNAVValueDay; }
    double maxNAVValue() { return m_maxNAVValue; }
    int maxNAVValueDay() { return m_maxNAVValueDay; }
    double minTotalValue() { return m_minTotalValue; }
    int minTotalValueDay() { return m_minTotalValueDay; }
    double maxTotalValue() { return m_maxTotalValue; }
    int maxTotalValueDay() { return m_maxTotalValueDay; }

private:
    const portfolio *m_portfolio;
    const dailyInfoPortfolio *m_endInfo;
    const int m_startDate;
    const int m_previousClose;
    double m_startNAV;
    double m_startTotalValue;
    double m_endNAV;
    double m_endTotalValue;
    double m_count;
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

    void setNAV();
    void setTotalValue();
};

#endif // STATISTICINFO_H
