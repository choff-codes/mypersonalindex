#ifndef AVGPRICEPERSHARE_H
#define AVGPRICEPERSHARE_H

#include <QList>
#include <QPair>
#include "executedTrade.h"
#include "account.h"
#include "splits.h"
#include "epsilon.h"

class avgPricePerShare
{
public:
    static double calculate(const QString &symbol, const QList<executedTrade> &trades, const account::costBasisType &type, const int &date);

private:
    typedef QPair<double /* shares */, double /* price */> sharePricePair;

    static double firstOut(const QString &symbol, const QList<executedTrade> &trades, const account::costBasisType &type, const int &date);
    static double average(const QString &symbol, const QList<executedTrade> &trades, const int &date);
    static double hifo(const QString &symbol, const QList<executedTrade> &trades, const int &date);
};

#endif // AVGPRICEPERSHARE_H
