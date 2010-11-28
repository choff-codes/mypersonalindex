#ifndef SECURITY_H
#define SECURITY_H

#include "objectKey.h"

#ifndef DIVIDEND_REINVESTMENT_TRADE_ID
#define DIVIDEND_REINVESTMENT_TRADE_ID -2
#endif

template <class T, class V>
class QMap;
class QSqlQuery;
class queries;
class securityData;
class assetAllocationTarget;
class executedTradeMap;
class trade;
class historicalPrices;
class security: public objectKey
{
public:

    security(int id_ = UNASSIGNED, int parent_ = UNASSIGNED, const QString &description_ = QString());
    security(const security &other_);

    ~security();

    security& operator=(const security &other_);

    bool operator==(const security &other_) const;
    bool operator!=(const security &other_) const { return !(*this == other_); }

    int account() const;
    void setAccount(int account_);

    double expenseRatio() const;
    void setExpenseRatio(double expenseRatio_);

    bool dividendReinvestment() const;
    void setDividendReinvestment(bool dividendReinvestment_);

    bool dividendNAVAdjustment() const ;
    void setDividendNAVAdjustment(bool dividendNAVAdjustment);

    bool cashAccount() const;
    void setCashAccount(bool cashAccount_);

    bool includeInCalc() const;
    void setIncludeInCalc(bool includeInCalc_);

    bool hidden() const;
    void setHidden(bool hidden_);

    QString note() const;
    void setNote(const QString &note_);

    assetAllocationTarget& targets() const;

    QMap<int, trade>& trades() const;

    executedTradeMap& executedTrades() const;

    double price(int date_) const;
    double splitAdjustedPriorDayPrice(int date_) const;

    double dividend(int date_) const;
    QMap<int, double> dividends() const;

    double split(int date_) const;
    QMap<int, double> splits() const;

    void setHistoricalPrices(const historicalPrices &prices_);

    int endDate() const;
    int beginDate() const;

    objectType type() const;
    QString validate() const;
    QString displayText() const;

    void save(const queries &dataSource_);
    void remove(const queries &dataSource_) const;
    static security load(const QSqlQuery &q_);

    friend QDataStream& operator>>(QDataStream &stream_, security &sec_);

    void detach();

private:
    QExplicitlySharedDataPointer<securityData> d;

    objectKeyData* data() const;
};

QDataStream& operator<<(QDataStream &stream_, const security &sec_);
QDataStream& operator>>(QDataStream &stream_, security &sec_);

#endif // SECURITY_H
