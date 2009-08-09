#include "frmMain.h"
#include "frmMain_UI.h"
#include <QtGui>

frmMain::frmMain(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUI(this);
    ui.stbLastUpdated->setText(QString(ui.LAST_UPDATED_TEXT).append("1/2/2009"));
    ui.holdingsDateDropDown->setText(QString(ui.DATE).append(ui.DateCalendar->selectedDate().toString(Qt::SystemLocaleShortDate)).append(" "));
    connect(ui.DateCalendar, SIGNAL(clicked(QDate)), this, SLOT(holdingsDateChanged(QDate)));
    connect(ui.holdingsDateDropDown, SIGNAL(pressDown()), this, SLOT(pressed()));
}

void frmMain::holdingsDateChanged(QDate d)
{
    ui.DateMenu->hide();
    ui.holdingsDateDropDown->setText(QString(ui.DATE).append(d.toString(Qt::SystemLocaleShortDate)).append(" "));
    ui.holdingsDateDropDown->currentDate = d;
    ui.DateCalendar->setSelectedDate(d.addYears(10));
}

void frmMain::pressed()
{
    ui.DateCalendar->setSelectedDate(static_cast<mpiToolButton*>(sender())->currentDate);
}
