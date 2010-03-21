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
            securities.values.insert(i, QString("%1: %2").arg(info.description, s.symbol));
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

}
