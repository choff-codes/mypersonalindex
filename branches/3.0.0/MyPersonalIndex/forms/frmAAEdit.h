#ifndef FRMAAEDIT_H
#define FRMAAEDIT_H

#include <QtGui>
#include "frmAAEdit_UI.h"
#include "globals.h"

class frmAAEdit : public QDialog
{
    Q_OBJECT

public:
    const globals::assetAllocation& getReturnValues() const { return m_aa; }

    frmAAEdit(QWidget *parent = 0, const globals::assetAllocation &aa = globals::assetAllocation());

private:
    frmAAEdit_UI ui;
    globals::assetAllocation m_aa;

private slots:
    void accept();
    void adjustSpinBox(double);
};

#endif // FRMAAEDIT_H
