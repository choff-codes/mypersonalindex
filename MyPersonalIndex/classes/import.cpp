#include "import.h"

import::import()
{
    int securityIdentity = 0, aaIdentity = 0, acctIdentity = 0;
    foreach(const portfolioInfo &info, portfolio::instance().info())
    {
         securityIdentity = getMappings<security>(portfolio::instance().securities(info.id), &securities, info.description, securityIdentity);
         aaIdentity = getMappings<assetAllocation>(portfolio::instance().aa(info.id), &assetAllocations, info.description, aaIdentity);
         acctIdentity = getMappings<account>(portfolio::instance().acct(info.id), &accounts, info.description, acctIdentity);
    }
}

template <class T>
int import::getMappings(const QMap<int, T> &values, importData *data, const QString &portfolioDescription, const int &startID)
{
    int i = startID;
    foreach(const T &value, values)
    {
        data->mapping.insert(i, value.id);
        data->reverseMapping.insert(value.id, i);
        data->values.insert(i, QString("%1: %2").arg(portfolioDescription, value.description));
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
        sec.clearID();
        sec.account = newAcctIDs.contains(sec.account) ? newAcctIDs.value(sec.account) : -1;
        sec.save(portfolioID);

        QMap<int, double> oldAA = sec.aa;
        sec.aa.clear();
        foreach(const int &x, oldAA.keys())
            if (newAAIDs.contains(x))
                sec.aa.insert(newAAIDs.value(x), oldAA.value(x));

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
