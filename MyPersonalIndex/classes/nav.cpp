#define portfolios portfolio::instance()
#define priceManager prices::instance()
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
        calculateNAVValues(p);
        insertNAVValues();
        clearNAVValues();
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
    calculations calc(portfolioID);

    deleteNAVValues(portfolioID, calculationDate, calculateFromStartDate);

    QList<int>::const_iterator dateIterator = priceManager.iteratorPreviousDate(calculationDate);
    if (priceManager.iteratorEnd() - dateIterator <= 1) // cannot calculate day over day change
        return;

    const QMap<int, navTradeList> trades = calculateExecutedTrades(portfolioID, calculationDate, calculateFromStartDate);
    const QList<int> securityReinvestments = portfolios.securityReinvestments(portfolioID);
    if (calculateFromStartDate)
        insertExecutedTradesPreStartDate(portfolioID, calculationDate, trades);

    int date = *dateIterator;
    dailyInfoPortfolio previousInfo = calc.portfolioValues(date);
    double navValue = calculateFromStartDate ? info.startValue : portfolios.nav(portfolioID).nav(date).nav;

    if (calculateFromStartDate)
        addToNAVList(portfolioID, date, previousInfo.totalValue, navValue);

    for (++dateIterator; dateIterator != priceManager.iteratorEnd(); ++dateIterator)
    {
        date = *dateIterator;
        insertPortfolioReinvestments(portfolioID, date, securityReinvestments, previousInfo);
        insertExecutedTrades(portfolioID, date, previousInfo, appendNavTradeLists(trades.value(date), trades.value(-1)));

        const dailyInfoPortfolio currentInfo = calc.portfolioValues(date);
        navValue = calculations::change(currentInfo.totalValue, previousInfo.totalValue, currentInfo.costBasis - previousInfo.costBasis, info.dividends ? currentInfo.dividendAmount : 0, navValue);
        addToNAVList(portfolioID, date, currentInfo.totalValue, navValue);
        previousInfo = currentInfo;
    }
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

void nav::insertNAVValues()
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

void nav::clearNAVValues()
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

void nav::deleteNAVValues(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate)
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

int nav::checkCalculationDate(const int &portfolioID, int calculationDate, bool *calcuateFromStartDate)
{
    int lastNavDate = portfolios.nav(portfolioID).isEmpty() ? -1 : portfolios.nav(portfolioID).lastDate();
    // check if the portfolio needs to be recalculated even before the mindate
    if (lastNavDate < calculationDate)
        calculationDate = lastNavDate + 1;

    if (calculationDate <= portfolios.startDate(portfolioID))
    {
        // portfolio will recalculate from its startdate
        calculationDate = portfolios.startDate(portfolioID);
        *calcuateFromStartDate = true;
    }

    return calculationDate;
}

const QMap<int, nav::navTradeList> nav::calculateExecutedTrades(const int &portfolioID, const int &calculationDate, const bool &calculateFromStartDate)
{
    QMap<int, navTradeList> trades;

    foreach(const security &s, portfolios.securities(portfolioID))
        if (s.includeInCalc)
            for(navTradePointer singleTrade = s.trades.constBegin(); singleTrade != s.trades.constEnd(); ++singleTrade)
                foreach(const int &i, singleTrade->tradeDates(priceManager.dates(), calculationDate, calculateFromStartDate))
                    trades[i][s.id].append(singleTrade);

    return trades;
}

void nav::insertPortfolioReinvestments(const int &portfolioID, const int &date, const QList<int> &securityReinvestments, const dailyInfoPortfolio &previousInfo)
{
    foreach(const int &securityID, securityReinvestments)
    {
        QString symbol = portfolios.securities(portfolioID, securityID).description;
        securityPrice s = priceManager.dailyPriceInfo(symbol, previousInfo.date);
        if (s.dividend == 0 || s.close == 0)
            continue;

        double split = priceManager.split(symbol, date);
        addToExecutedTradeList(portfolioID, securityID, date, (s.dividend * previousInfo.securitiesInfo.value(securityID).shares) / s.close, s.close / split, 0);
    }
}

void nav::insertPortfolioCashTrade(const int &portfolioID, const int &cashAccount, const dailyInfoPortfolio &previousInfo, const int &date, const double &tradeValue)
{
    if (!portfolios.securities(portfolioID).contains(cashAccount))
        return;

    QString symbol = portfolios.securities(portfolioID, cashAccount).description;
    double close = priceManager.price(symbol, previousInfo.date) / priceManager.split(symbol, date);

    if (close == 0)
        return;

    addToExecutedTradeList(portfolioID, cashAccount, date, -1 * tradeValue / close, close, 0);
}

void nav::insertExecutedTrades(const int &portfolioID, const int &date, const dailyInfoPortfolio &previousInfo, const navTradeList &trades)
{
    for(navTradeList::const_iterator i = trades.constBegin(); i != trades.constEnd(); ++i)
    {
        int securityID = i.key();
        const security s = portfolios.securities(portfolioID, securityID);
        double close = previousInfo.isNull() ? 0 : priceManager.price(s.description, previousInfo.date) / priceManager.split(s.description, date);

        foreach(const navTradePointer &singleTrade, i.value())
        {
            double price = singleTrade->purchasePrice(close);
            double sharesToBuy = singleTrade->shares(price, previousInfo.securitiesInfo.value(securityID).totalValue, previousInfo.totalValue,
                s.aa, portfolios.aa(portfolioID));

            if (price > 0 && sharesToBuy != 0)
                addToExecutedTradeList(portfolioID, securityID, date, sharesToBuy, price, singleTrade->commission);

            if (singleTrade->cashAccount != -1 && !previousInfo.isNull() && sharesToBuy > 0)
                insertPortfolioCashTrade(portfolioID, singleTrade->cashAccount, previousInfo, date, sharesToBuy * price);
        }
    }
}

void nav::insertExecutedTradesPreStartDate(const int &portfolioID, const int &startDate, const QMap<int, navTradeList> &allTrades)
{
    for(QMap<int, navTradeList>::const_iterator i = allTrades.begin(); i != allTrades.end(); ++i)
    {
        int date = i.key();

        if (date >= startDate)
            break;

        if (date == -1)
            continue;

        insertExecutedTrades(portfolioID, date, 0, i.value());
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
