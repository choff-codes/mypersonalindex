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

    explicit account(int id_, int parent_, const QString &description_):
        objectKeyData(description_, id_, parent_),
        taxRate(0),
        taxDeferred(false),
        costBasis(account::costBasisMethod_FIFO),
        hidden(false)
    {}
};

account::account(int id_, int parent_, const QString &description_):
    objectKey(new accountData(id_, parent_, description_))
{}

account::account(const account &other_):
    objectKey(other_)
{}

account::~account()
{}

account& account::operator=(const account &other_)
{
    d = other_.d;
    return *this;
}

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

costBasisMethod account::costBasis() const { return d->costBasis; }
void account::setCostBasis(costBasisMethod costBasis_) { d->costBasis = costBasis_; }

bool account::hidden() const { return d->hidden; }
void account::setHidden(bool hidden_) { d->hidden = hidden_; }

void account::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_PortfolioID), this->parent());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_Description), this->description());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxRate), this->taxRate());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_TaxDeferred), (int)this->taxDeferred());
    values.insert(queries::portfolioAccountColumns.at(queries::portfolioAccountColumns_CostBasis), (int)this->costBasis());
    values.insert(queries::portfolioAAColumns.at(queries::portfolioAccountColumns_Hide), (int)this->hidden());

    this->setID(dataSource_.insert(queries::table_PortfolioAccount, values, this->id()));
}

void account::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioAccount, this->id());
}

account account::load(const QSqlQuery &q_)
{
    account acct(
        q_.value(queries::portfolioAccountColumns_ID).toInt(),
        q_.value(queries::portfolioAccountColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioAccountColumns_Description).toString()
    );

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
    return stream_;
}
