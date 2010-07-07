#ifndef NAVINFO_H
#define NAVINFO_H

#include <QMap>
#include <QHash>
#include "queries.h"
#include "objectBase.h"

struct navPair
{
    double nav;
    double totalValue;

    navPair(): nav(0), totalValue(0) {}
    navPair(const double &p_nav, const double &p_totalValue): nav(p_nav), totalValue(p_totalValue) {}
};

class navInfoPortfolio: public objectBase
{
public:
    navInfoPortfolio(const int &parent_):
            objectBase(parent_)
    {}

    void insert(const int &date, const double &nav, const double &totalValue);

    const QMap<int, navPair> navHistory() const { return m_nav; }
    navPair nav(const int &date) const { return m_nav.value(date); }

    int count() const { return m_nav.count(); }
    bool isEmpty() const { return m_nav.isEmpty(); }

    // must check IsEmpty first when calling
    int firstDate() const { return m_nav.constBegin().key(); }
    int lastDate() const { return (m_nav.constEnd() - 1).key(); }

    void remove(const queries &dataSource);
    void remove(const queries &dataSource, const int &startDate);

private:
    QMap<int, navPair> m_nav;
};

class navInfoStatistic
{
public:
    double costBasis;
    double expenseRatio;
    double taxLiability;

    navInfoStatistic(): costBasis(0), expenseRatio(0), taxLiability(0), m_firstDate(0), m_lastDate(0) {}

    void insert(const int &date, const double &nav, const double &totalValue);

    double nav(const int &date) const { return m_nav.value(date).nav; }
    double totalValue(const int &date) const { return m_nav.value(date).totalValue; }

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

#endif // NAVINFO_H
