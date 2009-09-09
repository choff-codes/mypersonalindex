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

    static QList<QPair<QString, int> >* qMapToList(QMap<int, globals::assetAllocation>* aa)
    {
        QList<QPair<QString, int> > * returnValue = new QList<QPair<QString, int> >();
        foreach (const globals::assetAllocation &value, aa)
            returnValue->append(QPair(value.name, value.id));

        return returnValue;
    }

    static QList<QPair<QString, int> >* qMapToList(QMap<int, globals::account>* acct)
    {
        QList<QPair<QString, int> > * returnValue = new QList<QPair<QString, int> >();
        foreach (const globals::account &value, acct)
            returnValue->append(QPair(value.name, value.id));

        return returnValue;
    }

    static QStandardModel* qPairListToModel(QList<QPair<QString, int> >* list)
    {

    }
};

#endif // FUNCTIONS_H
