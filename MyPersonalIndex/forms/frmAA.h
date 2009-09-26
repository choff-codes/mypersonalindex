#ifndef FRMAA_H
#define FRMAA_H

#include <QtGui>
#include "frmTableViewBase.h"
#include "globals.h"
#include "modelWithNoEdit.h"
#include "queries.h"
#include "frmAAEdit.h"

class frmAA : public frmTableViewBase<globals::assetAllocation, frmAAEdit>
{
    Q_OBJECT

public:
    frmAA(const int &portfolioID, QWidget *parent = 0, queries *sql = 0, const QMap<int, globals::assetAllocation> &aa = (QMap<int, globals::assetAllocation>()));

private:
    void updateList(const globals::assetAllocation &aa, const int &row = -1);
    void updateHeader();
    void connectSlots();
    void saveItem(const globals::assetAllocation &aa);
    void deleteItem(const globals::assetAllocation &aa);

private slots:
    void accept();
    void addAA();
    void editAA();
    void removeAA();
};

#endif // FRMAA_H
