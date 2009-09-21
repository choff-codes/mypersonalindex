#ifndef ACCTQUERIES_H
#define ACCTQUERIES_H

#include "queries.h"
#include "globals.h"

class acctQueries : public queries
{
public:
    queryInfo* updateAcct(globals::account* const acct);
    queryInfo* deleteAcct(const int &acctID);

    acctQueries(const int &portfolioID): queries(), m_portfolioID(portfolioID) {}

private:
    int m_portfolioID;
};

#endif // ACCTQUERIES_H
