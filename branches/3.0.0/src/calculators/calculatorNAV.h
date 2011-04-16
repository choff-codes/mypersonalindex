#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QHash>

class portfolio;
class symbol;
class objectKeyBase;
class snapshot;
class snapshotSecurity;
class historicalNAV;
class calculatorNAV
{
public:
    /* Note:
       Each instance of this class should only be called with a portfolio pointer with the same underlying
       shared data.  Since the portfolio class is a shared data pointer, there is no constant pointer to a
       parent object as the underlying data is passed around.  Thus it has to be passed into each call.
    */

    void clearCache() {  m_securitiesCache.clear(); }

    // priorDate is a shortcut to pull data from cache, it is optional but can greatly speed up calculations
    snapshotSecurity securitySnapshot(const portfolio *portfolio_, int date_, int id_, int priorDate_ = 0);
    snapshot portfolioSnapshot(const portfolio *portfolio_, int date_, int priorDate_ = 0);
    snapshot assetAllocationSnapshot(const portfolio *portfolio_, int date_, int id_, int priorDate_ = 0);
    snapshot accountSnapshot(const portfolio *portfolio_, int date_, int id_, int priorDate_ = 0);
    // since there are no trades to start from like the methods above, a begin date is required for splits
    snapshot symbolSnapshot(int date_, const symbol &key_, int beginDate_);

    double nav(const portfolio *portfolio_, const objectKeyBase &key_, int beginDate_, int endDate_);
    historicalNAV changeOverTime(const portfolio *portfolio_, const objectKeyBase &key_, int beginDate_, int endDate_);

private:
    QHash<int, QHash<int, snapshotSecurity> > m_securitiesCache;

    snapshot snapshotByKey(const portfolio *portfolio_, int date_, const objectKeyBase &key_, int beginDate_, int priorDate_);
    int beginDateByKey(const portfolio *portfolio_, const objectKeyBase &key_);
    int endDateByKey(const objectKeyBase &key_);

    static double change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_);
};

#endif // CALCULATIONS_H
