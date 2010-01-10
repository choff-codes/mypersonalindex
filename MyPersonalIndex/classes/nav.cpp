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
    portfolio *currentPortfolio = m_data.value(portfolioID);
    m_calculations.setPortfolio(currentPortfolio);
    emit statusUpdate(QString("Calculating '%1'").arg(currentPortfolio->info.description));


    deleteOldValues(currentPortfolio, calculationDate, portfolioStartDate);
    if (m_dates.count() < 2)  // need at least 2 days of data
        return;

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate) - 1;
    if (*previousDate == m_dates.last())
        return;

    navTrades trades = getPortfolioTrades(portfolioID, calculationDate, portfolioStartDate);
    if (portfolioStartDate)
    {
        insertFirstPortfolioTrades(currentPortfolio, calculationDate, trades);
        insertPortfolioTradesToObject(currentPortfolio);
    }

    QList<int> securityReinvestments = getPortfolioSecurityReinvestment(portfolioID);
    calculations::portfolioDailyInfo *previousInfo = m_calculations.portfolioValues(*previousDate);
    double previousTotalValue = previousInfo->totalValue;
    double previousNAV = currentPortfolio->info.startValue;
    navInfo::navInfo &currentPortfolioNAV = currentPortfolio->data.nav;
    bool dividends = currentPortfolio->info.dividends;

    if (portfolioStartDate)
    {
        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(*previousDate);
        m_NAV_Totalvalue.append(previousTotalValue);
        m_NAV_Nav.append(previousNAV);
        currentPortfolioNAV.insert(*previousDate, previousNAV, previousTotalValue);
    }
    else
        previousNAV = currentPortfolioNAV.nav(*previousDate);

    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        int date = *currentDate;
        double newTotalValue = 0, dailyActivity = 0, newNAV = 0;

        insertPortfolioReinvestments(currentPortfolio, date, securityReinvestments, previousInfo);
        insertPortfolioTrades(currentPortfolio, date, previousInfo, trades.value(date) + trades.value(-1));
        insertPortfolioTradesToObject(currentPortfolio);

        calculations::portfolioDailyInfo *info = m_calculations.portfolioValues(date);

        m_NAV_Portfolio.append(portfolioID);
        m_NAV_Dates.append(date);

        newTotalValue = info->totalValue;
        m_NAV_Totalvalue.append(newTotalValue);

        dailyActivity = info->costBasis - previousInfo->costBasis + info->commission;
        newNAV = calculations::change(newTotalValue, previousTotalValue, dailyActivity, dividends ? info->dividends : 0, previousNAV);
        m_NAV_Nav.append(newNAV);
        currentPortfolioNAV.insert(date, newNAV, newTotalValue);

        previousNAV = newNAV;
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
        tableValues.insert(queries::executedTradesColumns.at(queries::executedTradesColumns_Code), m_ExecutedTrades_Code);
        queries::executeTableUpdate(queries::table_ExecutedTrades, tableValues);
    }
}

void nav::clearVariantLists()
{
    m_TradesPosition = 0;
    m_NAV_Portfolio.clear();
    m_NAV_Dates.clear();
    m_NAV_Totalvalue.clear();
    m_NAV_Nav.clear();
    m_ExecutedTrades_SecurityID.clear();
    m_ExecutedTrades_Dates.clear();
    m_ExecutedTrades_Shares.clear();
    m_ExecutedTrades_Price.clear();
    m_ExecutedTrades_Commission.clear();
    m_ExecutedTrades_Code.clear();
}

void nav::deleteOldValues(portfolio *currentPortfolio, const int &calculationDate, const bool &portfolioStartDate)
{
    // remove nav prices that are to be recalculated
    queries::deletePortfolioItems(queries::table_NAV, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate);
    // remove custom trades that are to be recalculated
    queries::deletePortfolioItems(queries::table_ExecutedTrades, currentPortfolio->info.id, portfolioStartDate ? 0 : calculationDate, true);

    if (portfolioStartDate)
    {
        currentPortfolio->data.executedTrades.clear();
        currentPortfolio->data.nav.clear();
        return;
    }

    for(portfolioData::executedTradeList::iterator i = currentPortfolio->data.executedTrades.begin(); i != currentPortfolio->data.executedTrades.end(); ++i)
    {
        QList<executedTrade> &list = i.value();
        QList<executedTrade>::iterator trade = list.begin();
        while (trade != list.end())
            if (trade->date >= calculationDate)
                trade = list.erase(trade);
            else
                ++trade;
    }

    currentPortfolio->data.nav.clear(calculationDate);
}

QList<int> nav::getPortfolioSecurityReinvestment(const int &portfolioID)
{
    QList<int> securities;

    foreach(const security &s, m_data.value(portfolioID)->data.securities)
        if (s.includeInCalc && s.divReinvest && !s.cashAccount)
            securities.append(s.id);

    return securities;
}

int nav::checkCalculationDate(const int &portfolioID, int calculationDate, bool &calcuateFromStartDate)
{
    portfolio *currentPortfolio = m_data.value(portfolioID);

    int lastNavDate = currentPortfolio->data.nav.isEmpty() ? -1 :
        currentPortfolio->data.nav.lastDate();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    int portfolioStartDate = currentPortfolio->info.startDate;
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
    if (qBinaryFind(m_dates, calculationDate) - m_dates.constBegin() >= 2)
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

nav::navTrades nav::getPortfolioTrades(const int &portfolioID, const int &minDate, const bool &portfolioStartDate)
{
    navTrades trades;
    int lastDate = m_dates.last();

    foreach(const security &s, m_data.value(portfolioID)->data.securities)
        if (s.includeInCalc)
            foreach(const trade &d, s.trades)
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
                    case trade::tradeFreq_Once:
                        // this takes minDate instead of start date since these are non-dynamic and trades before the start date need to be inserted
                        dates = getOnceTrades(d, minDate, endDate, portfolioStartDate);
                        break;
                    case trade::tradeFreq_Daily:
                        // -1 applies to every trading day
                        dates.append(-1);
                        break;
                    case trade::tradeFreq_Weekly:
                        dates = getWeeklyTrades(d, startDate, endDate);
                        break;
                    case trade::tradeFreq_Monthly:
                        dates = getMonthlyTrades(d, startDate, endDate);
                        break;
                    case trade::tradeFreq_Yearly:
                        dates = getYearlyTrades(d, startDate, endDate);
                        break;
                    default:
                        break;
                }
                foreach(const int &i, dates)
                    trades[i].append(navTrade(s.symbol, s.id, d));
            }

    return trades;
}

QList<int> nav::getOnceTrades(const trade &d, const int &minDate, const int &maxDate, const bool &portfolioStartDate)
{
    QList<int> dates;
    int date = d.date;

    if (date < d.startDate || date > maxDate)
        return dates;

    if (date < minDate && portfolioStartDate)
        dates.append(date);
    else
        if (date >= minDate && getCurrentDateOrNext(date))
            dates.append(date);

    return dates;
}

QList<int> nav::getWeeklyTrades(const trade &d, const int &minDate, const int &maxDate)
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

QList<int> nav::getMonthlyTrades(const trade &d, const int &minDate, const int &maxDate)
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

QList<int> nav::getYearlyTrades(const trade &d, const int &minDate, const int &maxDate)
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

void nav::insertPortfolioReinvestments(const portfolio *currentPortfolio, const int &date, const QList<int> &securityReinvestments, const calculations::portfolioDailyInfo *previousInfo)
{
    if (!previousInfo)
        return;
    
    foreach(const int &reinvest, securityReinvestments)
    {
        prices::securityPrice s = prices::instance().dailyPriceInfo(currentPortfolio->data.securities.value(reinvest).symbol, previousInfo->date);
        if (s.dividend <= 0 || s.close == 0)
            continue;
        m_ExecutedTrades_SecurityID.append(reinvest);
        m_ExecutedTrades_Dates.append(date);
        m_ExecutedTrades_Shares.append(s.dividend / s.close);
        m_ExecutedTrades_Price.append(s.close / s.split);
        m_ExecutedTrades_Commission.append(QVariant());
        m_ExecutedTrades_Code.append("R");
    }
}

void nav::insertPortfolioCashTrade(const portfolio *currentPortfolio, const int &cashAccount, const calculations::portfolioDailyInfo *previousInfo,
    const int &date, const double &reverseTradeValue)
{
    if (!previousInfo)
        return;

    const QMap<int, security> &securities = currentPortfolio->data.securities;
    if (!securities.contains(cashAccount))
        return;

    prices::securityPrice cashSecurity = prices::instance().dailyPriceInfo(securities.value(cashAccount).symbol, previousInfo->date);
    if (cashSecurity.close == 0)
        return;

    m_ExecutedTrades_SecurityID.append(cashAccount);
    m_ExecutedTrades_Dates.append(date);
    m_ExecutedTrades_Shares.append(reverseTradeValue / (cashSecurity.close / cashSecurity.split));
    m_ExecutedTrades_Price.append(cashSecurity.close / cashSecurity.split);
    m_ExecutedTrades_Commission.append(QVariant());
    m_ExecutedTrades_Code.append("C");
}

void nav::insertPortfolioTrades(const portfolio *currentPortfolio, const int &date, const calculations::portfolioDailyInfo *previousInfo, const navTradeList &trades)
{
    foreach(const navTrade &d, trades)
    {
        prices::securityPrice s = previousInfo ? prices::instance().dailyPriceInfo(d.symbol, previousInfo->date) : prices::securityPrice();
        if (d.singleTrade.price == -1 && s.close == 0)
            continue;

        m_ExecutedTrades_SecurityID.append(d.securityID);
        m_ExecutedTrades_Dates.append(date);
        m_ExecutedTrades_Price.append(d.singleTrade.type == trade::tradeType_Interest ? 0 :
                              d.singleTrade.price >= 0 ? d.singleTrade.price :
                              s.close / s.split);
        m_ExecutedTrades_Commission.append(d.singleTrade.commission >= 0 ? d.singleTrade.commission : QVariant());

        double sharesToBuy = 0;
        QString code;
        switch(d.singleTrade.type)
        {
            case trade::tradeType_Purchase:
                sharesToBuy = d.singleTrade.value;
                code = "P";
                break;
            case trade::tradeType_Sale:
                sharesToBuy = d.singleTrade.value * -1;
                code = "S";
                break;
            case trade::tradeType_DivReinvest:
                sharesToBuy = d.singleTrade.value;
                code = "R";
                break;
            case trade::tradeType_Interest:
                sharesToBuy = d.singleTrade.value;
                code = "I";
                break;
            case trade::tradeType_Fixed:
                if (s.close != 0)
                    sharesToBuy = d.singleTrade.value / (s.close / s.split);
                code = "F";
                break;
            case trade::tradeType_TotalValue:
                if (previousInfo && s.close != 0)
                    sharesToBuy = (previousInfo->totalValue * (d.singleTrade.value / 100)) / (s.close / s.split);
                code = "T";
                break;
            case trade::tradeType_AA:
                if (previousInfo && s.close != 0)
                    foreach(const aaTarget &aa, currentPortfolio->data.securities.value(d.securityID).aa)
                    {
                        if (currentPortfolio->data.aa.value(aa.id).target <= 0)
                            continue;

                        sharesToBuy += ((previousInfo->totalValue * (currentPortfolio->data.aa.value(aa.id).target * aa.target / 100)) -
                            previousInfo->securityValues.value(d.securityID).totalValue) / (s.close / s.split);
                    }
                code = "A";
                break;
            default:
                break;
        }
        m_ExecutedTrades_Shares.append(sharesToBuy);
        m_ExecutedTrades_Code.append(code);

        if (d.singleTrade.cashAccount == - 1 || !previousInfo)
            continue;

        insertPortfolioCashTrade(currentPortfolio, d.singleTrade.cashAccount, previousInfo, date, sharesToBuy * m_ExecutedTrades_Price.last().toDouble());
    }
}

void nav::insertFirstPortfolioTrades(const portfolio *currentPortfolio, const int &startDate, const navTrades &allTrades)
{
    for(navTrades::const_iterator i = allTrades.begin(); i != allTrades.end(); ++i)
    {
        int date = i.key();

        if (date >= startDate)
            break;

        if (date == -1)
            continue;

        insertPortfolioTrades(currentPortfolio, date, 0, i.value());
    } 
}

void nav::insertPortfolioTradesToObject(portfolio *currentPortfolio)
{
    for(int i = m_TradesPosition; i < m_ExecutedTrades_Dates.count(); ++i)
    {
        executedTrade t;
        t.date = m_ExecutedTrades_Dates.at(i).toInt();
        t.price = m_ExecutedTrades_Price.at(i).toDouble();
        t.shares = m_ExecutedTrades_Shares.at(i).toDouble();
        t.commission = m_ExecutedTrades_Commission.at(i).toDouble();
        currentPortfolio->data.executedTrades[m_ExecutedTrades_SecurityID.at(i).toInt()].append(t);
    }
    m_TradesPosition = m_ExecutedTrades_Dates.count();
}
