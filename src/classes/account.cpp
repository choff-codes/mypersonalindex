#include "account.h"
#include <QSqlQuery>
#include "queries.h"
#include "functions.h"

bool account::operator==(const account &other_) const
{
    return objectKey::operator ==(other_)
            && this->taxRate == other_.taxRate
            && this->taxDeferred == other_.taxDeferred
            && this->costBasis == other_.costBasis;
}

void account::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_Description), this->description);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxRate), this->taxRate);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxDeferred), (int)this->taxDeferred);
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_CostBasis), (int)this->costBasis);

    this->id = dataSource_.insert(queries::table_PortfolioAccount, values, this->id);
}

void account::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAccount, this->id);
}

account account::load(const QSqlQuery &q_)
{
    account acct(
        q_.value(queries::portfolioAccountColumns_ID).toInt(),
        q_.value(queries::portfolioAccountColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioAccountColumns_Description).toString()
    );

    acct.taxRate = q_.value(queries::portfolioAccountColumns_TaxRate).toDouble();
    acct.taxDeferred = q_.value(queries::portfolioAccountColumns_TaxDeferred).toBool();
    acct.costBasis = (costBasisMethod)q_.value(queries::portfolioAccountColumns_CostBasis).toInt();

    return acct;
}

QString account::validate() const
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    return QString();
}

QDataStream& operator<<(QDataStream &stream_, const account &acct_)
{
    stream_ << acct_.costBasis;
    stream_ << acct_.description;
    stream_ << acct_.taxDeferred;
    stream_ << acct_.taxRate;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, account &acct_)
{
    int tmp;
    stream_ >> tmp;
    acct_.costBasis = (account::costBasisMethod)tmp;
    stream_ >> acct_.description;
    stream_ >> acct_.taxDeferred;
    stream_ >> acct_.taxRate;
    return stream_;
}
