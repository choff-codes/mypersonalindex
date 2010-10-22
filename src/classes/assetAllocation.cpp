#include "assetAllocation.h"
#include <QSqlQuery>
#include <QMap>
#include <QVariant>
#include "queries.h"

bool assetAllocation::operator==(const assetAllocation &other_) const
{
    return objectKey::operator==(other_)
            && this->target == other_.target
            && this->rebalanceBand == other_.rebalanceBand
            && this->threshold == other_.threshold;
}

void assetAllocation::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_PortfolioID), this->parent);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Description), this->description);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Target), this->target);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_RebalanceBand), this->rebalanceBand);
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Threshold), (int)this->threshold);

    this->id = dataSource_.insert(queries::table_PortfolioAA, values, this->id);
}

void assetAllocation::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAA, this->id);
}

assetAllocation assetAllocation::load(const QSqlQuery &q_)
{
    assetAllocation aa(
        q_.value(queries::portfolioAAColumns_ID).toInt(),
        q_.value(queries::portfolioAAColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioAAColumns_Description).toString()
    );

    aa.target = q_.value(queries::portfolioAAColumns_Target).toDouble();
    aa.rebalanceBand = q_.value(queries::portfolioAAColumns_RebalanceBand).toDouble();
    aa.threshold = (thresholdMethod)q_.value(queries::portfolioAAColumns_Threshold).toInt();

    return aa;
}

QString assetAllocation::validate() const
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    return QString();
}

QDataStream& operator<<(QDataStream &stream_, const assetAllocation &aa_)
{
    stream_ << aa_.description;
    stream_ << aa_.rebalanceBand;
    stream_ << aa_.target;
    stream_ << aa_.threshold;
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_)
{
    stream_ >> aa_.description;
    stream_ >> aa_.rebalanceBand;
    stream_ >> aa_.target;
    int tmp;
    stream_ >> tmp;
    aa_.threshold = (assetAllocation::thresholdMethod)tmp;
    return stream_;
}
