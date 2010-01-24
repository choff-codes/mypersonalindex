#include "nav.h"

void nav::run()
{
    QList<int> portfolios;
    if (m_portfolioID == -1)
        portfolios.append(m_data.keys());
    else
        portfolios.append(m_portfolioID);

    foreach(const int &p, portfolios)
    {
        portfolio *currentPortfolio = m_data.value(p);
        m_calculations.setPortfolio(currentPortfolio);
        emit statusUpdate(QString("Calculating '%1'").arg(currentPortfolio->info.description));
        calculateNAVValues(currentPortfolio);
    }

    emit calculationFinished();
    exec();
}

void nav::calculateNAVValues(portfolio *currentPortfolio)
{
    bool portfolioStartDate = false;
    int calculationDate = checkCalculationDate(currentPortfolio, m_calculationDate, &portfolioStartDate);

    deleteOldValues(currentPortfolio, calculationDate, portfolioStartDate);

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate) - 1;
    if (m_dates.isEmpty() || previousDate == m_dates.constEnd() || *previousDate == m_dates.last())
        return;

    navTrades trades = calculateExecutedTrades(currentPortfolio, calculationDate, portfolioStartDate);
    if (portfolioStartDate)
        insertFirstPortfolioTrades(currentPortfolio, calculationDate, trades);

    QList<int> securityReinvestments = getPortfolioSecurityReinvestment(currentPortfolio);
    dailyInfoPortfolio *previousInfo = m_calculations.portfolioValues(*previousDate);
    double previousNAV = portfolioStartDate ? currentPortfolio->info.startValue : currentPortfolio->data.nav.nav(*previousDate);

    if (portfolioStartDate)
        addToNAVList(currentPortfolio, *previousDate, previousInfo->totalValue, previousNAV);

    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        insertPortfolioReinvestments(currentPortfolio, *currentDate, securityReinvestments, previousInfo);
        insertPortfolioTrades(currentPortfolio, *currentDate, previousInfo, trades.value(*currentDate) + trades.value(-1));

        dailyInfoPortfolio *info = m_calculations.portfolioValues(*currentDate);
        double dailyActivity = info->costBasis - previousInfo->costBasis + info->commission;
        double newNAV = calculations::change(info->totalValue, previousInfo->totalValue, dailyActivity, currentPortfolio->info.dividends ? info->dividends : 0, previousNAV);

        addToNAVList(currentPortfolio, *currentDate, info->totalValue, newNAV);

        previousNAV = newNAV;
        delete previousInfo;
        previousInfo = info;
    }

    delete previousInfo;
    insertVariantLists();
    clearVariantLists();
}

void nav::addToNAVList(portfolio *currentPortfolio, const int &date, const double &totalValue, const double &nav)
{
    m_NAV_Portfolio.append(currentPortfolio->info.id);
    m_NAV_Dates.append(date);
    m_NAV_Totalvalue.append(totalValue);
    m_NAV_Nav.append(nav);
    currentPortfolio->data.nav.insert(date, nav, totalValue);
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
    if (portfolioStartDate)
    {
        currentPortfolio->data.executedTrades.remove(currentPortfolio->info.id);
        currentPortfolio->data.nav.remove(currentPortfolio->info.id);
        return;
    }

    currentPortfolio->data.executedTrades.remove(currentPortfolio->info.id, calculationDate);
    currentPortfolio->data.nav.remove(currentPortfolio->info.id, calculationDate);
}

QList<int> nav::getPortfolioSecurityReinvestment(const portfolio *currentPortfolio)
{
    QList<int> securities;
    foreach(const security &s, currentPortfolio->data.securities)
        if (s.includeInCalc && s.divReinvest && !s.cashAccount)
            securities.append(s.id);

    return securities;
}

int nav::checkCalculationDate(const portfolio *currentPortfolio, int calculationDate, bool *calcuateFromStartDate)
{
    int lastNavDate = currentPortfolio->data.nav.isEmpty() ? -1 : currentPortfolio->data.nav.lastDate();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    int portfolioStartDate = currentPortfolio->info.startDate;
    if (calculationDate <= portfolioStartDate)
    {
        // portfolio will recalculate from its startdate
        calculationDate = portfolioStartDate;
        *calcuateFromStartDate = true;
    }

    // if start date is not a market day, find the next day
    getCurrentDateOrNext(calculationDate);

    // if there is a day before, return successfully
    // otherwise, there needs to be 1 day before to pull previous day closing prices
    if (qBinaryFind(m_dates, calculationDate) - m_dates.constBegin() >= 2)
        return calculationDate;

    // recalculate portfolio from the start of the 2nd day of pricing
    *calcuateFromStartDate = true;
    return m_dates.count() > 1 ? m_dates.at(1) : qMax(m_calculationDate, calculationDate);
}


bool nav::getCurrentDateOrNext(int &date)
{
    QList<int>::const_iterator place = qLowerBound(m_dates, date);
    if (place == m_dates.constEnd())
        return false;

    date = *place;
    return true;
}

navTrades nav::calculateExecutedTrades(const portfolio *currentPortfolio, const int &minDate, const bool &portfolioStartDate)
{
    navTrades trades;
    int lastDate = m_dates.last();

    foreach(const security &s, currentPortfolio->data.securities)
        if (s.includeInCalc)
            for(QMap<int, trade>::const_iterator singleTrade = s.trades.constBegin(); singleTrade != s.trades.constEnd(); ++singleTrade)
            {
                int startDate = minDate;
                if (singleTrade->startDate != 0)
                    startDate = qMax(singleTrade->startDate, minDate);

                int endDate = lastDate;
                if (singleTrade->endDate != 0)
                    endDate = qMin(singleTrade->endDate, lastDate);

                QList<int> dates;
                switch(singleTrade->frequency)
                {
                    case trade::tradeFreq_Once:
                        // this takes minDate instead of start date since these are not calculated on the fly and trades before the start date need to be inserted
                        dates = computeOnceTrades(singleTrade.value(), minDate, endDate, portfolioStartDate);
                        break;
                    case trade::tradeFreq_Daily:
                        // -1 applies to every trading day
                        dates.append(-1);
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
                    trades[i].append(navTrade(s.symbol, s.id, singleTrade));
            }

    return trades;
}

QList<int> nav::computeOnceTrades(const trade &singleTade, const int &minDate, const int &maxDate, const bool &portfolioStartDate)
{
    QList<int> dates;
    int date = singleTade.date;

    if (date < singleTade.startDate || date > maxDate)
        return dates;

    if (date < minDate && portfolioStartDate)
        dates.append(date);
    else
        if (date >= minDate && getCurrentDateOrNext(date))
            dates.append(date);

    return dates;
}

QList<int> nav::computeWeeklyTrades(const int &tradeDate, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    int firstWeekDay = minDate;
    while (firstWeekDay % 7 != tradeDate % 7)
        ++firstWeekDay;

    do
    {
        int date = firstWeekDay;
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);
        firstWeekDay += 7;
    }
    while (firstWeekDay <= maxDate);

    return dates;
}

QList<int> nav::computeMonthlyTrades(const int &tradeDate, const int &minDate, const int &maxDate)
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
        if (getCurrentDateOrNext(date))
            if (date >= minDate && date <= maxDate)
                dates.append(date);

        minQDate = minQDate.addMonths(1);
    }
    while (minQDate.toJulianDay() <= maxDate);

    return dates;
}

QList<int> nav::computeYearlyTrades(const int &tradeDate, const int &minDate, const int &maxDate)
{
    QList<int> dates;

    QDate minQDate = QDate::fromJulianDay(minDate);
    int dayOfYear = QDate::fromJulianDay(tradeDate).dayOfYear();

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

void nav::insertPortfolioReinvestments(portfolio *currentPortfolio, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio *previousInfo)
{
    if (!previousInfo)
        return;
    
    foreach(const int &securityID, securityReinvestments)
    {
        securityPrice s = prices::instance().dailyPriceInfo(currentPortfolio->data.securities.value(securityID).symbol, previousInfo->date);
        if (s.dividend <= 0 || s.close == 0)
            continue;

        addToExecutedTradeList(currentPortfolio, securityID, date, s.dividend / s.close, s.close / s.split, -1, "R");
    }
}

void nav::insertPortfolioCashTrade(portfolio *currentPortfolio, const int &cashAccount, const dailyInfoPortfolio *previousInfo, const int &date, const double &tradeValue)
{
    if (!previousInfo || !currentPortfolio->data.securities.contains(cashAccount))
        return;

    securityPrice cashSecurity = prices::instance().dailyPriceInfo(currentPortfolio->data.securities.value(cashAccount).symbol, previousInfo->date);
    if (cashSecurity.close == 0)
        return;

    addToExecutedTradeList(currentPortfolio, cashAccount, date, -1 * tradeValue / (cashSecurity.close / cashSecurity.split),
        cashSecurity.close / cashSecurity.split, -1, "C");
}

void nav::insertPortfolioTrades(portfolio *currentPortfolio, const int &date, const dailyInfoPortfolio *previousInfo, const navTradeList &trades)
{
    foreach(const navTrade &t, trades)
    {
        securityPrice s = previousInfo ? prices::instance().dailyPriceInfo(t.symbol, previousInfo->date) : securityPrice();
        if (t.singleTrade->price == -1 && s.close == 0)
            continue;

        double sharesToBuy = 0;
        QString code;
        switch(t.singleTrade->type)
        {
            case trade::tradeType_Purchase:
                sharesToBuy = t.singleTrade->value;
                code = "P";
                break;
            case trade::tradeType_Sale:
                sharesToBuy = t.singleTrade->value * -1;
                code = "S";
                break;
            case trade::tradeType_DivReinvest:
                sharesToBuy = t.singleTrade->value;
                code = "R";
                break;
            case trade::tradeType_Interest:
                sharesToBuy = t.singleTrade->value;
                code = "I";
                break;
            case trade::tradeType_Fixed:
                if (s.close != 0)
                    sharesToBuy = t.singleTrade->value / (s.close / s.split);
                code = "F";
                break;
            case trade::tradeType_TotalValue:
                if (previousInfo && s.close != 0)
                    sharesToBuy = (previousInfo->totalValue * (t.singleTrade->value / 100)) / (s.close / s.split);
                code = "T";
                break;
            case trade::tradeType_AA:
                if (previousInfo && s.close != 0)
                    foreach(const assetAllocationTarget &aa, currentPortfolio->data.securities.value(t.securityID).aa)
                    {
                        if (currentPortfolio->data.aa.value(aa.id).target <= 0)
                            continue;

                        sharesToBuy += ((previousInfo->totalValue * (currentPortfolio->data.aa.value(aa.id).target * aa.target / 100)) -
                            previousInfo->securitiesInfo.value(t.securityID).totalValue) / (s.close / s.split);
                    }
                code = "A";
                break;
            default:
                break;
        }

        double price = t.singleTrade->type == trade::tradeType_Interest ? 0 : t.singleTrade->price >= 0 ? t.singleTrade->price : s.close / s.split;

        addToExecutedTradeList(currentPortfolio, t.securityID, date, sharesToBuy, price, t.singleTrade->commission, code);

        if (t.singleTrade->cashAccount == - 1 || !previousInfo)
            continue;

        insertPortfolioCashTrade(currentPortfolio, t.singleTrade->cashAccount, previousInfo, date, sharesToBuy * price);
    }
}

void nav::insertFirstPortfolioTrades(portfolio *currentPortfolio, const int &startDate, const navTrades &allTrades)
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

void nav::addToExecutedTradeList(portfolio *currentPortfolio, const int &securityID, const int &date, const double &shares, const double &price, const double &commission, const QString &code)
{
    m_ExecutedTrades_SecurityID.append(securityID);
    m_ExecutedTrades_Dates.append(date);
    m_ExecutedTrades_Shares.append(shares);
    m_ExecutedTrades_Price.append(price);
    m_ExecutedTrades_Commission.append(commission < 0 ? QVariant() : commission);
    m_ExecutedTrades_Code.append(code);

    executedTrade t;
    t.date = date;
    t.price = price;
    t.shares = shares;
    t.commission = commission < 0 ? 0 : commission;
    currentPortfolio->data.executedTrades[securityID].append(t);
}
