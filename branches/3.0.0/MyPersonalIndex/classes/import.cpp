#include "import.h"

import::import(const QMap<int, portfolio> &portfolios_)
{
    int securityIdentity = 0, aaIdentity = 0, acctIdentity = 0;
    foreach(const portfolio p, portfolios_)
         securityIdentity = getMappings<security>(p.securities(), &securities, p.attributes().description, securityIdentity);
         aaIdentity = getMappings<assetAllocation>(p.assetAllocations(), &assetAllocations, p.attributes().description, aaIdentity);
         acctIdentity = getMappings<account>(p.accounts(), &accounts, p.attributes().description, acctIdentity);
    }
}

template <class T>
int import::getMappings(const QMap<int, T> &values_, importData *data_, const QString &portfolioDescription_, int beginID)
{
    int i = beginID;
    foreach(const T &value, values_)
    {
        data_->mapping.insert(i, value.id);
        data_->reverseMapping.insert(value.id, i);
        data_->values.insert(i, QString("%1: %2").arg(portfolioDescription_, value.description));
        ++i;
    }
    return i;
}

void import::updateBasedOnSelectedSecurities()
{
    foreach(const int &i, securities.selected)
    {
        int acctToAdd = portfolio::instance().securityFromID(securities.mapping.value(i)).account;
        if (acctToAdd != -1 && !accounts.selected.contains(accounts.reverseMapping.value(acctToAdd)))
            accounts.selected.append(accounts.reverseMapping.value(acctToAdd));
    }

    foreach(const int &i, securities.selected)
        foreach(const int &z, portfolio::instance().securityFromID(securities.mapping.value(i)).aa.keys())
            if (!assetAllocations.selected.contains(assetAllocations.reverseMapping.value(z)))
                assetAllocations.selected.append(assetAllocations.reverseMapping.value(z));
}

void import::save(const int &portfolioID)
{
    QMap<int, int> newAAIDs;
    QMap<int, int> newAcctIDs;

    foreach(const int &i, assetAllocations.selected)
        newAAIDs.insert(assetAllocations.mapping.value(i),
            saveObject<assetAllocation>(portfolioID, portfolio::instance().assetAllocationFromID(assetAllocations.mapping.value(i))));

    foreach(const int &i, accounts.selected)
        newAcctIDs.insert(accounts.mapping.value(i),
            saveObject<account>(portfolioID, portfolio::instance().accountFromID(accounts.mapping.value(i))));

    foreach(const int &i, securities.selected)
    {
        security sec = portfolio::instance().securityFromID(securities.mapping.value(i));
        sec.clearIdentity();
        sec.account = newAcctIDs.contains(sec.account) ? newAcctIDs.value(sec.account) : -1;
        sec.save(portfolioID);

        QMap<int, double> oldAA = sec.targets;
        sec.targets.clear();
        foreach(const int &x, oldAA.keys())
            if (newAAIDs.contains(x))
                sec.targets.insert(newAAIDs.value(x), oldAA.value(x));

        sec.saveAATargets();

        foreach(const int &x, sec.trades.keys())
        {
            sec.trades[x].clearID();
            sec.trades[x].save(sec.id);
        }

        portfolio::instance().insert(portfolioID, sec);
    }
}

template <class T>
int import::saveObject(const int &portfolioID, T object)
{
    object.clearID();
    object.save(portfolioID);
    portfolio::instance().insert(portfolioID, object);
    return object.id;
}
