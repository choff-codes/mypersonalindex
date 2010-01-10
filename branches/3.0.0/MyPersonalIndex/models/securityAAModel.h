#ifndef SECURITYAAMODEL_H
#define SECURITYAAMODEL_H

#include "functions.h"
#include "security.h"
#include "assetAllocation.h"

class securityAAModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    QList<aaTarget> getList() { return m_list; }

    securityAAModel(const QList<aaTarget> &values, const QMap<int, assetAllocation> &aaValues, const int &cols, QTableView *parent = 0):
            QAbstractTableModel(parent), m_aaValues(aaValues), m_parent(parent), m_columns(cols), m_list(values) {}

    double totalPercentage();

private:
    const QMap<int, assetAllocation> &m_aaValues;
    QTableView *m_parent;
    int m_columns;
    QList<aaTarget> m_list;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex&) const { return m_list.count(); }
    int columnCount (const QModelIndex&) const { return m_columns; }

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
