#ifndef SECURITYAAMODEL_H
#define SECURITYAAMODEL_H

#include <QtCore>
#include "functions.h"
#include "security.h"
#include "assetAllocation.h"

class securityAAModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    QMap<int, double> getList() const { return m_list; }

    securityAAModel(const QMap<int, double> &values, const QMap<int, assetAllocation> &aaValues, QTableView *parent = 0):
            QAbstractTableModel(parent), m_aaValues(aaValues), m_parent(parent), m_list(values), m_keys(values.keys())
    {
        insertRows(0, m_list.count());
    }

    double totalPercentage() const;

private:
    const QMap<int, assetAllocation> m_aaValues;
    QTableView *m_parent;
    QMap<int, double> m_list;
    QList<int> m_keys;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex&) const { return m_keys.count(); }
    int columnCount (const QModelIndex&) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int, Qt::Orientation, int) const { return QVariant(); }
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    void addNew(const int &id);
    void deleteSelected();

signals:
    void updateHeader();
};

#endif // SECURITYAAMODEL_H
