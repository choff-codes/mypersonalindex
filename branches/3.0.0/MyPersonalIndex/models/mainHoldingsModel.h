#ifndef MAINHOLDINGSMODEL_H
#define MAINHOLDINGSMODEL_H

#include <QtGui>
#include "mpiViewModelBase.h"
#include "prices.h"
#include "cachedCalculations.h"

class holdingsRow: public baseRow
{
public:
    enum { row_Active, row_Symbol, row_Cash, row_Price, row_Shares, row_Avg, row_Cost, row_Value, row_ValueP, row_Gain, row_GainP, row_Acct, row_AA, row_TaxLiability, row_NetValue, row_ID };
    static const QStringList columns;
    static const QVariantList columnsType;

    holdingsRow(const security &s, const calculations::portfolioDailyInfo *info,
        const QMap<int, account> &accounts, const QMap<int, assetAllocation> &aa, const QString &sort);

    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();
};

class mainHoldingsModel: public mpiViewModelBase
{
public:

    mainHoldingsModel(const QList<baseRow*> &rows, QList<int> viewableColumns, const calculations::portfolioDailyInfo *info, QTableView *parent = 0):
        mpiViewModelBase(rows, viewableColumns, parent), m_totalValue(info->totalValue), m_costBasis(info->costBasis), m_taxLiability(info->taxLiability) { }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    double m_totalValue;
    double m_costBasis;
    double m_taxLiability;
};


#endif // MAINHOLDINGSMODEL_H
