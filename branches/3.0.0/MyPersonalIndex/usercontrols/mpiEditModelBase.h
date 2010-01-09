#ifndef MPIEDITMODELBASE_H
#define MPIEDITMODELBASE_H

#include <QtGui>

template<class T, class editForm>
class mpiEditModelBase: public QAbstractTableModel
{    
public:

    QList<T> getList() { return m_list; }

    mpiEditModelBase(const QList<T> &values, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
            QAbstractTableModel(parent), m_parent(parent), m_dialog(dialog), m_columns(cols), m_list(values)
    {
        insertRows(0, m_list.count());
        updateHeader();
    }

    int rowCount(const QModelIndex&) const { return m_list.count(); }
    int columnCount (const QModelIndex&) const { return m_columns; }
    void beginCopy();
    void beginPaste();
    void editItems();
    void addItem();
    void removeItems();
    QMap<int, T> saveList(const QMap<int, T> &originalValues, const int parentID);

protected:
    QTableView *m_parent;
    QDialog *m_dialog;
    int m_columns;
    QList<T> m_list;

    virtual void editSelected() = 0;
    virtual void deleteSelected() = 0;
    virtual void addNew() = 0;
    virtual QString internalCopy(const T &item) = 0;
    virtual T internalPaste(const QStringList &value, bool *ok) = 0;

    void updateHeader() { emit headerDataChanged(Qt::Horizontal, 0, m_columns - 1); }
    QList<int> getSelectedRows() const;

private:
    void selectItem(const QModelIndex &index)  { m_parent->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select | QItemSelectionModel::Rows); }
};

#endif // MPIEDITMODELBASE_H
