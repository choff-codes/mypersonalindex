#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QSharedData>

#ifdef CLOCKTIME
#include <QTime>
#endif

class portfolio;
class objectKey;
class snapshot;
class snapshotSecurity;
class historicalNAV;
class calculatorNAVData;
class calculatorNAV
{
public:
    calculatorNAV(const portfolio &portfolio_);
    calculatorNAV(const calculatorNAV &other_);
    ~calculatorNAV();

    calculatorNAV& operator=(const calculatorNAV &other_);

    snapshotSecurity securitySnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot portfolioSnapshot(int date_, int priorDate_ = 0);
    snapshot assetAllocationSnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot accountSnapshot(int date_, int id_, int priorDate_ = 0);
    snapshot symbolSnapshot(int date_, int id_, int beginDate_);

    historicalNAV changeOverTime(const objectKey &key_, int beginDate_, int endDate_, bool dividends_, double navValue_ = 1);

private:
    QExplicitlySharedDataPointer<calculatorNAVData> d;

    snapshot snapshotByKey(int date_, const objectKey &key_, int beginDate_, int priorDate_);
    int beginDateByKey(const objectKey &key_);
    int endDateByKey(const objectKey &key_);

    double change(double beginValue_, double endValue_, double activity_, double dividends_, double beginNAV_ = 1);
};


#endif // CALCULATIONS_H
