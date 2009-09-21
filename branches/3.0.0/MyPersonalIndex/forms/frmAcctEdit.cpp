#include "frmAcctEdit.h"

frmAcctEdit::frmAcctEdit(QWidget *parent, const globals::account &acct): QDialog(parent), m_acct(acct)
{
    ui.setupUI(this);
    this->setWindowTitle(m_acct.id == -1 ? "Add Account" : "Edit Account");

    ui.txtDesc->setText(m_acct.description);
    ui.sbTaxRate->setValue(m_acct.taxRate);
    ui.chkTaxDeferred->setChecked(m_acct.taxDeferred);
    ui.txtDesc->setFocus();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.sbTaxRate, SIGNAL(valueChanged(double)), this, SLOT(adjustSpinBox(double)));
}

void frmAcctEdit::accept()
{
    m_acct.description = ui.txtDesc->text();
    m_acct.taxRate = ui.sbTaxRate->value();
    m_acct.taxDeferred = ui.chkTaxDeferred->isChecked();

    QDialog::accept();
}

void frmAcctEdit::adjustSpinBox(double d)
{
    if (d < 0 && d != -1)
        ui.sbTaxRate->setValue(-1);
}
