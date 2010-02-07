#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtGui>
#include "qwt_plot.h"

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
    static void exportTable(const QTableView *table, const bool &includeRowLabels, QMainWindow *parent = 0);
    static QString exportClean(const QVariant &s, const QString &delimiter);
    static void exportChart(QwtPlot *chart, QMainWindow *parent);
    static bool lessThan(const QVariant &left, const QVariant &right, const QVariant &type);
    static bool equal(const QVariant &left, const QVariant &right, const QVariant &type);
    static QStringList exceptLeft(const QStringList &list1, const QStringList &list2);
};

#endif // FUNCTIONS_H
