#include "portfolio.h"
#include <QDate>
#include <QMap>
#include <QSqlQuery>
#include <QVariant>
#include "queries.h"
#include "security.h"
#include "account.h"
#include "assetAllocation.h"
#include "tradeDateCalendar.h"
#include "assetAllocationTarget.h"
#include "trade.h"
#include "executedTrade.h"

class portfolioData: public objectKeyData
{
public:
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;

    int startDate;

    explicit portfolioData(int id_, const QString &description_):
        objectKeyData(description_, id_),
        startDate(QDate::currentDate().toJulianDay())
    {}
};

int portfolio::IDENTITY_COUNTER = UNASSIGNED_RESERVED - 1;

portfolio::portfolio(int id_, const QString &description_):
    d(new portfolioData(id_, description_))
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

portfolio portfolio::load(const QSqlQuery &q_)
{
    portfolio p(
        q_.value(queries::portfolioColumns_ID).toInt(),
        q_.value(queries::portfolioColumns_Description).toString()
    );

    p.setStartDate(q_.value(queries::portfolioColumns_StartDate).toInt());

    return p;
}

// make private?  this shouldn't be called directly
void portfolio::save(const queries &dataSource_)
{
    // Note: order of saving matters!

    // portfolio deleted?
    if (deleted())
    {
        remove(dataSource_);
        return;
    }

    // save portfolio
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_Description), description());
    values.insert(queries::portfolioColumns.at(queries::portfolioColumns_StartDate), startDate());

    this->setID(dataSource_.insert(queries::table_Portfolio, values, this->id()));

    // keep track of original aa id -> aa securty id for security asset class targets
    QMap<int, int> aaIDMapping;

    // save asset allocation
    QList<assetAllocation> aaList = assetAllocations().values();
    assetAllocations().clear();
    foreach(assetAllocation aa, aaList)
    {
        if (aa.deleted())
        {
            aa.remove(dataSource_);
            continue;
        }

        int origID = aa.id();
        aa.setParent(id());
        aa.save(dataSource_);
        assetAllocations().insert(aa.id(), aa);

        if (origID < UNASSIGNED)
            aaIDMapping.insert(origID, aa.id());
    }
    aaList.clear();

    // keep track of original security id -> new securty id for trades cash account
    QMap<int, int> acctIDMapping;

    // save accounts
    QList<account> acctList = accounts().values();
    accounts().clear();
    foreach(account acct, acctList)
    {
        if (acct.deleted())
        {
            acct.remove(dataSource_);
            continue;
        }

        int origID = acct.id();
        acct.setParent(id());
        acct.save(dataSource_);
        accounts().insert(acct.id(), acct);

        if (origID < UNASSIGNED)
            acctIDMapping.insert(origID, acct.id());
    }
    acctList.clear();

    // keep track of original security id -> new securty id for trades cash account
    QMap<int, int> secIDMapping;

    // save securities
    QList<security> secList = securities().values();
    securities().clear();
    foreach(security sec, secList)
    {
        if (sec.deleted())
        {
            sec.remove(dataSource_);
            continue;
        }

        int origID = sec.id();
        sec.setParent(id());
        sec.setAccount(acctIDMapping.value(sec.account(), sec.account()));
        sec.save(dataSource_);
        securities().insert(sec.id(), sec);

        if (origID < UNASSIGNED)
            secIDMapping.insert(origID, sec.id());

        // save AA targets
        sec.targets().parent = sec.id();
        sec.targets().updateAssetAllocationID(aaIDMapping);
        sec.targets().insertBatch(dataSource_);
    }
    secList.clear();

    // keep track of original trade id -> new trade id for executed trades associated trade id
    QMap<int, int> tradeIDMapping;

    // save trades (need all securities to be saved first for ids)
    foreach(security sec, securities())
    {
        // save trades
        QList<trade> tradeList = sec.trades().values();
        sec.trades().clear();
        foreach(trade t, tradeList)
        {
            if (t.deleted())
            {
                t.remove(dataSource_);
                continue;
            }

            int origID = t.id();
            t.setParent(sec.id());
            // properly update cash account
            t.setCashAccount(secIDMapping.value(t.cashAccount(), t.cashAccount()));
            t.save(dataSource_);
            sec.trades().insert(t.id(), t);

            if (origID < UNASSIGNED)
                tradeIDMapping.insert(origID, t.id());
        }
    }

    // save executed trades (need all trades to be saved first to properly set associatedTradeID)
    foreach(security sec, securities())
    {
        // set executed trades parent
        sec.executedTrades().parent = sec.id();
        sec.executedTrades().updateAssociatedTradeID(tradeIDMapping);
        sec.executedTrades().insertBatch(dataSource_);
    }
}

QMap<int, int> portfolio::save(QMap<int, portfolio> *portfolios_, const queries &dataSource_)
{
    if (!portfolios_)
        return QMap<int, int>();

    QMap<int, int> returnMap;
    QList<portfolio> portfolioList = portfolios_->values();
    portfolios_->clear();

    //truncate tables...
    dataSource_.deleteTable(queries::table_PortfolioSecurityTradeExecution);
    dataSource_.deleteTable(queries::table_PortfolioSecurityAA);

    foreach(portfolio p, portfolioList)
    {
        int oldID = p.id();
        p.save(dataSource_);

        if (p.deleted())
            continue;

        portfolios_->insert(p.id(), p);
        returnMap.insert(oldID, p.id());
    }

    return returnMap;
}

void portfolio::remove(const queries &dataSource_) const
{
    if (!hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_Portfolio, this->id());
}

void portfolio::detach()
{
    d.detach();

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
        if (!s.cashAccount() && !s.deleted())
            list.append(s.description());

    list.removeDuplicates();
    return list;
}

QMap<QString, int> portfolio::symbols(const QMap<int, portfolio> &portfolios_)
{
    QMap<QString, int> returnMap;
    foreach(const portfolio &p, portfolios_)
        if (!p.deleted())
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

int portfolio::endDate() const
{
    int date = 0;
    bool nonCashAccount = false;

    foreach(const security &s, securities())
    {
        if (s.cashAccount() || s.deleted())
            continue;

        nonCashAccount = true;
        date = qMax(date, s.endDate());
    }

    return date == 0 || !nonCashAccount ? tradeDateCalendar::endDate() : date;
}
