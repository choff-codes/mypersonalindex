#ifndef FRMTABLEVIEWBASE_H
#define FRMTABLEVIEWBASE_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "queries.h"

template<class T, class editForm, class queryClass>
class frmTableViewBase : public QDialog
{
public:
    const QMap<int, T>& getReturnValues() const { return m_map; }

    frmTableViewBase(const int &portfolioID, QWidget *parent = 0, const QMap<int, T> &map = QMap<int, T>(),
                     const bool &showRightSideButtons = false, const QString &groupText = "", const int &columns = 0):
                     QDialog(parent), m_map(map), m_portfolioID(portfolioID)
    {
        sql = new queryClass(m_portfolioID);

        ui.setupUI(this, groupText, showRightSideButtons);
        ui.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

        m_list = m_map.values();
        m_model = new modelWithNoEdit(m_list.count(), columns, ui.table);
        ui.table->setModel(m_model);
    }

    ~frmTableViewBase() { delete sql; }

protected:
    frmTableViewBase_UI ui;
    QMap<int, T> m_map;
    QList<T> m_list;
    modelWithNoEdit *m_model;
    int m_portfolioID;
    queryClass *sql;


    virtual void updateList(const T &item, const int &row = -1) = 0;
    virtual void saveItem(const T &item) = 0;
    virtual void deleteItem(const T &item) = 0;
    virtual void updateHeader() { }

    void loadItems()
    {
        int i = 0;
        foreach(const T &item, m_list)
        {
            updateList(item, i);
            i++;
        }
        updateHeader();
    }

    void editItem()
    {
        QModelIndexList il = ui.table->selectionModel()->selectedRows();

        foreach(const QModelIndex &q, il)
        {
            int i = q.row();
            editForm f(this, m_list.at(i));

            if (f.exec())
            {
                T item = f.getReturnValues();
                m_list[i] = item;
                updateList(item, i);
            }
            ui.table->selectionModel()->setCurrentIndex(q, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
        updateHeader();
    }

    void addItem()
    {
        editForm f(this);

        if (f.exec())
        {
            T item = f.getReturnValues();
            m_list.append(item);
            updateList(item);
            updateHeader();
        }
    }

    void removeItem()
    {
        QModelIndexList il = ui.table->selectionModel()->selectedRows();
        QList<int> indexes;
        foreach(const QModelIndex &q, il)
            indexes.append(q.row());
        qSort(indexes);

        for(int i = indexes.count() - 1; i >= 0; i--)
        {
            m_list.removeAt(indexes.at(i));
            m_model->removeRow(indexes.at(i));
        }
    }

    void accept()
    {
        if(!sql)
            return;

        QMap<int, T> toReturn;
        bool changes = false;

        sql->getDatabase().transaction();

        for(int i = 0; i < m_list.count(); i++)
        {
            if (m_list[i].id == -1 || m_map.value(m_list[i].id) != m_list[i])
            {
                changes = true;
                saveItem(m_list[i]);
                if (m_list[i].id == -1)
                    m_list[i].id = sql->executeScalar(sql->getIdentity()).toInt();
            }
            toReturn.insert(m_list[i].id, m_list[i]);
        }

        foreach(const T &item, m_map)
            if(!toReturn.contains(item.id))
            {
                changes = true;
                deleteItem(item);
            }

        sql->getDatabase().commit();

        if (changes)
        {
            m_map = toReturn;
            QDialog::accept();
        }
        else
            QDialog::reject();
    }
};

#endif // FRMTABLEVIEWBASE_H
