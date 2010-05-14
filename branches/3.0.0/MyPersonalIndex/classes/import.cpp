#include "import.h"

import::import()
{
    getSecurities();
    getAcct();
    getAA();
}

void import::getSecurities()
{
    int i = 0;
    foreach(const portfolioInfo &info, portfolio::instance().info())
        foreach(const security &s, portfolio::instance().securities(info.id))
        {
            securities.mapping.insert(i, s.id);
            securities.reverseMapping.insert(s.id, i);
            securities.values.insert(i, QString("%1: %2").arg(info.description, s.description));
            ++i;
        }
}

void import::getAcct()
{
    int i = 0;
    foreach(const portfolioInfo &info, portfolio::instance().info())
        foreach(const account &acct, portfolio::instance().acct(info.id))
        {
            accounts.mapping.insert(i, acct.id);
            accounts.reverseMapping.insert(acct.id, i);
            accounts.values.insert(i, QString("%1: %2").arg(info.description, acct.description));
            ++i;
        }
}

void import::getAA()
{
    int i = 0;
    foreach(const portfolioInfo &info, portfolio::instance().info())
        foreach(const assetAllocation &aa, portfolio::instance().aa(info.id))
        {
            assetAllocations.mapping.insert(i, aa.id);
            assetAllocations.reverseMapping.insert(aa.id, i);
            assetAllocations.values.insert(i, QString("%1: %2").arg(info.description, aa.description));
            ++i;
        }
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
    {
        assetAllocation aa = portfolio::instance().assetAllocationFromID(assetAllocations.mapping.value(i));
        aa.id = -1;
        aa.save(portfolioID);
        portfolio::instance().insert(portfolioID, aa);
        newAAIDs.insert(assetAllocations.mapping.value(i), aa.id);
    }

    foreach(const int &i, accounts.selected)
    {
        account acct = portfolio::instance().accountFromID(accounts.mapping.value(i));
        acct.id = -1;
        acct.save(portfolioID);
        portfolio::instance().insert(portfolioID, acct);
        newAcctIDs.insert(accounts.mapping.value(i), acct.id);
    }

    foreach(const int &i, securities.selected)
    {
        security sec = portfolio::instance().securityFromID(securities.mapping.value(i));
        sec.id = -1;

        if (newAcctIDs.contains(sec.account))
            sec.account = newAcctIDs.value(sec.account);
        else
            sec.account = -1;

        sec.save(portfolioID);

        QMap<int, double> newAATargets;
        foreach(const int &x, sec.aa.keys())
            if (newAAIDs.contains(x))
                newAATargets.insert(newAAIDs.value(x), sec.aa.value(x));

        sec.aa = newAATargets;
        sec.saveAATargets();

        foreach(const int &x, sec.trades.keys())
        {
            trade t = sec.trades.value(x);
            t.id = -1;
            t.save(sec.id);
        }

        portfolio::instance().insert(portfolioID, sec);
    }
}
