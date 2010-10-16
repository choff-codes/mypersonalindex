#include "mpiDateEdit.h"

mpiDateEdit::mpiDateEdit(QWidget *parent_):
    QDateEdit(parent_)
{
    setDate(QDate::currentDate());
    setCalendarPopup(true);
    setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
}
