#include "calculatorTrade.h"
#include "snapshot.h"
#include "portfolio.h"
#include "security.h"
#include "calculatorNAV.h"
#include "portfolioAttributes.h"
#include "assetAllocation.h"
#include "functions.h"

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
    calculate(portfolio_, m_beginDate);
    removeDividendReinvestmentPlaceholders(portfolio_);

#ifdef CLOCKTIME
    qDebug("Time elapsed (trades): %d ms", t.elapsed());
#endif
    return true;
}

void calculatorTrade::insertDividendReinvestmentPlaceholders(portfolio portfolio_)
{
    for(QMap<int, security>::iterator sec = portfolio_.securities().begin(); sec != portfolio_.securities().end(); ++sec)
        if (sec.value().dividendReinvestment)
            // use DIVIDEND_REINVESTMENT_TRADE_ID placeholder
            sec.value().trades.insert(DIVIDEND_REINVESTMENT_TRADE_ID, trade(DIVIDEND_REINVESTMENT_TRADE_ID, sec.value().id));
}

void calculatorTrade::removeDividendReinvestmentPlaceholders(portfolio portfolio_)
{
    for(QMap<int, security>::iterator sec = portfolio_.securities().begin(); sec != portfolio_.securities().end(); ++sec)
        sec.value().trades.remove(DIVIDEND_REINVESTMENT_TRADE_ID);
}

void calculatorTrade::clearExecutedTrades(portfolio portfolio_, int beginDate_, bool recalculateAll_)
{
    for(QMap<int, security>::iterator sec = portfolio_.securities().begin(); sec != portfolio_.securities().end(); ++sec)
        if (recalculateAll_)
            sec.value().executedTrades.remove();
        else
            sec.value().executedTrades.remove(beginDate_);
}

void calculatorTrade::calculate(portfolio portfolio_, int beginDate_)
{
    // keep the date greater than or equal to the portfolio start date
    beginDate_ = qMax(beginDate_, portfolio_.attributes().startDate);
    bool recalculateAll = beginDate_ == portfolio_.attributes().startDate;

    clearExecutedTrades(portfolio_, beginDate_, recalculateAll);

    QMap<int, tradeDay> trades = calculateTradeDates(portfolio_, beginDate_, recalculateAll);
    calculatorNAV calc(portfolio_); // keep a cache going, pass by reference/pointer ONLY (cache can get large)

    // loop through each day
    for(QMap<int, tradeDay>::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        int date = i.key();

        // loop through each security that has a trade that day
        for(tradeDay::const_iterator trades = i.value().constBegin(); trades != i.value().constEnd(); ++trades)
        {
            security s = portfolio_.securities().value(trades.key());

            // loop through each trade and calculate
            foreach(const tradeIterator &trade, trades.value())
            {
                // insert executed trade
                executedTrade e = calculateExecutedTrade(
                        date,
                        calc,
                        portfolio_.assetAllocations(),
                        s,
                        trade.value()
                    );

                insertExecutedTrade(portfolio_, s.id, date, e);

                if (trade.value().cashAccount == UNASSIGNED)
                    continue;

                // reverse trade
                executedTrade reversal = calculateExecutedTradeReversal(
                        portfolio_.securities().value(trade.value().cashAccount),
                        date,
                        e.shares,
                        e.price,
                        trade.key()
                    );

                insertExecutedTrade(portfolio_, trade.value().cashAccount, date, reversal);
            }
        }
    }
}

// note this must have the portfolio object passed in so that the iterator pointers do not invalidate
QMap<int, calculatorTrade::tradeDay> calculatorTrade::calculateTradeDates(portfolio portfolio_, int date_, bool recalculateAll_) const
{
    QMap<int, calculatorTrade::tradeDay> calculatedTrades;

    for(QMap<int, security>::const_iterator sec = portfolio_.securities().constBegin(); sec != portfolio_.securities().constEnd(); ++sec)
    {
        const security &security = sec.value();

        if (!security.includeInCalc || security.deleted)
            continue;

        for(QMap<int, trade>::const_iterator x = sec->trades.constBegin(); x != sec->trades.constEnd(); ++x)
        {
            const trade &trade = x.value();

            if (trade.deleted)
                continue;

            if (trade.action == trade::tradeAction_ReinvestDividendsAuto) // get each dividend date greater than or equal to current date
            {
                foreach(const int &date, calculateDividendReinvestmentDates(date_, security.dividends()))
                    calculatedTrades[date][security.id].append(x);

                continue;
            }

            // these are not calculated on the fly and trades before the start date need to be inserted
            if (recalculateAll_ && trade.frequency == tradeDateCalendar::frequency_Once && trade.date < date_)
            {
                calculatedTrades[trade.date][security.id].append(x);
                continue;
            }

            // compute dates
            QList<int> dates =
                tradeDateCalendar::computeFrequencyTradeDates(
                    trade.date,
                    qMax(trade.startDate, date_),
                    trade.endDate == 0 ?
                        security.endDate():
                        qMin(trade.endDate, security.endDate()),
                    trade.frequency
                );

            foreach(const int &date, dates)
                calculatedTrades[date][security.id].append(x);
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

executedTrade calculatorTrade::calculateExecutedTrade(int date_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa,
    const security &parent_, const trade &trade_) const
{
    double purchasePrice = calculateTradePrice(
            trade_.action,
            trade_.price,
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

    return executedTrade(shares, purchasePrice, trade_.commission, trade_.id);
}

double calculatorTrade::calculateTradePrice(trade::tradeAction type_, double price_, double priorDayPrice_) const
{
    if (type_ == trade::tradeAction_ReceiveInterest || type_ == trade::tradeAction_ReceiveInterestPercent)
        return 0;

    if (price_ >= 0)
        return price_;

    return priorDayPrice_;
}

double calculatorTrade::calculateTradeShares(int date_, double price_, calculatorNAV calc_, const QMap<int, assetAllocation> &aa,
    const security &parent_, const trade &trade_) const
{
    if (functions::isZero(price_))
        if (    // these types are allowed a price of 0
                trade_.action != trade::tradeAction_Purchase &&
                trade_.action != trade::tradeAction_ReinvestDividends &&
                trade_.action != trade::tradeAction_ReceiveInterest &&
                trade_.action != trade::tradeAction_Sell
            )
            return 0;

    date_ = tradeDateCalendar::previousTradeDate(date_);

    switch(trade_.action)
    {
        case trade::tradeAction_Purchase:
        case trade::tradeAction_ReinvestDividends:
        case trade::tradeAction_ReceiveInterest:
            return trade_.value;
        case trade::tradeAction_Sell:
            return trade_.value * -1;
        case trade::tradeAction_PurchaseFixedAmount:
            return trade_.value / price_;
        case trade::tradeAction_SellFixedAmount:
            return trade_.value / price_ * -1;
        case trade::tradeAction_PurchasePercentOfSecurityValue:
        case trade::tradeAction_ReceiveInterestPercent:
            return (calc_.securitySnapshot(date_, trade_.parent).totalValue * (trade_.value / 100)) / price_;
        case trade::tradeAction_PurchasePercentOfPortfolioValue:
            return (calc_.portfolioSnapshot(date_).totalValue * (trade_.value / 100)) / price_;
        case trade::tradeAction_ReinvestDividendsAuto:
            return (calc_.securitySnapshot(date_, trade_.parent).shares * parent_.dividend(date_)) / price_;
        case trade::tradeAction_PurchasePercentOfAATarget:
        {
            double shares = 0;
            assetAllocationTarget targets = parent_.targets;
            for(QMap<int, double>::const_iterator i = targets.constBegin(); i != targets.constEnd(); ++i)
            {
                double target = aa.value(i.key()).target;
                if (functions::massage(target) == 0)
                    continue;

                shares +=
                    // Total portfolio value * ratio [which is AA target * % AA of security * trade value]
                    // less current security value = amount to purchase / sell.
                    // Divide by price to get shares.
                        (
                                (calc_.portfolioSnapshot(date_).totalValue * (target * i.value() * trade_.value / 100))
                            -
                                (calc_.securitySnapshot(date_, trade_.parent).totalValue)
                        )
                    /
                        price_;
            }
            return shares;
        }
    }

    return 0;
}

void calculatorTrade::insertExecutedTrade(portfolio portfolio_, int id_, int date_, const executedTrade &trade_)
{
    if (functions::isZero(trade_.shares))
        return;

    portfolio_.securities()[id_].executedTrades.insert(date_, trade_);
}

executedTrade calculatorTrade::calculateExecutedTradeReversal(const security &security_, int date_, double shares_, double price_, int spawningID_) const
{
    double price = security_.splitAdjustedPriorDayPrice(date_);

    return executedTrade(-1 * (shares_ * price_) / price, price, 0, spawningID_);
}
