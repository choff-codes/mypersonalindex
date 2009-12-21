#include "nav.h"
#include "functions.h"
#include "calculations.h"

void nav::run()
{
    if (qLowerBound(m_dates, m_calculationDate) != m_dates.constEnd())
    {
        QList<int> portfolios;
        if (m_portfolioID == -1)
            portfolios.append(m_data.keys());
        else
            portfolios.append(m_portfolioID);

        foreach(int p, portfolios)
        {
            int calculationDate = m_calculationDate;
            bool calcuateFromStartDate = false;
            calculationDate = checkCalculationDate(p, calculationDate, calcuateFromStartDate);
            getPortfolioNAVValues(p, calculationDate, calcuateFromStartDate);
            clearVariantLists();
        }
    }

    emit calculationFinished();
    exec();
}

void nav::getPortfolioNAVValues(const int &portfolioID, const int &calculationDate, const bool &portfolioStartDate)
{
    globals::myPersonalIndex *currentPortfolio = m_data.value(portfolioID);
    m_calculations.setPortfolio(currentPortfolio);
    emit statusUpdate(QString("Calculating '%1'").arg(currentPortfolio->info.description));


    deleteOldValues(currentPortfolio, calculationDate, portfolioStartDate);
    if (m_dates.count() < 2)  // need at least 2 days of data
        return;

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate) - 1;
    if (*previousDate == m_dates.last())
        return;

    dynamicTrades trades = getPortfolioTrades(portfolioID, calculationDate, portfolioStartDate);
    if (portfolioStartDate)
    {
        insertFirstPortfolioTrades(currentPortfolio, calculationDate, trades);
        insertPortfolioTradesToObject(currentPortfolio);
    }

    QList<int> tickerReinvestments = getPortfolioTickerReinvestment(portfolioID);
    calculations::portfolioDailyInfo *previousInfo = m_calculations.portfolioValues(*previousDate);
    double previousTotalValue = previousInfo->totalValue;
    double previousNAV = currentPortfolio->info.startValue;
    QMap<int, globals::navInfo> &currentPortfolioNAV = currentPortfolio->data.nav;

    if (portfolioStartDate)
    {
        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(*previousDate);
        m_NAV_Totalvalue.append(previousTotalValue);
        m_NAV_Nav.append(previousNAV);
        currentPortfolioNAV.insert(*previousDate, globals::navInfo(previousNAV, previousTotalValue));
    }
    else
        previousNAV = currentPortfolio->data.nav.value(*previousDate).nav;

    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        int date = *currentDate;
        double newTotalValue = 0, dailyActivity = 0, newNAV = 0;

        insertPortfolioReinvestments(currentPortfolio, date, tickerReinvestments, previousInfo);
        insertPortfolioTrades(currentPortfolio, date, previousInfo, trades.value(date) + trades.value(-1));
        insertPortfolioTradesToObject(currentPortfolio);

        calculations::portfolioDailyInfo *info = m_calculations.portfolioValues(date);

        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(date);

        newTotalValue = info->totalValue;
        m_NAV_Totalvalue.append(newTotalValue);

        dailyActivity = info->costBasis - previousInfo->costBasis + info->commission - (currentPortfolio->info.dividends ? info->dividends : 0);

        if (dailyActivity < 0)
            newNAV = (newTotalValue - dailyActivity) / (previousTotalValue / previousNAV);
        else
            newNAV = newTotalValue / ((previousTotalValue + dailyActivity) / previousNAV);

        if (isnan(newNAV) || isinf(newNAV))
            m_NAV_Nav.append(previousNAV);
        else
        {
            m_NAV_Nav.append(newNAV);
            previousNAV = newNAV;
        }

        currentPortfolioNAV.insert(date, globals::navInfo(m_NAV_Nav.last().toDouble(), m_NAV_Totalvalue.last().toDouble()));
        previousTotalValue = newTotalValue;
        delete previousInfo;
        previousInfo = info;
    }

    delete previousInfo;
    insertVariantLists();
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
        m_sql->executeTableUpdate(queries::table_NAV, tableValues);
    }

    if (!m_Trades_Dates.isEmpty())
    {
        QMap<QString, QVariantList> tableValues;
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Date), m_Trades_Dates);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_TickerID), m_Trades_TickerID);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Shares), m_Trades_Shares);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Price), m_Trades_Price);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Commission), m_Trades_Commission);
        tableValues.insert(queries::tradesColumns.at(queries::tradesColumns_Code), m_Trades_Code);
        m_sql->executeTableUpdate(queries::table_Trades, tableValues);
    }
}

void nav::clearVariantLists()
{
    m_TradesPosition = 0;
    m_NAV_Portfolio.clear();
    m_NAV_Dates.clear();
    m_NAV_Totalvalue.clear();
    m_NAV_Nav.clear();
    m_Trades_TickerID.clear();
    m_Trades_Dates.clear();
    m_Trades_Shares.clear();
    m_Trades_Price.clear();
    m_Trades_Commission.clear();
    m_Trades_Code.clear();
}

void nav::deleteOldValues(globals::myPersonalIndex *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate)
{
    // remove nav prices that are to be recalculated
    m_sql->executeNonQuery(queries::deletePortfolioItems(queries::table_NAV, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate, false));
    // remove custom trades that are to be recalculated
    m_sql->executeNonQuery(queries::deletePortfolioItems(queries::table_Trades, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate, true));

    if (portfolioStartDate)
    {
        currentPortfolio->data.trades.clear();
        currentPortfolio->data.nav.clear();
        return;
    }

    for(globals::tradeList::iterator i = currentPortfolio->data.trades.begin(); i != currentPortfolio->data.trades.end(); ++i)
    {
        QList<globals::trade> &list = i.value();
        QList<globals::trade>::iterator trade = list.begin();
        while (trade != list.end())
            if (trade->date >= calculationDate)
                trade = list.erase(trade);
            else
                ++trade;
    }

    QMap<int, globals::navInfo> &currentPortfolionav = currentPortfolio->data.nav;
    QMap<int, globals::navInfo>::iterator i = portfolioStartDate ? currentPortfolionav.begin() : currentPortfolionav.lowerBound(calculationDate);
    while (i != currentPortfolionav.end())
        i = currentPortfolionav.erase(i);
}

QList<int> nav::getPortfolioTickerReinvestment(const int &portfolioID)
{
    QList<int> tickers;

    foreach(const globals::security &s, m_data.value(portfolioID)->data.tickers)
        if (s.includeInCalc && s.divReinvest && !s.cashAccount)
            tickers.append(s.id);

    return tickers;
}

int nav::checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate)
{
    globals::myPersonalIndex *currentPortfolio = m_data.value(portfolioID);

    int lastNavDate = currentPortfolio->data.nav.isEmpty() ? -1 :
        (--currentPortfolio->data.nav.constEnd()).key();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    int portfolioStartDate = currentPortfolio->info.origStartDate;
    if (calculationDate <= portfolioStartDate)
    {
        // portfolio will recalculate from its startdate
        calculationDate = portfolioStartDate;
        calcuateFromStartDate = true;
    }

    // if start date is not a market day, find the next day
    getCurrentDateOrNext(calculationDate);

    // if there is a day before, return successfully
    // otherwise, there needs to be 1 day before to pull previous day closing prices
    if (qFind(m_dates, calculationDate) - m_dates.constBegin() >= 2)
        return calculationDate;

    // recalculate portfolio from the start of the 2nd day of pricing
    calcuateFromStartDate = true;
    return m_dates.count() > 1 ? m_dates.at(1) : qMax(m_calculationDate, calculationDate);
}


bool nav::getCurrentDateOrNext(int &date)
{
    QList<int>::const_iterator place = qLowerBound(m_dates, date);
    if (place != m_dates.constEnd())
    {
        date = *place;
        return true;
    }

    return false;
}

nav::dynamicTrades nav::getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate)
{
    dynamicTrades trades;
    int lastDate = m_dates.last();

    foreach(const globals::security &s, m_data.value(portfolioID)->data.tickers)
        if (s.includeInCalc)
            foreach(const globals::dynamicTrade &d, s.trades)
            {
                int startDate = minDate;
                if (d.startDate != 0)
                    startDate = qMax(d.startDate, minDate);

                int endDate = lastDate;
                if (d.endDate != 0)
                    endDate = qMin(d.endDate, lastDate);

                QList<int> dates;
                switch(d.frequency)
                {
                    case globals::tradeFreq_Once:
                        // this takes minDate instead of start date since these are non-dynamic and trades before the start date need to be inserted
                        dates = getOnceTrades(d, minDate, endDate, portfolioStartDate);
                        break;
                    case globals::tradeFreq_Daily:
                        // -1 applies to every trading day
                        dates.append(-1);
                        break;
                    case globals::tradeFreq_Weekly:
                        dates = getWeeklyTrades(d, startDate, endDate);
                        break;
                    case globals::tradeFreq_Monthly:
                        dates = getMonthlyTrades(d, startDate, endDate);
                        break;
                    case globals::tradeFreq_Yearly:
                        dates = getYearlyTrades(d, startDate, endDate);
                        break;
                    default:
                        break;
                }
                foreach(const int &i, dates)
                    trades[i].append(globals::dynamicTradeInfo(s.ticker, s.id, d));
            }

    return trades;
}

QList<int> nav::getOnceTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate)
{
    QList<int> dates;
    int date = d.date;
    if (date >= d.startDate && date <= maxDate)
        if (date < minDate && portfolioStartDate)
            dates.append(date);
        else
            if (date >= minDate && getCurrentDateOrNext(date))
                dates.append(date);

    return dates;
}

QList<int> nav::getWeeklyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    int firstWeekDay = minDate;
    while (firstWeekDay % 7 != d.date % 7)
        firstWeekDay++;

    do
    {
        int date = firstWeekDay;
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        firstWeekDay = firstWeekDay + 7;
    }
    while (firstWeekDay <= maxDate);

    return dates;
}

QList<int> nav::getMonthlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfMonth = QDate::fromJulianDay(d.date).day();

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
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addMonths(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

QList<int> nav::getYearlyTrades(const globals::dynamicTrade &d, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfYear = QDate::fromJulianDay(d.date).dayOfYear();

    do
    {
        QDate yearday = minQDate;
        int leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);

        if (yearday.dayOfYear() > leapDayofYear)
        {
            yearday = yearday.addYears(1);
            leapDayofYear = dayOfYear + (QDate::isLeapYear(yearday.year()) ? 1 : 0);
        }

        int date = QDate(yearday.year(), 1, 1).toJulianDay() + leapDayofYear - 1;
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addYears(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

void nav::insertPortfolioReinvestments(const globals::myPersonalIndex *currentPortfolio, const int &date, const QList<int> &tickerReinvestments, const calculations::portfolioDailyInfo *previousInfo)
{
    if (!previousInfo)
        return;
    
    foreach(const int &reinvest, tickerReinvestments)
    {
        prices::securityPrice s = prices::dailyPriceInfo(currentPortfolio->data.tickers.value(reinvest).ticker, previousInfo->date);
        if (s.dividend <= 0 || s.close == 0)
            continue;
        m_Trades_TickerID.append(reinvest);
        m_Trades_Dates.append(date);
        m_Trades_Shares.append(s.dividend / s.close);
        m_Trades_Price.append(s.close / s.split);
        m_Trades_Commission.append(QVariant());
        m_Trades_Code.append("R");
    }
}

void nav::insertPortfolioCashTrade(const globals::myPersonalIndex *currentPortfolio, const int &cashAccount, const calculations::portfolioDailyInfo *previousInfo,
    const int &date, const double &reverseTradeValue)
{
    if (!previousInfo)
        return;

    const QMap<int, globals::security> &tickers = currentPortfolio->data.tickers;
    if (!tickers.contains(cashAccount))
        return;

    prices::securityPrice cashSecurity = prices::dailyPriceInfo(tickers.value(cashAccount).ticker, previousInfo->date);
    if (cashSecurity.close == 0)
        return;

    m_Trades_TickerID.append(cashAccount);
    m_Trades_Dates.append(date);
    m_Trades_Shares.append(reverseTradeValue / (cashSecurity.close / cashSecurity.split));
    m_Trades_Price.append(cashSecurity.close / cashSecurity.split);
    m_Trades_Commission.append(QVariant());
    m_Trades_Code.append("C");
}

void nav::insertPortfolioTrades(const globals::myPersonalIndex *currentPortfolio, const int &date, const calculations::portfolioDailyInfo *previousInfo, const dynamicTradeList &trades)
{
    foreach(const globals::dynamicTradeInfo &d, trades)
    {
        prices::securityPrice s = previousInfo ? prices::dailyPriceInfo(d.ticker, previousInfo->date) : prices::securityPrice();
        if (d.trade.price == -1 && s.close == 0)
            continue;

        m_Trades_TickerID.append(d.tickerID);
        m_Trades_Dates.append(date);
        m_Trades_Price.append(d.trade.tradeType == globals::tradeType_Interest ? 0 :
                              d.trade.price >= 0 ? d.trade.price :
                              s.close / s.split);
        m_Trades_Commission.append(d.trade.commission >= 0 ? d.trade.commission : QVariant());

        double sharesToBuy = 0;
        QString code;
        switch(d.trade.tradeType)
        {
            case globals::tradeType_Purchase:
                sharesToBuy = d.trade.value;
                code = "P";
                break;
            case globals::tradeType_Sale:
                sharesToBuy = d.trade.value * -1;
                code = "S";
                break;
            case globals::tradeType_DivReinvest:
                sharesToBuy = d.trade.value;
                code = "R";
                break;
            case globals::tradeType_Interest:
                sharesToBuy = d.trade.value;
                code = "I";
                break;
            case globals::tradeType_Fixed:
                if (s.close != 0)
                    sharesToBuy = d.trade.value / (s.close / s.split);
                code = "F";
                break;
            case globals::tradeType_TotalValue:
                if (previousInfo && s.close != 0)
                    sharesToBuy = (previousInfo->totalValue * (d.trade.value / 100)) / (s.close / s.split);
                code = "T";
                break;
            case globals::tradeType_AA:
                if (previousInfo && s.close != 0)
                    foreach(const globals::securityAATarget &aa, currentPortfolio->data.tickers.value(d.tickerID).aa)
                    {
                        if (currentPortfolio->data.aa.value(aa.id).target <= 0)
                            continue;

                        sharesToBuy += ((previousInfo->totalValue * (currentPortfolio->data.aa.value(aa.id).target * aa.target / 100)) -
                            previousInfo->tickerValue.value(d.tickerID).totalValue) / (s.close / s.split);
                    }
                code = "A";
                break;
            default:
                break;
        }
        m_Trades_Shares.append(sharesToBuy);
        m_Trades_Code.append(code);

        if (d.trade.cashAccount == - 1 || !previousInfo)
            continue;

        insertPortfolioCashTrade(currentPortfolio, d.trade.cashAccount, previousInfo, date, sharesToBuy * m_Trades_Price.last().toDouble());
    }
}

void nav::insertFirstPortfolioTrades(const globals::myPersonalIndex *currentPortfolio, const int &startDate, const dynamicTrades &allTrades)
{
    for(dynamicTrades::const_iterator i = allTrades.begin(); i != allTrades.end(); ++i)
    {
        int date = i.key();

        if (date >= startDate)
            break;

        if (date == -1)
            continue;

        insertPortfolioTrades(currentPortfolio, date, 0, i.value());
    } 
}

void nav::insertPortfolioTradesToObject(globals::myPersonalIndex *currentPortfolio)
{
    for(int i = m_TradesPosition; i < m_Trades_Dates.count(); ++i)
    {
        globals::trade t;
        t.date = m_Trades_Dates.at(i).toInt();
        t.price = m_Trades_Price.at(i).toDouble();
        t.shares = m_Trades_Shares.at(i).toDouble();
        t.commission = m_Trades_Commission.at(i).toDouble();
        currentPortfolio->data.trades[m_Trades_TickerID.at(i).toInt()].append(t);
    }
    m_TradesPosition = m_Trades_Dates.count();
}
