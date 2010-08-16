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

QMap<int, security>& portfolio::securities()
{
    return d->securities;
}

QMap<int, assetAllocation>& portfolio::assetAllocations()
{
    return d->assetAllocations;
}

QMap<int, account>& portfolio::accounts()
{
    return d->accounts;
}

portfolioAttributes& portfolio::attributes()
{
    return d->attributes;
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
    int date = 0;
    bool nonCashAccount = false;
    foreach(const security &s, d->securities)
    {
        if (s.cashAccount)
            nonCashAccount = true;
        else
            date = qMax(date, s.endDate());
    }

    if (!nonCashAccount)
        return tradeDateCalendar::endDate();

    return date;
}

//bool portfolio::datesOutsidePriceData() const
//{
//    int firstDate = prices::instance().firstDate();
//    if (firstDate == 0)
//        return true;
//
//    foreach(const portfolioData &d, m_portfolios)
//        if (!d.nav.isEmpty() && d.nav.firstDate() < firstDate)
//            return true;
//
//    return false;
//}

//int portfolio::minimumDate(const int &currentMinimumDate, const int &date) const
//{
//    int returnDate = currentMinimumDate;
//    if (date != -1 && (date < currentMinimumDate || currentMinimumDate == -1))
//        returnDate = date;
//
//    return returnDate;
//}
//
//int portfolio::minimumDate(const int &currentMinimumDate, const int &portfolioID, const assetAllocation &aa) const
//{
//    int returnDate = currentMinimumDate;
//    foreach(const security &s, securities(portfolioID))
//        if(s.aa.contains(aa.id))
//            foreach(const trade &t, s.trades)
//                if (t.type == trade::tradeType_AA)
//                {
//                    returnDate = minimumDate(currentMinimumDate, s.firstTradeDate());
//                    break;
//                }
//
//    return returnDate;
//}
