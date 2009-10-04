#ifndef FRMTABLEVIEWBASE_H
#define FRMTABLEVIEWBASE_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "functions.h"
#include "queries.h"

template<class T, class editForm>
class frmTableViewBase : public QDialog
{
public:
    const QMap<int, T>& getReturnValues() const { return m_map; }

    frmTableViewBase(const int &portfolioID, QWidget *parent = 0, queries *sql = 0, const QMap<int, T> &map = (QMap<int, T>()),
        const bool &showRightSideButtons = false, const QString &groupText = "", const int &columns = 0):
        QDialog(parent), m_portfolioID(portfolioID), m_sql(sql),  m_mapOriginal(map)
    {
        ui.setupUI(this, groupText, showRightSideButtons);

        m_list = m_mapOriginal.values();
        m_model = new modelWithNoEdit(m_list.count(), columns, ui.table);
        ui.table->setModel(m_model); 

        // derived class must call load items in their load method
    }

protected:
    frmTableViewBase_UI ui;
    int m_portfolioID;
    queries *m_sql;
    QMap<int, T> m_map; // stores final values, not modified until closing
    QMap<int, T> m_mapOriginal; // stores original values
    QList<T> m_list; // **use this, updated as user adds/edits items
    modelWithNoEdit *m_model;

    virtual void updateList(const T &item, const int &row = -1) = 0;
    virtual void saveItem(const T &item) = 0;
    virtual void deleteItem(const T &item) = 0;
    virtual void updateHeader() { /* not required */ }

    void loadItems()
    {
        for(int i = 0; i < m_list.count(); ++i)
            updateList(m_list.at(i), i);

        updateHeader();
    }

    void editItem()
    {
        QModelIndexList il = ui.table->selectionModel()->selectedRows();
        if(il.count() == 0)
            return;

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
            selectItem(q);
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
        QList<int> indexes = functions::getSelectedRows(ui.table->selectionModel()->selectedRows());
        if(indexes.count() == 0)
            return;

        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            m_list.removeAt(indexes.at(i));
            m_model->removeRow(indexes.at(i));
        }
        updateHeader();
    }

    void accept(bool changes = false)
    {
        m_sql->getDatabase().transaction();

        for(int i = 0; i < m_list.count(); ++i) // save all items
        {
            T item = m_list.value(i);
            if (item.id == -1 || m_mapOriginal.value(item.id) != item)
            {
                changes = true;
                saveItem(item);
                if (item.id == -1)
                {
                    item.id = m_sql->executeScalar(m_sql->getIdentity()).toInt();
                    m_list[i].id = item.id;
                }
            }
            m_map.insert(item.id, item);
        }

        foreach(const T &item, m_mapOriginal) // delete items that have been removed
            if(!m_map.contains(item.id))
            {
                changes = true;
                deleteItem(item);
            }

        m_sql->getDatabase().commit();

        if (changes)
            QDialog::accept();
        else
            QDialog::reject();
    }

    void selectAll()
    {
        for(int i = 0; i < m_model->rowCount(); ++i)
            m_model->item(i, 0)->setCheckState(Qt::Checked);
        updateHeader();
    }

    void clearAll()
    {
        for(int i = 0; i < m_model->rowCount(); ++i)
            m_model->item(i, 0)->setCheckState(Qt::Unchecked);
        updateHeader();
    }

    void moveUp()
    {
        QList<int> indexes = functions::getSelectedRows(ui.table->selectionModel()->selectedRows());
        if(indexes.count() == 0)
            return;

        if (functions::isContiguous(indexes, true, m_model->rowCount()))
            return;

        for(int i = 0; i < indexes.count(); ++i)
        {
            int row = indexes.value(i);
            if (row == 0)
                continue;

            m_model->insertRow(row-1, m_model->takeRow(row));
            m_list.swap(row, row-1);

            selectItem(m_model->index(row-1, 0));
        }
        updateHeader();
    }

    void moveDown()
    {
        QList<int> indexes = functions::getSelectedRows(ui.table->selectionModel()->selectedRows());
        if(indexes.count() == 0)
            return;

        int rowCount = m_model->rowCount();
        if (functions::isContiguous(indexes, false, rowCount))
            return;

        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            int row = indexes.value(i);
            if (row == rowCount - 1)
                continue;

            m_model->insertRow(row+1, m_model->takeRow(row));
            m_list.swap(row, row+1);

            selectItem(m_model->index(row+1, 0));
        }
        updateHeader();
    }

    void selectItem(const QModelIndex &index)
    {
        ui.table->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
};

#endif // FRMTABLEVIEWBASE_H
