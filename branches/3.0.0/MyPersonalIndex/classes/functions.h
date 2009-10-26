#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include "globals.h"

class functions
{
public:
    static QDateEdit* createDateEdit(QWidget* parent = 0)
    {
        QDateEdit *d = new QDateEdit(parent);
        d->setDate(QDate::currentDate());
        d->setCalendarPopup(true);
        d->setDisplayFormat(globals::shortDateFormat);
        return d;
    }

    static void showWelcomeMessage(QWidget *parent = 0)
    {
        QString welcomeMessage = "Welcome to My Personal Index!\n\nThere is no documentation yet,"
                                 " but I recommend starting in the following way:\n\n1. Set the start date under options (on the top toolbar).\n"
                                 "2. Add a new Portfolio\n3. Set your asset allocation \n4. Set your accounts\n5. Add holdings\n"
                                 "6. Add relevant portfolio statistics\n7. Update prices!";
        QMessageBox msg(QMessageBox::Information, "My Personal Index", welcomeMessage, QMessageBox::Ok, parent);
        QPushButton *copyButton = msg.addButton("Copy To Clipboard", QMessageBox::RejectRole);

        msg.exec();

        if (msg.clickedButton() == copyButton)
            QApplication::clipboard()->setText(welcomeMessage);
    }

    static QVariant doubleToNull(const double &value)
    {
        return value < 0 ? QVariant(QVariant::Double) : value;
    }

    static QVariant intToNull(const int &value)
    {
        return value < 0 ? QVariant(QVariant::Int) : value;
    }

    static QVariant dateToNull(const QDate &value)
    {
        return value.isValid() ? QVariant(QVariant::Date) : value;
    }

    static QString doubleToCurrency(const double &value)
    {
        return QString("$%L1").arg(value, 0, 'f', 2);
    }

    static QString doubleToPercentage(const double &value)
    {
        return QString("%L1%").arg(value, 0, 'f', 2);
    }

    static QString doubleToLocalFormat(const double &value)
    {
        return QString("%L1").arg(value, 0, 'f', 2);
    }

    static double stringToDouble(const QString &value, bool *ok)
    {
        QString s = value;
        return s.remove('%').replace("None", "-1", Qt::CaseInsensitive).toDouble(ok);
    }
};

#endif // FUNCTIONS_H
