#include "statModel.h"

statModel::statModel(const QList<globals::statistic> &values, const QList<int> &selected, const int &cols, QTableView *parent, QDialog *dialog):
        mpiEditModelBase<globals::statistic, frmStatEdit>(values, cols, parent, dialog)
{
    qSort(m_list);
    for(int i = 0; i < selected.count(); ++i)
    {
        int id = selected.at(i) ;
        for (int x = 0; x < m_list.count(); ++x)
            if (id == m_list[x].id)
            {
                m_list.move(x, i);
                break;
            }
    }

    for(int i = 0; i < m_list.count(); ++i)
        if (selected.contains(m_list.at(i).id))
            m_selected.insert(i, true);
        else
            m_selected.insert(i, false);

    emit dataChanged(index(0, 0), index(values.count() - 1, 0));

    connect(this, SIGNAL(rowsRemoved(const QModelIndex, int, int)), this, SLOT(itemDeleted(QModelIndex,int,int)));
}

QList<int> statModel::getSelected()
{
    QList<int> returnValues;
    for(int i = 0; i < m_list.count(); ++i)
        if (m_selected.value(i))
            returnValues.append(m_list.at(i).id);

    return returnValues;
}

Qt::ItemFlags statModel::flags(const QModelIndex &index) const
{
    return mpiEditModelBase<globals::statistic, frmStatEdit>::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant statModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_list.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        if (index.column() == 0)
            return m_list.at(index.row()).description;

    if (role == Qt::CheckStateRole)
        if (index.column() == 0)
            return m_selected.value(index.row()) ? Qt::Checked : Qt::Unchecked;

    return QVariant();
}

QVariant statModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    if (section == 0)
        return "Statistic";

    return QVariant();
}

bool statModel::setData (const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::CheckStateRole)
    {
        m_selected[index.row()] = value.toBool();
        return true;
    }

    return false;
}

void statModel::selectAll()
{
    for (int i = 0; i < m_selected.count(); ++i)
        m_selected[i] = true;
    emit dataChanged(index(0, 0), index(m_selected.count() - 1, 0));
}
void statModel::clearAll()
{
    for (int i = 0; i < m_selected.count(); ++i)
        m_selected[i] = false;
    emit dataChanged(index(0, 0), index(m_selected.count() - 1, 0));
}
void statModel::itemDeleted(const QModelIndex&, int start, int end)
{
    for(int i = start; i <= end; ++i)
        m_selected.remove(i);
}
