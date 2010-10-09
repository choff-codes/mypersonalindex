#ifndef TRADE_H
#define TRADE_H

#include <QString>
#include <QDate>
#include "queries.h"
#include "functions.h"
#include "objectKey.h"
#include "tradeDateCalendar.h"

class trade: public objectKey
{
public:

    static const int tradeAction_Count = 11;

    enum tradeAction {
        tradeAction_Purchase,
        tradeAction_Sale,
        tradeAction_FixedPurchase,
        tradeAction_FixedSale,
        tradeAction_DivReinvest,
        tradeAction_Interest,
        tradeAction_InterestPercent,
        tradeAction_Value,
        tradeAction_TotalValue,
        tradeAction_AA,
        tradeAction_DivReinvestAuto,
        tradeAction_Reversal
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

    trade(int id_ = UNASSIGNED, int parent_ = UNASSIGNED):
        objectKey(QString(), id_, parent_),
        action(tradeAction_Purchase),
        value(UNASSIGNED),
        price(UNASSIGNED),
        commission(0),
        cashAccount(UNASSIGNED),
        frequency(tradeDateCalendar::frequency_Once),
        date(0),
        startDate(0),
        endDate(0)
    {}

    bool operator==(const trade &other_) const;
    bool operator!=(const trade &other_) const { return !(*this == other_); }

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static trade load(QSqlQuery q_);

    objectType type() const { return objectType_Trade; }
    QString validate() const;

    static QString tradeTypeToString(tradeAction type_);
    static QString frequencyToString(tradeDateCalendar::frequency freq_);
    static QString valueToString(tradeAction type_, double value_);
    static QString dateToString(tradeDateCalendar::frequency freq_, int date_);
};

#endif // TRADE_H
