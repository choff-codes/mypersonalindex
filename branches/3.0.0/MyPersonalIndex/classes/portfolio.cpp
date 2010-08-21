#include "portfolio.h"

class portfolioData: public QSharedData
{
public:
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;
    portfolioAttributes attributes;

    portfolioData(int id_):
        attributes(portfolioAttributes(id_))
    {}


    void save(const queries &dataSource_)
    {
        QMap<QString, QVariant> values;
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_Description), attributes.description);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_StartValue), attributes.startValue);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AAThreshold), attributes.aaThreshold);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_ThresholdMethod), (int)attributes.aaThresholdMethod);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_CostBasis), (int)attributes.defaultCostBasis);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_StartDate), attributes.startDate);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_Dividends), (int)attributes.dividends);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_HoldingsShowHidden), (int)attributes.holdingsShowHidden);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_HoldingsSort), attributes.holdingsSort);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AAShowBlank), (int)attributes.aaShowBlank);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AASort), attributes.aaSort);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_CorrelationShowHidden), (int)attributes.correlationShowHidden);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AcctShowBlank), (int)attributes.acctShowBlank);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_AcctSort), attributes.acctSort);
        values.insert(queries::portfolioColumns.at(queries::portfoliosColumns_NAVSortDesc), (int)attributes.navSortDesc);

        attributes.id = dataSource_.insert(queries::table_Portfolio, values, attributes.id);

        for(QMap<int, security>::iterator i = securities.begin(); i != securities.end(); ++i)
            i.value().parent = attributes.id;

        for(QMap<int, assetAllocation>::iterator i = assetAllocations.begin(); i != assetAllocations.end(); ++i)
            i.value().parent = attributes.id;

        for(QMap<int, account>::iterator i = accounts.begin(); i != accounts.end(); ++i)
            i.value().parent = attributes.id;

    }

    void remove(const queries &dataSource_) const
    {
        if (!attributes.hasIdentity())
            return;

        dataSource_.deleteItem(queries::table_Portfolio, attributes.id);
    }

};

portfolio::portfolio(int id_):
    d(new portfolioData(id_))
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
    d->remove(dataSource_);
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
