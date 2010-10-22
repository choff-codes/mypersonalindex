#ifndef PORTFOLIOFACTORY_H
#define PORTFOLIOFACTORY_H

#include <QMap>
#include "portfolio.h"
#include "queries.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class portfolioFactory
{
public:
    portfolioFactory(const queries &dataSource_):
        m_dataSource(dataSource_)
    { }

    QMap<int, portfolio> getPortfolios(bool includePricing_);

private:
    QMap<int, portfolio> m_portfolios;
    queries m_dataSource;

    void loadPortfolio();
    void loadPortfolioSecurity(bool includePricing_);
    void loadPortfolioSecurityAA();
    void loadPortfolioSecurityTrades();
    void loadPortfolioSecurityTradesExecution();
    void loadPortfolioAA();
    void loadPortfolioAccount();
};

#endif // PORTFOLIOFACTORY_H
