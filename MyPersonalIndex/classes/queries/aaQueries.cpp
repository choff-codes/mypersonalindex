#include "aaQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* aaQueries::updateAA(globals::assetAllocation* const aa)
{
    if (!aa)
        return 0;

    QVariant target(QVariant::Double);
    if (target != -1)
        target = aa->target;

    if(aa->id == -1) // insert new
    {
        return new queryInfo(
            "INSERT INTO AA (PortfolioID, Description, Target)"
            " VALUES (:PortfolioID, :Description, :Target)",
            QList<parameter>()
                << parameter(":PortfolioID", m_portfolioID)
                << parameter(":Description", aa->description)
                << parameter(":Target", target)
        );
    }
    else // update
    {
        return new queryInfo(
            "UPDATE AA SET Description = :Description, Target = :Target WHERE ID = :AAID",
            QList<parameter>()
                << parameter(":Description", aa->description)
                << parameter(":Target", target)
                << parameter(":AAID", aa->id)
        );
    }
}

queries::queryInfo* aaQueries::deleteAA(const int &aaID)
{
    return new queryInfo(
            "DELETE FROM AA WHERE ID = :AAID",
            QList<parameter>()
                << parameter(":AAID", aaID)
        );
}
