#ifndef FRMAA_H
#define FRMAA_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "aaQueries.h"

class frmAA : public QDialog
{
    Q_OBJECT

public:
    const QMap<int, globals::assetAllocation>& getReturnValues() const { return m_aaMap; }

    frmAA(const int &portfolioID, QWidget *parent = 0, const QMap<int, globals::assetAllocation> &aa = QMap<int, globals::assetAllocation>());
    ~frmAA() { delete sql; }

private:
    frmTableViewBase_UI ui;
    aaQueries *sql;
    QMap<int, globals::assetAllocation> m_aaMap;
    QList<globals::assetAllocation> m_aa;
    modelWithNoEdit *m_model;
    int m_portfolioID;

    void updateList(const globals::assetAllocation &aa, const int &row = -1);
    void updateHeader();
    void loadAA();
    void connectSlots();

private slots:
    void accept();
    void addAA();
    void editAA();
    void deleteAA();
};

#endif // FRMAA_H
