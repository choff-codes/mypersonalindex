#ifndef FRMACCT_H
#define FRMACCT_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "acctModel.h"

class frmAcct : public QDialog
{
    Q_OBJECT

public:
    QMap<int, account> getReturnValues() { return m_map; }

    frmAcct(const int &portfolioID, const QMap<int, account> &acct, QWidget *parent = 0);
    ~frmAcct() { delete m_model; }

private:
    frmTableViewBase_UI ui;
    int m_portfolioID;
    queries m_sql;
    acctModel *m_model;
    QMap<int, account> m_map;

    void connectSlots();

private slots:
    void accept();
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMACCT_H
