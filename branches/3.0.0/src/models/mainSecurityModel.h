#ifndef MAINHOLDINGSMODEL_H
#define MAINHOLDINGSMODEL_H

#include <QStringList>
#include "mpiViewModelBase.h"
#include "snapshot.h"
#include "account.h"

class historicalNAV;
class calculatorNAV;
class security;
class assetAllocation;
class account;
class securityRow: public baseRow
{
public:
    enum {
        row_Active,
        row_Symbol,
        row_Note,
        row_Cash,
        row_Price,
        row_Shares,
        row_Avg,
        row_Cost,
        row_Value,
        row_ValueP,
        row_Gain,
        row_GainP,
        row_Acct,
        row_AA,
        row_TaxLiability,
        row_NetValue,
        row_ReinvestDividends,
        row_AdjustForDividends
    };

    static const QStringList columns;
    static const QVariantList columnsType;

    securityRow(double nav_, const snapshotSecurity &snapshot_, account::costBasisMethod costBasis_, const snapshot &portfolioSnapshot_,
        const security &security_, const QString &account_, const QString &assetAllocation_, const QList<orderBy> &columnSort_);

    QVariant columnType(int column) const { return columnsType.at(column); }
    static QMap<int, QString> fieldNames();

    static QList<baseRow*> getRows(const QMap<int, security> &securities_, const QMap<int, assetAllocation> &assetAllocation_,
        const QMap<int, account> &accounts_, int beginDate_, int endDate_, calculatorNAV calculator_,
        const snapshot &portfolioSnapshot_, const QList<orderBy> &columnSort_);
};

class mainSecurityModel: public mpiViewModelBase
{
public:
    mainSecurityModel(const QList<baseRow*> &rows_, const snapshot &portfolioSnapshot_, double portfolioNAV_, const QList<int> &viewableColumns_, QObject *parent_ = 0);

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int section_, Qt::Orientation orientation_, int role_) const;

private:
    snapshot m_portfolioSnapshot;
    double m_portfolioNAV;
};


#endif // MAINHOLDINGSMODEL_H
