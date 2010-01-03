#include "assetAllocation.h"

void assetAllocation::save(const int &portfolioID)
{
    queries sql("assetAllocation");

    if(this->id == -1) // insert new
    {
        sql.executeNonQuery(new sqliteQuery(
            "INSERT INTO AA (PortfolioID, Description, Target)"
            " VALUES (:PortfolioID, :Description, :Target)",
            QList<sqliteParameter>()
                << sqliteParameter(":PortfolioID", portfolioID)
                << sqliteParameter(":Description", this->description)
                << sqliteParameter(":Target", functions::doubleToNull(this->target))
        ));

        this->id = sql.getIdentity();
    }
    else // update
    {
        sql.executeNonQuery(new sqliteQuery(
            "UPDATE AA SET Description = :Description, Target = :Target WHERE ID = :AAID",
            QList<sqliteParameter>()
                << sqliteParameter(":Description", this->description)
                << sqliteParameter(":Target", functions::doubleToNull(this->target))
                << sqliteParameter(":AAID", this->id)
        ));
    }
}

void assetAllocation::remove() const
{
    queries sql("assetAllocation");
    sql.executeNonQuery(queries::deleteItem(queries::table_AA, this->id));
}
