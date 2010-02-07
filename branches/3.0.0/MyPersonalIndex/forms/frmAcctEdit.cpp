#include "frmAcctEdit.h"

frmAcctEdit::frmAcctEdit(const int &portfolioID, QWidget *parent, const account &acct)
    : QDialog(parent), m_portfolioID(portfolioID), m_acct(acct), m_acctOriginal(acct)
{
    ui.setupUI(this);
    this->setWindowTitle(QString("%1 Account").arg(acct.id == -1 ? "Add" : "Edit"));

    ui.txtDesc->setText(m_acct.description);
    ui.sbTaxRate->setValue(m_acct.taxRate * 100);
    ui.chkTaxDeferred->setChecked(m_acct.taxDeferred);
    ui.cmbCostBasis->setCurrentIndex(m_acct.costBasis);
    ui.txtDesc->setFocus();
    ui.txtDesc->selectAll();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.sbTaxRate, SIGNAL(valueChanged(double)), this, SLOT(adjustSpinBox(double)));
    connect(ui.btnClearTaxRate, SIGNAL(clicked()), this, SLOT(resetSpinBox()));
}

void frmAcctEdit::accept()
{
    if (hasValidationErrors())
        return;

    m_acct.description = ui.txtDesc->text();
    m_acct.taxRate = ui.sbTaxRate->value() / 100;
    m_acct.taxDeferred = ui.chkTaxDeferred->isChecked();
    m_acct.costBasis = (account::costBasisType)ui.cmbCostBasis->currentIndex();

    if (m_acct == m_acctOriginal)
    {
        QDialog::reject();
        return;
    }

    m_acct.save(m_portfolioID);
    QDialog::accept();
}

bool frmAcctEdit::hasValidationErrors()
{
    if (ui.txtDesc->text().isEmpty())
    {
        QMessageBox::critical(this, "Description", "The description cannot be blank!");
        return true;
    }

    return false;
}

void frmAcctEdit::adjustSpinBox(double d)
{
    if (d < 0 && d != -1)
        ui.sbTaxRate->setValue(-1);
}

void frmAcctEdit::resetSpinBox()
{
    ui.sbTaxRate->setValue(-1);
}
