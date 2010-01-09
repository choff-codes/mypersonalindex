#include "frmAAEdit.h"

frmAAEdit::frmAAEdit(const int &portfolioID, QWidget *parent, const assetAllocation &aa)
    : QDialog(parent), m_portfolioID(portfolioID), m_aa(aa), m_aaOriginal(aa)
{
    ui.setupUI(this);
    this->setWindowTitle("Edit AA");

    ui.txtDesc->setText(m_aa.description);
    ui.sbTarget->setValue(m_aa.target);
    ui.txtDesc->setFocus();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.sbTarget, SIGNAL(valueChanged(double)), this, SLOT(adjustSpinBox(double)));
    connect(ui.btnClearTarget, SIGNAL(clicked()), this, SLOT(resetSpinBox()));
}

void frmAAEdit::accept()
{
    m_aa.description = ui.txtDesc->text();
    m_aa.target = ui.sbTarget->value();

    if (m_aa == m_aaOriginal)
    {
        QDialog::reject();
        return;
    }

    m_aa.save(m_portfolioID);
    QDialog::accept();
}

void frmAAEdit::adjustSpinBox(double d)
{
    if (d < 0 && d != -1)
        ui.sbTarget->setValue(-1);
}

void frmAAEdit::resetSpinBox()
{
    ui.sbTarget->setValue(-1);
}
