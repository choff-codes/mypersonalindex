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

    static QVariant dateToNull(const int &value)
    {
        return value == 0 ? QVariant(QVariant::Int) : value;
    }

    static QString doubleToCurrency(const double &value)
    {
        return QString("$%L1").arg(value, 0, 'f', 2);
    }

    static QString doubleToPercentage(const double &value)
    {
        return QString("%L1%").arg(value, 0, 'f', 2);
    }

    static QString doubleToLocalFormat(const double &value, const int &precision = 2)
    {
        return QString("%L1").arg(value, 0, 'f', precision);
    }

    static double stringToDouble(const QString &value, bool *ok)
    {
        QString s = value;
        return s.remove('%').replace("None", "-1", Qt::CaseInsensitive).toDouble(ok);
    }

    static void exportTable(const QTableView *table, QMainWindow *parent)
    {
        QString fileType, filePath;
        filePath = QFileDialog::getSaveFileName(parent, "Export to...", QDir::homePath(),
            "Tab Delimited File (*.txt);;Comma Delimited File (*.csv);;Pipe Delimited File (*.txt)", &fileType);
    }

    static bool lessThan(const QVariant &left, const QVariant &right, const QVariant &type)
    {
        if ((left.isNull() && right.isNull()) || right.isNull())
            return false;

        if (left.isNull())
            return true;

        switch (type.type())
        { 
            case QVariant::Double:
                return left.toDouble() < right.toDouble();
            case QVariant::Int:
                return left.toInt() < right.toInt();
            case QVariant::String:
            default:
                return left.toString().compare(right.toString(), Qt::CaseInsensitive) < 0;
        };
    }

    static bool greaterThan(const QVariant &left, const QVariant &right, const QVariant &type)
    {
        if ((left.isNull() && right.isNull()) || left.isNull())
            return false;

        if (right.isNull())
            return true;

        switch (type.type())
        {
            case QVariant::Double:
                return left.toDouble() > right.toDouble();
            case QVariant::Int:
                return left.toInt() > right.toInt();
            case QVariant::String:
            default:
                return left.toString().compare(right.toString(), Qt::CaseInsensitive) > 0;
        };
    }
};

#endif // FUNCTIONS_H
