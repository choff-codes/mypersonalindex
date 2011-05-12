#ifndef PORTFOLIOFACTORY_H
#define PORTFOLIOFACTORY_H

#include <QMap>
#include "portfolio.h"
#include "queries.h"

class fileStateIdentity;
class portfolioFactory
{
public:
    portfolioFactory(const queries &dataSource_):
        m_dataSource(dataSource_)
    {}

    QMap<int, portfolio> getPortfolios(const fileStateIdentity &identities_);

private:
    queries m_dataSource;

    int loadPortfolio(QMap<int, portfolio> &portfolios_);
    int loadPortfolioSecurity(QMap<int, portfolio> &portfolios);
    void loadPortfolioSecurityAA(QMap<int, portfolio> &portfolios_);
    int loadPortfolioSecurityTrades(QMap<int, portfolio> &portfolios_);
    void loadPortfolioSecurityTradesExecution(QMap<int, portfolio> &portfolios_);
    int loadPortfolioAA(QMap<int, portfolio> &portfolios_);
    int loadPortfolioAccount(QMap<int, portfolio> &portfolios_);
};

#endif // PORTFOLIOFACTORY_H
