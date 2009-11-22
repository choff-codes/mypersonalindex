#ifndef TICKERAAMODEL_H
#define TICKERAAMODEL_H

#include "globals.h"
#include "functions.h"
#include "mpiModelBase.h"

class tickerAAModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    QList<globals::tickerAATarget> getList() { return m_list; }

    tickerAAModel(const QList<globals::tickerAATarget> &values, const QMap<int, globals::assetAllocation> &aaValues, const int &cols, QTableView *parent = 0):
            QAbstractTableModel(parent), m_aaValues(aaValues), m_parent(parent), m_columns(cols), m_list(values) {}

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        if (index.column() == 1)
                flags = flags | Qt::ItemIsEditable;

        return flags;
    }

    int rowCount(const QModelIndex&) const
    {
        return m_list.count();
    }

    int columnCount (const QModelIndex&) const
    {
        return m_columns;
    }

    double totalPercentage()
    {
        double total = 0;

        foreach(const globals::tickerAATarget &aa, m_list)
            total += aa.second;

        return total;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= m_list.size())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            if (index.column() == 0)
                return m_aaValues.value(m_list.at(index.row()).first).description;

            if (index.column() == 1)
                return functions::doubleToPercentage(m_list.at(index.row()).second);
        }

        if (role == Qt::EditRole)
        {
            if (index.column() == 0)
                return m_aaValues.value(m_list.at(index.row()).first).description;

            if (index.column() == 1)
                return m_list.at(index.row()).second;
        }

        return QVariant();
    }

    QVariant headerData (int, Qt::Orientation, int) const
    {
        return QVariant();
    }

    bool setData (const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && index.column() == 1 && role == Qt::EditRole)
        {
            m_list[index.row()].second = value.toDouble();
            emit updateHeader();
            return true;
        }

        return false;
    }

public slots:
    void addNew(const int &id)
    {
        beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
        double total = totalPercentage();
        m_list.append(qMakePair(id, total >= 100 ? 0 : 100 - total));
        endInsertRows();
        emit updateHeader();
    }

    void deleteSelected()
    {
        QList<int> indexes;
        foreach(const QModelIndex &q, m_parent->selectionModel()->selectedRows())
            indexes.append(q.row());
        qSort(indexes);

        if(indexes.count() == 0)
            return;

        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_list.removeAt(indexes.at(i));
            endRemoveRows();
        }
        emit updateHeader();
    }

signals:
    void updateHeader();

private:
    const QMap<int, globals::assetAllocation> &m_aaValues;
    QTableView *m_parent;
    int m_columns;
    QList<globals::tickerAATarget> m_list;
};

#endif // TICKERAAMODEL_H
