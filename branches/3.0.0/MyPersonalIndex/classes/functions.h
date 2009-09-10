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

//    static QList<QPair<QString, int> >* qMapToList(QMap<int, globals::assetAllocation>* aa)
//    {
//        QList<QPair<QString, int> > *returnValue = new QList<QPair<QString, int> >();
//        foreach (const globals::assetAllocation &value, aa)
//            returnValue->append(QPair(value.name, value.id));
//
//        return returnValue;
//    }
//
//    static QList<QPair<QString, int> >* qMapToList(QMap<int, globals::account>* acct)
//    {
//        QList<QPair<QString, int> > *returnValue = new QList<QPair<QString, int> >();
//        foreach (const globals::account &value, acct)
//            returnValue->append(QPair(value.name, value.id));
//
//        return returnValue;
//    }
};

#endif // FUNCTIONS_H
