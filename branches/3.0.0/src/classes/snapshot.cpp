#include "snapshot.h"
#include "functions.h"

void snapshot::add(const snapshot &other_, double multiplier_)
{
    if (other_.isNull())
        return;

    double startingTotalValue = this->totalValue;

    this->costBasis += other_.costBasis * multiplier_;
    this->totalValue += other_.totalValue * multiplier_;
    this->taxLiability += other_.taxLiability * multiplier_;
    this->dividendAmount += other_.dividendAmount * multiplier_;
    this->dividendAmountNAV += other_.dividendAmountNAV * multiplier_;

    if (functions::isZero(this->totalValue))
        this->expenseRatio = 0;
    else
        this->expenseRatio = // allocate based on total value percentage
            (
                (startingTotalValue / this->totalValue) * this->expenseRatio
            )
                +
            (
                ((other_.totalValue * multiplier_) / this->totalValue) * other_.expenseRatio
            );

    ++this->count;
}

void snapshotSecurity::setTaxLiability(double taxRate_, bool taxDeferred_)
{
    if (functions::isZero(taxRate_ ))
        return;

    if (taxDeferred_)
        taxLiability = totalValue * taxRate_;
    else if (totalValue > costBasis)
        taxLiability = (totalValue - costBasis) * taxRate_;
}
