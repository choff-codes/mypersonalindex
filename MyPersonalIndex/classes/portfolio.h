#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QMap>
#include <QSharedData>
#include "portfolioData.h"
#include "queries.h"
#include "executedTrade.h"
#include "navInfo.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "portfolioInfo.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

class portfolioData;
class portfolio
{
public:
    portfolio(int id_);
    portfolio(const portfolio &other);
    ~portfolio();

    portfolio& operator=(const portfolio &other);

    QStringList symbols() const;
//    bool datesOutsidePriceData() const;

    void save(const queries &dataSource_) {d->info.save(dataSource_); }

    void insert(const portfolioInfo &info_) { d->info = info_; }
    void insert(const assetAllocation &aa_) { d->aa[aa.id] = aa; }
    void insert(const account &acct_) { d->acct[acct.id] = acct; }
    void insert(const security &security_) { d->securities[security_.id] = security_; }
    void insertNAV(const queries &dataSource_, int date_, double nav_, double totalValue_) { d->nav.insert(dataSource_, date_, nav_, totalValue_); }
    void insertExecutedTrade(const queries &dataSource_, int id_, const executedTrade &trade_) {d->executedTrades.insert(dateSource_, id_, trade_); }

    void beginNAVBatch() { d->nav.beginBatch(); }
    void insertNAVBatch(const queries &dataSource_) { d->nav.insertBatch(dataSource_); }
    void beginExecutedTradesBatch() { d->executedTrades.beginBatch(); }
    void insertExecutedTradesBatch(const queries &dataSource_) { d->executedTrades.insertBatch(dataSource_); }

    void remove(const queries &dataSource_);
    void remove(const queries &dataSource_, const assetAllocation &aa_);
    void remove(const queries &dataSource_, const account &acct_);
    void remove(const queries &dataSource_, const security &security_);
    void removeNAV(const queries &dataSource_, ) { d->nav.remove(dataSource_); }
    void removeNAV(const queries &dataSource_, int beginDate_) { d->nav.remove(dataSource_, beginDate_); }
    void removeExecutedTrades(const queries &dataSource_, ) { d->executedTrades.remove(dataSource_); }
    void removeExecutedTrades(const queries &dataSource_, int beginDate_) { d->executedTrades.remove(dataSource_, beginDate_); }

    const QMap<int, security> securities() const { return d->securities; }
    const security securities(int id_) const { return d->securities.value(id_); }
    const QList<int> securityReinvestments();

    const QMap<int, assetAllocation> aa() const { return d->aa; }
    const assetAllocation aa(int id_) const { return d->aa.value(id_); }

    const QMap<int, account> acct() const { return d->acct; }
    const account acct(int id_) const { return d->acct.value(id_); }

    const executedTradeList executedTrades() const { return d->executedTrades; }
    const navInfoPortfolio nav() const { return d->nav; }

    portfolioInfo info() const { return d->info; }
    objectKey key() const { return d->info.key(); }

    int startDate() const { return d->info.startDate; }
    double startValue() const { return d->info.startValue; }

    //int minimumDate(const int &currentMinimumDate, const int &date) const;
    //int minimumDate(const int &currentMinimumDate, const int &portfolioID, const assetAllocation &aa) const;

private:
    QExplicitlySharedDataPointer<portfolioData> d;

//    void loadPortfoliosInfo();
//    void loadPortfoliosSecurity();
//    void loadPortfoliosSecurityAA();
//    void loadPortfoliosSecurityTrades();
//    void loadPortfoliosExecutedTrades();
//    void loadPortfoliosAA();
//    void loadPortfoliosAcct();
//    void loadPortfoliosNAV();
};

#endif // PORTFOLIO_H
