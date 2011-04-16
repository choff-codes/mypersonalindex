#include "portfolio.h"
#include <QMap>
#include <QSqlQuery>
#include <QVariant>
#include <QDate>
#include "queries.h"
#include "tradeDateCalendar.h"
#include "assetAllocationTarget.h"
#include "calculatorNAV.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "trade.h"
#include "executedTrade.h"
#include "snapshot.h"
#include "historicalNAV.h"

class portfolioData: public objectKeyData
{
public:
    calculatorNAV calculator;
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;
    int startDate;

    portfolioData():
        startDate(QDate::currentDate().toJulianDay())
    {}
};

portfolio::portfolio():
    d(new portfolioData())
{
}

portfolio::portfolio(const portfolio &other_):
    d(other_.d)
{
}

portfolio::~portfolio()
{
}

portfolio& portfolio::operator=(const portfolio &other_)
{
    d = other_.d;
    return *this;
}

objectKeyData* portfolio::data() const { return d.data(); }

bool portfolio::operator==(const portfolio &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
        && d->securities == other_.d->securities
        && d->assetAllocations == other_.d->assetAllocations
        && d->accounts == other_.d->accounts
        && d->startDate == other_.d->startDate;
}

int portfolio::startDate() const { return d->startDate; }
void portfolio::setStartDate(int startDate_) { d->startDate = startDate_; }

QMap<int, security>& portfolio::securities() const { return d->securities; }

QMap<int, assetAllocation>& portfolio::assetAllocations() const { return d->assetAllocations; }

QMap<int, account>& portfolio::accounts() const { return d->accounts; }

void portfolio::clearCache() const { d->calculator.clearCache(); }

snapshotSecurity portfolio::securitySnapshot(int date_, int id_, int priorDate_) const { return d->calculator.securitySnapshot(this, date_, id_, priorDate_); }

snapshot portfolio::portfolioSnapshot(int date_, int priorDate_) const { return d->calculator.portfolioSnapshot(this, date_, priorDate_); }

snapshot portfolio::assetAllocationSnapshot(int date_, int id_, int priorDate_) const { return d->calculator.assetAllocationSnapshot(this, date_, id_, priorDate_); }

snapshot portfolio::accountSnapshot(int date_, int id_, int priorDate_) const { return d->calculator.accountSnapshot(this, date_, id_, priorDate_); }

snapshot portfolio::symbolSnapshot(int date_, const symbol &key_, int beginDate_) const { return d->calculator.symbolSnapshot(date_, key_, beginDate_); }

double portfolio::nav(const objectKeyBase &key_, int beginDate_, int endDate_) const { return d->calculator.nav(this, key_, beginDate_, endDate_); }

historicalNAV portfolio::changeOverTime(const objectKeyBase &key_, int beginDate_, int endDate_) const { return d->calculator.changeOverTime(this, key_, beginDate_, endDate_); }

portfolio portfolio::load(const QSqlQuery &q_)
{
    portfolio p;
    p.setID(q_.value(queries::portfolioColumns_ID).toInt());
    p.setDescription(q_.value(queries::portfolioColumns_Description).toString());
    p.setStartDate(q_.value(queries::portfolioColumns_StartDate).toInt());

    return p;
}

bool portfolio::save(const queries &dataSource_)
{
    // save portfolio
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_ID), id());
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_Description), description());
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_StartDate), startDate());

    if (!dataSource_.insert(queries::table_Portfolio, values))
        return false;

    // save asset allocation
    foreach(const assetAllocation &aa, assetAllocations())
        if (!aa.save(dataSource_))
            return false;

    // save accounts
    foreach(const account &acct, accounts())
        if (!acct.save(dataSource_))
            return false;

    // save securities
    foreach(const security &sec, securities())
    {
        if (!sec.save(dataSource_))
            return false;
        if (!sec.targets().insertBatch(dataSource_))
            return false;
        if (!sec.executedTrades().insertBatch(dataSource_))
            return false;

        foreach(const trade &t, sec.trades())
            if (!t.save(dataSource_))
                return false;
    }

    return true;
}

bool portfolio::save(const QMap<int, portfolio> &portfolios_, const queries &dataSource_)
{
    //truncate tables...
    if (!dataSource_.deleteTable(queries::table_Portfolio))
        return false;

    foreach(portfolio p, portfolios_)
        if (!p.save(dataSource_))
            return false;

    return true;
}

void portfolio::detach()
{
    d.detach();
    d->calculator.clearCache();

    for(QMap<int, account>::iterator i = accounts().begin(); i != accounts().end(); ++i)
        i.value().detach();

    for(QMap<int, assetAllocation>::iterator i = assetAllocations().begin(); i != assetAllocations().end(); ++i)
        i.value().detach();

    for(QMap<int, security>::iterator i = securities().begin(); i != securities().end(); ++i)
    {
        i.value().detach();

        for(QMap<int, trade>::iterator x = i.value().trades().begin(); x != i.value().trades().end(); ++x)
            x.value().detach();
    }
}

QStringList portfolio::symbols() const
{
    QStringList list;
    foreach(const security &s, securities())
        if (!s.cashAccount())
            list.append(s.description());

    list.removeDuplicates();
    return list;
}

QMap<QString, int> portfolio::symbols(const QMap<int, portfolio> &portfolios_)
{
    QMap<QString, int> returnMap;
    foreach(const portfolio &p, portfolios_)
        foreach(const QString &symbol, p.symbols())
            if (returnMap.contains(symbol))
                returnMap[symbol] = qMin(returnMap.value(symbol), tradeDateCalendar::previousTradeDate(p.startDate()));
            else
                returnMap.insert(symbol, tradeDateCalendar::previousTradeDate(p.startDate()));

    return returnMap;
}

QString portfolio::validate() const
{
    if (this->description().isEmpty())
        return "Please enter a description!";

    return QString();
}

objectType portfolio::type() const
{
    return objectType_Portfolio;
}

void portfolio::setID(int id_)
{
    objectKey::setID(id_);
    for(QMap<int, security>::iterator i = d->securities.begin(); i != d->securities.end(); ++i)
        i.value().setParent(id_);
    for(QMap<int, assetAllocation>::iterator i = d->assetAllocations.begin(); i != d->assetAllocations.end(); ++i)
        i.value().setParent(id_);
    for(QMap<int, account>::iterator i = d->accounts.begin(); i != d->accounts.end(); ++i)
        i.value().setParent(id_);
}

int portfolio::endDate() const
{
    int date = 0;
    bool nonCashAccount = false;

    foreach(const security &s, securities())
    {
        if (s.cashAccount())
            continue;

        nonCashAccount = true;
        date = qMax(date, s.endDate());
    }

    return date == 0 || !nonCashAccount ? tradeDateCalendar::endDate() : date;
}
