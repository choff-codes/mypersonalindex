#include "frmAAEdit.h"

frmAAEdit::frmAAEdit(const int &portfolioID, QWidget *parent, const assetAllocation &aa)
    : QDialog(parent), m_portfolioID(portfolioID), m_aa(aa), m_aaOriginal(aa)
{
    ui.setupUI(this);
    this->setWindowTitle(QString("%1 AA").arg(aa.id == -1 ? "Add" : "Edit"));

    ui.txtDesc->setText(m_aa.description);
    ui.sbTarget->setValue(m_aa.target * 100);
    ui.txtDesc->setFocus();
    ui.txtDesc->selectAll();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.sbTarget, SIGNAL(valueChanged(double)), this, SLOT(adjustSpinBox(double)));
    connect(ui.btnClearTarget, SIGNAL(clicked()), this, SLOT(resetSpinBox()));
}

void frmAAEdit::accept()
{
    if (hasValidationErrors())
        return;

    m_aa.description = ui.txtDesc->text();
    m_aa.target = ui.sbTarget->value() / 100;

    if (m_aa == m_aaOriginal)
    {
        QDialog::reject();
        return;
    }

    m_aa.save(m_portfolioID);
    QDialog::accept();
}

bool frmAAEdit::hasValidationErrors()
{
    if (ui.txtDesc->text().isEmpty())
    {
        QMessageBox::critical(this, "Description", "The description cannot be blank!");
        return true;
    }

    return false;
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
