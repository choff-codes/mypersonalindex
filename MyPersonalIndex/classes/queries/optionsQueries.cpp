#include "optionsQueries.h"
#include "queries.h"
#include "globals.h"

queries::queryInfo* optionsQueries::updateSettings(globals::mpiSettings* const s)
{
    if (!s)
        return 0;

    return new queryInfo(
            "UPDATE Settings SET Splits = :Splits, DataStartDate = :DataStartDate",
            QList<parameter>()
                << parameter(":Splits", (int)s->splits)
                << parameter(":DataStartDate", s->dataStartDate.toJulianDay())
    );
}
