#include "mainSecurityModel.h"
#include <QColor>
#include "security.h"
#include "assetAllocation.h"
#include "snapshot.h"
#include "calculatorNAV.h"
#include "functions.h"
#include "calculatorAveragePrice.h"
#include "splits.h"
#include "assetAllocationTarget.h"

//enum {
//    row_Active,
//    row_Symbol,
//    row_Note,
//    row_Cash,
//    row_Price,
//    row_Shares,
//    row_Avg,
//    row_Cost,
//    row_Value,
//    row_ValueP,
//    row_Gain,
//    row_GainP,
//    row_NAV,
//    row_Acct,
//    row_AA,
//    row_TaxLiability,
//    row_NetValue,
//    row_ReinvestDividends,
//    row_AdjustForDividends
//};

const QStringList securityRow::columns = QStringList()
                                         << "Active"
                                         << "Symbol"
                                         << "Note"
                                         << "Cash"
                                         << "Closing Price"
                                         << "Shares"
                                         << "Avg Price\nPer Share"
                                         << "Cost Basis"
                                         << "Total Value"
                                         << "% of\nPortfolio"
                                         << "Gain/Loss"
                                         << "% Gain/Loss"
                                         << "Account"
                                         << "Asset Allocation"
                                         << "Tax Liability"
                                         << "After Tax Value"
                                         << "Reinvest\nDividends"
                                         << "Adjust NAV\nFor Dividends";

const QVariantList securityRow::columnsType = QVariantList()
                                              << QVariant(QVariant::Int)
                                              << QVariant(QVariant::String)
                                              << QVariant(QVariant::String)
                                              << QVariant(QVariant::Int)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::String)
                                              << QVariant(QVariant::String)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Double)
                                              << QVariant(QVariant::Int)
                                              << QVariant(QVariant::Int);

securityRow::securityRow(double nav_, const snapshotSecurity &snapshot_, account::costBasisMethod costBasis_, const snapshot &portfolioSnapshot_,
    const security &security_, const QString &account_, const QString &assetAllocation_, const QList<orderBy> &columnSort_):
    baseRow(columnSort_)
{
    //    row_Active,
    this->values.append((int)security_.includeInCalc());
    //    row_Symbol,
    this->values.append(security_.description());
    //    row_Note,
    this->values.append(functions::fitString(functions::removeNewLines(security_.note()), 20));
    //    row_Cash,
    this->values.append((int)security_.cashAccount());
    //    row_Price,
    double price = security_.price(snapshot_.date);
    this->values.append(functions::isZero(price) ? QVariant() : price);
    //    row_Shares,
    this->values.append(snapshot_.shares);
    //    row_Avg,
    // cash should always be computed as average
    //s.cashAccount() ? account::costBasisMethod_AVG : portfolio_.accounts().value(s.account()).costBasis(),
    splits splitRatio(security_.splits(), snapshot_.date);
    this->values.append(
        functions::isZero(snapshot_.shares) ?
            QVariant() :
            security_.cashAccount() ?
                1 :
                calculatorAveragePrice::calculate(snapshot_.date, security_.executedTrades(), costBasis_, splitRatio)
    );
    //    row_Cost,
    this->values.append(snapshot_.costBasis);
    //    row_Value,
    this->values.append(snapshot_.totalValue);
    //    row_ValueP,
    this->values.append(functions::isZero(portfolioSnapshot_.totalValue) ? QVariant() : snapshot_.totalValue / portfolioSnapshot_.totalValue);
    //    row_Gain,
    this->values.append(snapshot_.totalValue - snapshot_.costBasis);
    //    row_GainP,
    this->values.append(nav_ - 1);
    //    row_Acct,
    this->values.append(account_);
    //    row_AA,
    this->values.append(assetAllocation_);
    //    row_TaxLiability,
    this->values.append(snapshot_.taxLiability);
    //    row_NetValue,
    this->values.append(snapshot_.totalValue - snapshot_.taxLiability);
    //    row_ReinvestDividends,
    this->values.append((int)security_.dividendReinvestment());
    //    row_AdjustForDividends
    this->values.append((int)security_.dividendNAVAdjustment());
}

QList<baseRow*> securityRow::getRows(const QMap<int, security> &securities_, const QMap<int, assetAllocation> &assetAllocation_,
    const QMap<int, account> &accounts_, int beginDate_, int endDate_, calculatorNAV calculator_,
    const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_)
{
    QList<baseRow*> returnList;

    foreach(const security &sec, securities_)
    {
        if (sec.deleted() || sec.hidden())
            continue;

        QStringList aaDescription;
        for(QMap<int, double>::const_iterator i = sec.targets().constBegin(); i != sec.targets().constEnd(); ++i)
            aaDescription.prepend(
                QString("%1 - %2").arg(i.key() == UNASSIGNED ? "(Unassigned)" : assetAllocation_.value(i.key()).displayText(), functions::doubleToPercentage(i.value()))
            );

        returnList.append(
            new securityRow(
                calculator_.nav(sec, beginDate_, endDate_),
                calculator_.securitySnapshot(endDate_, sec.id()),
                accounts_.value(sec.account()).costBasis(),
                portfolioSnapshot_,
                sec,
                accounts_.value(sec.account()).displayText(),
                aaDescription.join(", "),
                columnSort_
            )
        );
    }

    return returnList;
}

QMap<int, QString> securityRow::fieldNames()
{
    QMap<int, QString> names;

    for (int i = 0; i < columns.count(); ++i)
        names[i] = functions::removeNewLines(columns.at(i));

    return names;
}

mainSecurityModel::mainSecurityModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QObject *parent_ ):
    mpiViewModelBase(rows_, viewableColumns_, false, parent_),
    m_portfolioSnapshot(portfolioSnapshot_),
    m_portfolioNAV(portfolioNAV_)
{
}

QVariant mainSecurityModel::data(const QModelIndex &index_, int role_) const
{
    if (!index_.isValid())
        return QVariant();

    int column = m_viewableColumns.at(index_.column());
    QVariant value = m_rows.at(index_.row())->values.at(column);

    if (role_ == Qt::DisplayRole)
    {
        if (value.isNull() || column == securityRow::row_Active || column == securityRow::row_Cash ||column == securityRow::row_ReinvestDividends || column == securityRow::row_AdjustForDividends)
            return QVariant();

        switch (column)
        {
            case securityRow::row_Avg:
            case securityRow::row_Cost:
            case securityRow::row_Gain:
            case securityRow::row_Price:
            case securityRow::row_Value:
            case securityRow::row_TaxLiability:
            case securityRow::row_NetValue:
                return functions::doubleToCurrency(value.toDouble());
            case securityRow::row_GainP:
            case securityRow::row_ValueP:
                return functions::doubleToPercentage(value.toDouble());
            case securityRow::row_Shares:
                return functions::doubleToLocalFormat(value.toDouble(), 4);
        }

        return value;
    }

    if (role_ == Qt::CheckStateRole && (column == securityRow::row_Active || column == securityRow::row_Cash || column == securityRow::row_ReinvestDividends || column == securityRow::row_AdjustForDividends))
    {
        return value.toInt() == 1 ? Qt::Checked : Qt::Unchecked;
    }

    if (role_ == Qt::TextColorRole && column == securityRow::row_GainP)
        return functions::isZero(value.toDouble()) ?
            QVariant() :
            value.toDouble() > 0 ?
                qVariantFromValue(QColor(Qt::darkGreen)) :
                qVariantFromValue(QColor(Qt::red));

    return QVariant();
}

QVariant mainSecurityModel::headerData(int section_, Qt::Orientation orientation_, int role_) const
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
        case securityRow::row_Cost:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.costBasis));
            break;
        case securityRow::row_Value:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue));
            break;
        case securityRow::row_Gain:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue - m_portfolioSnapshot.costBasis));
            break;
        case securityRow::row_GainP:
            extra = QString("\n[%1]").arg(functions::doubleToPercentage(m_portfolioNAV));
            break;
        case securityRow::row_TaxLiability:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.taxLiability));
            break;
        case securityRow::row_NetValue:
            extra = QString("\n[%1]").arg(functions::doubleToCurrency(m_portfolioSnapshot.totalValue - m_portfolioSnapshot.taxLiability));
            break;
    }

    return QString(securityRow::columns.at(column)).append(extra);
}
