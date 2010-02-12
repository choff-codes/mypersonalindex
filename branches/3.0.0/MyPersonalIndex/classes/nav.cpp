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
    bool calculateFromStartDate = false;
    int calculationDate = checkCalculationDate(currentPortfolio, m_calculationDate, &calculateFromStartDate);

    deleteOldValues(currentPortfolio, calculationDate, calculateFromStartDate);

    QList<int>::const_iterator previousDate = qLowerBound(m_dates, calculationDate - 1);
    if (*previousDate != calculationDate - 1 && previousDate != m_dates.constBegin())
        --previousDate;

    if (m_dates.constEnd() - previousDate < 2)
        return;

    QMap<int, navTradeList> trades = calculateExecutedTrades(currentPortfolio, calculationDate, calculateFromStartDate);
    if (calculateFromStartDate)
        insertFirstPortfolioTrades(currentPortfolio, calculationDate, trades);

    QList<int> securityReinvestments = getPortfolioSecurityReinvestment(currentPortfolio);
    dailyInfoPortfolio *previousInfo = m_calculations.portfolioValues(*previousDate);
    double navValue = calculateFromStartDate ? currentPortfolio->info.startValue : currentPortfolio->data.nav.nav(*previousDate);

    if (calculateFromStartDate)
        addToNAVList(currentPortfolio, *previousDate, previousInfo->totalValue, navValue);

    for (QList<int>::const_iterator currentDate = previousDate + 1; currentDate != m_dates.constEnd(); ++currentDate)
    {
        int date = *currentDate;
        insertPortfolioReinvestments(currentPortfolio, date, securityReinvestments, previousInfo);
        insertPortfolioTrades(currentPortfolio, date, previousInfo, appendNavTrades(trades.value(date), trades.value(-1)));

        dailyInfoPortfolio *info = m_calculations.portfolioValues(date);
        navValue = calculations::change(info->totalValue, previousInfo->totalValue, info->costBasis - previousInfo->costBasis, currentPortfolio->info.dividends ? info->dividends : 0, navValue);

        addToNAVList(currentPortfolio, date, info->totalValue, navValue);

        delete previousInfo;
        previousInfo = info;
    }

    delete previousInfo;
    insertVariantLists();
    clearVariantLists();
}

nav::navTradeList nav::appendNavTrades(const navTradeList &first, const navTradeList &second)
{
    navTradeList returnList = first;
    for(navTradeList::const_iterator i = second.constBegin(); i != second.constEnd(); ++i)
        returnList[i.key()].append(i.value());
    return returnList;
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

void nav::deleteOldValues(portfolio *currentPortfolio, const int &calculationDate, const bool &calculateFromStartDate)
{
    if (calculateFromStartDate)
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

    if (calculationDate <= currentPortfolio->info.startDate)
    {
        // portfolio will recalculate from its startdate
        calculationDate = currentPortfolio->info.startDate;
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

QMap<int, nav::navTradeList> nav::calculateExecutedTrades(const portfolio *currentPortfolio, const int &calculationDate, const bool &calculateFromStartDate)
{
    QMap<int, navTradeList> trades;
    int lastDate = m_dates.last();

    foreach(const security &s, currentPortfolio->data.securities)
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

void nav::insertPortfolioReinvestments(portfolio *currentPortfolio, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio *previousInfo)
{
    if (!previousInfo)
        return;
    
    foreach(const int &securityID, securityReinvestments)
    {
        QString symbol = currentPortfolio->data.securities.value(securityID).symbol;
        securityPrice s = prices::instance().dailyPriceInfo(symbol, previousInfo->date);
        if (s.dividend == 0 || s.close == 0)
            continue;

        double split = prices::instance().split(symbol, date);
        addToExecutedTradeList(currentPortfolio, securityID, date, s.dividend / s.close, s.close / split, -1, "R");
    }
}

void nav::insertPortfolioCashTrade(portfolio *currentPortfolio, const int &cashAccount, const dailyInfoPortfolio *previousInfo, const int &date, const double &tradeValue)
{
    if (!previousInfo || !currentPortfolio->data.securities.contains(cashAccount))
        return;

    QString symbol = currentPortfolio->data.securities.value(cashAccount).symbol;
    double close = prices::instance().price(symbol, previousInfo->date) / prices::instance().split(symbol, date);

    if (close == 0)
        return;

    addToExecutedTradeList(currentPortfolio, cashAccount, date, -1 * tradeValue / close, close, -1, "C");
}

void nav::insertPortfolioTrades(portfolio *currentPortfolio, const int &date, const dailyInfoPortfolio *previousInfo, const navTradeList &trades)
{
    for(navTradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        int securityID = i.key();
        QString symbol = currentPortfolio->data.securities.value(securityID).symbol;
        double close = 0;
        if (previousInfo)
            close = prices::instance().price(symbol, previousInfo->date) / prices::instance().split(symbol, date);

        foreach(const navTradePointer &singleTrade, i.value())
        {
            if (singleTrade->price == -1 && close == 0)
                continue;

            double sharesToBuy = 0;
            QString code;
            switch(singleTrade->type)
            {
                case trade::tradeType_Purchase:
                    sharesToBuy = singleTrade->value;
                    code = "P";
                    break;
                case trade::tradeType_Sale:
                    sharesToBuy = singleTrade->value * -1;
                    code = "S";
                    break;
                case trade::tradeType_DivReinvest:
                    sharesToBuy = singleTrade->value;
                    code = "R";
                    break;
                case trade::tradeType_Interest:
                    sharesToBuy = singleTrade->value;
                    code = "I";
                    break;
                case trade::tradeType_FixedPurchase:
                case trade::tradeType_FixedSale:
                    if (close != 0)
                        sharesToBuy = singleTrade->value / close;
                    if (singleTrade->type == trade::tradeType_FixedSale)
                        sharesToBuy *= -1;
                    code = "F";
                    break;
                case trade::tradeType_TotalValue:
                    if (previousInfo && close != 0)
                        sharesToBuy = (previousInfo->totalValue * (singleTrade->value / 100)) / close;
                    code = "T";
                    break;
                case trade::tradeType_AA:
                    if (previousInfo && close != 0)
                    {
                        const QMap<int, double> aaList = currentPortfolio->data.securities.value(securityID).aa;
                        for(QMap<int, double>::const_iterator x = aaList.constBegin(); x != aaList.constEnd(); ++x)
                        {
                            int aa = x.key();
                            if (currentPortfolio->data.aa.value(aa).target <= 0)
                                continue;

                            sharesToBuy += ((previousInfo->totalValue * (currentPortfolio->data.aa.value(aa).target * x.value() * singleTrade->value / 100)) -
                                previousInfo->securitiesInfo.value(securityID).totalValue) / close;
                        }
                    }
                    code = "A";
                    break;
                default:
                    break;
            }

            double price = singleTrade->type == trade::tradeType_Interest ? 0 :
                                                singleTrade->price >= 0 ? singleTrade->price :
                                                close;

            addToExecutedTradeList(currentPortfolio, securityID, date, sharesToBuy, price, singleTrade->commission, code);

            if (singleTrade->cashAccount != -1 && previousInfo)
                insertPortfolioCashTrade(currentPortfolio, singleTrade->cashAccount, previousInfo, date, sharesToBuy * price);
        }
    }
}

void nav::insertFirstPortfolioTrades(portfolio *currentPortfolio, const int &startDate, const QMap<int, navTradeList> &allTrades)
{
    for(QMap<int, navTradeList>::const_iterator i = allTrades.begin(); i != allTrades.end(); ++i)
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
    m_ExecutedTrades_Commission.append(commission);
    m_ExecutedTrades_Code.append(code);

    currentPortfolio->data.executedTrades[securityID].append(executedTrade(date, shares, price, commission));
}
