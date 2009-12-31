#include "frmStatEdit.h"

frmStatEdit::frmStatEdit(QWidget *parent, const statistic &stat): QDialog(parent), m_stat(stat)
{
    ui.setupUI(this);
    this->setWindowTitle("Edit Statistic");

    ui.txtDesc->setText(m_stat.description);
    ui.txtSql->setPlainText(m_stat.sql);
    ui.cmbFormat->setCurrentIndex((int)m_stat.format);
    ui.txtDesc->setFocus();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmStatEdit::accept()
{
    m_stat.description = ui.txtDesc->text();
    m_stat.sql = ui.txtSql->toPlainText();
    m_stat.format = (statistic::outputFormat)ui.cmbFormat->currentIndex();

    QDialog::accept();
}
