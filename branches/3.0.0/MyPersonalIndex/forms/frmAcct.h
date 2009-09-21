#ifndef FRMACCT_H
#define FRMACCT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "acctQueries.h"

class frmAcct : public QDialog
{
    Q_OBJECT

public:
    const QMap<int, globals::account>& getReturnValues() const { return m_acctMap; }

    frmAcct(const int &portfolioID, QWidget *parent = 0, const QMap<int, globals::account> &acct = QMap<int, globals::account>());
    ~frmAcct() { delete sql; }

private:
    frmTableViewBase_UI ui;
    acctQueries *sql;
    QMap<int, globals::account> m_acctMap;
    QList<globals::account> m_acct;
    modelWithNoEdit *m_model;
    int m_portfolioID;

    void updateList(const globals::account &acct, const int &row = -1);
    void loadAcct();
    void connectSlots();

private slots:
    void accept();
    void addAcct();
    void editAcct();
    void deleteAcct();
};

#endif // FRMACCT_H
