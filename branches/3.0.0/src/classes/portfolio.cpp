#include "portfolio.h"
#include "queries.h"
#include "security.h"
#include "account.h"
#include "assetAllocation.h"
#include "portfolioAttributes.h"

class portfolioData: public QSharedData
{
public:
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;
    portfolioAttributes attributes;

    portfolioData(int id_, const QString &description_):
        attributes(portfolioAttributes(id_, description_))
    {}


    void save(const queries &dataSource_)
    {
        // Note: order of saving matters!

        // save portfolio
        attributes.save(dataSource_);

        // save asset allocation
        QList<assetAllocation> aaList = assetAllocations.values();
        assetAllocations.clear();

        for(QList<assetAllocation>::iterator i = aaList.begin(); i != aaList.end(); ++i)
        {
            int origID = i->id;
            i->parent = attributes.id;
            i->save(dataSource_);
            assetAllocations.insert(i->id, *i);

            if (origID < UNASSIGNED)
                for(QMap<int, security>::iterator x = securities.begin(); x != securities.end(); ++x)
                    x.value().targets.updateAssetAllocationID(origID, i->id);
        }
        aaList.clear();

        // save accounts
        QList<account> acctList = accounts.values();
        accounts.clear();
        for(QList<account>::iterator i = acctList.begin(); i != acctList.end(); ++i)
        {
            int origID = i->id;
            i->parent = attributes.id;
            i->save(dataSource_);
            accounts.insert(i->id, *i);

            if (origID < UNASSIGNED)
                for(QMap<int, security>::iterator x = securities.begin(); x != securities.end(); ++x)
                    if (x.value().account == origID)
                        x.value().account = i->id;
        }
        acctList.clear();

        QHash<int, int> tradeIDMapping;

        // save securities
        QList<security> secList = securities.values();
        securities.clear();
        for(QList<security>::iterator i = secList.begin(); i != secList.end(); ++i)
        {
            i->parent = attributes.id;
            i->save(dataSource_);

            // save AA targets
            i->targets.parent = i->id;
            i->targets.insertBatch(dataSource_);

            // save trades (keep map of old id -> new id for executed trades associated trade id)
            QList<trade> tradeList = i->trades.values();
            i->trades.clear();
            for(QList<trade>::iterator x = tradeList.begin(); x != tradeList.end(); ++x)
            {
                int origID = x->id;
                x->parent = i->id;
                x->save(dataSource_);
                i->trades.insert(x->id, *x);
                if (origID < UNASSIGNED)
                    tradeIDMapping.insert(origID, x->id);
            }

            // set executed trades parent (save after all trades are saved)
            i->executedTrades.parent = i->id;
            securities.insert(i->id, *i);
        }
        secList.clear();

        // save executed trades (need all securities and trades to be saved first to properly set associatedTradeID)
        for(QMap<int, security>::iterator i = securities.begin(); i != securities.end(); ++i)
        {
            i.value().executedTrades.updateAssociatedTradeID(tradeIDMapping);
            i.value().executedTrades.insertBatch(dataSource_);
        }
    }
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

QMap<int, security>& portfolio::securities() const
{
    return d->securities;
}

QMap<int, assetAllocation>& portfolio::assetAllocations() const
{
    return d->assetAllocations;
}

QMap<int, account>& portfolio::accounts() const
{
    return d->accounts;
}

portfolioAttributes& portfolio::attributes() const
{
    return d->attributes;
}

void portfolio::save(const queries &dataSource_)
{
    d->save(dataSource_);
}

void portfolio::remove(const queries &dataSource_) const
{
    d->attributes.remove(dataSource_);
}

void portfolio::detach()
{
    d.detach();
}

bool portfolio::operator==(const portfolio &other_) const
{
    return d->securities == other_.d->securities
        && d->assetAllocations == other_.d->assetAllocations
        && d->accounts == other_.d->accounts
        && d->attributes == other_.d->attributes;
}

QStringList portfolio::symbols() const
{
    QStringList list;
    foreach(const security &s, d->securities)
        if (!s.cashAccount)
            list.append(s.description);

    list.removeDuplicates();
    return list;
}

int portfolio::endDate() const
{
    //TODO
    int date = 0;
    bool nonCashAccount = false;
    foreach(const security &s, d->securities)
    {
        if (s.cashAccount)
            continue;

        nonCashAccount = true;
        date = qMax(date, s.endDate());
    }

    if (!nonCashAccount)
        return tradeDateCalendar::endDate();

    return date;
}
