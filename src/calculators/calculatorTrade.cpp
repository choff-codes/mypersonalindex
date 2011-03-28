#include "calculatorTrade.h"
#include "snapshot.h"
#include "portfolio.h"
#include "security.h"
#include "calculatorNAV.h"
#include "assetAllocation.h"
#include "functions.h"
#include "executedTrade.h"
#include "assetAllocationTarget.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

bool calculatorTrade::operator()(const portfolio &portfolio_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    insertDividendReinvestmentPlaceholders(portfolio_);

    // keep the date greater than or equal to the portfolio start date
    int beginDate = qMax(m_beginDate, portfolio_.startDate());
    // clear out calculated trades from the begin date (or all trades if its the portfolio start date)
    clearExecutedTrades(portfolio_, beginDate, beginDate == portfolio_.startDate());
    // clear calculator cache
    portfolio_.calculator().clearCache();
    // calculate
    calculate(portfolio_, beginDate);

    removeDividendReinvestmentPlaceholders(portfolio_);

#ifdef CLOCKTIME
    qDebug("Time elapsed (trades): %d ms", t.elapsed());
#endif

    return true;
}

void calculatorTrade::insertDividendReinvestmentPlaceholders(portfolio portfolio_)
{
    foreach(security s, portfolio_.securities())
        if (s.dividendReinvestment())
        {
            trade t;
            t.setID(DIVIDEND_REINVESTMENT_TRADE_ID);
            t.setParent(s.id());
            t.setAction(trade::tradeAction_ReinvestDividendsAuto);
            s.trades().insert(DIVIDEND_REINVESTMENT_TRADE_ID, t);
        }
}

void calculatorTrade::removeDividendReinvestmentPlaceholders(portfolio portfolio_)
{
    foreach(security s, portfolio_.securities())
        s.trades().remove(DIVIDEND_REINVESTMENT_TRADE_ID);
}

void calculatorTrade::clearExecutedTrades(portfolio portfolio_, int beginDate_, bool recalculateAll_)
{
    foreach(security s, portfolio_.securities())
        if (recalculateAll_)
            s.executedTrades().remove();
        else
            s.executedTrades().remove(beginDate_);
}

void calculatorTrade::calculate(portfolio portfolio_, int beginDate_)
{
    tradeMapByDate trades = calculateTradeDates(portfolio_, beginDate_, beginDate_ == portfolio_.startDate());

    // loop through each day
    for(tradeMapByDate::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        int date = i.key();

        // loop through each security that has a trade that day
        for(tradeListBySecurity::const_iterator sec = i.value().constBegin(); sec != i.value().constEnd(); ++sec)
        {
            const security s = portfolio_.securities().value(sec.key());

            // loop through each trade and calculate
            foreach(const trade &t, sec.value())
            {
                // insert executed trade
                executedTrade e = calculateExecutedTrade(
                        date,
                        portfolio_.calculator(),
                        portfolio_.assetAllocations(),
                        s,
                        t
                    );

                insertExecutedTrade(s, date, e);

                if (t.cashAccount() == UNASSIGNED)
                    continue;

                // reverse trade
                executedTrade reversal = calculateExecutedTradeReversal(
                        portfolio_.securities().value(t.cashAccount()),
                        date,
                        e.shares,
                        e.price,
                        s.id()
                    );

                insertExecutedTrade(portfolio_.securities().value(t.cashAccount()), date, reversal);
            }
        }
    }
}

// note this must have the portfolio object passed in so that the iterator pointers do not invalidate
calculatorTrade::tradeMapByDate calculatorTrade::calculateTradeDates(portfolio portfolio_, int date_, bool recalculateAll_) const
{
    tradeMapByDate calculatedTrades;

    foreach(const security &sec, portfolio_.securities())
    {
        if (!sec.includeInCalc() || sec.deleted())
            continue;

        foreach(const trade &t, sec.trades())
        {
            if (t.deleted())
                continue;

            if (t.action() == trade::tradeAction_ReinvestDividendsAuto) // get each dividend date greater than or equal to current date
            {
                foreach(int date, calculateDividendReinvestmentDates(date_, sec.dividends()))
                    calculatedTrades[date][sec.id()].append(t);

                continue;
            }

            // these are not calculated on the fly and trades before the start date need to be inserted
            if (recalculateAll_ && t.frequency() == tradeDateCalendar::frequency_Once && t.date() < date_)
            {
                calculatedTrades[t.date()][sec.id()].append(t);
                continue;
            }

            // compute dates
            QList<int> dates =
                tradeDateCalendar::computeFrequencyTradeDates(
                    t.date(),
                    qMax(
                        t.frequency() == tradeDateCalendar::frequency_Once ?
                            0 :
                            sec.beginDate() + (int)t.priceType(), // add 1 for trade price type of previous
                        qMax(t.startDate(), date_)
                    ),
                    t.frequency() == tradeDateCalendar::frequency_Once ?
                        t.endDate() == 0 ?
                            tradeDateCalendar::endDate() :
                            qMin(t.endDate(), tradeDateCalendar::endDate()) :
                        t.endDate() == 0 ?
                            sec.endDate() + (int)t.priceType() : // add 1 for trade price type of previous
                            qMin(t.endDate(), sec.endDate() + (int)t.priceType()), // add 1 for trade price type of previous
                    t.frequency()
                );

            foreach(int date, dates)
                calculatedTrades[date][sec.id()].append(t);
        }
    }

    return calculatedTrades;
}

QList<int> calculatorTrade::calculateDividendReinvestmentDates(int date_, const QMap<int, double> dividends_) const
{
    QList<int> dates;
    int endDate = tradeDateCalendar::endDate();

    // add a day to the dividend date since it wouldn't be received until EOD, closing price that day
    for(QMap<int, double>::const_iterator dividend = dividends_.lowerBound(date_); dividend != dividends_.constEnd(); ++dividend)
    {
        int date = tradeDateCalendar::nextTradeDate(dividend.key());
        if (date <= endDate)
            dates.append(tradeDateCalendar::nextTradeDate(dividend.key()));
    }

    return dates;
}

executedTrade calculatorTrade::calculateExecutedTrade(int date_, const calculatorNAV &calc_, const QMap<int, assetAllocation> &aa, const security &parent_, const trade &trade_) const
{
    double purchasePrice = calculateTradePrice(
        trade_.action(),
        trade_.priceType() == trade::tradePriceType_UserDefined ?
            trade_.price() :
            trade_.priceType() == trade::tradePriceType_CurrentClose ?
                parent_.price(date_) :
                parent_.splitAdjustedPriorDayPrice(date_)
    );

    double shares = calculateTradeShares(
        date_,
        purchasePrice,
        calc_,
        aa,
        parent_,
        trade_
    );

    return executedTrade(shares, purchasePrice, trade_.commission(), trade_.id());
}

double calculatorTrade::calculateTradePrice(trade::tradeAction type_, double price_) const
{
    if (type_ == trade::tradeAction_ReceiveInterest || type_ == trade::tradeAction_ReceiveInterestPercent)
        return 0;

    return price_;
}

double calculatorTrade::calculateTradeShares(int date_, double price_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa,
    const security &parent_, const trade &trade_) const
{
    if (functions::isZero(price_) &&
        // these types are allowed a price of 0
        trade_.action() != trade::tradeAction_Purchase &&
        trade_.action() != trade::tradeAction_ReinvestDividends &&
        trade_.action() != trade::tradeAction_ReceiveInterest &&
        trade_.action() != trade::tradeAction_Sell )
            return 0;

    date_ = tradeDateCalendar::previousTradeDate(date_);

    switch(trade_.action())
    {
        case trade::tradeAction_Purchase:
        case trade::tradeAction_ReinvestDividends:
        case trade::tradeAction_ReceiveInterest:
            return trade_.value();
        case trade::tradeAction_Sell:
            return trade_.value() * -1;
        case trade::tradeAction_PurchaseFixedAmount:
            return trade_.value() / price_;
        case trade::tradeAction_SellFixedAmount:
            return trade_.value() / price_ * -1;
        case trade::tradeAction_PurchasePercentOfSecurityValue:
        case trade::tradeAction_ReceiveInterestPercent:
            return (calc_.securitySnapshot(date_, parent_.id()).totalValue * (trade_.value() / 100)) / price_;
        case trade::tradeAction_PurchasePercentOfPortfolioValue:
            return (calc_.portfolioSnapshot(date_).totalValue * (trade_.value() / 100)) / price_;
        case trade::tradeAction_ReinvestDividendsAuto:
            return (calc_.securitySnapshot(date_, parent_.id()).shares * parent_.dividend(date_)) / price_;
        case trade::tradeAction_PurchasePercentOfAATarget:
        {
            double shares = 0;
            const assetAllocationTarget &targets = parent_.targets();
            for(QMap<int, double>::const_iterator i = targets.constBegin(); i != targets.constEnd(); ++i)
            {
                double target = aa.value(i.key()).target();
                if (functions::isZero(target))
                    continue;

                shares +=
                    // Total portfolio value * ratio [which is AA target * % AA of security * trade value]
                    // less current security value = amount to purchase / sell.
                    // Divide by price to get shares.
                        (
                                (calc_.portfolioSnapshot(date_).totalValue * (target * i.value() * trade_.value() / 100))
                            -
                                (calc_.securitySnapshot(date_, parent_.id()).totalValue)
                        )
                    /
                        price_;
            }
            return shares;
        }
    }

    return 0;
}

void calculatorTrade::insertExecutedTrade(security security_, int date_, const executedTrade &trade_)
{
    if (functions::isZero(trade_.shares))
        return;

    security_.executedTrades().insert(date_, trade_);
}

executedTrade calculatorTrade::calculateExecutedTradeReversal(const security &security_, int date_, double shares_, double price_, int spawningID_) const
{
    double price = security_.splitAdjustedPriorDayPrice(date_);

    return executedTrade(-1 * (shares_ * price_) / price, price, 0, spawningID_);
}
