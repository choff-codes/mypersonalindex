#include "frmOptions.h"

frmOptions::frmOptions(QWidget *parent, queries *sql, const globals::settings& s): QDialog(parent), m_sql(sql), m_settings(s), m_settingsOriginal(s)
{
    if (!m_sql || !m_sql->isOpen())
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

    if (m_settings == m_settingsOriginal)
    {
        QDialog::reject();
        return;
    }

    m_sql->executeNonQuery(m_sql->updateSettings(m_settings));
    QDialog::accept();
}
