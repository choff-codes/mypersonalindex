#ifndef FRMAA_H
#define FRMAA_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "aaModel.h"


class frmAA : public QDialog
{
    Q_OBJECT

public:
    QMap<int, assetAllocation> getReturnValues()
    {
        return m_map;
    }

    frmAA(const int &portfolioID, const QMap<int, assetAllocation> &aa, QWidget *parent = 0);
    ~frmAA() { delete m_model; }

private:
    frmTableViewBase_UI ui;
    QMap<int, assetAllocation> m_map;
    int m_portfolioID;
    aaModel *m_model;

    void connectSlots();

private slots:
    void accept();
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMAA_H
