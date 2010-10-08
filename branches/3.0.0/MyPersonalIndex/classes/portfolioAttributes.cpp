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

void portfolioAttributes::save(const queries &dataSource_)
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_Description), description);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_StartValue), startValue);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AAThreshold), aaThreshold);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_ThresholdMethod), (int)aaThresholdMethod);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_CostBasis), (int)defaultCostBasis);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_StartDate), startDate);
    values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_Dividends), (int)dividends);

    this->id = dataSource_.insert(queries::table_Portfolio, values, this->id);
}

QString portfolioAttributes::validate()
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    if (this->startValue < 1 || this->startValue > 1000000)
        return "Beginning NAV must be between 1 and 1000000!";

    return QString();
}
