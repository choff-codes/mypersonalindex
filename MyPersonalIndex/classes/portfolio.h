#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QtCore>
#include "portfolioData.h"
#include "queries.h"
#include "prices.h"

class portfolio
{
public:
    static portfolio& instance()
    {
        static portfolio m_instance; // Guaranteed to be destroyed and instantiated on first use.
        return m_instance;
    }

    bool isEmpty() const { return m_portfolios.isEmpty(); }
    int count() const { return m_portfolios.count(); }
    QList<int> ids() const { return m_portfolios.keys(); }
    QStringList symbols() const;
    bool datesOutsidePriceData() const;

    bool exists(const int &portfolioID) const { return m_portfolios.contains(portfolioID); }
    void save() { foreach(const int &i, ids())  m_portfolios[i].info.save(); }

    void insert(const portfolioInfo &info) { m_portfolios[info.id].info = info; }
    void insert(const int &portfolioID, const assetAllocation &aa) { m_portfolios[portfolioID].aa[aa.id] = aa; }
    void insert(const int &portfolioID, const account &acct) { m_portfolios[portfolioID].acct[acct.id] = acct; }
    void insert(const int &portfolioID, const security &sec) { m_portfolios[portfolioID].securities[sec.id] = sec; }
    void insertTrade(const int &portfolioID, const int &securityID, const trade &t) { m_portfolios[portfolioID].securities[securityID].trades[t.id] = t; }
    void insertAAPercentage(const int &portfolioID, const int &securityID, const int &aaID, const double &value) { m_portfolios[portfolioID].securities[securityID].aa[aaID] = value; }
    void insertNAV(const int &portfolioID, const int &date, const double &nav, const double &totalValue) { m_portfolios[portfolioID].nav.insert(date, nav, totalValue); }
    void insertExecutedTrade(const int &portfolioID, const int &securityID, const executedTrade &t) { m_portfolios[portfolioID].executedTrades[securityID].append(t); }

    void remove(const int &portfolioID);
    void remove(const int &portfolioID, const assetAllocation &aa);
    void remove(const int &portfolioID, const account &acct);
    void remove(const int &portfolioID, const security &sec);
    void removeNAV(const int &portfolioID) { m_portfolios[portfolioID].nav.remove(portfolioID); }
    void removeNAV(const int &portfolioID, const int &startDate) { m_portfolios[portfolioID].nav.remove(portfolioID, startDate); }
    void removeExecutedTrades(const int &portfolioID) { m_portfolios[portfolioID].executedTrades.remove(portfolioID); }
    void removeExecutedTrades(const int &portfolioID, const int &startDate) { m_portfolios[portfolioID].executedTrades.remove(portfolioID, startDate); }

    const QMap<int, security> securities(const int &portfolioID) const { return m_portfolios.value(portfolioID).securities; }
    const security securities(const int &portfolioID, const int &id) const { return m_portfolios.value(portfolioID).securities.value(id); }
    const security securityFromID(const int &id) const;

    const QMap<int, assetAllocation> aa(const int &portfolioID) const { return m_portfolios.value(portfolioID).aa; }
    const assetAllocation aa(const int &portfolioID, const int &id) const { return m_portfolios.value(portfolioID).aa.value(id); }

    const QMap<int, account> acct(const int &portfolioID) const { return m_portfolios.value(portfolioID).acct; }
    const account acct(const int &portfolioID, const int &id) const { return m_portfolios.value(portfolioID).acct.value(id); }

    const executedTradeList executedTrades(const int &id) const { return m_portfolios.value(id).executedTrades; }
    const navInfo nav(const int &id) const { return m_portfolios.value(id).nav; }

    portfolioInfo info(const int &id) const { return m_portfolios.value(id).info; }
    QList<portfolioInfo> info() const { QList<portfolioInfo> list; foreach(const portfolioData &d, m_portfolios) list.append(d.info); return list; }

    int startDate(const int &portfolioID) const { return m_portfolios.value(portfolioID).info.startDate; }
    double startValue(const int &portfolioID) const { return m_portfolios.value(portfolioID).info.startValue; }

    int minimumDateBetweenTrades(const int &currentMinimumDate, const int &date);
    int minimumDateBetweenTrades(const int &currentMinimumDate, const int &portfolioID, const assetAllocation &aa);

private:
    QMap<int, portfolioData> m_portfolios;

    portfolio();
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
    portfolio(portfolio const&);  // Don't Implement
    void operator=(portfolio const&); // Don't implement

    void loadPortfoliosInfo();
    void loadPortfoliosSecurity();
    void loadPortfoliosSecurityAA();
    void loadPortfoliosSecurityTrades();
    void loadPortfoliosExecutedTrades();
    void loadPortfoliosAA();
    void loadPortfoliosAcct();
    void loadPortfoliosNAV();
};

#endif // PORTFOLIO_H
