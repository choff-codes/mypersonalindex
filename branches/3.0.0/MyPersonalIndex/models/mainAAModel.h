#ifndef MAINAAMODEL_H
#define MAINAAMODEL_H

#include <QtGui>
#include "mpiViewModelBase.h"

class aaRow: public baseRow
{
public:
    enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_Target, row_Offset, row_Holdings };
    static const QStringList columns;
    static const QVariantList columnsType;

    aaRow(const QString &sort): baseRow(sort) {}

    QVariant columnType(int column) const { return columnsType.at(column); }

    static aaRow* getAARow(const globals::portfolioCache *cache, const globals::assetAllocation &aa, const QMap<int, globals::security> tickers, const QString &sort)
    {
        aaRow *row = new aaRow(sort);

        int count = 0; double value = 0, costBasis = 0;
        foreach(const globals::security &s, tickers)
        {
            bool included = false;

            if (aa.id == -1 and s.aa.isEmpty())
            {
                included = true;
                globals::securityValue sv = cache->tickerValue.value(s.id);
                value += sv.totalValue;
                costBasis += sv.costBasis;
            }
            else
                foreach(const globals::securityAATarget &target, s.aa)
                    if (target.id == aa.id)
                    {
                        included = true;
                        globals::securityValue sv = cache->tickerValue.value(s.id);
                        value += sv.totalValue * aa.target / 100;
                        costBasis += sv.costBasis;
                    }

            if (included)
                ++count;
        }

        //row_Description
        row->values.append(aa.description);
        //row_CostBasis
        row->values.append(costBasis);
        //row_Value
        row->values.append(value);
        //row_ValueP
        row->values.append(cache->totalValue == 0 ? QVariant() : 100.0 * value / cache->totalValue);
        //row_Gain
        row->values.append(value - costBasis);
        //row_GainP
        row->values.append(costBasis == 0 ? QVariant() : ((value / costBasis) - 1) * 100);
        //row_Target
        row->values.append(aa.target < 0 ? QVariant() : aa.target);
        //row_Offset
        row->values.append(cache->totalValue == 0 || aa.target < 0 ? QVariant() : 100.0 * ((value / cache->totalValue) - (aa.target / 100.0)));
        //row_Holdings
        row->values.append(count);

        return row;
    }

    static QMap<int, QString> fieldNames()
    {
        QMap<int, QString> names;

        for (int i = 0; i < columns.count(); ++i)
            names[i] = QString(columns.at(i)).replace('\n', ' ');

        return names;
    }
};

class mainAAModel: public mpiViewModelBase
{
    Q_OBJECT

public:

    mainAAModel(const QList<baseRow*> &rows, QList<int> viewableColumns, const globals::portfolioCache *cache, const double &aaThreshold, QTableView *parent = 0):
        mpiViewModelBase(rows, viewableColumns, parent), m_totalValue(cache->totalValue), m_threshold(aaThreshold), m_costBasis(cache->costBasis) { }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        int column = m_viewableColumns.at(index.column());

        if (role == Qt::DisplayRole)
        {
            if (m_rows.at(index.row())->values.at(column).isNull())
                return QVariant();

            switch (column)
            {
                case aaRow::row_Value:
                case aaRow::row_CostBasis:
                case aaRow::row_Gain:
                    return functions::doubleToCurrency(m_rows.at(index.row())->values.at(column).toDouble());
                case aaRow::row_ValueP:
                case aaRow::row_Target:
                case aaRow::row_Offset:
                case aaRow::row_GainP:
                    return functions::doubleToPercentage(m_rows.at(index.row())->values.at(column).toDouble());
            }

            return m_rows.at(index.row())->values.at(column);
        }

        if (role == Qt::TextColorRole && column == aaRow::row_Offset)
        {
            double value = m_rows.at(index.row())->values.at(column).toDouble();
            return fabs(value) <= m_threshold ? QVariant() :
                value > m_threshold ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
        }

        if (role == Qt::TextColorRole && column == aaRow::row_GainP)
        {
            double value = m_rows.at(index.row())->values.at(column).toDouble();
            return value == 0 ? QVariant() :
                value > 0 ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
         if (section >= m_viewableColumns.count())
            return QVariant();

        if (role == Qt::TextAlignmentRole)
            return (int)Qt::AlignLeft | Qt::AlignVCenter;

        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        int column = m_viewableColumns.at(section);
        QString extra;
        switch(column)
        {
            case aaRow::row_CostBasis:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_costBasis));
                break;
            case aaRow::row_Value:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue));
                break;
            case aaRow::row_Gain:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_costBasis));
                break;
        }

        return QString(aaRow::columns.at(column)).append(extra);
    }

private:
    double m_totalValue;
    double m_threshold;
    double m_costBasis;
};

#endif // MAINAAMODEL_H
