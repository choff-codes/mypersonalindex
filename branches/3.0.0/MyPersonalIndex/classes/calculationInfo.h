#ifndef CALCULATIONINFO_H
#define CALCULATIONINFO_H

#include <QMap>
#include <QPair>

class dailyInfo
{
public:
    int date;
    int count;
    double totalValue;
    double costBasis;
    double taxLiability;
    double dividendAmount;

    dailyInfo(const int &p_date): date(p_date), count(0), totalValue(0), costBasis(0), taxLiability(0), dividendAmount(0), m_baseExpenseRatio(0) {}
    bool isNull() const { return date == 0; }
    double expenseRatio() const;

    void add(const dailyInfo &other, const double &multiplier = 1);
    // overrides any expense ratios set during "add" method
    void setExpenseRatio(const double &expenseRatio) { if (expenseRatio > 0) m_baseExpenseRatio = expenseRatio; }

private:
    typedef QPair<double /* total value */, double /* expense ratio */> expensePair;
    QList<expensePair> m_expenseRatio;
    double m_baseExpenseRatio;
};

class securityInfo: public dailyInfo
{
public:
    double shares;

    securityInfo(): dailyInfo(0), shares(0) { count = 1; }
    securityInfo(const int &p_date): dailyInfo(p_date), shares(0) { count = 1; }

    void setTaxLiability(const double &taxRate, const bool &taxDeferred);
};

class dailyInfoPortfolio: public dailyInfo
{
public:
    QMap<int, double> avgPrices;
    QMap<int, securityInfo> securitiesInfo;

    dailyInfoPortfolio(): dailyInfo(0) {}
    dailyInfoPortfolio(const int &p_date): dailyInfo(p_date) {}
};

#endif // CALCULATIONINFO_H
