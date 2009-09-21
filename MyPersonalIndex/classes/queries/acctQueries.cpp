#include "acctQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* acctQueries::updateAcct(globals::account* const acct)
{
    if (!acct)
        return 0;

    QVariant taxRate(QVariant::Double);
    if (acct->taxRate >= 0)
        taxRate = acct->taxRate;

    if(acct->id == -1) // insert new
    {
        return new queryInfo(
            "INSERT INTO Acct (PortfolioID, Description, TaxRate, TaxDeferred)"
            " VALUES (:PortfolioID, :Description, :TaxRate, :TaxDeferred)",
            QList<parameter>()
                << parameter(":PortfolioID", m_portfolioID)
                << parameter(":Description", acct->description)
                << parameter(":TaxRate", taxRate)
                << parameter(":TaxDeferred", (int)acct->taxDeferred)
        );
    }
    else // update
    {
        return new queryInfo(
            "UPDATE Acct SET Description = :Description, TaxRate = :TaxRate, TaxDeferred = :TaxDeferred WHERE ID = :AcctID",
            QList<parameter>()
                << parameter(":Description", acct->description)
                << parameter(":TaxRate", taxRate)
                << parameter(":TaxRate", taxRate)
                << parameter(":AcctID", acct->id)
        );
    }
}

queries::queryInfo* acctQueries::deleteAcct(const int &acctID)
{
    return new queryInfo(
            "DELETE FROM Acct WHERE ID = :AcctID",
            QList<parameter>()
                << parameter(":AcctID", acctID)
        );
}
