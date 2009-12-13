#ifndef MAINACCTMODEL_H
#define MAINACCTMODEL_H

#include <QtGui>
#include "mpiViewModelBase.h"

class acctRow: public baseRow
{
public:
    enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_TaxRate, row_TaxLiability, row_Net, row_Holdings };
    static const QStringList columns;
    static const QVariantList columnsType;

    acctRow(const QString &sort): baseRow(sort) {}

    QVariant columnType(int column) const { return columnsType.at(column); }

    static acctRow* getAcctRow(const globals::portfolioCache *cache, const globals::account &acct, const QMap<int, globals::security> tickers, const QString &sort)
    {
        acctRow *row = new acctRow(sort);

        int count = 0; double value = 0, taxLiability = 0, costBasis = 0;
        foreach(const globals::security &s, tickers)
        {
            if (acct.id == s.account)
            {
                globals::securityValue sv = cache->tickerValue.value(s.id);
                value += sv.totalValue;
                taxLiability += sv.taxLiability;
                costBasis += sv.costBasis;
                ++count;
            }
        }

        //row_Description
        row->values.append(acct.description);
        //row_CostBasis
        row->values.append(costBasis);
        //row_Value
        row->values.append(value);
        //row_ValueP
        row->values.append(cache->totalValue == 0 ? QVariant() : 100.0 * value / cache->totalValue);
        //row_Gain
        row->values.append(value - costBasis);
        //row_GainP
        row->values.append((value / costBasis - 1) * 100);
        //row_TaxRate
        row->values.append(acct.taxRate < 0 ? QVariant() : acct.taxRate);
        //row_TaxLiability
        row->values.append(taxLiability);
        //row_Net
        row->values.append(value - taxLiability);
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

class mainAcctModel: public mpiViewModelBase
{
    Q_OBJECT

public:

    mainAcctModel(const QList<baseRow*> &rows, QList<int> viewableColumns, const globals::portfolioCache *cache, QTableView *parent = 0):
        mpiViewModelBase(rows, viewableColumns, parent), m_totalValue(cache->totalValue), m_taxLiability(cache->taxLiability), m_costBasis(cache->costBasis) { }

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
                case acctRow::row_CostBasis:
                case acctRow::row_Value:
                case acctRow::row_Gain:
                case acctRow::row_TaxLiability:
                case acctRow::row_Net:
                    return functions::doubleToCurrency(m_rows.at(index.row())->values.at(column).toDouble());
                case acctRow::row_ValueP:
                case acctRow::row_GainP:
                case acctRow::row_TaxRate:
                    return functions::doubleToPercentage(m_rows.at(index.row())->values.at(column).toDouble());
            }

            return m_rows.at(index.row())->values.at(column);
        }

        if (role == Qt::TextColorRole && column == acctRow::row_GainP)
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
            case acctRow::row_CostBasis:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_costBasis));
                break;
            case acctRow::row_Value:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue));
                break;
            case acctRow::row_Gain:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_costBasis));
                break;
            case acctRow::row_Net:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_taxLiability));
                break;
            case acctRow::row_TaxLiability:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_taxLiability));
                break;
        }

        return QString(acctRow::columns.at(column)).append(extra);
    }

private:
    double m_totalValue;
    double m_taxLiability;
    double m_costBasis;
};

#endif // MAINACCTMODEL_H
