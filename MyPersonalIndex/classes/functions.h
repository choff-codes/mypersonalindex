#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include <globals.h>

class functions
{
public:
    static QDateEdit* createDateEdit(QWidget* parent)
    {
        QDateEdit *d = new QDateEdit(parent);
        d->setDate(QDate::currentDate());
        d->setCalendarPopup(true);
        d->setDisplayFormat(globals::shortDateFormat);
        //d->setButtonSymbols(QAbstractSpinBox::NoButtons);
        return d;
    }
};

#endif // FUNCTIONS_H
