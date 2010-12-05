#include "mainAcctModel.h"

#include <QColor>
#include "functions.h"
#include "account.h"
#include "calculatorNAV.h"
#include "historicalNAV.h"

//enum {
//    row_Description,
//    row_CostBasis,
//    row_Value,
//    row_ValueP,
//    row_Gain,
//    row_GainP,
//    row_TaxRate,
//    row_TaxLiability,
//    row_Net,
//    row_Holdings
//};

const QStringList acctRow::columns = QStringList()
                                     << "Account"
                                     << "Cost Basis"
                                     << "Total Value"
                                     << "% of Portfolio"
                                     << "Gain/Loss"
                                     << "% Gain/Loss"
                                     << "Tax Rate"
                                     << "Tax Liability"
                                     << "After Tax Value"
                                     << "# of Holdings";

const QVariantList acctRow::columnsType = QVariantList()
                                        << QVariant(QVariant::String)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Double)
                                        << QVariant(QVariant::Int);


acctRow::acctRow(double nav_, const snapshot &snapshot_, const snapshot &portfolioSnapshot_, const account &account_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{
    //    row_Description,
    this->values.append(account_.description());
    //    row_CostBasis,
    this->values.append(snapshot_.costBasis);
    //    row_Value,
    this->values.append(snapshot_.totalValue);
    //    row_ValueP,
    this->values.append(functions::checkDivisor(portfolioSnapshot_.totalValue, snapshot_.totalValue));
    //    row_Gain,
    this->values.append(snapshot_.totalValue - snapshot_.costBasis);
    //    row_GainP,
    this->values.append(nav_ - 1);
    //    row_TaxRate,
    this->values.append(account_.taxRate());
    //    row_TaxLiability,
    this->values.append(snapshot_.taxLiability);
    //    row_Net,
    this->values.append(snapshot_.totalValue - snapshot_.taxLiability);
    //    row_Holdings
    this->values.append(snapshot_.count);
}

QMap<int, QString> acctRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = functions::removeNewLines(columns.at(i));

    return names;
}

QList<baseRow*> acctRow::getRows(const QMap<int, account> &accounts_, int beginDate_, int endDate_, calculatorNAV calculator_,
    const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_)
{
    QList<baseRow*> returnList;

    foreach(const account &acct, accounts_)
    {
        if (acct.deleted() || acct.hidden())
            continue;

        returnList.append(getRow(acct, beginDate_, endDate_, calculator_, portfolioSnapshot_, columnSort_));
    }

    // check if any securities have an unassigned value
    snapshot unassigned = calculator_.accountSnapshot(endDate_, UNASSIGNED);
    if (unassigned.count != 0)
        returnList.append(getRow(account(UNASSIGNED, UNASSIGNED, "(Unassigned)"), beginDate_, endDate_, calculator_, portfolioSnapshot_, columnSort_));

    return returnList;
}

baseRow* acctRow::getRow(const account &account_, int beginDate_, int endDate_, calculatorNAV calculator_, const snapshot &portfolioSnapshot_,
    const QList<orderBy> &columnSort_)
{
    return new acctRow(
        calculator_.nav(account_, beginDate_, endDate_),
        calculator_.accountSnapshot(endDate_, account_.id()), portfolioSnapshot_, account_, columnSort_
    );
}

mainAcctModel::mainAcctModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QObject *parent_):
    mpiViewModelBase(rows_, viewableColumns_, false, parent_),
    m_portfolioSnapshot(portfolioSnapshot_),
    m_portfolioNAV(portfolioNAV_)
{
}

QVariant mainAcctModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index_.column());
    QVariant value = m_rows.at(index_.row())->values.at(column);

    if (role_ == Qt::DisplayRole)
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

    if (role_ == Qt::TextColorRole && column == acctRow::row_GainP)
        return functions::isZero(value.toDouble()) ?
            QVariant() :
            value.toDouble() > 0 ?
                qVariantFromValue(QColor(Qt::darkGreen)) :
                qVariantFromValue(QColor(Qt::red));

    return QVariant();
}

QVariant mainAcctModel::headerData(int section_, Qt::Orientation orientation_, int role_) const
{
    if (section_ >= m_viewableColumns.count())
        return QVariant();

    if (role_ == Qt::TextAlignmentRole)
        return (int)Qt::AlignLeft | Qt::AlignVCenter;

    if (orientation_ != Qt::Horizontal || role_ != Qt::DisplayRole)
        return QVariant();

    int column = m_viewableColumns.at(section_);
    QString extra;
    switch(column)
    {
        case acctRow::row_CostBasis:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.costBasis));
            break;
        case acctRow::row_Value:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue));
            break;
        case acctRow::row_Gain:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue - m_portfolioSnapshot.costBasis));
            break;
        case acctRow::row_GainP:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_portfolioNAV - 1));
            break;
        case acctRow::row_Net:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue - m_portfolioSnapshot.taxLiability));
            break;
        case acctRow::row_TaxLiability:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.taxLiability));
            break;
    }

    return QString(acctRow::columns.at(column)).append(extra);
}
