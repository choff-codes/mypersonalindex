#ifndef FRMACCT_H
#define FRMACCT_H

#include <QtGui>
#include "frmTableViewBase.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "acctQueries.h"
#include "frmAcctEdit.h"

class frmAcct : public frmTableViewBase<globals::account, frmAcctEdit, acctQueries>
{
    Q_OBJECT

public:
    frmAcct(const int &portfolioID, QWidget *parent = 0, const QMap<int, globals::account> &acct = (QMap<int, globals::account>()));

private:
    void updateList(const globals::account &acct, const int &row = -1);
    void connectSlots();
    void saveItem(const globals::account &acct);
    void deleteItem(const globals::account &acct);

private slots:
    void accept();
    void addAcct();
    void editAcct();
    void removeAcct();
};

#endif // FRMACCT_H
