#include "mpiDateEdit.h"

mpiDateEdit::mpiDateEdit(QWidget *parent): QDateEdit(parent)
{
    setDate(QDate::currentDate());
    setCalendarPopup(true);
    setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
}
