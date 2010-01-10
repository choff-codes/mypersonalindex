#ifndef SECURITY_H
#define SECURITY_H

#include <QtGui>
#include "queries.h"

class trade
{
public:
    enum tradeType { tradeType_Purchase, tradeType_Sale, tradeType_DivReinvest, tradeType_Interest, tradeType_Fixed, tradeType_TotalValue, tradeType_AA, tradeType_Count };
    enum tradeFreq { tradeFreq_Once, tradeFreq_Daily, tradeFreq_Weekly, tradeFreq_Monthly, tradeFreq_Yearly, tradeFreq_Count };

    int id;
    tradeType type;
    double value;
    double price;
    double commission;
    int cashAccount;
    tradeFreq frequency;
    int date;
    int startDate;
    int endDate;

    trade(): id(-1), type(tradeType_Purchase), value(-1), price(-1), commission(-1), cashAccount(-1), frequency(tradeFreq_Once), date(0), startDate(0), endDate(0) {}

    bool operator==(const trade &other) const
    {
        return this->type == other.type
                && this->value == other.value
                && this->price == other.price
                && this->commission == other.commission
                && this->cashAccount == other.cashAccount
                && this->frequency == other.frequency
                && this->date == other.date
                && this->startDate == other.startDate
                && this->endDate == other.endDate;
    }

    bool operator!=(const trade &other) const
    {
        return !(*this == other);
    }

    void save(const int &securityID);
    void remove() const;

    static QString tradeTypeToString(const tradeType &type);
    static QString frequencyToString(const tradeFreq &freq);
    static QString valueToString(const tradeType &type, const double &value);
    static QString dateToString(const tradeFreq &freq, const int &date);
};


class aaTarget
{
public:
    int id;
    double target;

    aaTarget(int p_id, double p_target): id(p_id), target(p_target) {}

    bool operator==(const aaTarget &other) const
    {
        return this->id == other.id
                && this->target == other.target;
    }

    bool operator!=(const aaTarget &other) const
    {
        return !(*this == other);
    }

};

class security
{
public:
    int id;
    QString symbol;
    int account;
    double expense;
    bool divReinvest;
    bool cashAccount;
    bool includeInCalc;
    bool hide;
    QList<aaTarget> aa;
    QMap<int, trade> trades;

    security(): id(-1), account(-1), expense(-1), divReinvest(false), cashAccount(false),
        includeInCalc(true), hide(false) {}

    bool operator==(const security &other) const
    {
        return this->id == other.id
                && this->symbol == other.symbol
                && this->account == other.account
                && this->expense == other.expense
                && this->divReinvest == other.divReinvest
                && this->cashAccount == other.cashAccount
                && this->includeInCalc == other.includeInCalc
                && this->hide == other.hide
                && this->aa == other.aa
                && this->trades == other.trades;
    }

    bool operator!=(const security &other) const
    {
        return !(*this == other);
    }

    int firstTradeDate() const;
    void save(const int &portfolioID);
    void saveAATargets();
};

#endif // SECURITY_H
