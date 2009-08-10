#include "frmMain.h"
#include <QtGui>

frmMain::frmMain(QWidget *parent) : QMainWindow(parent)
{    
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, "MyPersonalIndex", "MPI");
    QString appDir = QFileInfo(cfg.fileName()).absolutePath().append("/MPI.sqlite");
    if (!QFile::exists(appDir))
        if (!QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), appDir))
            QMessageBox::critical(this, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);

    ui.setupUI(this);
    ui.stbLastUpdated->setText(QString(ui.LAST_UPDATED_TEXT).append("1/2/2009"));

    connectDateButton(ui.holdingsDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.statStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.statEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.chartStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.chartEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.correlationsStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.correlationsEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.accountsDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.aaDateDropDown, QDate(2009, 8, 9));
    connect(ui.DateCalendar, SIGNAL(clicked(QDate)), this, SLOT(dateChanged(QDate)));

    m_sql = new queries();
}

void frmMain::connectDateButton(mpiToolButton *t, const QDate &date)
{
    t->setDate(date);
    setDateDropDownText(t);
    connect(t, SIGNAL(pressDown()), this, SLOT(dateButtonPressed()));
}

void frmMain::dateChanged(QDate d)
{
    if (!ui.DateMenu->isVisible())
        return;

    ui.DateMenu->hide();

    if (!m_mpiButtonPressed || !d.isValid())
        return;

    m_mpiButtonPressed->setDate(d);
    setDateDropDownText(m_mpiButtonPressed);
}

void frmMain::setDateDropDownText(mpiToolButton *t)
{
    if (!t)
        return;

    QString str;
    switch(t->Type())
    {
        case mpiToolButton::StartDate:
            str = ui.START_DATE;
            break;
        case mpiToolButton::EndDate:
            str = ui.END_DATE;
            break;
        default:
            str = ui.DATE;
    }
    t->setText(str.append(t->date().toString(Qt::SystemLocaleShortDate)).append(" "));
}

void frmMain::dateButtonPressed()
{
    m_mpiButtonPressed = static_cast<mpiToolButton*>(sender());
    ui.DateCalendar->setSelectedDate(m_mpiButtonPressed->date());
}
