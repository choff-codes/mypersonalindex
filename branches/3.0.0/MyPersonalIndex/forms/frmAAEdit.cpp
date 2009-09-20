#include "frmAAEdit.h"

frmAAEdit::frmAAEdit(QWidget *parent, const globals::assetAllocation &aa): QDialog(parent), m_aa(aa)
{
    ui.setupUI(this);
    this->setWindowTitle(m_aa.id == -1 ? "Add AA" : "Edit AA");

    ui.txtDesc->setText(m_aa.description);
    ui.sbTarget->setValue(m_aa.target);
    ui.txtDesc->setFocus();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.sbTarget, SIGNAL(valueChanged(double)), this, SLOT(adjustSpinBox(double)));
}

void frmAAEdit::accept()
{
    m_aa.description = ui.txtDesc->text();
    m_aa.target = ui.sbTarget->value();

    QDialog::accept();
}

void frmAAEdit::adjustSpinBox(double d)
{
    if (d < 0 && d != -1)
        ui.sbTarget->setValue(-1);
}
