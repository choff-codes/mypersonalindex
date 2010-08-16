#ifndef PORTFOLIOFACTORY_H
#define PORTFOLIOFACTORY_H

#include <QMap>
#include "portfolio.h"
#include "priceFactory.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

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

    void loadPortfoliosInfo();
    void loadPortfoliosSecurity();
    void loadPortfoliosSecurityAA();
    void loadPortfoliosSecurityTrades();
    void loadPortfoliosExecutedTrades();
    void loadPortfoliosAA();
    void loadPortfoliosAcct();
};

#endif // PORTFOLIOFACTORY_H
