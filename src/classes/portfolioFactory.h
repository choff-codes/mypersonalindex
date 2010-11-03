#ifndef PORTFOLIOFACTORY_H
#define PORTFOLIOFACTORY_H

#include <QMap>
#include "portfolio.h"
#include "queries.h"

class portfolioFactory
{
public:
    portfolioFactory(const queries &dataSource_):
        m_dataSource(dataSource_)
    { }

    QMap<int, portfolio> getPortfolios();

private:
    QMap<int, portfolio> m_portfolios;
    queries m_dataSource;

    void loadPortfolio();
    void loadPortfolioSecurity();
    void loadPortfolioSecurityAA();
    void loadPortfolioSecurityTrades();
    void loadPortfolioSecurityTradesExecution();
    void loadPortfolioAA();
    void loadPortfolioAccount();
};

#endif // PORTFOLIOFACTORY_H
