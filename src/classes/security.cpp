#include "security.h"
#include <QSqlQuery>
#include "queries.h"
#include "functions.h"
#include "assetAllocationTarget.h"
#include "executedTrade.h"
#include "trade.h"
#include "historicalPrices.h"

class securityData: public objectKeyData
{
    int account;
    double expenseRatio;
    bool dividendReinvestment;
    bool dividendNAVAdjustment;
    bool cashAccount;
    bool includeInCalc;
    bool hidden;
    QString note;
    assetAllocationTarget targets;
    QMap<int, trade> trades;
    executedTradeMap executedTrades;
    historicalPrices prices;

    explicit security(int id_, int parent_, const QString &description_):
        objectKeyData(description_, id_, parent_),
        account(UNASSIGNED),
        expenseRatio(0),
        dividendReinvestment(false),
        dividendNAVAdjustment(true),
        cashAccount(false),
        includeInCalc(true),
        hide(false),
        executedTrades(id_),
        prices(historicalPrices(description_))
    {}
};

bool security::operator==(const security &other_) const
{
    return d->objectKeyData::operator==(*other_.d)
            && d->account == other_.d->account
            && d->expenseRatio == other_.d->expenseRatio
            && d->dividendReinvestment == other_.d->dividendReinvestment
            && d->cashAccount == other_.d->cashAccount
            && d->includeInCalc == other_.d->includeInCalc
            && d->hide == other_.d->hide
            && d->targets == other_.d->targets
            && d->trades == other_.d->trades
            && d->note == other_.d->note
            && d->dividendNAVAdjustment == other_.d->dividendNAVAdjustment
            && d->executedTrades == other_.d->executedTrades;
}

int security::account() const { return d->account; }
void security::setAccount(int account_) { d->account = account_; }

double security::expenseRatio() const { return d->expenseRatio; }
void security::setExpenseRatio(double expenseRatio_) { d->expenseRatio = expenseRatio_; }

bool security::dividendReinvestment() const { return d->dividendReinvestment; }
void security::setDividendReinvestment(bool dividendReinvestment_) { d->dividendReinvestment = dividendReinvestment_; }

bool security::dividendNAVAdjustment() const { return d->dividendNAVAdjustment; }
void security::setDividendNAVAdjustment(bool dividendNAVAdjustment) { d->dividendNAVAdjustment = dividendNAVAdjustment_; }

bool security::cashAccount() const { return d->cashAccount; }
void security::setCashAccount(bool cashAccount_) { d->cashAccount = cashAccount_; }

bool security::includeInCalc() const { return d->includeInCalc; }
void security::setIncludeInCalc(bool includeInCalc_) { d->includeInCalc = includeInCalc_; }

bool security::hidden() const { return d->hidden; }
void security::setHidden(bool hidden_) { d->hidden = hidden_; }

QString security::note() const { return d->note; }
void security::setNote(const QString &note_) { d->note = note_; }

assetAllocationTarget& security::targets() const { return d->targets; }

QMap<int, trade>& security::trades() const { return d->trades; }

executedTradeMap& security::executedTrades() const { return d->executedTrades; }

double security::price(int date_) const { return cashAccount() ? 1 : d->prices.value(date_, historicalPrices::type_price); }
double security::splitAdjustedPriorDayPrice(int date_) const { return price(tradeDateCalendar::previousTradeDate(date_)) / split(date_); }

double security::dividend(int date_) const { return cashAccount() ? 0 : d->prices.value(date_, historicalPrices::type_dividend); }
QMap<int, double> security::dividends() const { return cashAccount() ? QMap<int, double>() : d->prices.values(historicalPrices::type_dividend); }

double security::split(int date_) const { return cashAccount() ? 1 : d->prices.value(date_, historicalPrices::type_split); }
QMap<int, double> security::splits() const { return cashAccount() ? QMap<int, double>() : d->prices.values(historicalPrices::type_split); }

void security::setHistoricalPrices(const historicalPrices &prices_) { d->prices = prices_; }

int security::endDate() const { return cashAccount() ? tradeDateCalendar::endDate() : d->prices.endDate(historicalPrices::type_price); }
int security::beginDate() const { return cashAccount() ? 0 : d->prices.beginDate(historicalPrices::type_price); }

void security::save(const queries &dataSource_)
{
    if (!this->hasParent())
        return;

    QMap<QString, QVariant> values;
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_PortfolioID), this->parent());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Symbol), this->description());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Account), functions::intToNull(this->account()));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Expense), functions::doubleToNull(this->expenseRatio()));
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_DivReinvest), (int)this->dividendReinvestment());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_CashAccount), (int)this->cashAccount());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_IncludeInCalc), (int)this->includeInCalc());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Hide), (int)this->hidden());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Note), this->note());
    values.insert(queries::portfolioSecurityColumns.at(queries::portfolioSecurityColumns_Dividends), (int)this->dividendNAVAdjustment());

    this->setID(dataSource_.insert(queries::table_PortfolioSecurity, values, this->id()));
}

void security::remove(const queries &dataSource_) const
{
    if (!this->hasIdentity())
        return;

    dataSource_.deleteItem(queries::table_PortfolioSecurity, this->id());
}

security security::load(const QSqlQuery &q_)
{
    security sec(
        q_.value(queries::portfolioSecurityColumns_ID).toInt(),
        q_.value(queries::portfolioSecurityColumns_PortfolioID).toInt(),
        q_.value(queries::portfolioSecurityColumns_Symbol).toString()
    );

    if (!q_.value(queries::portfolioSecurityColumns_Account).isNull())
        sec.setAccount(q_.value(queries::portfolioSecurityColumns_Account).toInt());
    if (!q_.value(queries::portfolioSecurityColumns_Expense).isNull())
        sec.setExpenseRatio(q_.value(queries::portfolioSecurityColumns_Expense).toDouble());
    sec.setDividendReinvestment(q_.value(queries::portfolioSecurityColumns_DivReinvest).toBool());
    sec.setCashAccount(q_.value(queries::portfolioSecurityColumns_CashAccount).toBool());
    sec.setIncludeInCalc(q_.value(queries::portfolioSecurityColumns_IncludeInCalc).toBool());
    sec.setHidden(q_.value(queries::portfolioSecurityColumns_Hide).toBool());
    sec.setNote(q_.value(queries::portfolioSecurityColumns_Note).toString());
    sec.setDividendReinvestment(q_.value(queries::portfolioSecurityColumns_Dividends).toBool());

    return sec;
}

QString security::validate() const
{
    if (this->description.isEmpty())
        return "Please enter a description!";

    if (functions::massage(1 - this->targets.totalAssignedPercentage()) < 0)
        return "Asset allocation should not sum to more than 100!";

    return QString();
}

objectType security::type() const
{
    return objectType_Security;
}

QDataStream& operator<<(QDataStream &stream_, const security &sec_)
{
    stream_ << sec_.account();
    stream_ << sec_.cashAccount();
    stream_ << sec_.description();
    stream_ << sec_.dividendNAVAdjustment();
    stream_ << sec_.dividendReinvestment();
    stream_ << sec_.expenseRatio();
    stream_ << sec_.hide;
    stream_ << sec_.includeInCalc();
    stream_ << sec_.note();
    stream_ << sec_.targets();
    return stream_;
}

QDataStream& operator>>(QDataStream &stream_, security &sec_)
{
    stream_ >> sec_.d->account;
    stream_ >> sec_.d->cashAccount;
    stream_ >> sec_.d->description;
    stream_ >> sec_.d->dividendNAVAdjustment;
    stream_ >> sec_.d->dividendReinvestment;
    stream_ >> sec_.d->expenseRatio;
    stream_ >> sec_.d->hide;
    stream_ >> sec_.d->includeInCalc;
    stream_ >> sec_.d->note;
    stream_ >> sec_.d->targets;
    return stream_;
}
