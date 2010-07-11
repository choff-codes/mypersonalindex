#define portfolios portfolio::instance()
#define priceManager prices::instance()
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
        calculateNAVValues(p, date_);
        p.insertExecutedTradesBatch(m_dataSource);
        p.insertNAVBatch(m_dataSource);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (nav): %d ms", t.elapsed());
#endif
}

void nav::calculateNAVValues(portfolio portfolio_, int date_)
{
    if (!portfolio_.navHistory().isEmpty())
        date_ = qMin(date_, (portfolio_.navHistory().constEnd() - 1).key());

    bool recalculateAll = calculationDate <= portfolio_.attributes().startDate;

    removeHistoricalValues(portfolio_, date_, recalculateAll);

    tradeDateCalendar calendar(date_);
    calculations calc(portfolio_);
    const QMap<int, navTradeList> trades = calculateExecutedTrades(portfolioID, calculationDate, calculateFromStartDate);
    const QList<int> securityReinvestments = portfolio_.securityReinvestments();

    if (recalculateAll)
        insertExecutedTradesPreStartDate(portfolio_, date_, trades);

    snapshotPortfolio previousSnapshot = calc.portfolioSnapshot(calendar.date());
    double navValue = recalculateAll ? portfolio_.attributes().startValue : portfolio_.navHistory().nav(calendar.date());

    if (recalculateAll)
        insertNAV(portfolio_, calendar.date(), previousSnapshot.totalValue, navValue);

    ++calendar;
    foreach(const int &date, calendar)
    {
        insertPortfolioReinvestments(portfolio_, date, securityReinvestments, previousSnapshot);
        insertExecutedTrades(portfolio_, date, previousSnapshot, trades.value(date));

        const snapshotPortfolio presentSnapshot = calc.portfolioSnapshot(date);

        navValue = calc.change(
            previousSnapshot.totalValue,
            presentSnapshot.totalValue,
            presentSnapshot.costBasis - previousSnapshot.costBasis,
            portfolio_.attributes().dividends ? presentSnapshot.dividendAmount : 0,
            navValue
        );

        insertNAV(portfolio_, date, presentSnapshot.totalValue, navValue);
        previousSnapshot = presentSnapshot;
    }
}

void nav::insertNAV(portfolio portfolio_, int date_, double endValue_, double nav_)
{
    portfolio_.navHistory().insert(m_dataSource, date_, nav_, endValue_);
}

void nav::insertExecutedTrade(portfolio portfolio_, int id_, int date_, double shares_, double price_, double commission_)
{
    if (shares_ == 0)
        return;

    portfolio_.securities()[id_].executedTrades.insert(m_dataSource, date_, executedTrade(shares_, price_, commission_));
}

void nav::removeHistoricalValues(portfolio portfolio_, int beginDate_, bool recalculateAll_)
{
    if (recalculateAll_)
    {
        portfolio_.executedTrades().remove(m_dataSource);
        portfolio_.navHistory().remove(m_dataSource);
        return;
    }

    portfolio_.executedTrades().remove(m_dataSource, beginDate_);
    portfolio_.navHistory().remove(m_dataSource, beginDate_);
}

const QMap<int, nav::navTradeList> nav::calculateExecutedTrades(portfolio portfolio_, int date_, bool recalculateAll_)
{
    QMap<int, navTradeList> trades;

    foreach(const security &s, portfolio_.securities())
    {
        if (!s.includeInCalc)
            continue;

        for(navTradePointer trade = s.trades.constBegin(); trade != s.trades.constEnd(); ++trade)
        {
            QList<int> dates;

            // these are not calculated on the fly and trades before the start date need to be inserted
            if (recalculateAll_ && trade->frequency = tradeDateCalendar::frequency_Once && trade->date < date_)
                dates.append(trade->date);
            else
                dates =
                    tradeDateCalendar::computeFrequencyTradeDates(
                        trade->date,
                        qMax(trade->startDate, date_),
                        qMin(trade->endDate, s.endDate()),
                        trade->frequency
                    );

            foreach(const int &i, dates)
                trades[i][s.id].append(trade);
        }
    }

    return trades;
}

void nav::insertPortfolioReinvestments(portfolio portfolio_, int date_, const QList<int> &securityReinvestments_, const snapshotPortfolio &priorDaySnapshot_)
{
    foreach(const int &securityID, securityReinvestments_)
    {
        security s = portfolio_.securities().value(securityID);
        double dividend = s.dividend(priorDaySnapshot_.date);
        double price = s.price(priorDaySnapshot_.date);

        if (dividend == 0 || price == 0)
            continue;

        insertExecutedTrade(
            portfolio_,
            securityID,
            date_,
            (dividend * priorDaySnapshot_.securitiesInfo.value(securityID).shares) / price,
            price / s.split(date_),
            0
        );
    }
}

void nav::insertPortfolioCashTrade(portfolio portfolio_, int cashAccountID_, int date_, int priorDate_, double &value_)
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

void nav::insertExecutedTrades(portfolio portfolio_, int date_, const snapshotPortfolio &priorDaySnapshot_, const navTradeList &trades_)
{
    for(navTradeList::const_iterator i = trades_.constBegin(); i != trades_.constEnd(); ++i)
    {
        security s = portfolio_.securities().value(i.key());
        double price = priorDaySnapshot_.isNull() ?
                       0 :
                       s.price(priorDaySnapshot_.date) / s.split(date_);

        foreach(const navTradePointer &trade, i.value())
        {
            double purchasePrice =
                type == tradeType_Interest || type == tradeType_InterestPercent ?
                    0 :
                trade->price >= 0 ?
                    trade->price :
                    price;

            double sharesToBuy = trade->shares(purchasePrice, priorDaySnapshot_.securitiesInfo.value(s.id).totalValue, priorDaySnapshot_.totalValue,
                s.aa, portfolio_.assetAllocations());

            if (purchasePrice > 0 && sharesToBuy != 0)
                insertExecutedTrade(portfolio_, s.id, date_, sharesToBuy, purchasePrice, trade->commission);

            if (trade->cashAccount != -1 && sharesToBuy > 0)
                insertPortfolioCashTrade(portfolio_, trade->cashAccount, date_, priorDaySnapshot_.date, sharesToBuy * purchasePrice);
        }
    }
}

void nav::insertExecutedTradesPreStartDate(portfolio portfolio_, int beginDate_, const QMap<int, navTradeList> &trades_)
{
    for(QMap<int, navTradeList>::const_iterator i = trades_.begin(); i != trades_.end(); ++i)
    {
        int date = i.key();
        if (date >= beginDate_)
            break;

        insertExecutedTrades(portfolio_, date_, snapshotPortfolio(), i.value());
    }
}
