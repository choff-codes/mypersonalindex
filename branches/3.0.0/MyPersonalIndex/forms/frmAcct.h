#ifndef FRMACCT_H
#define FRMACCT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "acctModel.h"
#include "queries.h"
#include "functions.h"

class frmAcct : public QDialog
{
    Q_OBJECT

public:
    QMap<int, globals::account> getReturnValues() { return m_map; }

    frmAcct(const int &portfolioID, const QMap<int, globals::account> &acct, const queries &sql, QWidget *parent = 0);
    ~frmAcct() { delete m_model; }

private:
    frmTableViewBase_UI ui;
    int m_portfolio;
    const queries &m_sql;
    acctModel *m_model;
    QMap<int, globals::account> m_map;

    void connectSlots();

private slots:
    void accept();
    void saveItem(globals::account *acct);
    void deleteItem(const globals::account &acct);
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMACCT_H
