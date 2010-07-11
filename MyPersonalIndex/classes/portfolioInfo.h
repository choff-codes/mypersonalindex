#ifndef PORTFOLIOINFO_H
#define PORTFOLIOINFO_H

#include <QString>
#include <QDate>
#include "queries.h"
#include "account.h"
#include "objectKey.h"

class portfolioInfo: public objectKey
{
public:
    enum thesholdMethod { threshold_Portfolio, theshold_AA };

    bool dividends;
    costBasis defaultCostBasis;
    int startValue;
    int aaThreshold;
    thesholdMethod aaThresholdMethod;
    int startDate;
    bool holdingsShowHidden;
    bool navSortDesc;
    bool aaShowBlank;
    bool correlationShowHidden;
    bool acctShowBlank;
    QString holdingsSort;
    QString aaSort;
    QString acctSort;

    portfolioInfo(const int &id_ = -1);
    void save(const queries &dataSource);

    bool operator==(const portfolioInfo &other) const;
    bool operator!=(const portfolioInfo &other) const { return !(*this == other); }
};

#endif // PORTFOLIOINFO_H
