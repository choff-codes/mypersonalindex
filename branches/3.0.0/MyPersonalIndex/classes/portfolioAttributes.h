#ifndef PORTFOLIOATTRIBUTES_H
#define PORTFOLIOATTRIBUTES_H

#include <QString>
#include <QDate>
#include "queries.h"
#include "costBasis.h"
#include "objectKey.h"

class portfolioAttributes: public objectKey
{
public:
    enum thesholdMethod {
        threshold_Portfolio,
        theshold_AA
    };

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

    portfolioAttributes(const int &id_ = -1);
    void save(const queries &dataSource_);

    bool operator==(const portfolioAttributes &other_) const;
    bool operator!=(const portfolioAttributes &other_) const { return !(*this == other_); }
};

#endif // PORTFOLIOATTRIBUTES_H
