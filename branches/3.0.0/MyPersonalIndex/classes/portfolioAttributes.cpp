#include "portfolioAttributes.h"

portfolioAttributes::portfolioAttributes(const int &id_ = -1):
    objectKey(objectType_Portfolio, QString(), id_),
    dividends(true),
    costBasis(costBasis_FIFO),
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
    return this->key() == other_.key()
            && this->dividends == other_.dividends
            && this->costBasis == other_.costBasis
            && this->startValue == other_.startValue
            && this->aaThreshold == other_.aaThreshold
            && this->aaThresholdMethod == other_.aaThresholdMethod
            && this->startDate == other_.startDate
            && this->holdingsShowHidden == other_.holdingsShowHidden
            && this->navSortDesc == other_.navSortDesc
            && this->aaShowBlank == other_.aaShowBlank
            && this->correlationShowHidden == other_.correlationShowHidden
            && this->acctShowBlank == other_.acctShowBlank
            && this->holdingsSort == other_.holdingsSort
            && this->aaSort == other_.aaSort
            && this->acctSort == other_.acctSort;
}

void portfolioAttributes::save(const queries &dataSource_)
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_Description), this->description);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartValue), this->startValue);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AAThreshold), this->aaThreshold);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_ThresholdMethod), (int)this->aaThresholdMethod);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_CostBasis), (int)this->costBasis);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_StartDate), this->startDate);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_Dividends), (int)this->dividends);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_HoldingsShowHidden), (int)this->holdingsShowHidden);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_HoldingsSort), this->holdingsSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AAShowBlank), (int)this->aaShowBlank);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AASort), this->aaSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_CorrelationShowHidden), (int)this->correlationShowHidden);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AcctShowBlank), (int)this->acctShowBlank);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_AcctSort), this->acctSort);
    values.insert(queries::portfoliosColumns.at(queries::portfoliosColumns_NAVSortDesc), (int)this->navSortDesc);

    this->id = dataSource_.insert(queries::table_Portfolios, values, this->id);
}
