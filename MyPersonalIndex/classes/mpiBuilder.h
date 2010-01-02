#ifndef MPIBUILDER_H
#define MPIBUILDER_H

#include <QtGui>
#include "queries.h"
#include "prices.h"

class mpiBuilder
{
public:
    mpiBuilder();

    QMap<int, portfolio*> loadPortfolios() { return m_portfolios; }
    settings loadSettings() { return m_settings; }

private:
    QMap<int, portfolio*> m_portfolios;
    settings m_settings;

    void loadPortfoliosInfo(QSqlQuery *q);
    void loadPortfoliosTickers(QSqlQuery *q);
    void loadPortfoliosTickersAA(QSqlQuery *q);
    void loadPortfoliosTickersTrades(QSqlQuery *q);
    void loadPortfoliosTrades(QSqlQuery *q);
    void loadPortfoliosAA(QSqlQuery *q);
    void loadPortfoliosAcct(QSqlQuery *q);
    void loadPortfoliosStat(QSqlQuery *q);
    void loadPortfoliosNAV(QSqlQuery *q);

    void loadSettingsInfo(QSqlQuery *q);
    void loadSettingsColumns(QSqlQuery *q);
};

#endif // MPIBUILDER_H
