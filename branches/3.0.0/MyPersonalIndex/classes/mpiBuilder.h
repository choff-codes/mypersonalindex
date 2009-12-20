#ifndef MPIBUILDER_H
#define MPIBUILDER_H

#include <QtGui>
#include "queries.h"
#include "globals.h"

class mpiBuilder
{
public:
    QMap<int, globals::myPersonalIndex*> loadPortfolios();

private:
    QMap<int, globals::myPersonalIndex*> m_portfolios;

    void loadPortfoliosInfo(QSqlQuery *q);
    void loadPortfoliosTickers(QSqlQuery *q);
    void loadPortfoliosTickersAA(QSqlQuery *q);
    void loadPortfoliosTickersTrades(QSqlQuery *q);
    void loadPortfoliosTrades(QSqlQuery *q);
    void loadPortfoliosAA(QSqlQuery *q);
    void loadPortfoliosAcct(QSqlQuery *q);
    void loadPortfoliosStat(QSqlQuery *q);
    void loadPortfoliosNAV(QSqlQuery *q);
};

#endif // MPIBUILDER_H
