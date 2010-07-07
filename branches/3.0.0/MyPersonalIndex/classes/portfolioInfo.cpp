#include "portfolioInfo.h"

portfolioInfo::portfolioInfo(const int &id_ = -1): objectKey(objectType_Portfolio, QString(), id_), dividends(true), costBasis(account::costBasisType_FIFO),
    startValue(100), aaThreshold(5), aaThresholdMethod(threshold_Portfolio), startDate(QDate::currentDate().toJulianDay()),
    holdingsShowHidden (true), navSortDesc(true), aaShowBlank(true), correlationShowHidden(true), acctShowBlank(true)
{
}

bool portfolioInfo::operator==(const portfolioInfo &other) const
{
    return this->key() == other.key()
            && this->dividends == other.dividends
            && this->costBasis == other.costBasis
            && this->startValue == other.startValue
            && this->aaThreshold == other.aaThreshold
            && this->aaThresholdMethod == other.aaThresholdMethod
            && this->startDate == other.startDate
            && this->holdingsShowHidden == other.holdingsShowHidden
            && this->navSortDesc == other.navSortDesc
            && this->aaShowBlank == other.aaShowBlank
            && this->correlationShowHidden == other.correlationShowHidden
            && this->acctShowBlank == other.acctShowBlank
            && this->holdingsSort == other.holdingsSort
            && this->aaSort == other.aaSort
            && this->acctSort == other.acctSort;
}

void portfolioInfo::save(const queries &dataSource)
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

    this->id = dataSource.insert(queries::table_Portfolios, values, this->id);
}
