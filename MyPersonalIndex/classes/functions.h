#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>

class functions
{
public:
    static QDateEdit* createDateEdit(QWidget* parent = 0);
    static void showWelcomeMessage(QWidget *parent = 0);
    static QVariant doubleToNull(const double &value);
    static QVariant intToNull(const int &value);
    static QVariant dateToNull(const int &value);
    static QString doubleToCurrency(const double &value);
    static QString doubleToPercentage(const double &value);
    static QString doubleToLocalFormat(const double &value, const int &precision = 2);
    static double stringToDouble(const QString &value, bool *ok);
    static void exportTable(const QTableView *table, QMainWindow *parent);
    static bool lessThan(const QVariant &left, const QVariant &right, const QVariant &type);
    static bool greaterThan(const QVariant &left, const QVariant &right, const QVariant &type);
    static bool equal(const QVariant &left, const QVariant &right, const QVariant &type);
};

#endif // FUNCTIONS_H
