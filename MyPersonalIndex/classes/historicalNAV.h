#ifndef HISTORICALNAV_H
#define HISTORICALNAV_H

#include <QMap>
#include <QHash>
#include "queries.h"
#include "objectBase.h"

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

class historicalNAVPortfolio: public objectBase, public queriesBatch
{
public:
    historicalNAVPortfolio(const int &parent_):
        objectBase(parent_)
    {}

    void insert(int date_, double nav_, double totalValue_) { m_nav.insert(date_, navPair(nav_, totalValue_)); }
    void insert(const queries &dataSource_, int date_, double nav_, double totalValue_);

    QMap<int, navPair>::const_iterator constBegin() const { return m_nav.constBegin(); }
    QMap<int, navPair>::const_iterator constEnd() const { return m_nav.constEnd(); }

    int count() const { return m_nav.count(); }
    int isEmpty() const { return m_nav.isEmpty(); }

    double nav(int date_) const { return m_nav.value(date_).nav; }
    double totalValue(int date_) const { return m_nav.value(date_).totalValue; }

    void remove(const queries &dataSource_);
    void remove(const queries &dataSource_, int beginDate_);

    void insertBatch(queries dataSource_);

    int rowsToBeInserted() { return m_valuesToBeInserted.count(); }
    QVariant value(int row_, int column_);

private:
    QMap<int, navPair> m_nav;
    QList<int> m_valuesToBeInserted;
};

#endif // HISTORICALNAV_H
