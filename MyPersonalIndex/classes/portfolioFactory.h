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

    QMap<int, portfolio> getPortfolios(bool includePricing_);

private:
    QMap<int, portfolio> m_portfolios;
    queries m_dataSource;

    void loadPortfoliosInfo();
    void loadPortfoliosSecurity(bool includePricing_);
    void loadPortfoliosSecurityAA();
    void loadPortfoliosSecurityTrades();
    void loadPortfoliosSecurityTradesExecution();
    void loadPortfoliosAA();
    void loadPortfoliosAccount();
};

#endif // PORTFOLIOFACTORY_H
