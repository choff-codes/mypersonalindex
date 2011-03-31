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
    {}

    QMap<int, portfolio> getPortfolios();

private:
    queries m_dataSource;

    QMap<int, portfolio> loadPortfolio();
    void loadPortfolioSecurity(QMap<int, portfolio> &portfolios_);
    void loadPortfolioSecurityAA(QMap<int, portfolio> &portfolios_);
    void loadPortfolioSecurityTrades(QMap<int, portfolio> &portfolios_);
    void loadPortfolioSecurityTradesExecution(QMap<int, portfolio> &portfolios_);
    void loadPortfolioAA(QMap<int, portfolio> &portfolios_);
    void loadPortfolioAccount(QMap<int, portfolio> &portfolios_);
};

#endif // PORTFOLIOFACTORY_H
