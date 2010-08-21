#include "portfolioAttributes.h"

portfolioAttributes::portfolioAttributes(int id_):
    objectKey(objectType_Portfolio, QString(), id_),
    dividends(true),
    defaultCostBasis(costBasis_FIFO),
    startValue(100),
    aaThreshold(5),
    aaThresholdMethod(threshold_Portfolio),
    startDate(QDate::currentDate().toJulianDay()),
    holdingsShowHidden(true),
    navSortDesc(true),
    aaShowBlank(true),
    correlationShowHidden(true),
    acctShowBlank(true)
{}

bool portfolioAttributes::operator==(const portfolioAttributes &other_) const
{
    return objectKey::operator ==(other_)
            && this->dividends == other_.dividends
            && this->defaultCostBasis == other_.defaultCostBasis
            && this->startValue == other_.startValue
            && this->aaThreshold == other_.aaThreshold
            && this->aaThresholdMethod == other_.aaThresholdMethod
            && this->startDate == other_.startDate;
}
