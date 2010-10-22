#ifndef HISTORICALNAV_H
#define HISTORICALNAV_H

#include <QHash>

struct navPair
{
    double nav;
    double totalValue;

    navPair():
        nav(0),
        totalValue(0)
    {}

    navPair(double nav_, double totalValue_):
        nav(nav_),
        totalValue(totalValue_)
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

    void insert(int date_, double nav_, double totalValue_);

    double nav(int date_) const { return m_nav.value(date_).nav; }
    double totalValue(int date_) const { return m_nav.value(date_).totalValue; }

    int count() const { return m_nav.count(); }
    bool isEmpty() const { return m_nav.isEmpty(); }

    // must check IsEmpty first when calling
    int firstDate() const { return m_firstDate; }
    int lastDate() const { return m_lastDate; }

private:
    QHash<int, navPair> m_nav;
    int m_firstDate;
    int m_lastDate;
};

#endif // HISTORICALNAV_H
