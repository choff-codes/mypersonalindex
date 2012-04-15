#ifndef MAINPERFORMANCEMODEL_H
#define MAINPERFORMANCEMODEL_H

#include "mpiViewModelBase.h"

class historicalNAV;
class performanceRow: public baseRow
{
public:
    enum {
        row_Date,
        row_TotalValue,
        row_Dividend,
        row_Change,
        row_Gain
    };

    static const QStringList columns;
    static const QVariantList columnsType;

    performanceRow(int date_, double totalValue_, double dividend_, double nav_, double change_, const QList<orderBy> &columnSort_);

    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();

    static QList<baseRow*> getRows(const historicalNAV &nav_, const QList<orderBy> &columnSort_);
};

class mainPerformanceModel: public mpiViewModelBase
{
public:
    mainPerformanceModel(const QList<baseRow*> &rows_, const QList<int> &viewableColumns_, QObject *parent_ = 0);

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int section, Qt::Orientation orientation_, int role_) const;
};

#endif // MAINPERFORMANCEMODEL_H
