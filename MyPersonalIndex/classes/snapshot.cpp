#include "snapshot.h"

void snapshot::add(const snapshot &other, const double &multiplier)
{
    if (other.isNull())
        return;

    double startingTotalValue = this->totalValue;

    this->costBasis += other.costBasis * multiplier;
    this->totalValue += other.totalValue * multiplier;
    this->taxLiability += other.taxLiability * multiplier;
    this->dividendAmount += other.dividendAmount * multiplier;
    this->expenseRatio = // allocate based on total value percentage
        (
            (startingTotalValue / this->totalValue) * this->expenseRatio
        )
            +
        (
            (other.totalValue / this->totalValue) * other.expenseRatio
        );

    ++this->count;
}

void snapshotSecurity::setTaxLiability(const double &taxRate, const bool &taxDeferred)
{
    if (taxRate <= 0)
        return;

    if (taxDeferred)
        taxLiability = totalValue * taxRate;
    else if (totalValue > costBasis)
        taxLiability = (totalValue - costBasis) * taxRate;
}
