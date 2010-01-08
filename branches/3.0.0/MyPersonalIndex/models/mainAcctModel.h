#ifndef MAINACCTMODEL_H
#define MAINACCTMODEL_H

#include <QtGui>
#include "mpiViewModelBase.h"
#include "cachedCalculations.h"

class acctRow: public baseRow
{
public:
    enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_TaxRate, row_TaxLiability, row_Net, row_Holdings };
    static const QStringList columns;
    static const QVariantList columnsType;

    acctRow(const calculations::portfolioDailyInfo *info, const cachedCalculations::dailyInfo &acctInfo,
            const account &acct, const QString &sort);

    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();
};

class mainAcctModel: public mpiViewModelBase
{
public:

    mainAcctModel(const QList<baseRow*> &rows, QList<int> viewableColumns, const calculations::portfolioDailyInfo *info, QTableView *parent = 0):
        mpiViewModelBase(rows, viewableColumns, parent), m_totalValue(info->totalValue), m_taxLiability(info->taxLiability), m_costBasis(info->costBasis) { }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    double m_totalValue;
    double m_taxLiability;
    double m_costBasis;
};

#endif // MAINACCTMODEL_H
