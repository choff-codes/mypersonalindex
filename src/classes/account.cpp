#include "account.h"
#include <QSqlQuery>
#include "queries.h"
#include "functions.h"

class accountData: public objectKeyData
{
public:
    double taxRate;
    bool taxDeferred;
    account::costBasisMethod costBasis;
    bool hidden;

    accountData():
        taxRate(0),
        taxDeferred(false),
        costBasis(account::costBasisMethod_FIFO),
        hidden(false)
    {}
};

account::account():
    objectKey(),
    d(new accountData())
{}

account::account(const account &other_):
    objectKey(),
    d(other_.d)
{}

account::~account()
{}

account& account::operator=(const account &other_)
{
    d = other_.d;
    return *this;
}

objectKeyData* account::data() const { return d.data(); }

bool account::operator==(const account &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
            && d->taxRate == other_.d->taxRate
            && d->taxDeferred == other_.d->taxDeferred
            && d->costBasis == other_.d->costBasis
            && d->hidden == other_.d->hidden;
}

double account::taxRate() const { return d->taxRate; }
void account::setTaxRate(double taxRate_) { d->taxRate = taxRate_; }

bool account::taxDeferred() const { return d->taxDeferred; }
void account::setTaxDeferred(bool taxDeferred_) { d->taxDeferred = taxDeferred_; }

account::costBasisMethod account::costBasis() const { return d->costBasis; }
void account::setCostBasis(costBasisMethod costBasis_) { d->costBasis = costBasis_; }

bool account::hidden() const { return d->hidden; }
void account::setHidden(bool hidden_) { d->hidden = hidden_; }

bool account::save(const queries &dataSource_) const
{
    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_ID), this->id());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_PortfolioID), this->parent());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_Description), this->description());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxRate), this->taxRate());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxDeferred), (int)this->taxDeferred());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_CostBasis), (int)this->costBasis());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAccountColumns_Hide), (int)this->hidden());

    return dataSource_.insert(queries::table_PortfolioAccount, values);
}

account account::load(const QSqlQuery &q_)
{
    account acct;
    acct.setID(q_.value(queries::portfolioAccountColumns_ID).toInt());
    acct.setParent(q_.value(queries::portfolioAccountColumns_PortfolioID).toInt());
    acct.setDescription(q_.value(queries::portfolioAccountColumns_Description).toString());
    acct.setTaxRate(q_.value(queries::portfolioAccountColumns_TaxRate).toDouble());
    acct.setTaxDeferred(q_.value(queries::portfolioAccountColumns_TaxDeferred).toBool());
    acct.setCostBasis((costBasisMethod)q_.value(queries::portfolioAccountColumns_CostBasis).toInt());
    acct.setHidden(q_.value(queries::portfolioAccountColumns_Hide).toBool());

    return acct;
}

QString account::validate() const
{
    if (this->description().isEmpty())
        return "Please enter a description!";

    return QString();
}

objectType account::type() const
{
    return objectType_Account;
}

void account::detach()
{
    d.detach();
}

QDataStream& operator<<(QDataStream &stream_, const account &acct_)
{
    stream_ << acct_.description();
    stream_ << acct_.costBasis();
    stream_ << acct_.taxDeferred();
    stream_ << acct_.taxRate();
    stream_ << acct_.hidden();
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, account &acct_)
{
    stream_ >> acct_.d->description;
    int tmp;
    stream_ >> tmp;
    acct_.d->costBasis = (account::costBasisMethod)tmp;
    stream_ >> acct_.d->taxDeferred;
    stream_ >> acct_.d->taxRate;
    stream_ >> acct_.d->hidden;
    return stream_;
}
