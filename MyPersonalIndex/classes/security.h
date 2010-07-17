#ifndef SECURITY_H
#define SECURITY_H

#include <QString>
#include <QMap>
#include "trade.h"
#include "queries.h"
#include "functions.h"
#include "objectKey.h"
#include "executedTrade.h"
#include "historicalPrices.h"
#include "assetAllocationTarget.h"

class security: public objectKey
{
public:
    int account;
    double expense;
    bool divReinvest;
    bool cashAccount;
    bool includeInCalc;
    bool hide;
    QString note;
    assetAllocationTarget targets;
    QMap<int, trade> trades;
    executedTradeList executedTrades;

    security(const int &id_ = -1, const int &parent_ = -1, const QString &description_ = QString()):
        objectKey(objectType_Security, description_, id_, parent_),
        account(-1),
        expense(0),
        divReinvest(false),
        cashAccount(false),
        includeInCalc(true),
        hide(false),
        executedTrades(id_)
    {}

    bool operator==(const security &other_) const;
    bool operator!=(const security &other_) const { return !(*this == other_); }

    int firstTradeDate() const;

    void setHistoricalPrices(const historicalPrices &prices_) { m_prices = prices_; }
    double price(int date_) const { return cashAccount ? 1 : m_prices.price(date_); }
    double dividend(int date_) const { return cashAccount ? 0 : m_prices.dividend(date_); }
    double split(int date_) const { return cashAccount ? 1 : m_prices.split(date_); }
    QMap<int, double> splits() const { return cashAccount ? QMap<int, double>() : m_prices.splits(); }
    int endDate() const { return cashAccount ? tradeDateCalendar::endDate() : m_prices.endDate(); }
    int beginDate() const { return cashAccount ? 0 : m_prices.beginDate(); }
    
    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;

private:
    historicalPrices m_prices;
};

#endif // SECURITY_H
