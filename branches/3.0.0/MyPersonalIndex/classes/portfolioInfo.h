#ifndef PORTFOLIOINFO_H
#define PORTFOLIOINFO_H

#include <QtCore>
#include "queries.h"
#include "account.h"

class portfolioInfo
{
public:
    enum thesholdMethod { threshold_Portfolio, theshold_AA };

    int id;
    QString description;
    bool dividends;
    account::costBasisType costBasis;
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

    portfolioInfo();
    void save();

    bool operator==(const portfolioInfo &other) const;
    bool operator!=(const portfolioInfo &other) const { return !(*this == other); }
};

#endif // PORTFOLIOINFO_H
