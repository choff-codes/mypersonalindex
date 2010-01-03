#include "account.h"

void account::save(const int &portfolioID)
{
    queries sql("account");

    if(this->id == -1) // insert new
    {
        sql.executeNonQuery(new sqliteQuery(
            "INSERT INTO Acct (PortfolioID, Description, TaxRate, TaxDeferred)"
            " VALUES (:PortfolioID, :Description, :TaxRate, :TaxDeferred)",
            QList<sqliteParameter>()
                << sqliteParameter(":PortfolioID", portfolioID)
                << sqliteParameter(":Description", this->description)
                << sqliteParameter(":TaxRate", functions::doubleToNull(this->taxRate))
                << sqliteParameter(":TaxDeferred", (int)this->taxDeferred)
        ));

        this->id = sql.getIdentity();
    }
    else // update
    {
        sql.executeNonQuery(new sqliteQuery(
            "UPDATE Acct SET Description = :Description, TaxRate = :TaxRate, TaxDeferred = :TaxDeferred WHERE ID = :AcctID",
            QList<sqliteParameter>()
                << sqliteParameter(":Description", this->description)
                << sqliteParameter(":TaxRate", functions::doubleToNull(this->taxRate))
                << sqliteParameter(":TaxDeferred", this->taxDeferred)
                << sqliteParameter(":AcctID", this->id)
        ));
    }
}

void account::remove() const
{
    queries sql("account");
    sql.executeNonQuery(queries::deleteItem(queries::table_Acct, this->id));
}
