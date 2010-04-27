#ifndef NAVINFO_H
#define NAVINFO_H

#include <QtCore>
#include "queries.h"

class navInfo
{
public:
    void insert(const int &date, const double &nav, const double &totalValue);

    QMap<int, double> navHistory() const { return m_nav; }
    QMap<int, double> totalValueHistory() const { return m_totalValue; }
    double nav(const int &date) const { return m_nav.value(date); }
    double totalValue(const int &date) const { return m_totalValue.value(date); }

    QList<int> dates() const { return m_nav.keys(); }
    int count() const { return m_nav.count(); }
    bool isEmpty() const { return m_nav.isEmpty(); }

    // must check IsEmpty first when calling
    int firstDate() const { return m_nav.constBegin().key(); }
    int lastDate() const { return (m_nav.constEnd() - 1).key(); }

protected:
    QMap<int, double> m_nav;
    QMap<int, double> m_totalValue;
};

class navInfoPortfolio: public navInfo
{
public:
    void remove(const int &portfolioID);
    void remove(const int &portfolioID, const int &startDate);
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
    struct navPair
    {
        double nav;
        double totalValue;

        navPair(): nav(0), totalValue(0) {}
        navPair(const double &p_nav, const double &p_totalValue): nav(p_nav), totalValue(p_totalValue) {}
    };

    QHash<int, navPair> m_nav;
    int m_firstDate;
    int m_lastDate;
};

#endif // NAVINFO_H
