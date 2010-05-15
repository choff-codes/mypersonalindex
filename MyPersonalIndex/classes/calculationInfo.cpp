#include "calculationInfo.h"

void dailyInfo::add(const dailyInfo &other, const double &multiplier)
{
    if (other.isNull())
        return;

    this->costBasis += other.costBasis * multiplier;
    this->totalValue += other.totalValue * multiplier;
    this->taxLiability += other.taxLiability * multiplier;
    this->dividendAmount += other.dividendAmount * multiplier;

    double er = other.expenseRatio();
    if (er > 0)
        m_expenseRatio.append(qMakePair(other.totalValue * multiplier, er));

    ++this->count;
}

double dailyInfo::expenseRatio() const
{
    if (m_baseExpenseRatio > 0)
        return m_baseExpenseRatio;

    double er = 0;
    foreach(const expensePair &pair, m_expenseRatio)
        er += (pair.first / totalValue) * pair.second;

    return er;
}

void securityInfo::setTaxLiability(const double &taxRate, const bool &taxDeferred)
{
    if (taxRate <= 0)
        return;

    if (taxDeferred)
        taxLiability = totalValue * taxRate;
    else if (totalValue > costBasis)
        taxLiability = (totalValue - costBasis) * taxRate;
}
