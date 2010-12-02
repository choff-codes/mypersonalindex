#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QSharedData>

class portfolio;
class symbol;
class objectKeyBase;
class snapshot;
class snapshotSecurity;
class historicalNAV;
class calculatorNAVData;
class calculatorNAV
{
public:
    calculatorNAV();
    calculatorNAV(const portfolio &portfolio_);
    calculatorNAV(const calculatorNAV &other_);
    ~calculatorNAV();

    calculatorNAV& operator=(const calculatorNAV &other_);

    void setPortfolio(const portfolio &portfolio_);

    snapshotSecurity securitySnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot portfolioSnapshot(int date_, int priorDate_ = 0);
    snapshot assetAllocationSnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot accountSnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot symbolSnapshot(int date_, const symbol &key_, int beginDate_);

    double nav(const objectKeyBase &key_, int beginDate_, int endDate_);
    historicalNAV changeOverTime(const objectKeyBase &key_, int beginDate_, int endDate_);

private:
    QExplicitlySharedDataPointer<calculatorNAVData> d;

    snapshot snapshotByKey(int date_, const objectKeyBase &key_, int beginDate_, int priorDate_);
    int beginDateByKey(const objectKeyBase &key_);
    int endDateByKey(const objectKeyBase &key_);

    double change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_);
};

#endif // CALCULATIONS_H
