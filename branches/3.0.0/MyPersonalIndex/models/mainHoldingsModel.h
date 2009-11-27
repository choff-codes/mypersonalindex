#ifndef MAINHOLDINGSMODEL_H
#define MAINHOLDINGSMODEL_H

#include <QtGui>
#include <QtSql>
#include "queries.h"
#include "functions.h"

class holdingsModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    holdingsModel(QSqlQuery *query, QList<int> viewableColumns, const globals::gainLossInfo &gainLossInfo, QTableView *parent = 0):
            QAbstractTableModel(parent), m_parent(parent), m_query(query), m_viewableColumns(viewableColumns), m_gainLossInfo(gainLossInfo)
    {
        m_rowCount = 0;
        if (!m_query)
            return;

        do
        {
            ++m_rowCount;
        }
        while (m_query->next());
        insertRows(0, m_rowCount);
    }

    ~holdingsModel() { delete m_query; }

    QMap<int, QString> fieldNames()
    {
        QMap<int, QString> names;
        QSqlRecord record = m_query->record();

        for (int i = 0; i < record.count(); ++i)
            names[i] = record.fieldName(i).replace('|', ' ');

        names.remove(queries::getPortfolioHoldings_ID);
        return names;
    }

    globals::gainLossInfo gainLossInfo () { return m_gainLossInfo; }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (m_viewableColumns.at(index.column()) == queries::getPortfolioHoldings_Symbol)
                return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
        return QAbstractTableModel::flags(index);
    }

    int rowCount(const QModelIndex&) const
    {
        return m_rowCount;
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
            m_query->seek(index.row());

            if (m_query->value(column).isNull() || column == queries::getPortfolioHoldings_Active)
                return QVariant();

            switch (column)
            {
                case queries::getPortfolioHoldings_AveragePrice:
                case queries::getPortfolioHoldings_CostBasis:
                case queries::getPortfolioHoldings_Gain:
                case queries::getPortfolioHoldings_Price:
                case queries::getPortfolioHoldings_TotalValue:
                    return functions::doubleToCurrency(m_query->value(column).toDouble());
                case queries::getPortfolioHoldings_GainP:
                case queries::getPortfolioHoldings_TotalValueP:
                    return functions::doubleToPercentage(m_query->value(column).toDouble());
                case queries::getPortfolioHoldings_Shares:
                    return functions::doubleToLocalFormat(m_query->value(column).toDouble(), 4);
            }

            return m_query->value(column);
        }

        if (role == Qt::CheckStateRole && column == queries::getPortfolioHoldings_Active)
        {
            m_query->seek(index.row());
            return m_query->value(queries::getPortfolioHoldings_Active).toInt() == 1 ? Qt::Checked : Qt::Unchecked;
        }

        if (role == Qt::TextColorRole && column == queries::getPortfolioHoldings_GainP)
        {
            m_query->seek(index.row());
            double value =  m_query->value(queries::getPortfolioHoldings_GainP).toDouble();
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
            case queries::getPortfolioHoldings_CostBasis:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_gainLossInfo.costBasis));
                break;
            case queries::getPortfolioHoldings_TotalValue:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_gainLossInfo.totalValue));
                break;
            case queries::getPortfolioHoldings_Gain:
                extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_gainLossInfo.totalValue - m_gainLossInfo.costBasis));
                break;
        }

        return m_query->record().fieldName(column).replace('|', '\n').append(extra);
    }

    QList<int> selectedItems()
    {
        QList<int> items;

        QModelIndexList model = m_parent->selectionModel()->selectedRows();
        if (model.count() == 0)
            return items;

        foreach(const QModelIndex &q, model)
        {
            m_query->seek(q.row());
            items.append(m_query->value(queries::getPortfolioHoldings_ID).toInt());
        }

        return items;
    }

public slots:

private:
    QTableView *m_parent;
    QSqlQuery *m_query;
    QList<int> m_viewableColumns;
    int m_rowCount;
    globals::gainLossInfo m_gainLossInfo;
};


#endif // MAINHOLDINGSMODEL_H
