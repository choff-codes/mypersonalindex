#ifndef AAQUERIES_H
#define AAQUERIES_H

#include "queries.h"
#include "globals.h"

class aaQueries : public queries
{
public:
    queryInfo* updateAA(globals::assetAllocation* const aa);
    queryInfo* deleteAA(const int &aaID);

    aaQueries(const int &portfolioID): queries(), m_portfolioID(portfolioID) {}

private:
    int m_portfolioID;
};

#endif // AAQUERIES_H
