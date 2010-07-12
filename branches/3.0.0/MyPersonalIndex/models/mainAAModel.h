#ifndef MAINAAMODEL_H
#define MAINAAMODEL_H

#include "mpiViewModelBase.h"
#include "epsilon.h"
#include "calculations.h"

class aaRow: public baseRow
{
public:
    enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_Target, row_Variance, row_Rebalance, row_Holdings, row_ID };
    static const QStringList columns;
    static const QVariantList columnsType;

    aaRow(const snapshotPortfolio &info, const snapshot &aaInfo, portfolioAttributes::thesholdMethod method, const assetAllocation &aa, const QString &sort);

    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();
};

class mainAAModel: public mpiViewModelBase
{
public:

    mainAAModel(const QList<baseRow*> &rows, QList<int> viewableColumns, const snapshotPortfolio &info, const double &aaThreshold, QTableView *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    double m_totalValue;
    double m_threshold;
    double m_costBasis;
    double m_target;
};

#endif // MAINAAMODEL_H
