#include "mainAcctModel.h"

//enum { row_Description, row_CostBasis, row_Value, row_ValueP, row_Gain, row_GainP, row_TaxRate, row_TaxLiability, row_Net, row_Holdings };
const QStringList acctRow::columns = QStringList() << "Account" << "Cost Basis" << "Total Value" << "% of Portfolio" << "Gain/Loss" << "% Gain/Loss"
    << "Tax Rate" << "Tax Liability" << "After Tax Value" << "# of Holdings" << "ID";

const QVariantList acctRow::columnsType = QVariantList() << QVariant(QVariant::String) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double) << QVariant(QVariant::Double)
     << QVariant(QVariant::Double) << QVariant(QVariant::Int) << QVariant(QVariant::Int);

acctRow::acctRow(const dailyInfoPortfolio &info, const dailyInfo &acctInfo, const account &acct, const QString &sort)
    : baseRow(sort)
{
    //row_Description
    this->values.append(acct.description);
    //row_CostBasis
    this->values.append(acctInfo.costBasis);
    //row_Value
    this->values.append(acctInfo.totalValue);
    //row_ValueP
    this->values.append(info.totalValue == 0 ? QVariant() : acctInfo.totalValue / info.totalValue);
    //row_Gain
    this->values.append(acctInfo.totalValue - acctInfo.costBasis);
    //row_GainP
    this->values.append(acctInfo.costBasis == 0 ? QVariant() : (acctInfo.totalValue / acctInfo.costBasis) - 1);
    //row_TaxRate
    this->values.append(acct.taxRate < 0 ? QVariant() : acct.taxRate);
    //row_TaxLiability
    this->values.append(acctInfo.taxLiability);
    //row_Net
    this->values.append(acctInfo.totalValue - acctInfo.taxLiability);
    //row_Holdings
    this->values.append(acctInfo.count);
    //row_ID
    this->values.append(acct.id);
}

QMap<int, QString> acctRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = QString(columns.at(i)).replace('\n', ' ');

    names.remove(row_ID);
    return names;
}

QVariant mainAcctModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index.column());
    QVariant value = m_rows.at(index.row())->values.at(column);

    if (role == Qt::DisplayRole)
    {
        if (value.isNull())
            return QVariant();

        switch (column)
        {
            case acctRow::row_CostBasis:
            case acctRow::row_Value:
            case acctRow::row_Gain:
            case acctRow::row_TaxLiability:
            case acctRow::row_Net:
                return functions::doubleToCurrency(value.toDouble());
            case acctRow::row_ValueP:
            case acctRow::row_GainP:
            case acctRow::row_TaxRate:
                return functions::doubleToPercentage(value.toDouble());
        }

        return value;
    }

    if (role == Qt::TextColorRole && column == acctRow::row_GainP)
        return value.toDouble() == 0 ? QVariant() :
            value.toDouble() > 0 ? qVariantFromValue(QColor(Qt::darkGreen)) : qVariantFromValue(QColor(Qt::red));

    return QVariant();
}

QVariant mainAcctModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        case acctRow::row_GainP:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_costBasis == 0 ? 0 : (m_totalValue - m_costBasis) / m_costBasis));
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
