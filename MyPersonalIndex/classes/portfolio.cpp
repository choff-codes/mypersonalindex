#include "portfolio.h"

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
        for(QMap<int, assetAllocation>::iterator i = assetAllocations.begin(); i != assetAllocations.end(); ++i)
        {
            int origID = i.value().id;

            i.value().parent = attributes.id;
            i.value().save(dataSource_);

            if (origID > UNASSIGNED)
                continue;

            for(QMap<int, security>::iterator x = securities.begin(); x != securities.end(); ++i)
                x.value().targets.updateAssetAllocationID(origID, i.value().id);
        }

        // save accounts
        for(QMap<int, account>::iterator i = accounts.begin(); i != accounts.end(); ++i)
        {
            int origID = i.value().id;

            i.value().parent = attributes.id;
            i.value().save(dataSource_);

            if (origID > UNASSIGNED)
                continue;

            for(QMap<int, security>::iterator x = securities.begin(); x != securities.end(); ++i)
                if (x.value().account == origID)
                    x.value().account = i.value().id;
        }

        QHash<int, int> tradeIDMapping;

        // save securities
        for(QMap<int, security>::iterator i = securities.begin(); i != securities.end(); ++i)
        {
            i.value().parent = attributes.id;
            i.value().save(dataSource_);

            // save AA targets
            i.value().targets.parent = i.value().id;
            i.value().targets.insertBatch(dataSource_);

            // save trades (keep map of old id -> new id for executed trades associated trade id)
            for(QMap<int, trade>::iterator x = i.value().trades.begin(); x != i.value().trades.end(); ++x)
            {
                int origID = x.value().id;

                x.value().parent = i.value().id;
                x.value().save(dataSource_);

                if (origID < UNASSIGNED)
                    tradeIDMapping.insert(origID, x.value().id);
            }

            // set executed trades parent (save after all trades are saved)
            i.value().executedTrades.parent = i.value().id;
        }

        // save executed trades (need all securities and trades to be saved first to properly set associatedTradeID)
        for(QMap<int, security>::iterator i = securities.begin(); i != securities.end(); ++i)
        {
            i.value().executedTrades.updateAssociatedTradeID(tradeIDMapping);
            i.value().executedTrades.insertBatch(dataSource_);
        }
    }
};

int portfolio::IDENTITY_COUNTER = -10;

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
