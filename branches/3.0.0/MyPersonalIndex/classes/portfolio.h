#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QtCore>
#include "portfolioData.h"
#include "queries.h"
#include "prices.h"

class portfolio
{

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

public:
    static portfolio& instance()
    {
        static portfolio m_instance; // Guaranteed to be destroyed and instantiated on first use.
        return m_instance;
    }

    bool exists(const int &portfolioID) { return m_portfolios.contains(portfolioID); }
    void save()
    {
        foreach(const int &i, ids())
            m_portfolios[i].info.save();
    }
    bool isEmpty() { return m_portfolios.isEmpty(); }
    int count() { return m_portfolios.count(); }
    void insert(const portfolioInfo &info) { m_portfolios[info.id].info = info; }
    void insertNAV(const int &portfolioID, const int &date, const double &nav, const double &totalValue) { m_portfolios[portfolioID].nav.insert(date, nav, totalValue); }
    void insertExecutedTrade(const int &portfolioID, const int &securityID, const executedTrade &trade) { m_portfolios[portfolioID].executedTrades[securityID].append(trade); }
    void insert(const int &portfolioID, const assetAllocation &aa) { m_portfolios[portfolioID].aa[aa.id] = aa; }
    void insert(const int &portfolioID, const account &acct) { m_portfolios[portfolioID].acct[acct.id] = acct; }
    void insert(const int &portfolioID, const security &sec) { m_portfolios[portfolioID].securities[sec.id] = sec; }

    void remove(const int &portfolioID);
    void remove(const int &portfolioID, const assetAllocation &aa)
    {
        aa.remove();
        m_portfolios[portfolioID].aa.remove(aa.id);
        for(QMap<int, security>::iterator i = m_portfolios[portfolioID].securities.begin(); i != m_portfolios[portfolioID].securities.begin(); ++i)
            i->removeAATarget(aa.id);
    }

    void remove(const int &portfolioID, const account &acct)
    {
        acct.remove();
        m_portfolios[portfolioID].acct.remove(acct.id);
        for(QMap<int, security>::iterator i = m_portfolios[portfolioID].securities.begin(); i != m_portfolios[portfolioID].securities.begin(); ++i)
            i->removeAccount(acct.id, portfolioID);
    }
    void remove(const int &portfolioID, const security &sec) { sec.remove(); m_portfolios[portfolioID].securities.remove(sec.id); }

    void update(const portfolioInfo &info) { if(exists(info.id)) { m_portfolios[info.id].info = info; m_portfolios[info.id].info.save(); } }
    const QList<int> ids() { return m_portfolios.keys(); }
    const QStringList symbols()
    {
        QStringList list;
        foreach(const portfolioData &d, m_portfolios)
            foreach(const security &s, d.securities)
                list.append(s.symbol);
        list.removeDuplicates();
        return list;
    }


    bool invalidNAVDates()
    {
        int firstDate = prices::instance().firstDate();
        if (firstDate == 0)
            return true;

        foreach(const portfolioData &d, m_portfolios)
            if (!d.nav.isEmpty() && d.nav.firstDate() < firstDate)
                return true;

        return false;
    }


    const QMap<int, security> securities(const int &portfolioID) { return m_portfolios.value(portfolioID).securities; }
    const security securities(const int &portfolioID, const int &id) { return m_portfolios.value(portfolioID).securities.value(id); }
    const QMap<int, assetAllocation> aa(const int &portfolioID) { return m_portfolios.value(portfolioID).aa; }
    const assetAllocation aa(const int &portfolioID, const int &id) { return m_portfolios.value(portfolioID).aa.value(id); }
    const QMap<int, account> acct(const int &portfolioID) { return m_portfolios.value(portfolioID).acct; }
    const account acct(const int &portfolioID, const int &id) { return m_portfolios.value(portfolioID).acct.value(id); }
    const executedTradeList executedTrades(const int &id) { return m_portfolios.value(id).executedTrades; }
    const navInfo nav(const int &id) { return m_portfolios.value(id).nav; }
    portfolioInfo info(const int &id) { return m_portfolios.value(id).info; }
    QList<portfolioInfo> info() { QList<portfolioInfo> list; foreach(const portfolioData &d, m_portfolios) list.append(d.info); return list; }

    int startDate(const int &portfolioID) { return m_portfolios.value(portfolioID).info.startDate; }
    double startValue(const int &portfolioID) { return m_portfolios.value(portfolioID).info.startValue; }
    void removeNAV(const int &portfolioID) { m_portfolios[portfolioID].nav.remove(portfolioID); }
    void removeNAV(const int &portfolioID, const int &startDate) { m_portfolios[portfolioID].nav.remove(portfolioID, startDate); }
    void removeExecutedTrades(const int &portfolioID) { m_portfolios[portfolioID].executedTrades.remove(portfolioID); }
    void removeExecutedTrades(const int &portfolioID, const int &startDate) { m_portfolios[portfolioID].executedTrades.remove(portfolioID, startDate); }
};

#endif // PORTFOLIO_H
