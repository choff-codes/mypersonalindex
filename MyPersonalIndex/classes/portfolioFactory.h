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
    QMap<int, portfolio> getPortfolios(const queries &dataSource_);

private:
    QMap<int, portfolio> m_portfolios;

    void loadPortfoliosInfo(const queries &dataSource_);
    void loadPortfoliosSecurity(const queries &dataSource_);
    void loadPortfoliosSecurityAA(const queries &dataSource_);
    void loadPortfoliosSecurityTrades(const queries &dataSource_);
    void loadPortfoliosExecutedTrades(const queries &dataSource_);
    void loadPortfoliosAA(const queries &dataSource_);
    void loadPortfoliosAcct(const queries &dataSource_);
    void loadPortfoliosNAV(const queries &dataSource_);
};

#endif // PORTFOLIOFACTORY_H
