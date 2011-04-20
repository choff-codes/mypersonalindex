#ifndef MAINCORRELATIONMODEL_H
#define MAINCORRELATIONMODEL_H

#include "mpiViewModelBase.h"

class correlationRow: public baseRow
{
public:
    enum {
        row_ObjectType,
        row_ID,
        row_Description
    };

    static const QList<orderBy> correlationOrder;
    static const QVariantList columnsType;

    QMap<correlationRow, double> correlationValues;

    correlationRow(int type_, int id_, const QString &description_);
    correlationRow(int type_, int id_, const QString &description_, const QMap<correlationRow, double> &correlationValues_);

    bool operator==(const correlationRow &other_) const;
    bool operator!=(const correlationRow &other_) const { return !(*this == other_); }
    bool operator<(const correlationRow &other_) const;
    QVariant columnType(int column) const { return columnsType.at(column); }
};

class mainCorrelationModel: public mpiViewModelBase
{
public:
    mainCorrelationModel(const QList<baseRow*> &rows_, QObject *parent_ = 0);

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int section_, Qt::Orientation orientation_, int role_) const;

    void add(correlationRow *row_, const correlationRow &key_);
    void remove(const correlationRow &key_);
};

#endif // MAINCORRELATIONMODEL_H
