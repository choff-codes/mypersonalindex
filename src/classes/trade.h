#ifndef TRADE_H
#define TRADE_H

#include <QString>
#include <QDate>
#include "objectKey.h"
#include "tradeDateCalendar.h"

class QSqlQuery;
class queries;
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
        tradeAction_PurchasePercentOfSecurityValue,
        tradeAction_PurchasePercentOfPortfolioValue,
        tradeAction_PurchasePercentOfAATarget,
        tradeAction_ReinvestDividendsAuto
    };

    tradeAction action;
    double value;
    double price;
    double commission;
    int cashAccount;
    tradeDateCalendar::frequency frequency;
    int date;
    int startDate;
    int endDate;

    explicit trade(int id_ = UNASSIGNED, int parent_ = UNASSIGNED):
        objectKey(QString(), id_, parent_),
        action(tradeAction_Purchase),
        value(0),
        price(UNASSIGNED),
        commission(0),
        cashAccount(UNASSIGNED),
        frequency(tradeDateCalendar::frequency_Once),
        date(QDate::currentDate().toJulianDay()),
        startDate(0),
        endDate(0)
    {}

    bool operator==(const trade &other_) const;
    bool operator!=(const trade &other_) const { return !(*this == other_); }

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static trade load(const QSqlQuery &q_);

    objectType type() const { return objectType_Trade; }
    QString validate() const;
    QString displayText() const;

    static QString tradeTypeToString(tradeAction type_);
    static QString frequencyToString(tradeDateCalendar::frequency freq_);
    static QString valueToString(tradeAction type_, double value_);
    static QString dateToString(tradeDateCalendar::frequency freq_, int date_);
};

QDataStream& operator<<(QDataStream &stream_, const trade &trade_);
QDataStream& operator>>(QDataStream &stream_, trade &trade_);

#endif // TRADE_H
