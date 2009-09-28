#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include <globals.h>

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

    static bool isContiguous(const QList<int> &values, const bool &ascending, const int &count)
    {
        int x = ascending ? 0 : count - 1;
        bool contiguous = true;

        if (ascending)
        {
            for(int i = 0; i < values.count(); ++i, ++x)
                if (values.value(i) != x)
                {
                    contiguous = false;
                    break;
                }
        }
        else
        {
            for(int i = values.count() - 1; i >= 0; --i, --x)
                if (values.value(i) != x)
                {
                    contiguous = false;
                    break;
                }
        }
        return contiguous;
    }

    static QList<int> getSelectedRows(const QModelIndexList &model)
    {
        QList<int> indexes;
        if (model.count() == 0)
            return indexes;

        foreach(const QModelIndex &q, model)
            indexes.append(q.row());
        qSort(indexes);

        return indexes;
    }
};

#endif // FUNCTIONS_H
