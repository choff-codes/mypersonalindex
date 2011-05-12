#ifndef TRADE_H
#define TRADE_H

#include "objectKey.h"
#include "tradeDateCalendar.h"

class QString;
class QSqlQuery;
class queries;
class tradeData;
class trade: public objectKey
{
public:

    enum tradeAction {
        tradeAction_Purchase,
        tradeAction_Sell,
        tradeAction_PurchaseFixedAmount,
        tradeAction_SellFixedAmount,
        tradeAction_ReinvestDividends,
        tradeAction_ReceiveInterest,
        tradeAction_ReceiveInterestPercent,
        tradeAction_PurchasePercentOfSecurityShares,
        tradeAction_PurchasePercentOfPortfolioValue,
        tradeAction_PurchasePercentOfAATarget,
        tradeAction_ReinvestDividendsAuto,
        tradeAction_SellPercentOfSecurityShares
    };

    // order matters, calculatorTrade.h depends on tradePriceType_CurrentClose and tradePriceType_PreviousClose being 0 and 1
    enum tradePriceType {
        tradePriceType_CurrentClose,
        tradePriceType_PreviousClose,
        tradePriceType_UserDefined
    };

    trade();
    trade(const trade &other_);

    ~trade();

    trade& operator=(const trade &other_);

    bool operator==(const trade &other_) const;
    bool operator!=(const trade &other_) const { return !(*this == other_); }

    tradeAction action() const;
    void setAction(tradeAction action_);

    double value() const;
    void setValue(double value_);

    tradePriceType priceType() const;
    void setPriceType(tradePriceType priceType_);

    double price() const;
    void setPrice(double price_);

    double commission() const;
    void setCommission(double commission_);

    int cashAccount() const;
    void setCashAccount(int cashAccount_);

    tradeDateCalendar::frequency frequency() const;
    void setFrequency(tradeDateCalendar::frequency frequency_);

    int date() const;
    void setDate(int date_);

    int startDate() const;
    void setStartDate(int startDate_);

    int endDate() const;
    void setEndDate(int endDate_);

    bool save(const queries &dataSource_) const;
    static trade load(const QSqlQuery &q_);

    objectType type() const;
    QString validate() const;
    QString displayText() const;

    static QString tradeTypeToString(tradeAction type_);
    static QString frequencyToString(tradeDateCalendar::frequency freq_);
    static QString valueToString(tradeAction type_, double value_);
    static QString dateToString(tradeDateCalendar::frequency freq_, int date_);
    static QString priceToString(tradePriceType priceType_, double price_ = 0);

    friend QDataStream& operator>>(QDataStream &stream_, trade &trade_);

    void detach();

private:
    QExplicitlySharedDataPointer<tradeData> d;

    objectKeyData* data() const;
};

QDataStream& operator<<(QDataStream &stream_, const trade &trade_);
QDataStream& operator>>(QDataStream &stream_, trade &trade_);

#endif // TRADE_H
