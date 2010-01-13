#include "frmOptions.h"

frmOptions::frmOptions(const settings &s, QWidget *parent): QDialog(parent), m_settings(s), m_settingsOriginal(s)
{
    ui.setupUI(this);
    ui.chkSplits->setChecked(m_settings.splits);
    ui.deDownloadDate->setDate(QDate::fromJulianDay(m_settings.dataStartDate));

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmOptions::accept()
{
    m_settings.dataStartDate = ui.deDownloadDate->date().toJulianDay();
    m_settings.splits = ui.chkSplits->isChecked();

    if (m_settings == m_settingsOriginal)
    {
        QDialog::reject();
        return;
    }

    if (m_settings.dataStartDate != m_settingsOriginal.dataStartDate)
        if (QMessageBox::question(this, "Start Date Change", "Are you sure you want to change the data start date?"
            " This will cause you to redownload all price data. If any portfolios have a start date before this date,"
            " they will be adjusted to the new start date.", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
                return;

    m_settings.save();
    QDialog::accept();
}
