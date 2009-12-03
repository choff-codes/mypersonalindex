#ifndef MAINHOLDINGSMODEL_H
#define MAINHOLDINGSMODEL_H

#include <QtGui>
#include <QtSql>
#include "queries.h"
#include "functions.h"
#include "calculations.h"

class holdingsRow
{
public:
    enum { row_Active, row_Ticker, row_Cash, row_Price, row_Shares, row_Avg, row_Cost, row_Value, row_ValueP, row_Gain, row_GainP, row_Acct, row_ID };
    static const QStringList columns;
    static const QVariantList columnsType;
    QVariantList values;

    holdingsRow(const QString &sort): m_sort(sort) {}

    static holdingsRow getHoldingsRow(const globals::security &s, const globals::portfolioCache &cache, const QMap<int, globals::account> &accounts, const QString &sort)
    {
        holdingsRow row(sort);

        globals::securityValue value = cache.tickerValue.value(s.id);

        //row_Active
        row.values.append((int)s.includeInCalc);
        //row_Ticker
        row.values.append(s.ticker);
        //row_Cash
        row.values.append((int)s.cashAccount);
        //row_Price
        double price = cache.tickerInfo.value(s.ticker).closePrice;
        row.values.append(price == 0 ? QVariant() : price);
        //row_Shares
        row.values.append(value.shares);
        //row_Avg
        row.values.append(value.shares == 0 ? QVariant() : cache.avgPrices.value(s.id));
        //row_Cost
        row.values.append(value.shares == 0 ? QVariant() : value.costBasis);
        //row_Value
        row.values.append(value.shares == 0 ? QVariant() : value.totalValue);
        //row_ValueP
        row.values.append(cache.totalValue == 0 ? QVariant() : value.totalValue / cache.totalValue * 100);
        //row_Gain
        row.values.append(value.shares == 0 ? QVariant() : value.totalValue - value.costBasis);
        //row_GainP
        row.values.append(value.shares == 0 || value.costBasis == 0 ? QVariant() : ((value.totalValue / value.costBasis) - 1) * 100);
        //row_Acct
        row.values.append(s.account == -1 ? QVariant() : accounts.value(s.account).description);
        //row_ID
        row.values.append(s.id);

        return row;
    }

    static QMap<int, QString> fieldNames()
    {
        QMap<int, QString> names;

        for (int i = 0; i < columns.count(); ++i)
            names[i] = QString(columns.at(i)).replace('\n', ' ');

        names.remove(row_ID);
        return names;
    }

    bool operator< (const holdingsRow &other) const
    {
        if (m_sort.isEmpty())
            return false;

        QStringList strings = m_sort.split('|');
        foreach(const QString &s, strings)
        {
            bool lessThan = s.at(0) != 'D';
            int column = lessThan ? s.toInt() : QString(s).remove(0, 1).toInt();

            if (functions::equal(values.at(column), other.values.at(column), columnsType.at(column)))
                continue;

            if (functions::lessThan(values.at(column), other.values.at(column), columnsType.at(column)))
                return lessThan;

            return !lessThan;
        }
        return false;
    }

private:
    QString m_sort;
};

class holdingsModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    holdingsModel(const QList<holdingsRow> &rows, QList<int> viewableColumns, const globals::portfolioCache &cache, const bool &showHidden, QTableView *parent = 0):
            QAbstractTableModel(parent), m_parent(parent), m_rows(rows), m_viewableColumns(viewableColumns), m_totalValue(cache.totalValue), m_costBasis(cache.costBasis)
    {
        insertRows(0, rows.count());
    }

    int rowCount(const QModelIndex&) const
    {
        return m_rows.count();
    }

    int columnCount (const QModelIndex&) const
    {
        return m_viewableColumns.count();
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        int column = m_viewableColumns.at(index.column());

        if (role == Qt::DisplayRole)
        {
            if (m_rows.at(index.row()).values.at(column).isNull() || column == holdingsRow::row_Active || column == holdingsRow::row_Cash)
                return QVariant();

            switch (column)
            {
                case holdingsRow::row_Avg:
                case holdingsRow::row_Cost:
                case holdingsRow::row_Gain:
                case holdingsRow::row_Price:
                case holdingsRow::row_Value:
                    return functions::doubleToCurrency(m_rows.at(index.row()).values.at(column).toDouble());
                case holdingsRow::row_GainP:
                case holdingsRow::row_ValueP:
                    return functions::doubleToPercentage(m_rows.at(index.row()).values.at(column).toDouble());
                case holdingsRow::row_Shares:
                    return functions::doubleToLocalFormat(m_rows.at(index.row()).values.at(column).toDouble(), 4);
            }

            return m_rows.at(index.row()).values.at(column);
        }

        if (role == Qt::CheckStateRole && (column == holdingsRow::row_Active || column == holdingsRow::row_Cash))
        {
            return m_rows.at(index.row()).values.at(column).toInt() == 1 ? Qt::Checked : Qt::Unchecked;
        }

        if (role == Qt::TextColorRole && column == queries::getPortfolioHoldings_GainP)
        {
            double value = m_rows.at(index.row()).values.at(column).toDouble();
            return value == 0 ? QVariant() :
                value > 0 ?  qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));
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
            case holdingsRow::row_Cost:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_costBasis));
                break;
            case holdingsRow::row_Value:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue));
                break;
            case holdingsRow::row_Gain:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_totalValue - m_costBasis));
                break;
        }

        return QString(holdingsRow::columns.at(column)).append(extra);
    }

    QList<int> selectedItems()
    {
        QList<int> items;

        QModelIndexList model = m_parent->selectionModel()->selectedRows();
        if (model.count() == 0)
            return items;

        foreach(const QModelIndex &q, model)
            items.append(m_rows.at(q.row()).values.at(holdingsRow::row_ID).toInt());

        return items;
    }

public slots:

private:
    QTableView *m_parent;
    QList<holdingsRow> m_rows;
    QList<int> m_viewableColumns;
    int m_rowCount;
    double m_totalValue;
    double m_costBasis;
};


#endif // MAINHOLDINGSMODEL_H
