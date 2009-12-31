#ifndef FRMAA_H
#define FRMAA_H

#include <QtGui>
#include "functions.h"
#include "frmTableViewBase_UI.h"
#include "aaModel.h"
#include "queries.h"


class frmAA : public QDialog
{
    Q_OBJECT

public:
    QMap<int, assetAllocation> getReturnValues()
    {
        return m_map;
    }

    frmAA(const int &portfolioID, const QMap<int, assetAllocation> &aa, const queries &sql, QWidget *parent = 0);
    ~frmAA() { delete m_model; }

private:
    frmTableViewBase_UI ui;
    QMap<int, assetAllocation> m_map;
    const queries &m_sql;
    int m_portfolio;
    aaModel *m_model;

    void connectSlots();

private slots:
    void accept();
    void saveItem(assetAllocation *aa);
    void deleteItem(const assetAllocation &aa);
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMAA_H
