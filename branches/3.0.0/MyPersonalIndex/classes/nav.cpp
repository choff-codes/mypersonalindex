#define portfolios portfolio::instance()
#include "nav.h"

void nav::run()
{
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    QList<int> portfolioList;
    if (m_portfolioID == -1)
        portfolioList.append(portfolios.ids());
    else
        portfolioList.append(m_portfolioID);

    foreach(const int &p, portfolioList)
    {
        m_calculations.setPortfolio(p);
        calculateNAVValues(p);
    }

#ifdef CLOCKTIME
    qDebug("Time elapsed (nav): %d ms", t.elapsed());
#endif
}

void nav::calculateNAVValues(const int &portfolioID)
{
    bool calculateFromStartDate = false;
    int calculationDate = checkCalculationDate(portfolioID, m_calculationDate, &calculateFromStartDate);
    portfolioInfo info = portfolios.info(portfolioID);

    deleteOldValues(portfolioID, calculationDate, calculateFromStartDate);

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate - 1);
    if (*previousDate != calculationDate - 1 && previousDate != m_dates.constBegin())
        --previousDate;

    if (m_dates.constEnd() - previousDate < 2)
        return;

    QMap<int, navTradeList> trades = calculateExecutedTrades(portfolioID, calculationDate, calculateFromStartDate);
    if (calculateFromStartDate)
        insertFirstPortfolioTrades(portfolioID, calculationDate, trades);

    QList<int> securityReinvestments = getPortfolioSecurityReinvestment(portfolioID);
    dailyInfoPortfolio *previousInfo = m_calculations.portfolioValues(*previousDate);
    double navValue = calculateFromStartDate ? info.startValue : portfolios.nav(portfolioID).nav(*previousDate);

    if (calculateFromStartDate)
        addToNAVList(portfolioID, *previousDate, previousInfo->totalValue, navValue);

    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        int date = *currentDate;
        insertPortfolioReinvestments(portfolioID, date, securityReinvestments, previousInfo);
        insertPortfolioTrades(portfolioID, date, previousInfo, appendNavTradeLists(trades.value(date), trades.value(-1)));

        dailyInfoPortfolio *currentInfo = m_calculations.portfolioValues(date);
        navValue = calculations::change(currentInfo->totalValue, previousInfo->totalValue, currentInfo->costBasis - previousInfo->costBasis, info.dividends ? currentInfo->dividendAmount : 0, navValue);

        addToNAVList(portfolioID, date, currentInfo->totalValue, navValue);

        delete previousInfo;
        previousInfo = currentInfo;
    }

    delete previousInfo;
    insertVariantLists();
    clearVariantLists();
}

nav::navTradeList nav::appendNavTradeLists(const navTradeList &first, const navTradeList &second)
{
    navTradeList returnList = first;
    for(navTradeList::const_iterator i = second.constBegin(); i != second.constEnd(); ++i)
        returnList[i.key()].append(i.value());
    return returnList;
}

void nav::addToNAVList(const int &portfolioID, const int &date, const double &totalValue, const double &nav)
{
    m_NAV_Portfolio.append(portfolioID);
    m_NAV_Dates.append(date);
    m_NAV_Totalvalue.append(totalValue);
    m_NAV_Nav.append(nav);
    portfolios.insertNAV(portfolioID, date, nav, totalValue);
}

void nav::insertVariantLists()
{
    if (!m_NAV_Dates.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::navColumns.at(queries::navColumns_PortfolioID), m_NAV_Portfolio);
        tableValues.insert(queries::navColumns.at(queries::navColumns_Date), m_NAV_Dates);
        tableValues.insert(queries::navColumns.at(queries::navColumns_TotalValue), m_NAV_Totalvalue);
        tableValues.insert(queries::navColumns.at(queries::navColumns_NAV), m_NAV_Nav);
        queries::executeTableUpdate(queries::table_NAV, tableValues);
    }

    if (!m_ExecutedTrades_Dates.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_Date), m_ExecutedTrades_Dates);
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_SecurityID), m_ExecutedTrades_SecurityID);
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_Shares), m_ExecutedTrades_Shares);
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_Price), m_ExecutedTrades_Price);
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_Commission), m_ExecutedTrades_Commission);
        queries::executeTableUpdate(queries::table_ExecutedTrades, tableValues);
    }
}

void nav::clearVariantLists()
{
    m_NAV_Portfolio.clear();
    m_NAV_Dates.clear();
    m_NAV_Totalvalue.clear();
    m_NAV_Nav.clear();
    m_ExecutedTrades_SecurityID.clear();
    m_ExecutedTrades_Dates.clear();
    m_ExecutedTrades_Shares.clear();
    m_ExecutedTrades_Price.clear();
    m_ExecutedTrades_Commission.clear();
}

void nav::deleteOldValues(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate)
{
    if (calculateFromStartDate)
    {
        portfolios.removeExecutedTrades(portfolioID);
        portfolios.removeNAV(portfolioID);
        return;
    }

    portfolios.removeExecutedTrades(portfolioID, calculationDate);
    portfolios.removeNAV(portfolioID, calculationDate);
}

QList<int> nav::getPortfolioSecurityReinvestment(const int &portfolioID)
{
    QList<int> securities;
    foreach(const security &s, portfolios.securities(portfolioID))
        if (s.includeInCalc && s.divReinvest && !s.cashAccount)
            securities.append(s.id);

    return securities;
}

int nav::checkCalculationDate(const int &portfolioID, int calculationDate, bool *calcuateFromStartDate)
{
    int lastNavDate = portfolios.nav(portfolioID).isEmpty() ? -1 : portfolios.nav(portfolioID).lastDate();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    portfolioInfo info = portfolios.info(portfolioID);
    if (calculationDate <= info.startDate)
    {
        // portfolio will recalculate from its startdate
        calculationDate = info.startDate;
        if (calcuateFromStartDate)
            *calcuateFromStartDate = true;
    }

    return calculationDate;
}


bool nav::getCurrentDateOrNext(int *date) const
{
    QList<int>::const_iterator place = qLowerBound(m_dates, *date);
    if (place == m_dates.constEnd())
        return false;

    if (date)
        *date = *place;
    return true;
}

QMap<int, nav::navTradeList> nav::calculateExecutedTrades(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate)
{
    QMap<int, navTradeList> trades;
    int lastDate = m_dates.last();

    foreach(const security &s, portfolios.securities(portfolioID))
        if (s.includeInCalc)
            for(navTradePointer singleTrade = s.trades.constBegin(); singleTrade != s.trades.constEnd(); ++singleTrade)
            {
                int startDate = calculationDate;
                if (singleTrade->startDate != 0)
                    startDate = qMax(singleTrade->startDate, calculationDate);

                int endDate = lastDate;
                if (singleTrade->endDate != 0)
                    endDate = qMin(singleTrade->endDate, lastDate);

                QList<int> dates;
                switch(singleTrade->frequency)
                {
                    case trade::tradeFreq_Once:
                        // this takes minDate instead of start date since these are not calculated on the fly and trades before the start date need to be inserted
                        dates = computeOnceTrades(singleTrade.value(), calculationDate, endDate, calculateFromStartDate);
                        break;
                    case trade::tradeFreq_Daily:
                        // -1 applies to every trading day
                        if (startDate == calculationDate && endDate == lastDate)
                            dates.append(-1);
                        else
                            dates = computeDailyTrade(startDate, endDate);
                        break;
                    case trade::tradeFreq_Weekly:
                        dates = computeWeeklyTrades(singleTrade->date, startDate, endDate);
                        break;
                    case trade::tradeFreq_Monthly:
                        dates = computeMonthlyTrades(singleTrade->date, startDate, endDate);
                        break;
                    case trade::tradeFreq_Yearly:
                        dates = computeYearlyTrades(singleTrade->date, startDate, endDate);
                        break;
                    default:
                        break;
                }
                foreach(const int &i, dates)
                    trades[i][s.id].append(singleTrade);
            }

    return trades;
}

QList<int> nav::computeOnceTrades(const trade &singleTade, const int &minDate, const int &maxDate, const bool &calculateFromStartDate) const
{
    QList<int> dates;
    int date = singleTade.date;

    if (date < singleTade.startDate || date > maxDate)
        return dates;

    if (date < minDate && calculateFromStartDate)
        dates.append(date);
    else if (date >= minDate && getCurrentDateOrNext(&date))
        dates.append(date);

    return dates;
}

QList<int> nav::computeDailyTrade(const int &minDate, const int &maxDate) const
{
    QList<int> dates;

    for(QList<int>::const_iterator i = qLowerBound(m_dates, minDate); i != m_dates.constEnd(); ++i)
    {
        if (*i > maxDate)
            break;

        dates.append(*i);
    }

    return dates;
}

QList<int> nav::computeWeeklyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const
{
    QList<int> dates;

    int firstWeekDay = minDate;
    while (firstWeekDay % 7 != tradeDate % 7)
        ++firstWeekDay;

    do
    {
        int date = firstWeekDay;
        if (getCurrentDateOrNext(&date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        firstWeekDay += 7;
    }
    while (firstWeekDay <= maxDate);

    return dates;
}

QList<int> nav::computeMonthlyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfMonth = QDate::fromJulianDay(tradeDate).day();

    do
    {
        QDate monthday = minQDate;
        if (monthday.day() > dayOfMonth)
            monthday = monthday.addMonths(1);

        if (dayOfMonth > monthday.daysInMonth())
        {
            monthday = monthday.addMonths(1);
            monthday = QDate(monthday.year(), monthday.month(), 1);
        }
        else
            monthday = QDate(monthday.year(), monthday.month(), dayOfMonth);

        int date = monthday.toJulianDay();
        if (getCurrentDateOrNext(&date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addMonths(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

QList<int> nav::computeYearlyTrades(const int &tradeDate, const int &minDate, const int &maxDate) const
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfYear = QDate::fromJulianDay(tradeDate).dayOfYear();

    do
    {
        QDate yearday = minQDate;
        int leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(yearday.year()) ? 1 : 0);

        if (yearday.dayOfYear() > leapDayofYear)
        {
            yearday = yearday.addYears(1);
            leapDayofYear = dayOfYear + (dayOfYear > 59 /* Feb 28th */ && QDate::isLeapYear(yearday.year()) ? 1 : 0);
        }

        int date = QDate(yearday.year(), 1, 1).toJulianDay() + leapDayofYear - 1;
        if (getCurrentDateOrNext(&date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addYears(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

void nav::insertPortfolioReinvestments(const int &portfolioID, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio *previousInfo)
{
    if (!previousInfo)
        return;
    
    foreach(const int &securityID, securityReinvestments)
    {
        QString symbol = portfolios.securities(portfolioID, securityID).symbol;
        securityPrice s = prices::instance().dailyPriceInfo(symbol, previousInfo->date);
        if (s.dividend == 0 || s.close == 0)
            continue;

        double split = prices::instance().split(symbol, date);

        addToExecutedTradeList(portfolioID, securityID, date, (s.dividend * previousInfo->securitiesInfo.value(securityID).shares) / s.close, s.close / split, 0);
    }
}

void nav::insertPortfolioCashTrade(const int &portfolioID, const int &cashAccount, const dailyInfoPortfolio *previousInfo, const int &date, const double &tradeValue)
{
    if (!previousInfo || !portfolios.securities(portfolioID).contains(cashAccount))
        return;

    QString symbol = portfolios.securities(portfolioID, cashAccount).symbol;
    double close = prices::instance().price(symbol, previousInfo->date) / prices::instance().split(symbol, date);

    if (close == 0)
        return;

    addToExecutedTradeList(portfolioID, cashAccount, date, -1 * tradeValue / close, close, 0);
}

void nav::insertPortfolioTrades(const int &portfolioID, const int &date, const dailyInfoPortfolio *previousInfo, const navTradeList &trades)
{
    for(navTradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        int securityID = i.key();
        QString symbol = portfolios.securities(portfolioID, securityID).symbol;
        double close = 0;
        if (previousInfo)
            close = prices::instance().price(symbol, previousInfo->date) / prices::instance().split(symbol, date);

        foreach(const navTradePointer &singleTrade, i.value())
        {
            double price = singleTrade->type == trade::tradeType_Interest || singleTrade->type == trade::tradeType_InterestPercent ? 0 :
                                                singleTrade->price >= 0 ? singleTrade->price :
                                                close;

            double sharesToBuy = 0;

            switch(singleTrade->type)
            {
                case trade::tradeType_Purchase:
                    if (price != 0)
                        sharesToBuy = singleTrade->value;
                    break;
                case trade::tradeType_Sale:
                    if (price != 0)
                        sharesToBuy = singleTrade->value * -1;
                    break;
                case trade::tradeType_DivReinvest:
                    if (price != 0)
                        sharesToBuy = singleTrade->value;
                    break;
                case trade::tradeType_Interest:
                    sharesToBuy = singleTrade->value;
                    break;
                case trade::tradeType_FixedPurchase:
                case trade::tradeType_FixedSale:
                    if (price != 0)
                    {
                        sharesToBuy = singleTrade->value / price;
                        if (singleTrade->type == trade::tradeType_FixedSale)
                            sharesToBuy *= -1;
                    }
                    break;
                case trade::tradeType_Value:
                case trade::tradeType_InterestPercent:
                    if (previousInfo && price != 0)
                        sharesToBuy = (previousInfo->securitiesInfo.value(securityID).totalValue * (singleTrade->value / 100)) / price;
                    break;
                case trade::tradeType_TotalValue:
                    if (previousInfo && price != 0)
                        sharesToBuy = (previousInfo->totalValue * (singleTrade->value / 100)) / price;
                    break;
                case trade::tradeType_AA:
                    if (previousInfo && price != 0)
                    {
                        const QMap<int, double> aaList = portfolios.securities(portfolioID, securityID).aa;
                        for(QMap<int, double>::const_iterator x = aaList.constBegin(); x != aaList.constEnd(); ++x)
                        {
                            int aa = x.key();
                            if (portfolios.aa(portfolioID, aa).target <= 0)
                                continue;

                            sharesToBuy += ((previousInfo->totalValue * (portfolios.aa(portfolioID, aa).target * x.value() * singleTrade->value / 100)) -
                                previousInfo->securitiesInfo.value(securityID).totalValue) / price;
                        }
                    }
                    break;
                default:
                    break;
            }

            addToExecutedTradeList(portfolioID, securityID, date, sharesToBuy, price, singleTrade->commission);

            if (singleTrade->cashAccount != -1 && previousInfo && sharesToBuy != 0)
                insertPortfolioCashTrade(portfolioID, singleTrade->cashAccount, previousInfo, date, sharesToBuy * price);
        }
    }
}

void nav::insertFirstPortfolioTrades(const int &portfolioID, const int &startDate, const QMap<int, navTradeList> &allTrades)
{
    for(QMap<int, navTradeList>::const_iterator i = allTrades.begin(); i != allTrades.end(); ++i)
    {
        int date = i.key();

        if (date >= startDate)
            break;

        if (date == -1)
            continue;

        insertPortfolioTrades(portfolioID, date, 0, i.value());
    }
}

void nav::addToExecutedTradeList(const int &portfolioID, const int &securityID, const int &date, const double &shares, const double &price, const double &commission)
{
    if (shares == 0)
        return;

    m_ExecutedTrades_SecurityID.append(securityID);
    m_ExecutedTrades_Dates.append(date);
    m_ExecutedTrades_Shares.append(shares);
    m_ExecutedTrades_Price.append(price);
    m_ExecutedTrades_Commission.append(commission);

    portfolios.insertExecutedTrade(portfolioID, securityID, executedTrade(date, shares, price, commission));
}
