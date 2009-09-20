#include "frmOptions.h"

frmOptions::frmOptions(QWidget *parent, const globals::settings& s): QDialog(parent), m_settings(s)
{
    if (!sql.isOpen())
    {
        this->reject();
        return;
    }

    ui.setupUI(this);
    ui.chkSplits->setChecked(m_settings.splits);
    ui.deDownloadDate->setDate(m_settings.dataStartDate);

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmOptions::accept()
{
    m_settings.dataStartDate = ui.deDownloadDate->date();
    m_settings.splits = ui.chkSplits->isChecked();

    sql.executeNonQuery(sql.updateSettings(&m_settings));

    QDialog::accept();
}
