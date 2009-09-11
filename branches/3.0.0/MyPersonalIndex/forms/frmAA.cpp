#include "frmAA.h"

frmAA::frmAA(QWidget *parent, const QMap<int, globals::assetAllocation> &aa): QDialog(parent), m_aa(aa)
{
    ui.setupUI(this, "Desired Allocation", true);
}

void frmAA::accept()
{
    QDialog::accept();
}
