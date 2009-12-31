#ifndef ACCTMODEL_H
#define ACCTMODEL_H

#include "functions.h"
#include "mpiEditModelBase.h"
#include "frmAcctEdit.h"


class acctModel : public mpiEditModelBase<account, frmAcctEdit>
{
    Q_OBJECT

public:
    acctModel(const QList<account> &values, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
            mpiEditModelBase<account, frmAcctEdit>(values, cols, parent, dialog) { }

private:
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QString internalCopy(const account &item);
    account internalPaste(const QStringList &value, bool *ok);

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }
    inline void copy() { beginCopy(); }
    inline void paste() { beginPaste(); }

signals:
    void saveItem(account *acct);
    void deleteItem(const account& acct);
};

#endif // ACCTMODEL_H
