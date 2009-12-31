#ifndef NAVINFO_H
#define NAVINFO_H

#include <QtGui>

class navInfo
{
public:
    //navInfo() {}

    void insert(const int &date, const double &nav, const double &totalValue);

    QMap<int, double> navHistory() const { return m_nav; }
    double nav(const int &date) const { return m_nav.value(date); }
    double totalValue(const int &date) const { return m_totalValue.value(date); }

    QList<int> dates() const { return m_nav.keys(); }
    int count() const { return m_nav.count(); }
    bool isEmpty() const { return m_nav.isEmpty(); }
    int firstDate() const { return m_nav.constBegin().key(); }
    int lastDate() const { return (m_nav.constEnd() - 1).key(); }

    void clear() { m_nav.clear(); m_totalValue.clear(); }
    void clear(int startDate);

private:
    QMap<int, double> m_nav;
    QMap<int, double> m_totalValue;
};

#endif // NAVINFO_H
