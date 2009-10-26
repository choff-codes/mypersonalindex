#ifndef FRMSTAT_H
#define FRMSTAT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "mpiModelBase.h"
#include "queries.h"
#include "frmStatEdit.h"

class statModel;
class frmStat : public QDialog
{
    Q_OBJECT

public:
    QMap<int, globals::statistic> getReturnValues_Map()
    {
        return m_map;
    }

    QList<int> getReturnValues_Selected()
    {
        return m_selected;
    }

    frmStat(const int &portfolioID, QWidget *parent = 0, queries *sql = 0,
        const QMap<int, globals::statistic> &stat = (QMap<int, globals::statistic>()), const QList<int> &statList = (QList<int>()));

private:
    frmTableViewBase_UI ui;
    queries *m_sql;
    statModel *m_model;
    QMap<int, globals::statistic> m_map;
    int m_portfolio;
    QList<int> m_selected;

    void connectSlots();

private slots:
    void accept();
    void saveItem(globals::statistic *stat);
    void deleteItem(const globals::statistic &stat);
};

class statModel : public mpiModelBase<globals::statistic, frmStatEdit>
{
    Q_OBJECT

public:
    statModel(const QList<globals::statistic> &values, const QList<int> &selected, const int &cols = 0, QTableView *parent = 0, QDialog *dialog = 0):
            mpiModelBase<globals::statistic, frmStatEdit>(values, cols, parent, dialog)
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

    QList<int> getSelected()
    {
        QList<int> returnValues;
        for(int i = 0; i < m_list.count(); ++i)
            if (m_selected.value(i))
                returnValues.append(m_list.at(i).id);

        return returnValues;
    }

private:
    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        return mpiModelBase<globals::statistic, frmStatEdit>::flags(index) | Qt::ItemIsUserCheckable;
    }

    QVariant data(const QModelIndex &index, int role) const
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

    QVariant headerData (int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        if (section == 0)
            return "Statistic";

        return QVariant();
    }

    bool setData (const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && role == Qt::CheckStateRole)
        {
            m_selected[index.row()] = value.toBool();
            return true;
        }

        return false;
    }

    QString internalCopy(const globals::statistic&)
    {
        // not used
        return QString();
    }

    globals::statistic internalPaste(const QStringList&, bool*)
    {
        // not used
        return globals::statistic();
    }

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }
    inline void moveSelectedUp() { moveUp(); }
    inline void moveSelectedDown() { moveDown(); }
    void selectAll()
    {
        for (int i = 0; i < m_selected.count(); ++i)
            m_selected[i] = true;
        emit dataChanged(index(0, 0), index(m_selected.count() - 1, 0));
    }
    void clearAll()
    {
        for (int i = 0; i < m_selected.count(); ++i)
            m_selected[i] = false;
        emit dataChanged(index(0, 0), index(m_selected.count() - 1, 0));
    }
    void itemDeleted(const QModelIndex&, int start, int end)
    {
        for(int i = start; i <= end; ++i)
            m_selected.remove(i);
    }

signals:
    void saveItem(globals::statistic *stat);
    void deleteItem(const globals::statistic& stat);

private:
    QMap<int, bool> m_selected;
};

#endif // FRMSTAT_H
