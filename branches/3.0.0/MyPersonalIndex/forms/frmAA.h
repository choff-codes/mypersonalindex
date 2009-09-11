#ifndef FRMAA_H
#define FRMAA_H

#include <QtGui>
#include "frmTableViewBase_UI.h"
#include "globals.h"

class frmAA : public QDialog
{
    Q_OBJECT

public:
     frmAA(QWidget *parent = 0, const QMap<int, globals::assetAllocation> &aa = QMap<int, globals::assetAllocation>());

private:
     frmTableViewBase_UI ui;
     QMap<int, globals::assetAllocation> m_aa;

private slots:
    void accept();
};

#endif // FRMAA_H
