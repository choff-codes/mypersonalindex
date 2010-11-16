#include "assetAllocation.h"
#include <QSqlQuery>
#include <QMap>
#include <QVariant>
#include "queries.h"

class assetAllocationData
{
    double target;
    assetAllocation::thresholdMethod threshold;
    double rebalanceBand;
    bool hidden;

    explicit assetAllocationData(int id_, int parent_, const QString &description_):
        objectKey(description_, id_, parent_),
        target(0),
        threshold(assetAllocation::thresholdMethod_Portfolio),
        rebalanceBand(5),
        hide(false)
    {}
};

assetAllocation::assetAllocation(int id_, int parent_, const QString &description_):
    objectKey(new assetAllocationData(id_, parent_, description_))
{}

assetAllocation::assetAllocation(const assetAllocation &other_):
    objectKey(other_)
{}

assetAllocation::~assetAllocation()
{}

assetAllocation& assetAllocation::operator=(const assetAllocation &other_)
{
    d = other_.d;
    return *this;
}

bool assetAllocation::operator==(const assetAllocation &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
        && d->target == other_.d->target
        && d->rebalanceBand == other_.d->rebalanceBand
        && d->threshold == other_.d->threshold
        && d->hidden == other_.d->hidden;
}

double target() const { return d->target; }
void setTarget(double target_) { d->target = target_; }

bool rebalanceBand() const { return d->rebalanceBand; }
void setRebalanceBand(bool rebalanceBand_) { d->rebalanceBand = rebalanceBand_; }

thresholdMethod threshold() const { return d->threshold; }
void setThreshold(thresholdMethod threshold_) { d->threshold = threshold_; }

bool hidden() const { return d->hidden; }
void setHidden(bool hidden_) { d->hidden = hidden_; }

void assetAllocation::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_PortfolioID), this->parent());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Description), this->description());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Target), this->target());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_RebalanceBand), this->rebalanceBand());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Threshold), (int)this->threshold());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Hide), (int)this->hidden());

    this->setID(dataSource_.insert(queries::table_PortfolioAA, values, this->id()));
}

void assetAllocation::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAA, this->id());
}

assetAllocation assetAllocation::load(const QSqlQuery &q_)
{
    assetAllocation aa(
        q_.value(queries::portfolioAAColumns_ID).toInt(),
        q_.value(queries::portfolioAAColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioAAColumns_Description).toString()
    );

    aa.setTarget(q_.value(queries::portfolioAAColumns_Target).toDouble());
    aa.setRebalanceBand(q_.value(queries::portfolioAAColumns_RebalanceBand).toDouble());
    aa.setThreshold((thresholdMethod)q_.value(queries::portfolioAAColumns_Threshold).toInt());
    aa.setHidden(q_.value(queries::portfolioAAColumns_Hide).toBool());

    return aa;
}

QString assetAllocation::validate() const
{
    if (this->description().isEmpty())
        return "Please enter a description!";

    return QString();
}

objectType assetAllocation::type() const
{
    return objectType_AA;
}

QDataStream& operator<<(QDataStream &stream_, const assetAllocation &aa_)
{
    stream_ << aa_.description();
    stream_ << aa_.rebalanceBand();
    stream_ << aa_.target();
    stream_ << aa_.threshold();
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, assetAllocation &aa_)
{
    stream_ >> aa_.d->description;
    stream_ >> aa_.d->rebalanceBand;
    stream_ >> aa_.d->target;
    int tmp;
    stream_ >> tmp;
    aa_.d->threshold = (assetAllocation::thresholdMethod)tmp;
    return stream_;
}
