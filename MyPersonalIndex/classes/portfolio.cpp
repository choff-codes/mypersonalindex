#include "portfolio.h"

class portfolioData: public QSharedData
{
public:
    QMap<int, security> securities;
    QMap<int, assetAllocation> assetAllocations;
    QMap<int, account> accounts;
    historicalNAVPortfolio navHistory;
    portfolioAttributes attributes;

    portfolioData(int id_):
            attributes(portfolioAttributes(id_)),
            navHistory(historicalNAVPortfolio(id_))
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

void portfolio::beginExecutedTradesBatch()
{
    for(QMap<int, security>::iterator i = d->securities.begin(); i != d->securities.end(); ++i)
        i.value().executedTrades.beginBatch();
}

void portfolio::insertExecutedTradesBatch(const queries &dataSource_)
{
    for(QMap<int, security>::iterator i = d->securities.begin(); i != d->securities.end(); ++i)
        i.value().executedTrades.insertBatch(dataSource_);
}

void portfolio::beginNAVBatch()
{
    d->navHistory.beginBatch();
}

void portfolio::insertNAVBatch(const queries &dataSource_)
{
    d->navHistory.insertBatch(dataSource_);
}

QMap<int, security>& portfolio::securities()
{
    return &d->securities;
}

QMap<int, assetAllocation>& portfolio::assetAllocations()
{
    return &d->assetAllocations;
}

QMap<int, account>& portfolio::accounts()
{
    return &d->accounts;
}

historicalNAVPortfolio& portfolio::navHistory()
{
    return &d->navHistory;
}

portfolioAttributes& portfolio::attributes()
{
    return &d->attributes;
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

QList<int> portfolio::securityReinvestments()
{
    QList<int> reinvestments;
    foreach(const security &s, d->securities)
        if (s.includeInCalc && s.divReinvest && !s.cashAccount)
            reinvestments.append(s.id);

    return reinvestments;
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
