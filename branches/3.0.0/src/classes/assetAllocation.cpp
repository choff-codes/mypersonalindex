#include "assetAllocation.h"
#include <QSqlQuery>
#include <QMap>
#include <QVariant>
#include "queries.h"

class assetAllocationData: public objectKeyData
{
public:
    double target;
    assetAllocation::thresholdMethod threshold;
    double rebalanceBand;
    bool hidden;

    assetAllocationData():
        target(0),
        threshold(assetAllocation::thresholdMethod_Portfolio),
        rebalanceBand(0),
        hidden(false)
    {}
};

assetAllocation::assetAllocation():
    objectKey(),
    d(new assetAllocationData())
{}

assetAllocation::assetAllocation(const assetAllocation &other_):
    objectKey(),
    d(other_.d)
{}

assetAllocation::~assetAllocation()
{}

assetAllocation& assetAllocation::operator=(const assetAllocation &other_)
{
    d = other_.d;
    return *this;
}

objectKeyData* assetAllocation::data() const { return d.data(); }

bool assetAllocation::operator==(const assetAllocation &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
        && d->target == other_.d->target
        && d->rebalanceBand == other_.d->rebalanceBand
        && d->threshold == other_.d->threshold
        && d->hidden == other_.d->hidden;
}

double assetAllocation::target() const { return d->target; }
void assetAllocation::setTarget(double target_) { d->target = target_; }

double assetAllocation::rebalanceBand() const { return d->rebalanceBand; }
void assetAllocation::setRebalanceBand(double rebalanceBand_) { d->rebalanceBand = rebalanceBand_; }

assetAllocation::thresholdMethod assetAllocation::threshold() const { return d->threshold; }
void assetAllocation::setThreshold(thresholdMethod threshold_) { d->threshold = threshold_; }

bool assetAllocation::hidden() const { return d->hidden; }
void assetAllocation::setHidden(bool hidden_) { d->hidden = hidden_; }

bool assetAllocation::save(const queries &dataSource_) const
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_ID), this->id());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_PortfolioID), this->parent());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Description), this->description());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Target), this->target());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_RebalanceBand), this->rebalanceBand());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Threshold), (int)this->threshold());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAAColumns_Hide), (int)this->hidden());

    return dataSource_.insert(queries::table_PortfolioAA, values);
}

assetAllocation assetAllocation::load(const QSqlQuery &q_)
{
    assetAllocation aa;
    aa.setID(q_.value(queries::portfolioAAColumns_ID).toInt());
    aa.setParent(q_.value(queries::portfolioAAColumns_PortfolioID).toInt());
    aa.setDescription(q_.value(queries::portfolioAAColumns_Description).toString());
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

void assetAllocation::detach()
{
    d.detach();
}

QDataStream& operator<<(QDataStream &stream_, const assetAllocation &aa_)
{
    stream_ << aa_.description();
    stream_ << aa_.rebalanceBand();
    stream_ << aa_.target();
    stream_ << aa_.threshold();
    stream_ << aa_.hidden();
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
    stream_ >> aa_.d->hidden;
    return stream_;
}
