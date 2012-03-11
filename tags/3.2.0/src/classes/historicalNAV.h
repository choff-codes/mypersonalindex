#ifndef HISTORICALNAV_H
#define HISTORICALNAV_H

#include <QHash>

struct navValue
{
    double nav;
    double totalValue;
    double dividend;

    navValue():
        nav(1),
        totalValue(0),
        dividend(0)
    {}

    navValue(double nav_, double totalValue_, double dividend_):
        nav(nav_),
        totalValue(totalValue_),
        dividend(dividend_)
    {}
};

class historicalNAV
{
public:
    double costBasis;
    double expenseRatio;
    double taxLiability;

    historicalNAV():
        costBasis(0),
        expenseRatio(0),
        taxLiability(0),
        m_firstDate(0),
        m_lastDate(0)
    {}

    void insert(int date_, double nav_, double totalValue_, double dividend_);

    double nav(int date_) const { return m_nav.value(date_).nav; }
    double totalValue(int date_) const { return m_nav.value(date_).totalValue; }
    double dividend(int date_) const { return m_nav.value(date_).dividend; }

    navValue value(int date_) const { return m_nav.value(date_); }

    int count() const { return m_nav.count(); }
    bool isEmpty() const { return m_nav.isEmpty(); }

    int beginDate() const { return m_firstDate; }
    int endDate() const { return m_lastDate; }

private:
    QHash<int, navValue> m_nav;
    int m_firstDate;
    int m_lastDate;
};

#endif // HISTORICALNAV_H
