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

class navInfoStatistic: public navInfo
{
public:
    double costBasis;
    double expenseRatio;

    navInfoStatistic(): costBasis(0), expenseRatio(0) {}

    /*
        Since non-virtual functions are dispatched based on the static type of the pointer/reference rather than the dynamic type
        of the pointed-to/referenced object, the client-visible effects must be identical.
        http://www.parashift.com/c++-faq-lite/strange-inheritance.html#faq-23.12
    */
    double change(const int &date) const { return navInfo::nav(date); }
    QMap<int, double> changeHistory() const { return navInfo::navHistory(); }
    void insert(const int &date, const double &change, const double &totalValue) { navInfo::insert(date, change, totalValue); }

private:
    double nav(const int &date) const;
    QMap<int, double> navHistory() const;
};


#endif // NAVINFO_H
