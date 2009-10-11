#ifndef FRMACCT_H
#define FRMACCT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "queries.h"
#include "frmAcctEdit.h"

class acctModel;
class frmAcct : public QDialog
{
    Q_OBJECT

public:
    QMap<int, globals::account> getReturnValues()
    {
        return m_map;
    }

    frmAcct(const int &portfolioID, QWidget *parent = 0, queries *sql = 0, const QMap<int, globals::account> &acct = (QMap<int, globals::account>()));

private:
    frmTableViewBase_UI ui;
    queries *m_sql;
    acctModel *m_model;
    QMap<int, globals::account> m_map;
    int m_portfolio;

    void connectSlots();

private slots:
    void accept();
    void saveItem(globals::account *acct);
    void deleteItem(const globals::account &acct);
};

class acctModel : public modelWithNoEdit<globals::account, frmAcctEdit>
{
    Q_OBJECT

public:
    acctModel(const QList<globals::account> &values, const int &cols = 0, QTableView *parent = 0, QDialog *dialog = 0):
            modelWithNoEdit<globals::account, frmAcctEdit>(values, cols, parent, dialog) { }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= m_list.size())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            if (index.column() == 0)
            {
                return m_list.at(index.row()).description;
            }
            if (index.column() == 1)
            {
                return m_list.at(index.row()).taxRate < 0 ? "None" :
                    QLocale().toString(m_list.at(index.row()).taxRate, 'f', 2).append("%");
            }
            if (index.column() == 2)
            {
                return m_list.at(index.row()).taxDeferred  ? "Yes" : "No";
            }
        }

        return QVariant();
    }

    QVariant headerData (int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        if (section == 0)
            return "Description";

        if (section == 1)
            return "Tax Rate";

        if (section == 2)
            return "Tax Deferred";

        return QVariant();
    }

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }

signals:
    void saveItem(globals::account *acct);
    void deleteItem(const globals::account& acct);
};

#endif // FRMACCT_H
