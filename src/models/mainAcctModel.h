#ifndef MAINACCTMODEL_H
#define MAINACCTMODEL_H

#include "mpiViewModelBase.h"

#include <QStringList>
#include "mpiViewModelBase.h"
#include "snapshot.h"

class historicalNAV;
class calculatorNAV;
class account;
class acctRow: public baseRow
{
public:
    enum {
        row_Description,
        row_CostBasis,
        row_Value,
        row_ValueP,
        row_Gain,
        row_GainP,
        row_NAV,
        row_TaxRate,
        row_TaxLiability,
        row_Net,
        row_Holdings
    };

    static const QStringList columns;
    static const QVariantList columnsType;

    acctRow(double nav_, const snapshot &snapshot_, const snapshot &portfolioSnapshot_, const account &account_, const QList<orderBy> &columnSort_);

    QVariant columnType(int column_) const { return columnsType.at(column_); }
    static QMap<int, QString> fieldNames();

    static QList<baseRow*> getRows(const QMap<int, account> &accounts_, int beginDate_, int endDate_, calculatorNAV calculator_,
        const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_);

private:
    static baseRow* getRow(const account &account, int beginDate_, int endDate_, calculatorNAV calculator_,
        const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_);
};


class mainAcctModel: public mpiViewModelBase
{
public:
    mainAcctModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QObject *parent_ = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    snapshot m_portfolioSnapshot;
    double m_portfolioNAV;
};

#endif // MAINACCTMODEL_H
