#ifndef PORTFOLIOQUERIES_H
#define PORTFOLIOQUERIES_H

#include "queries.h"
#include "globals.h"

class portfolioQueries : public queries
{
public:
    queryInfo* updatePortfolio(globals::portfolio* const portfolio);
};

#endif // PORTFOLIOQUERIES_H
