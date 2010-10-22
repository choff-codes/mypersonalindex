#ifndef SECURITY_H
#define SECURITY_H

#include <QString>
#include <QMap>
#include "trade.h"
#include "objectKey.h"
#include "executedTrade.h"
#include "historicalPrices.h"
#include "assetAllocationTarget.h"

#ifndef DIVIDEND_REINVESTMENT_TRADE_ID
#define DIVIDEND_REINVESTMENT_TRADE_ID -2
#endif

class security: public objectKey
{
public:

    int account;
    double expenseRatio;
    bool dividendReinvestment;
    bool dividendNAVAdjustment;
    bool cashAccount;
    bool includeInCalc;
    bool hide;
    QString note;
    assetAllocationTarget targets;
    QMap<int, trade> trades;
    executedTradeMap executedTrades;

    security(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = QString()):
        objectKey(description_, id_, parent_),
        account(UNASSIGNED),
        expenseRatio(0),
        dividendReinvestment(false),
        dividendNAVAdjustment(true),
        cashAccount(false),
        includeInCalc(true),
        hide(false),
        executedTrades(id_)
    {}

    bool operator==(const security &other_) const;
    bool operator!=(const security &other_) const { return !(*this == other_); }

    void setHistoricalPrices(const historicalPrices &prices_) { m_prices = prices_; }
    double price(int date_) const { return cashAccount ? 1 : m_prices.value(date_, historicalPrices::type_price); }
    double dividend(int date_) const { return cashAccount ? 0 : m_prices.value(date_, historicalPrices::type_dividend); }
    double split(int date_) const { return cashAccount ? 1 : m_prices.value(date_, historicalPrices::type_split); }
    QMap<int, double> splits() const { return cashAccount ? QMap<int, double>() : m_prices.values(historicalPrices::type_split); }
    QMap<int, double> dividends() const { return cashAccount ? QMap<int, double>() : m_prices.values(historicalPrices::type_dividend); }
    int endDate() const { return cashAccount ? tradeDateCalendar::endDate() : m_prices.endDate(historicalPrices::type_price); }
    int beginDate() const { return cashAccount ? 0 : m_prices.beginDate(historicalPrices::type_price); }

    double splitAdjustedPriorDayPrice(int date_) const { return price(tradeDateCalendar::previousTradeDate(date_)) / split(date_); }
    
    objectType type() const { return objectType_Security; }
    QString validate() const;

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static security load(const QSqlQuery &q_);

private:
    historicalPrices m_prices;
};

QDataStream& operator<<(QDataStream &stream_, const security &sec_);
QDataStream& operator>>(QDataStream &stream_, security &sec_);

#endif // SECURITY_H
