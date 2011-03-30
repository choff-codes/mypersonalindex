#include "portfolio.h"
#include "portfolio_p.h"
#include <QMap>
#include <QSqlQuery>
#include <QVariant>
#include "queries.h"
#include "tradeDateCalendar.h"
#include "assetAllocationTarget.h"
#include "trade.h"
#include "executedTrade.h"

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

calculatorNAV& portfolio::calculator() const { d->calculator.setPortfolio(d); return d->calculator; }

QMap<int, security>& portfolio::securities() const { return d->securities; }

QMap<int, assetAllocation>& portfolio::assetAllocations() const { return d->assetAllocations; }

QMap<int, account>& portfolio::accounts() const { return d->accounts; }

portfolio portfolio::load(const QSqlQuery &q_)
{
    portfolio p;
    p.setID(q_.value(queries::portfolioColumns_ID).toInt());
    p.setDescription(q_.value(queries::portfolioColumns_Description).toString());
    p.setStartDate(q_.value(queries::portfolioColumns_StartDate).toInt());

    return p;
}

// make private?  this shouldn't be called directly
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
    d->calculator.detach();

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
                returnMap[symbol] = qMin(returnMap.value(symbol), p.startDate());
            else
                returnMap.insert(symbol, p.startDate());

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
