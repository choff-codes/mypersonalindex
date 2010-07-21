#include "nav.h"

void nav::run(int date_, const QList<portfolio> portfolios_)
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
        date_ = qMin(date_, (portfolio_.navHistory().constEnd() - 1).key());

    bool recalculateAll = date_ <= portfolio_.attributes().startDate;
    date_ = qMax(date_, portfolio_.attributes().startDate);

    clearHistoricalValues(portfolio_, date_, recalculateAll);

    tradeDateCalendar calendar(date_);
    if (calendar.date() > calendar.endDate())
        return;

    calculations calc(portfolio_);
    QMap<int, tradePointerMap> trades = calculateExecutedTrades(portfolio_, calendar.date(), recalculateAll);
    QList<int> securityReinvestments = portfolio_.securityReinvestments();

    if (recalculateAll)
        insertExecutedTradesPreStartDate(portfolio_, date_, trades);

    snapshotPortfolio previousSnapshot = calc.portfolioSnapshot(calendar.date());
    double navValue =
            recalculateAll ?
                portfolio_.attributes().startValue :
                portfolio_.navHistory().nav(calendar.date());

    if (recalculateAll) // cannot do this above since pre-start date trades must be inserted first
        insertNAV(portfolio_, calendar.date(), previousSnapshot.totalValue, navValue);

    foreach(const int &date, ++calendar)
    {
        insertPortfolioReinvestments(portfolio_, date, securityReinvestments, previousSnapshot);
        insertExecutedTrades(portfolio_, date, previousSnapshot, trades.value(date));

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

        insertNAV(portfolio_, date, presentSnapshot.totalValue, navValue);
        previousSnapshot = presentSnapshot;
    }
}

void nav::insertNAV(portfolio portfolio_, int date_, double endValue_, double nav_)
{
    portfolio_.navHistory().insert(date_, nav_, endValue_);
}

void nav::insertExecutedTrade(portfolio portfolio_, int id_, int date_, double shares_, double price_, double commission_)
{
    if (shares_ == 0)
        return;

    portfolio_.securities()[id_].executedTrades.insert(date_, executedTrade(shares_, price_, commission_));
}

void nav::clearHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_)
{
    if (recalculateAll_)
    {
        for(QMap<int, security>::iterator i = portfolio_.securities().begin(); i != portfolio_.securities().end(); ++i)
            i.value().executedTrades.remove(m_dataSource);

        portfolio_.navHistory().remove(m_dataSource);

        return;
    }

    for(QMap<int, security>::iterator i = portfolio_.securities().begin(); i != portfolio_.securities().end(); ++i)
        i.value().executedTrades.remove(m_dataSource, beginDate_);

    portfolio_.navHistory().remove(m_dataSource, beginDate_);
}

QMap<int, nav::tradePointerMap> nav::calculateExecutedTrades(portfolio portfolio_, int date_, bool recalculateAll_) const
{
    QMap<int, tradePointerMap> trades;

    foreach(const security &s, portfolio_.securities())
    {
        if (!s.includeInCalc)
            continue;

        for(tradePointer trade = s.trades.constBegin(); trade != s.trades.constEnd(); ++trade)
        {
            QList<int> dates;

            // these are not calculated on the fly and trades before the start date need to be inserted
            if (recalculateAll_ && trade->frequency == tradeDateCalendar::frequency_Once && trade->date < date_)
                dates.append(trade->date);
            else
                dates =
                    tradeDateCalendar::computeFrequencyTradeDates(
                        trade->date,
                        qMax(trade->startDate, date_),
                        trade->endDate == 0 ?
                            s.endDate():
                            qMin(trade->endDate, s.endDate()),
                        trade->frequency
                    );

            foreach(const int &i, dates)
                trades[i][s.id].append(trade);
        }
    }

    return trades;
}

double nav::calculateShares(trade::tradeType tradeType_, double value_, double purchasePrice_, int id_, portfolio portfolio_, const snapshotPortfolio &priorDaySnapshot_) const
{
    if (purchasePrice_ == 0)
        if (    // these types are allowed a price of 0
                tradeType_ != trade::tradeType_Purchase &&
                tradeType_ != trade::tradeType_DivReinvest &&
                tradeType_ != trade::tradeType_Interest &&
                tradeType_ != trade::tradeType_Sale
            )
            return 0;

    switch(tradeType_)
    {
        case trade::tradeType_Purchase:
        case trade::tradeType_DivReinvest:
        case trade::tradeType_Interest:
            return value_;
        case trade::tradeType_Sale:
            return value_ * -1;
        case trade::tradeType_FixedPurchase:
            return value_ / purchasePrice_;
        case trade::tradeType_FixedSale:
            return value_ / purchasePrice_ * -1;
        case trade::tradeType_Value:
        case trade::tradeType_InterestPercent:
            return (priorDaySnapshot_.securitiesInfo.value(id_).totalValue * (value_ / 100)) / purchasePrice_;
        case trade::tradeType_TotalValue:
            return (priorDaySnapshot_.totalValue * (value_ / 100)) / purchasePrice_;
        case trade::tradeType_AA:
        {
            double shares = 0;
            security s =  portfolio_.securities().value(id_);
            for(QMap<int, double>::const_iterator i = s.targets.constBegin(); i != s.targets.constEnd(); ++i)
            {
                double target = portfolio_.assetAllocations().value(i.key()).target;
                if (target <= 0)
                    continue;

                shares +=
                    // Total portfolio value * ratio [AA target * % AA of security * trade value]
                    // less current security value = amount to purchase / sell.
                    // Divide by price to get shares.
                        (
                                (priorDaySnapshot_.totalValue * (target * i.value() * value_ / 100))
                            -
                                (priorDaySnapshot_.securitiesInfo.value(id_).totalValue)
                        )
                    /
                        purchasePrice_;
            }
            return shares;
        }
    }

    return 0;
}

void nav::insertPortfolioReinvestments(portfolio portfolio_, int date_, const QList<int> &securityReinvestments_, const snapshotPortfolio &priorDaySnapshot_)
{
    foreach(const int &id, securityReinvestments_)
    {
        security s = portfolio_.securities().value(id);
        double dividend = s.dividend(priorDaySnapshot_.date);
        double price = s.price(priorDaySnapshot_.date);

        if (dividend == 0 || price == 0)
            continue;

        insertExecutedTrade(
            portfolio_,
            id,
            date_,
            (dividend * priorDaySnapshot_.securitiesInfo.value(id).shares) / price,
            price / s.split(date_),
            0
        );
    }
}

void nav::insertPortfolioCashReversal(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double value_)
{
    security s = portfolio_.securities().value(cashAccountID_);

    double price = s.price(priorDate_) / s.split(date_);
    if (price == 0)
        return;

    insertExecutedTrade(
        portfolio_,
        cashAccountID_,
        date_,
        -1 * value_ / price,
        price,
        0
    );
}

void nav::insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const tradePointerMap &trades_)
{
    for(tradePointerMap::const_iterator i = trades_.constBegin(); i != trades_.constEnd(); ++i)
    {
        security s = portfolio_.securities().value(i.key());

        foreach(const tradePointer &trade, i.value())
        {
            double purchasePrice =
                    trade->type == trade::tradeType_Interest || trade->type == trade::tradeType_InterestPercent ?
                    0 :
                    trade->price >= 0 ?
                        trade->price :
                        priorDaySnapshot_.isNull() ?
                            0 :
                            s.price(priorDaySnapshot_.date) / s.split(date_);

            double shares = calculateShares(trade->type, trade->value, purchasePrice, s.id, portfolio_, priorDaySnapshot_);

            if (shares == 0)
                continue;

            insertExecutedTrade(portfolio_, s.id, date_, shares, purchasePrice, trade->commission);

            if (trade->cashAccount != -1)
                insertPortfolioCashReversal(portfolio_, trade->cashAccount, date_, priorDaySnapshot_.date, shares * purchasePrice);
        }
    }
}

void nav::insertExecutedTradesPreStartDate(portfolio portfolio_, int beginDate_, const QMap<int, tradePointerMap> &trades_)
{
    for(QMap<int, tradePointerMap>::const_iterator i = trades_.begin(); i != trades_.end(); ++i)
    {
        int date = i.key();
        if (date >= beginDate_)
            break;

        insertExecutedTrades(portfolio_, date, snapshotPortfolio(), i.value());
    }
}
