#include "nav.h"

void nav::run(int date_, const QList<portfolio> &portfolios_)
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    foreach(portfolio p, portfolios_)
    {
        p.beginExecutedTradesBatch();
        p.beginNAVBatch();

        calculateNAV(p, date_);

        p.insertExecutedTradesBatch(m_dataSource);
        p.insertNAVBatch(m_dataSource);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (nav): %d ms", t.elapsed());
#endif
}

void nav::calculateNAV(portfolio portfolio_, int date_)
{
    if (!portfolio_.navHistory().isEmpty())
        date_ = qMax( // keep the date greater than or equal to the portfolio start date
                    qMin( // move the date back if the portfolio hasn't been calculated up to the passed in date
                        date_,
                        (portfolio_.navHistory().constEnd() - 1).key()
                    ),
                    portfolio_.attributes().startDate
                );

    bool recalculateAll = date_ <= portfolio_.attributes().startDate;

    clearHistoricalValues(portfolio_, date_, recalculateAll);

    tradeDateCalendar calendar(date_);
    if (calendar.date() > calendar.endDate())
        return;

    calculations calc(portfolio_);
    QList<tradePointerWithDates> trades = calculateTradesWithDates(portfolio_, calendar.date(), recalculateAll);

    if (recalculateAll)
        insertExecutedTradesPreStartDate(portfolio_, date_, trades);

    snapshotPortfolio previousSnapshot = calc.portfolioSnapshot(calendar.date());
    double navValue =
            recalculateAll ?
                portfolio_.attributes().startValue :
                portfolio_.navHistory().nav(calendar.date());

    if (recalculateAll) // cannot do this above since pre-start date trades must be inserted first
        portfolio_.navHistory().insert(calendar.date(), navValue, previousSnapshot.totalValue);

    foreach(const int &date, ++calendar)
    {
        insertExecutedTrades(portfolio_, date, previousSnapshot, trades);

        const snapshotPortfolio presentSnapshot = calc.portfolioSnapshot(date);

        navValue = calc.change(
            previousSnapshot.totalValue,
            presentSnapshot.totalValue,
            presentSnapshot.costBasis - previousSnapshot.costBasis,
            portfolio_.attributes().dividends ?
                presentSnapshot.dividendAmount :
                0,
            navValue
        );

        portfolio_.navHistory().insert(date, navValue, presentSnapshot.totalValue);
        previousSnapshot = presentSnapshot;
    }
}

void nav::clearHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_)
{
    for(QMap<int, security>::iterator i = portfolio_.securities().begin(); i != portfolio_.securities().end(); ++i)
        for(QMap<int, trade>::iterator x = i->trades.begin(); x != i->trades.end(); ++x)
            if (recalculateAll_)
                x.value().executedTrades.remove(m_dataSource);
            else
                x.value().executedTrades.remove(m_dataSource, beginDate_);

    if (recalculateAll_)
        portfolio_.navHistory().remove(m_dataSource);
    else
        portfolio_.navHistory().remove(m_dataSource, beginDate_);
}

QList<nav::tradePointerWithDates> nav::calculateTradesWithDates(portfolio portfolio_, int date_, bool recalculateAll_) const
{
    QList<tradePointerWithDates> calculatedTrades;

    for(QMap<int, security>::iterator security = portfolio_.securities().begin(); security != portfolio_.securities().end(); ++security)
    {
        if (!security->includeInCalc)
            continue;

        for(QMap<int, trade>::iterator trade = security->trades.begin(); trade != security->trades.end(); ++trade)
        {
            if (trade->type == trade::tradeType_Reversal)
                continue;

            if (trade->type == trade::tradeType_DivReinvestAuto) // get each dividend date
            {
                QMap<int, double> dividends = security->dividends();
                QList<int> dates;

                for(QMap<int, double>::iterator dividend = dividends.lowerBound(date_); dividend != dividends.constEnd(); ++dividend)
                    dates.append(dividend.key());

                if (!dates.isEmpty())
                    calculatedTrades.append(tradePointerWithDates(security, trade, dates));

                continue;
            }

            // these are not calculated on the fly and trades before the start date need to be inserted
            if (recalculateAll_ && trade->frequency == tradeDateCalendar::frequency_Once && trade->date < date_)
            {
                calculatedTrades.append(tradePointerWithDates(security, trade, QList<int>() << trade->date));
                continue;
            }

            QList<int> dates =
                tradeDateCalendar::computeFrequencyTradeDates(
                    trade->date,
                    qMax(trade->startDate, date_),
                    trade->endDate == 0 ?
                        security->endDate():
                        qMin(trade->endDate, security->endDate()),
                    trade->frequency
                );

            if (!dates.isEmpty())
                calculatedTrades.append(tradePointerWithDates(security, trade, dates));
        }
    }

    return calculatedTrades;
}

void nav::insertExecutedTradesPreStartDate(portfolio portfolio_, int beginDate_, const QList<tradePointerWithDates> &trades_)
{
    QSet<int> priorStartDates;
    foreach(const tradePointerWithDates &trade, trades_)
        foreach(const int &date, trade.dates)
        {
            if (date >= beginDate_)
                break;

            priorStartDates.insert(date);
        }

    foreach(const int &date, priorStartDates)
        insertExecutedTrades(portfolio_, date, snapshotPortfolio(), trades_);
}

void nav::insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const QList<tradePointerWithDates> &trades_)
{
    foreach(const tradePointerWithDates &trade, trades_)
    {
        if (qBinaryFind(trade.dates, date_) == trade.dates.constEnd()) // this date isn't found
            continue;

        double purchasePrice =
                trade.value().type == trade::tradeType_Interest || trade.value().type == trade::tradeType_InterestPercent ?
                0 :
                trade.value().price >= 0 ?
                    trade.value().price :
                    priorDaySnapshot_.isNull() ?
                        0 :
                        trade.parent().splitAdjustedPriorDayPrice(date_, priorDaySnapshot_.date);

        double shares = calculateShares(trade, purchasePrice, portfolio_, priorDaySnapshot_);
        if (functions::isZero(shares))
            continue;

        trade.value().executedTrades.insert(date_, executedTrade(shares, purchasePrice, trade.value().commission));

        if (trade.value().cashAccount != -1)
            insertPortfolioReversal(portfolio_, trade.value().cashAccount, date_, priorDaySnapshot_.date, shares * purchasePrice);
    }
}

double nav::calculateShares(const tradePointerWithDates &trade_, double purchasePrice_, portfolio portfolio_, const snapshotPortfolio &priorDaySnapshot_) const
{
    if (purchasePrice_ == 0)
        if (    // these types are allowed a price of 0
                trade_.value().type != trade::tradeType_Purchase &&
                trade_.value().type != trade::tradeType_DivReinvest &&
                trade_.value().type != trade::tradeType_Interest &&
                trade_.value().type != trade::tradeType_Sale
            )
            return 0;

    switch(trade_.value().type)
    {
        case trade::tradeType_Purchase:
        case trade::tradeType_DivReinvest:
        case trade::tradeType_Interest:
            return trade_.value().value;
        case trade::tradeType_Sale:
            return trade_.value().value * -1;
        case trade::tradeType_FixedPurchase:
            return trade_.value().value / purchasePrice_;
        case trade::tradeType_FixedSale:
            return trade_.value().value / purchasePrice_ * -1;
        case trade::tradeType_Value:
        case trade::tradeType_InterestPercent:
            return (priorDaySnapshot_.securitiesInfo.value(trade_.value().parent).totalValue * (trade_.value().value / 100)) / purchasePrice_;
        case trade::tradeType_TotalValue:
            return (priorDaySnapshot_.totalValue * (trade_.value().value / 100)) / purchasePrice_;
        case trade::tradeType_DivReinvestAuto:
            return (priorDaySnapshot_.securitiesInfo.value(trade_.value().parent).shares * trade_.parent().dividend(priorDaySnapshot_.date)) / purchasePrice_;
        case trade::tradeType_Reversal:
                return 0; // should not occur
        case trade::tradeType_AA:
        {
            double shares = 0;
            assetAllocationTarget targets = trade_.parent().targets;
            for(QMap<int, double>::const_iterator i = targets.constBegin(); i != targets.constEnd(); ++i)
            {
                double target = portfolio_.assetAllocations().value(i.key()).target;
                if (target <= 0)
                    continue;

                shares +=
                    // Total portfolio value * ratio [which is AA target * % AA of security * trade value]
                    // less current security value = amount to purchase / sell.
                    // Divide by price to get shares.
                        (
                                (priorDaySnapshot_.totalValue * (target * i.value() * trade_.value().value / 100))
                            -
                                (priorDaySnapshot_.securitiesInfo.value(trade_.value().parent).totalValue)
                        )
                    /
                        purchasePrice_;
            }
            return shares;
        }
    }

    return 0;
}

void nav::insertPortfolioReversal(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double value_)
{
    if (!portfolio_.securities().contains(cashAccountID_))
        return;

    double price = portfolio_.securities()[cashAccountID_].splitAdjustedPriorDayPrice(date_, priorDate_);
    if (functions::isZero(price))
        return;

    portfolio_.securities()[cashAccountID_].reversalTrade().
            executedTrades.insert(date_, executedTrade(-1 * value_ / price, price, 0));
}
