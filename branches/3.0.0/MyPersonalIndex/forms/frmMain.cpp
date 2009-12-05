#include <QtGui>
#include "frmMain.h"
#include "queries.h"
#include "frmPortfolio.h"
#include "frmTicker.h"
#include "frmOptions.h"
#include "frmAA.h"
#include "frmAcct.h"
#include "frmStat.h"
#include "frmColumns.h"
#include "frmSort.h"
#include "mainHoldingsModel.h"
#include "viewDelegates.h"

frmMain::frmMain(QWidget *parent) : QMainWindow(parent), m_currentPortfolio(0), m_updateThread(0), m_navThread(0)
{
    QString location = queries::getDatabaseLocation();
    if (!QFile::exists(location))
        if (!QDir().mkpath(QFileInfo(location).dir().absolutePath()) ||
            !QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), location))
        {
            QMessageBox::critical(this, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);
        }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "main");
    db.setDatabaseName(location);
    sql = new queries(db);
    if (!sql->isOpen() || !sql->executeScalar(sql->getVersion()).isValid())
    {
        delete sql;
        sql = 0;
        QMessageBox::critical(this, "Error", "Cannot read user settings folder!", QMessageBox::Ok);
        return;
    }

    ui.setupUI(this);

    loadPortfolios();
    loadSettings();
    loadPortfolioDropDown(m_currentPortfolio ? m_currentPortfolio->info.id : -1);
    loadPortfolio();
    connectSlots();
}

void frmMain::closeEvent(QCloseEvent *event)
{
    if (m_updateThread || m_navThread)
    {
        event->ignore();
        return;
    }

    savePortfolio();
    saveSettings();
    event->accept();
}

void frmMain::connectSlots()
{
    connect(ui.mainAdd, SIGNAL(triggered()), this, SLOT(addPortfolio()));
    connect(ui.mainEdit, SIGNAL(triggered()), this, SLOT(editPortfolio()));
    connect(ui.mainDelete, SIGNAL(triggered()), this, SLOT(deletePortfolio()));
    connect(ui.mainAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.mainOptions, SIGNAL(triggered()), this, SLOT(options()));
    connect(ui.mainUpdatePrices, SIGNAL(triggered()), this, SLOT(beginUpdate()));
    connect(ui.mainPortfolioCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPortfolio()));

    connect(ui.holdingsAdd, SIGNAL(triggered()), this, SLOT(addTicker()));
    connect(ui.holdingsEdit, SIGNAL(triggered()), this, SLOT(editTicker()));
    connect(ui.holdingsDelete, SIGNAL(triggered()), this, SLOT(deleteTicker()));
    connect(ui.holdings, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTicker()));
    connect(ui.holdingsDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(loadPortfolioHoldings()));
    connect(ui.holdingsShowHidden, SIGNAL(changed()), this, SLOT(loadPortfolioHoldings()));
    connect(ui.holdingsReorderColumns, SIGNAL(triggered()), this, SLOT(holdingsModifyColumns()));
    connect(ui.holdingsSortCombo, SIGNAL(activated(int)), this, SLOT(sortDropDownChange(int)));
    connect(ui.holdingsExport, SIGNAL(triggered()), this, SLOT(holdingsExport()));


    connect(ui.aaEdit, SIGNAL(triggered()), this, SLOT(aa()));

    connect(ui.accountsEdit, SIGNAL(triggered()), this, SLOT(acct()));

    connect(ui.statAddEdit, SIGNAL(triggered()), this, SLOT(stat()));
}

void frmMain::loadSettings()
{
    checkVersion();

    QSqlQuery *q = sql->executeResultSet(sql->getSettings());

    if (!q)
        return;

    m_settings.dataStartDate = q->value(queries::getSettings_DataStartDate).toInt();
    m_settings.splits = q->value(queries::getSettings_Splits).toBool();
    m_settings.tickersIncludeDividends = q->value(queries::getSettings_TickersIncludeDividends).toBool();
    m_settings.version = q->value(queries::getSettings_Version).toInt();
    if (!q->value(queries::getSettings_WindowState).isNull())
    {
        m_settings.windowSize = QSize(q->value(queries::getSettings_WindowWidth).toInt(),
            q->value(queries::getSettings_WindowHeight).toInt());
        resize(m_settings.windowSize);
        m_settings.windowLocation = QPoint(q->value(queries::getSettings_WindowX).toInt(),
                    q->value(queries::getSettings_WindowY).toInt());
        move(m_settings.windowLocation);
        m_settings.state = (Qt::WindowState)q->value(queries::getSettings_WindowState).toInt();
        if (m_settings.state != Qt::WindowNoState)
            this->setWindowState(this->windowState() | m_settings.state);
    }
    else
        functions::showWelcomeMessage(this); // first time being run

    if (!q->value(queries::getSettings_LastPortfolio).isNull())
    {
        m_settings.lastPortfolio = q->value(queries::getSettings_LastPortfolio).toInt();
        m_currentPortfolio = m_portfolios.value(m_settings.lastPortfolio.toInt());
    }

    delete q;

    loadSettingsColumns();
    loadStats();
    loadDates();
    loadSplits();
    resetLastDate();
}

void frmMain::loadSettingsColumns()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSettingsColumns());

    if (!q)
        return;

    do
    {
        m_settings.columns[q->value(queries::getSettingsColumns_ID).toInt()].append(
                q->value(queries::getSettingsColumns_ColumnID).toInt());
    }
    while (q->next());

    delete q;
}

void frmMain::loadStats()
{
    QSqlQuery *q = sql->executeResultSet(sql->getStat());

    if (!q)
        return;

    do
    {
        globals::statistic stat;
        stat.id = q->value(queries::getStat_ID).toInt();
        stat.description = q->value(queries::getStat_Description).toString();
        stat.sql = q->value(queries::getStat_SQL).toString();
        stat.format = (globals::outputFormat)q->value(queries::getStat_Format).toInt();

        m_statistics.insert(stat.id, stat);
    }
    while (q->next());

    delete q;
}

void frmMain::loadDates()
{
    m_dates.clear();
    QSqlQuery *q = sql->executeResultSet(sql->getDates());

    if (!q)
        return;

    do
    {
        m_dates.append(q->value(queries::getDates_Date).toInt());
    }
    while (q->next());

    delete q;
}

void frmMain::loadSplits()
{
    m_splits.clear();
    QSqlQuery *q = sql->executeResultSet(sql->getSplits());

    if (!q)
        return;

    do
    {
        m_splits[q->value(queries::getSplits_Date).toInt()].insert(q->value(queries::getSplits_Ticker).toString(), q->value(queries::getSplits_Ratio).toDouble());
    }
    while (q->next());

    delete q;
}

void frmMain::resetLastDate()
{
    int lastDate = getLastDate();
    ui.stbLastUpdated->setText(QString(" %1%2 ").arg(ui.LAST_UPDATED_TEXT,
                lastDate == m_settings.dataStartDate ?
                "Never" :
                QDate::fromJulianDay(lastDate).toString(Qt::SystemLocaleShortDate)));
}

void frmMain::checkVersion()
{
    if (m_settings.version == VERSION)
        return;
}

void frmMain::saveSettings()
{
    savePortfolios();
    QVariant portfolio(QVariant::Int);
    if (m_currentPortfolio)
        portfolio = m_currentPortfolio->info.id;

    m_settings.lastPortfolio = portfolio;
    m_settings.windowSize = size();
    m_settings.windowLocation = pos();
    m_settings.state = isMaximized() ? Qt::WindowMaximized : isMinimized() ? Qt::WindowMinimized : Qt::WindowNoState;
    sql->executeNonQuery(sql->updateSettings(m_settings));
}

void frmMain::loadPortfolioDropDown(const int &portfolioID = -1)
{
    ui.mainPortfolioCombo->blockSignals(true);
    ui.mainPortfolioCombo->setUpdatesEnabled(false);
    ui.mainPortfolioCombo->clear();

    foreach(globals::myPersonalIndex *p, m_portfolios)
        ui.mainPortfolioCombo->addItem(p->info.description, p->info.id);

    int row = portfolioID == -1 ? -1 : ui.mainPortfolioCombo->findData(portfolioID);
    if (row == -1)
        ui.mainPortfolioCombo->setCurrentIndex(0);
    else
        ui.mainPortfolioCombo->setCurrentIndex(row);

    ui.mainPortfolioCombo->setUpdatesEnabled(true);
    ui.mainPortfolioCombo->blockSignals(false);
}

void frmMain::disableItems(bool disabled)
{
    ui.tab->setDisabled(disabled);
}

void frmMain::loadPortfolio()
{
    if (m_portfolios.isEmpty()) // no portfolios to load
        disableItems(true);
    else
    {
        if (this->isVisible())
            savePortfolio(); // save currently loaded portfolio except on initial load
        disableItems(false);

        m_currentPortfolio = m_portfolios.value(ui.mainPortfolioCombo->itemData(ui.mainPortfolioCombo->currentIndex()).toInt());

        if (!m_currentPortfolio)
        {
            QMessageBox::information(this, "Error!", "Portfolio appears to be deleted. Please restart.");
            disableItems(true);
            return;
        }

        loadPortfolioSettings();
        int lastDate = getLastDate();
        resetCalendars(lastDate);
        loadPortfolioHoldings();
    }
}

void frmMain::resetCalendars(const int &date)
{
    int start = m_currentPortfolio->info.startDate;
    int end = date < start ? start : date;

    resetCalendar(end, start, ui.holdingsDateDropDown);
}

void frmMain::resetCalendar(const int &date, const int &minDate, QDateEdit *calendar)
{
    calendar->blockSignals(true);
    calendar->setMinimumDate(QDate::fromJulianDay(minDate));
    calendar->setDate(QDate::fromJulianDay(date));
    calendar->blockSignals(false);
}

void frmMain::resetCalendar(const int &date, const int &minDate, QDateEdit *calendarStart, QDateEdit *calendarEnd)
{
    calendarStart->blockSignals(true);
    calendarStart->setMinimumDate(QDate::fromJulianDay(minDate));
    calendarStart->setDate(QDate::fromJulianDay(minDate));
    calendarStart->blockSignals(false);
    calendarEnd->blockSignals(true);
    calendarEnd->setMinimumDate(QDate::fromJulianDay(minDate));
    calendarEnd->setDate(QDate::fromJulianDay(date));
    calendarEnd->blockSignals(false);
}

void frmMain::loadPortfolioHoldings()
{
    int currentDate = getDateDropDownDate(ui.holdingsDateDropDown);
    QAbstractItemModel *oldModel = ui.holdings->model();

    globals::portfolioCache *cache = m_currentPortfolio->cache.object(currentDate);;
    if (!cache)
    {
        cache = calculations::portfolioValues(m_currentPortfolio, currentDate, m_splits, *sql);
        m_currentPortfolio->cache.insert(currentDate, cache);
    }

    QList<holdingsRow> rows;
    foreach(const globals::security &s, m_currentPortfolio->data.tickers)
        if (ui.holdingsShowHidden->isChecked() || !s.hide)
            rows.append(
                holdingsRow::getHoldingsRow(s, cache, m_currentPortfolio->data.acct, m_currentPortfolio->info.holdingsSort)
            );

    qStableSort(rows);

    holdingsModel *model = new holdingsModel(rows, m_settings.columns.value(globals::columnIDs_Holdings), cache, true, ui.holdings);
    ui.holdings->setModel(model);
    ui.holdings->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    if (ui.holdingsSortCombo->count() == 0)
        loadSortDropDown(holdingsRow::fieldNames(), ui.holdingsSortCombo);

    if (m_currentPortfolio->info.holdingsSort.isEmpty())
        ui.holdingsSortCombo->setCurrentIndex(0);
    else if (m_currentPortfolio->info.holdingsSort.contains('|') || m_currentPortfolio->info.holdingsSort.contains('D'))
        ui.holdingsSortCombo->setCurrentIndex(ui.holdingsSortCombo->count() - 1);
    else
        ui.holdingsSortCombo->setCurrentIndex(ui.holdingsSortCombo->findData(m_currentPortfolio->info.holdingsSort.toInt()));

    delete oldModel;
}

void frmMain::loadPortfolioSettings()
{
    // set start date equal to earliest data day possible (may be after start date)
    //CheckPortfolioStartDate(rs.GetDateTime((int)queries.egetPortfolio.StartDate));
    // todo ^

    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, QDate::fromJulianDay(m_currentPortfolio->info.startDate).toString(Qt::SystemLocaleShortDate)));
    ui.holdingsShowHidden->setChecked(m_currentPortfolio->info.holdingsShowHidden);
    ui.performanceSortDesc->setChecked(m_currentPortfolio->info.navSortDesc);
    ui.aaShowBlank->setChecked(m_currentPortfolio->info.aaShowBlank);
    ui.correlationsShowHidden->setChecked(m_currentPortfolio->info.correlationShowHidden);
    ui.accountsShowBlank->setChecked(m_currentPortfolio->info.acctShowBlank);

    // todo
    //ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));
    // todo
    //ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));
    //mpi.account.sort = q->value(queries::getPortfolio_AcctSort).toString();
}

void frmMain::loadPortfolios()
{
    loadPortfoliosInfo();
    loadPortfoliosAA();
    loadPortfoliosAcct();
    loadPortfoliosStat();
    loadPortfoliosTickers();
    loadPortfoliosTickersAA();
    loadPortfoliosTickersTrades();
    loadPortfoliosTrades();
}

void frmMain::loadPortfoliosInfo()
{
    QSqlQuery *q = sql->executeResultSet(sql->getPortfolio());

    if (!q)
        return;

    do
    {
        globals::portfolio p;

        p.id = q->value(queries::getPortfolio_PortfolioID).toInt();
        p.description = q->value(queries::getPortfolio_Description).toString();
        p.origStartDate = q->value(queries::getPortfolio_StartDate).toInt();
        p.startDate = p.origStartDate;
        p.dividends = q->value(queries::getPortfolio_Dividends).toBool();
        p.costCalc = (globals::avgShareCalc)q->value(queries::getPortfolio_CostCalc).toInt();
        p.startValue = q->value(queries::getPortfolio_StartValue).toInt();
        p.aaThreshold = q->value(queries::getPortfolio_AAThreshold).toInt();
        p.aaThresholdMethod = (globals::thesholdMethod)q->value(queries::getPortfolio_AAThresholdMethod).toInt();
        p.holdingsShowHidden = q->value(queries::getPortfolio_HoldingsShowHidden).toBool();
        p.navSortDesc = q->value(queries::getPortfolio_NAVSortDesc).toBool();
        p.aaShowBlank = q->value(queries::getPortfolio_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(queries::getPortfolio_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(queries::getPortfolio_AcctShowBlank).toBool();
        p.holdingsSort = q->value(queries::getPortfolio_HoldingsSort).toString();
        p.aaSort = q->value(queries::getPortfolio_AASort).toString();
        p.acctSort = q->value(queries::getPortfolio_AcctSort).toString();
        p.lastNAVDate = q->value(queries::getPortfolio_LastNAVDate).toInt();

        m_portfolios.insert(p.id, new globals::myPersonalIndex(p));
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosTickers()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSecurity());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::security sec;

        sec.id = q->value(queries::getSecurity_ID).toInt();
        sec.ticker = q->value(queries::getSecurity_Ticker).toString();
        if (!q->value(queries::getSecurity_Account).isNull())
            sec.account = q->value(queries::getSecurity_Account).toInt();
        if (!q->value(queries::getSecurity_Expense).isNull())
            sec.expense = q->value(queries::getSecurity_Expense).toDouble();
        sec.divReinvest = q->value(queries::getSecurity_DivReinvest).toBool();
        sec.cashAccount = q->value(queries::getSecurity_CashAccount).toBool();
        sec.includeInCalc = q->value(queries::getSecurity_IncludeInCalc).toBool();
        sec.hide = q->value(queries::getSecurity_Hide).toBool();

        int portfolioID = q->value(queries::getSecurity_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.tickers.insert(sec.id, sec);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosTickersAA()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSecurityAA());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::tickerAATarget pair(
            q->value(queries::getSecurityAA_AAID).toInt(),
            q->value(queries::getSecurityAA_Percent).toDouble()
        );

        int portfolioID = q->value(queries::getSecurityAA_PortfolioID).toInt();
        int tickerID = q->value(queries::getSecurityAA_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].aa.append(pair);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosTickersTrades()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSecurityTrade());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::dynamicTrade trade;

        trade.id = q->value(queries::getSecurityTrade_ID).toInt();
        trade.tradeType = (globals::dynamicTradeType)q->value(queries::getSecurityTrade_Type).toInt();
        trade.value = q->value(queries::getSecurityTrade_Value).toDouble();
        if (!q->value(queries::getSecurityTrade_Price).isNull())
            trade.price = q->value(queries::getSecurityTrade_Price).toDouble();
        if (!q->value(queries::getSecurityTrade_Commission).isNull())
            trade.commission = q->value(queries::getSecurityTrade_Commission).toDouble();
        if (!q->value(queries::getSecurityTrade_CashAccountID).isNull())
            trade.cashAccount = q->value(queries::getSecurityTrade_CashAccountID).toInt();
        trade.frequency = (globals::dynamicTradeFreq)q->value(queries::getSecurityTrade_Frequency).toInt();
        if (!q->value(queries::getSecurityTrade_Date).isNull())
            trade.date = q->value(queries::getSecurityTrade_Date).toInt();
        if (!q->value(queries::getSecurityTrade_StartDate).isNull())
            trade.date = q->value(queries::getSecurityTrade_StartDate).toInt();
        if (!q->value(queries::getSecurityTrade_EndDate).isNull())
            trade.date = q->value(queries::getSecurityTrade_EndDate).toInt();

        int portfolioID = q->value(queries::getSecurityTrade_PortfolioID).toInt();
        int tickerID = q->value(queries::getSecurityTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.tickers[tickerID].trades.insert(trade.id, trade);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosTrades()
{
    QSqlQuery *q = sql->executeResultSet(sql->getTrade());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::trade trade;

        trade.date = q->value(queries::getTrade_Date).toInt();
        trade.shares = q->value(queries::getTrade_Shares).toDouble();
        trade.price = q->value(queries::getTrade_Price).toDouble();
        trade.commission = q->value(queries::getTrade_Commission).toDouble();

        int portfolioID = q->value(queries::getTrade_PortfolioID).toInt();
        int tickerID = q->value(queries::getTrade_TickerID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }
        p->data.trades[tickerID].append(trade);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosAA()
{
    QSqlQuery *q = sql->executeResultSet(sql->getAA());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::assetAllocation aa;

        aa.id = q->value(queries::getAA_ID).toInt();
        aa.description = q->value(queries::getAA_Description).toString();
        if (!q->value(queries::getAA_Target).isNull())
            aa.target = q->value(queries::getAA_Target).toDouble();

        int portfolioID = q->value(queries::getAA_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.aa.insert(aa.id, aa);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosAcct()
{
    QSqlQuery *q = sql->executeResultSet(sql->getAcct());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        globals::account acct;

        acct.id = q->value(queries::getAcct_ID).toInt();
        acct.description = q->value(queries::getAcct_Description).toString();
        if (!q->value(queries::getAcct_TaxRate).isNull())
            acct.taxRate = q->value(queries::getAcct_TaxRate).toDouble();
        acct.taxDeferred = q->value(queries::getAcct_TaxDeferred).toBool();

        int portfolioID = q->value(queries::getAcct_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.acct.insert(acct.id, acct);
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosStat()
{
    QSqlQuery *q = sql->executeResultSet(sql->getStatMapping());

    if (!q)
        return;

    globals::myPersonalIndex *p;
    int current = -1;

    do
    {
        int portfolioID = q->value(queries::getStatMapping_PortfolioID).toInt();
        if (portfolioID != current)
        {
            p = m_portfolios.value(portfolioID);
            current = portfolioID;
        }

       p->data.stats.append(q->value(queries::getStatMapping_StatID).toInt());
    }
    while(q->next());

    delete q;
}

void frmMain::savePortfolio()
{
    if (!m_currentPortfolio)
        return;

    m_currentPortfolio->info.holdingsShowHidden = ui.holdingsShowHidden->isChecked();
    m_currentPortfolio->info.navSortDesc = ui.performanceSortDesc->isChecked();
    m_currentPortfolio->info.aaShowBlank = ui.aaShowBlank->isChecked();
    m_currentPortfolio->info.correlationShowHidden = ui.correlationsShowHidden->isChecked();
    m_currentPortfolio->info.acctShowBlank = ui.accountsShowBlank->isChecked();
}

void frmMain::savePortfolios()
{
    sql->getDatabase().transaction();
    foreach(globals::myPersonalIndex *p, m_portfolios)
        sql->executeNonQuery(sql->updatePortfolio(p->info));
    sql->getDatabase().commit();
}

void frmMain::addPortfolio()
{
    frmPortfolio f(globals::portfolio(), m_settings.dataStartDate, *sql, this);
    if (f.exec())
    {
        globals::portfolio p = f.getReturnValues();

        m_portfolios.insert(p.id, new globals::myPersonalIndex(p));
        loadPortfolioDropDown(p.id);
        loadPortfolio();
    };
}

void frmMain::editPortfolio()
{
    if (!m_currentPortfolio)
        return;

    frmPortfolio f(m_currentPortfolio->info, m_settings.dataStartDate, *sql, this);
    if (f.exec())
    {
        m_currentPortfolio->info = f.getReturnValues();

        // toDo

        loadPortfolioDropDown(m_currentPortfolio->info.id);
    }
}

void frmMain::deletePortfolio()
{
    if (!m_currentPortfolio)
        return;

    if (QMessageBox::question(this, "Delete Portfolio?", QString("Are you sure you want to delete %1?").arg(m_currentPortfolio->info.description),
            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        int i = m_currentPortfolio->info.id;

        m_portfolios.remove(i);
        sql->executeNonQuery(sql->deleteItem(queries::table_Portfolios, i));

        int row = ui.mainPortfolioCombo->currentIndex();
        if (m_portfolios.count() == row)
            row--;

        loadPortfolioDropDown(-1);
        if (!m_portfolios.isEmpty())
            ui.mainPortfolioCombo->setCurrentIndex(row);
    }

}

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString(VERSIONTEXT) + "</h2>"
        "<p>Copyright &copy; 2009"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}

void frmMain::refreshPortfolioSecurities(const int &minDate)
{
    if (minDate != -1)
    {
        beginNAV(m_currentPortfolio->info.id, minDate);
        return;
    }

    m_currentPortfolio->cache.clear();
    loadPortfolioHoldings();
}

void frmMain::addTicker()
{
    int resultcode;
    bool change = false;
    bool showUpdatePrices = false;
    int minDate = -1;
    do
    {
        frmTicker f(m_currentPortfolio->info.id, m_currentPortfolio->data, globals::security(), *sql, this);
        resultcode = f.exec();
        if (resultcode == QDialog::Accepted || resultcode == QDialog::Accepted + 1)
        {
            change = true;
            globals::security s = f.getReturnValuesSecurity();

            m_currentPortfolio->data.tickers[s.id] = s;
            int currentMinDate = f.getReturnValuesMinDate();
            if (currentMinDate != -1 && (currentMinDate < minDate || minDate == -1))
                minDate = currentMinDate;

            globals::portfolioCache *cache = m_currentPortfolio->cache.object(ui.holdingsDateDropDown->date().toJulianDay());
            if (!s.cashAccount && (!cache || !cache->tickerInfo.contains(s.ticker)))
                showUpdatePrices = true;
        }
    }
    while (resultcode == QDialog::Accepted + 1);

    if (!change)
        return;

    if (showUpdatePrices)
        if (QMessageBox::question(this, "Update Prices", "Would you like to update prices for this new security?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            beginUpdate();
            return;
        }

    refreshPortfolioSecurities(minDate);
}

void frmMain::editTicker()
{
    bool change = false;
    int minDate = -1;
    foreach(int i, static_cast<holdingsModel*>(ui.holdings->model())->selectedItems())
    {
        frmTicker f(m_currentPortfolio->info.id, m_currentPortfolio->data, m_currentPortfolio->data.tickers.value(i), *sql, this);
        if (f.exec())
        {
            change = true;
            m_currentPortfolio->data.tickers[i] = f.getReturnValuesSecurity();
            int newMinDate = f.getReturnValuesMinDate();
            if (newMinDate != -1 && (newMinDate < minDate || minDate == -1))
                minDate = newMinDate;
        }
    }

    if (!change)
        return;

    refreshPortfolioSecurities(minDate);
}

void frmMain::deleteTicker()
{
    QStringList tickers;
    foreach(int i, static_cast<holdingsModel*>(ui.holdings->model())->selectedItems())
        tickers.append(m_currentPortfolio->data.tickers.value(i).ticker);

    if (tickers.isEmpty())
        return;

    if (QMessageBox::question(this, "Delete securities", QString("Are you sure you want to delete the following securities: %1?").arg(tickers.join(", ")),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    int minDate = -1;
    foreach(int i, static_cast<holdingsModel*>(ui.holdings->model())->selectedItems())
    {
        globals::security s = m_currentPortfolio->data.tickers.value(i);
        int newMinDate = calculations::firstTradeDate(s.trades);
        if (newMinDate < minDate || minDate == -1)
            minDate = newMinDate;

        sql->executeNonQuery(sql->deleteItem(queries::table_Tickers, s.id));
        sql->executeNonQuery(sql->deleteTickerItems(queries::table_TickersAA, s.id));
        sql->executeNonQuery(sql->deleteTickerItems(queries::table_TickersTrades, s.id));
        sql->executeNonQuery(sql->deleteTickerItems(queries::table_Trades, s.id));
        m_currentPortfolio->data.tickers.remove(s.id);
    }

    deleteUnusedInfo();
    refreshPortfolioSecurities(minDate);
}

void frmMain::deleteUnusedInfo()
{
    sql->executeNonQuery(sql->deleteUnusedPrices(queries::table_ClosingPrices));
    sql->executeNonQuery(sql->deleteUnusedPrices(queries::table_Dividends));
    sql->executeNonQuery(sql->deleteUnusedPrices(queries::table_Splits));
    loadDates();
    loadSplits();
}

void frmMain::options()
{
    frmOptions f(m_settings, *sql, this);
    if (f.exec())
        m_settings = f.getReturnValues();
}

void frmMain::aa()
{
    frmAA f(m_currentPortfolio->info.id, m_currentPortfolio->data.aa, *sql, this);
    if (f.exec())
    {
        m_currentPortfolio->data.aa = f.getReturnValues();
    }

}

void frmMain::acct()
{
    frmAcct f(m_currentPortfolio->info.id, m_currentPortfolio->data.acct, *sql, this);
    if (f.exec())
    {
        m_currentPortfolio->data.acct = f.getReturnValues();
    }
}

void frmMain::stat()
{
    frmStat f(m_currentPortfolio->info.id, m_statistics, m_currentPortfolio->data.stats, *sql, this);
    if (f.exec())
    {
        m_statistics = f.getReturnValues_Map();
        m_currentPortfolio->data.stats = f.getReturnValues_Selected();
    }

}

void frmMain::beginUpdate()
{
    if (!updatePrices::isInternetConnection())
    {
        QMessageBox::critical(this, "Update Error", "Cannot contact Yahoo! Finance, please check your internet connection.");
        return;
    }

    ui.stbProgress->setMaximum(0);
    m_updateThread = new updatePrices(m_portfolios, m_dates, m_splits, m_settings, this);
    connect(m_updateThread, SIGNAL(updateFinished(QStringList)), this, SLOT(finishUpdate(QStringList)));
    connect(m_updateThread, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdate(QString)));
    m_updateThread->start();
}

void frmMain::finishUpdate(const QStringList &invalidTickers)
{
    ui.stbProgress->setMaximum(100);
    ui.stbProgress->setValue(0);
    statusUpdate("");

    if (!invalidTickers.isEmpty())
        QMessageBox::information(this,
            "Update Error", "The following tickers were not updated (Yahoo! Finance may not yet have today's price):\n\n" +
            invalidTickers.join(", "));

    m_updateThread->quit();
    m_updateThread->wait();
    m_updateThread->disconnect();
    delete m_updateThread;
    m_updateThread = 0;
    loadPortfolio();
}

void frmMain::beginNAV(const int &portfolioID, const int &minDate)
{
    ui.stbProgress->setMaximum(0);
    m_navThread = new NAV(m_portfolios, m_dates, m_splits, minDate, this, portfolioID);
    connect(m_navThread, SIGNAL(calculationFinished()), this, SLOT(finishNAV()));
    connect(m_navThread, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdate(QString)));
    m_navThread->start();
}

void frmMain::finishNAV()
{
    ui.stbProgress->setMaximum(100);
    ui.stbProgress->setValue(0);
    statusUpdate("");
    m_navThread->quit();
    m_navThread->wait();
    m_navThread->disconnect();
    delete m_navThread;
    m_navThread = 0;
    loadPortfolio();
}

void frmMain::statusUpdate(const QString &message)
{
    ui.stbStatus->setText(QString("Status: ").append(message));
}

int frmMain::getCurrentDateOrPrevious(int date)
{
    QList<int>::const_iterator place = qLowerBound(m_dates, date);
    if (*place != date && place != m_dates.constBegin())
        return *(place - 1);
    else
        return *place;
}

int frmMain::getDateDropDownDate(QDateEdit *dateDropDown)
{
    int currentDate = qMax(getCurrentDateOrPrevious(dateDropDown->date().toJulianDay()), m_currentPortfolio->info.startDate);
    dateDropDown->blockSignals(true);
    dateDropDown->setDate(QDate::fromJulianDay(currentDate));
    dateDropDown->blockSignals(false);
    return currentDate;
}

void frmMain::loadSortDropDown(const QMap<int, QString> &fieldNames, QComboBox *dropDown)
{
    dropDown->blockSignals(true);
    dropDown->addItem("", -1);

    for (QMap<int, QString>::const_iterator i = fieldNames.constBegin(); i != fieldNames.constEnd(); ++i)
        dropDown->addItem(i.value(), i.key());
    dropDown->addItem("Custom...", -2);
    dropDown->blockSignals(false);
}

void frmMain::holdingsModifyColumns()
{
    frmColumns f(globals::columnIDs_Holdings, m_settings.columns.value(globals::columnIDs_Holdings), holdingsRow::fieldNames(), *sql, this);
    if (f.exec())
    {
        m_settings.columns[globals::columnIDs_Holdings] = f.getReturnValues();
        loadPortfolioHoldings();
    }
}

void frmMain::sortDropDownChange(int index)
{
    int columnID = ui.holdingsSortCombo->itemData(index).toInt();

    if (columnID == -1)
    {
        m_currentPortfolio->info.holdingsSort.clear();
        loadPortfolioHoldings();
        return;
    }

    if (columnID != -2)
    {
        m_currentPortfolio->info.holdingsSort = QString::number(columnID);
        loadPortfolioHoldings();
        return;
    }

    frmSort f(m_currentPortfolio->info.holdingsSort, holdingsRow::fieldNames(), this);

    if (f.exec())
    {
        m_currentPortfolio->info.holdingsSort = f.getReturnValues();
        loadPortfolioHoldings();
    }

    if (!m_currentPortfolio->info.holdingsSort.contains('|') && !m_currentPortfolio->info.holdingsSort.contains('D'))
        ui.holdingsSortCombo->setCurrentIndex(ui.holdingsSortCombo->findData(m_currentPortfolio->info.holdingsSort.toInt()));
}
