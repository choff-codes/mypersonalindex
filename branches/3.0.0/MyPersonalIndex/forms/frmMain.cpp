#define portfolios portfolio::instance()
#include "frmMain.h"

frmMain::frmMain(QWidget *parent): QMainWindow(parent), m_calculationInProgress(false)
{
    ui.setupUI(this);

    m_settings = settings::loadSettings();

    if (m_settings.state != Qt::WindowActive)
    {
        move(m_settings.windowLocation);
        resize(m_settings.windowSize);
        if (m_settings.state != Qt::WindowNoState)
            this->setWindowState(this->windowState() | m_settings.state);
    }
    else
        showWelcomeMessage(); // first time being run

    resetSortDropDowns();
    resetLastDate();
    resetPortfolioDropDown(m_settings.lastPortfolio.isNull() ? -1 : m_settings.lastPortfolio.toInt());
    loadPortfolio();
    connectSlots();
}

void frmMain::closeEvent(QCloseEvent *event)
{
    if (m_calculationInProgress)
    {
        this->setWindowTitle(QString("%1 - %2").arg(ui.WINDOW_TITLE, ui.BUSY));
        event->ignore();
        return;
    }

    this->hide();
    savePortfolio();
    portfolios.save();
    saveSettings();
}

void frmMain::connectSlots()
{
    connect(ui.mainAdd, SIGNAL(triggered()), this, SLOT(addPortfolio()));
    connect(ui.mainEdit, SIGNAL(triggered()), this, SLOT(editPortfolio()));
    connect(ui.mainImport, SIGNAL(triggered()), this, SLOT(import()));
    connect(ui.mainDelete, SIGNAL(triggered()), this, SLOT(deletePortfolio()));
    connect(ui.mainAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.mainOptions, SIGNAL(triggered()), this, SLOT(options()));
    connect(ui.mainUpdatePrices, SIGNAL(triggered()), this, SLOT(beginUpdate()));
    connect(ui.mainPortfolioCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPortfolio()));

    connect(ui.holdingsAdd, SIGNAL(triggered()), this, SLOT(addSecurity()));
    connect(ui.holdingsEdit, SIGNAL(triggered()), this, SLOT(editSecurity()));
    connect(ui.holdingsDelete, SIGNAL(triggered()), this, SLOT(deleteSecurity()));
    connect(ui.holdings, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editSecurity()));
    connect(ui.holdingsDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioHoldings()));
    connect(ui.holdingsShowHidden, SIGNAL(changed()), this, SLOT(resetPortfolioHoldings()));
    connect(ui.holdingsReorderColumns, SIGNAL(triggered()), this, SLOT(holdingsModifyColumns()));
    connect(ui.holdingsSortCombo, SIGNAL(activated(int)), this, SLOT(holdingsSortChanged(int)));
    connect(ui.holdingsExport, SIGNAL(triggered()), ui.holdings, SLOT(exportTable()));
    connect(ui.holdingsCopyShortcut, SIGNAL(activated()), ui.holdings, SLOT(copyTable()));

    connect(ui.performanceSortDesc, SIGNAL(triggered()), this, SLOT(resetPortfolioPerformance()));
    connect(ui.performanceExport, SIGNAL(triggered()), ui.performance, SLOT(exportTable()));
    connect(ui.performanceCopyShortcut, SIGNAL(activated()), ui.performance, SLOT(copyTable()));

    connect(ui.chartEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioChart()));
    connect(ui.chartStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioChart()));
    connect(ui.chartExport, SIGNAL(triggered()), ui.chart, SLOT(exportChart()));

    connect(ui.aaAdd, SIGNAL(triggered()), this, SLOT(addAA()));
    connect(ui.aaEdit, SIGNAL(triggered()), this, SLOT(editAA()));
    connect(ui.aa, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editAA()));
    connect(ui.aaDelete, SIGNAL(triggered()), this, SLOT(deleteAA()));
    connect(ui.aaDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioAA()));
    connect(ui.aaShowBlank, SIGNAL(changed()), this, SLOT(resetPortfolioAA()));
    connect(ui.aaReorderColumns, SIGNAL(triggered()), this, SLOT(aaModifyColumns()));
    connect(ui.aaSortCombo, SIGNAL(activated(int)), this, SLOT(aaSortChanged(int)));
    connect(ui.aaExport, SIGNAL(triggered()), ui.aa, SLOT(exportTable()));
    connect(ui.aaCopyShortcut, SIGNAL(activated()), ui.aa, SLOT(copyTable()));

    connect(ui.accountsAdd, SIGNAL(triggered()), this, SLOT(addAcct()));
    connect(ui.accountsEdit, SIGNAL(triggered()), this, SLOT(editAcct()));
    connect(ui.accounts, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editAcct()));
    connect(ui.accountsDelete, SIGNAL(triggered()), this, SLOT(deleteAcct()));
    connect(ui.accountsDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioAcct()));
    connect(ui.accountsShowBlank, SIGNAL(changed()), this, SLOT(resetPortfolioAcct()));
    connect(ui.accountsReorderColumns, SIGNAL(triggered()), this, SLOT(acctModifyColumns()));
    connect(ui.accountsSortCombo, SIGNAL(activated(int)), this, SLOT(acctSortChanged(int)));
    connect(ui.accountsExport, SIGNAL(triggered()), ui.accounts, SLOT(exportTable()));
    connect(ui.accountsCopyShortcut, SIGNAL(activated()), ui.accounts, SLOT(copyTable()));

    connect(ui.statEdit, SIGNAL(triggered()), this, SLOT(editStat()));
    connect(ui.statStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioStat()));
    connect(ui.statEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioStat()));
    connect(ui.statExport, SIGNAL(triggered()), ui.stat, SLOT(exportTable()));
    connect(ui.statCopyShortcut, SIGNAL(activated()), ui.stat, SLOT(copyTable()));

    connect(ui.correlationsShowHidden, SIGNAL(triggered()), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsExport, SIGNAL(triggered()), ui.correlations, SLOT(exportTable()));
    connect(ui.correlationsCopyShortcut, SIGNAL(activated()), ui.correlations, SLOT(copyTable()));

    connect(ui.tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int))); // hack for now, QWT doesn't render the chart correctly the first time
}

void frmMain::showWelcomeMessage()
{
    QString welcomeMessage = "Welcome to My Personal Index!\n\nThere is no documentation yet,"
                             " but I recommend starting in the following way:\n\n1. Set the start date under options (on the top toolbar).\n"
                             "2. Add a new Portfolio\n3. Set your asset allocation \n4. Set your accounts\n5. Add holdings\n"
                             "6. Add relevant portfolio statistics\n7. Update prices!";
    QMessageBox msg(QMessageBox::Information, "My Personal Index", welcomeMessage, QMessageBox::Ok, this);
    QPushButton *copyButton = msg.addButton("Copy To Clipboard", QMessageBox::RejectRole);

    msg.exec();

    if (msg.clickedButton() == copyButton)
        QApplication::clipboard()->setText(welcomeMessage);
}

void frmMain::resetSortDropDowns()
{
    resetSortDropDown(holdingsRow::fieldNames(), ui.holdingsSortCombo);
    resetSortDropDown(aaRow::fieldNames(), ui.aaSortCombo);
    resetSortDropDown(acctRow::fieldNames(), ui.accountsSortCombo);
}

void frmMain::resetSortDropDown(const QMap<int, QString> &fieldNames, QComboBox *dropDown)
{
    dropDown->blockSignals(true);
    dropDown->addItem("(None)", -1);

    for (QMap<int, QString>::const_iterator i = fieldNames.constBegin(); i != fieldNames.constEnd(); ++i)
        dropDown->addItem(i.value(), i.key());
    dropDown->addItem("Custom...", -2);
    dropDown->blockSignals(false);
}

void frmMain::setSortDropDown(const QString &sort, QComboBox *dropDown)
{
    dropDown->blockSignals(true);

    if (sort.isEmpty()) // no sort
        dropDown->setCurrentIndex(0);
    else if (sort.contains('|') || sort.contains('D')) // custom sort
        dropDown->setCurrentIndex(dropDown->count() - 1);
    else
        dropDown->setCurrentIndex(dropDown->findData(sort.toInt()));

    dropDown->blockSignals(false);
}

void frmMain::resetLastDate()
{
    int lastDate = prices::instance().lastDate();
    ui.stbLastUpdated->setText(QString(" %1%2 ").arg(ui.LAST_UPDATED_TEXT,
        lastDate == 0 ? "Never" : QDate::fromJulianDay(lastDate).toString(Qt::SystemLocaleShortDate)));
}

void frmMain::saveSettings()
{
    QVariant portfolio(QVariant::Int);
    if (portfolios.exists(m_portfolioID))
        portfolio = m_portfolioID;

    m_settings.lastPortfolio = portfolio;
    m_settings.windowSize = size();
    m_settings.windowLocation = pos();
    m_settings.state = isMaximized() ? Qt::WindowMaximized : isMinimized() ? Qt::WindowMinimized : Qt::WindowNoState;
    m_settings.save();
}

void frmMain::resetPortfolioDropDown(const int &portfolioID = -1)
{
    ui.mainPortfolioCombo->blockSignals(true);
    ui.mainPortfolioCombo->clear();

    foreach(const portfolioInfo &info, portfolios.info())
        ui.mainPortfolioCombo->addItem(info.description, info.id);

    int row = portfolioID == -1 ? 0 : ui.mainPortfolioCombo->findData(portfolioID);
    ui.mainPortfolioCombo->setCurrentIndex(row);

    ui.mainPortfolioCombo->blockSignals(false);
}

void frmMain::disableItems(bool disabled)
{
    ui.mainToolbar->setDisabled(disabled);
    ui.holdingsToolbar->setDisabled(disabled);
    ui.statToolbar->setDisabled(disabled);
    ui.chartToolbar->setDisabled(disabled);
    ui.performanceToolbar->setDisabled(disabled);
    ui.correlationsToolbar->setDisabled(disabled);
    ui.accountsToolbar->setDisabled(disabled);
    ui.aaToolbar->setDisabled(disabled);
    ui.holdings->blockSignals(disabled);
    ui.aa->blockSignals(disabled);
    ui.accounts->blockSignals(disabled);
}

void frmMain::loadPortfolio()
{
    if (portfolios.isEmpty()) // no portfolios to load
    {
        ui.tab->setDisabled(true);
        return;
    }

    savePortfolio();
    ui.tab->setDisabled(false);

    m_portfolioID = ui.mainPortfolioCombo->itemData(ui.mainPortfolioCombo->currentIndex()).toInt();

    if (!portfolios.exists(m_portfolioID))
    {
        ui.tab->setDisabled(true);
        return;
    }

#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif

    m_calculations.setPortfolio(m_portfolioID);
    resetPortfolioSettings();
    resetCalendars();
    resetPortfolioHoldings();
    resetPortfolioPerformance();
    resetPortfolioChart();
    resetPortfolioAA();
    resetPortfolioAcct();
    resetPortfolioCorrelation();
    resetPortfolioStat();

#ifdef CLOCKTIME
    qDebug("Time elapsed: %d ms (frmMain)", t.elapsed());
#endif

}

void frmMain::resetCalendars()
{
    int date = prices::instance().lastDate();
    if (date == 0)
        date = m_settings.dataStartDate;

    int start = portfolios.startDate(m_portfolioID);
    int end = date < start ? start : date;

    resetCalendar(end, start, ui.holdingsDateDropDown);
    resetCalendar(end, start, ui.aaDateDropDown);
    resetCalendar(end, start, ui.accountsDateDropDown);
    resetCalendar(end, start, ui.chartStartDateDropDown, ui.chartEndDateDropDown);
    resetCalendar(end, start, ui.correlationsStartDateDropDown, ui.correlationsEndDateDropDown);
    resetCalendar(end, start, ui.statStartDateDropDown, ui.statEndDateDropDown);
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

void frmMain::resetPortfolioHoldings()
{
    int currentDate = dateDropDownDate(ui.holdingsDateDropDown);
    QAbstractItemModel *oldModel = ui.holdings->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(currentDate);

    QList<baseRow*> rows;
    foreach(const security &s, portfolios.securities(m_portfolioID))
        if (ui.holdingsShowHidden->isChecked() || !s.hide)
            rows.append(new holdingsRow(s, info, portfolios.acct(m_portfolioID), portfolios.aa(m_portfolioID), portfolios.info(m_portfolioID).holdingsSort));

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainHoldingsModel *model = new mainHoldingsModel(rows, m_settings.viewableColumns.value(settings::columns_Holdings), info, ui.holdings);
    ui.holdings->setModel(model);
    ui.holdings->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(portfolios.info(m_portfolioID).holdingsSort, ui.holdingsSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioAA()
{
    int currentDate = dateDropDownDate(ui.aaDateDropDown);
    QAbstractItemModel *oldModel = ui.aa->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(currentDate);

    QList<baseRow*> rows;
    if (ui.aaShowBlank->isChecked()) // insert blank aa
        portfolios.insert(m_portfolioID, assetAllocation());

    foreach(const assetAllocation &aa, portfolios.aa(m_portfolioID))
        rows.append(new aaRow(info, m_calculations.aaValues(currentDate, aa.id), portfolios.info(m_portfolioID).aaThresholdMethod, aa, portfolios.info(m_portfolioID).aaSort));

    portfolios.remove(m_portfolioID, assetAllocation()); // remove blank aa

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainAAModel *model = new mainAAModel(rows, m_settings.viewableColumns.value(settings::columns_AA), info, portfolios.info(m_portfolioID).aaThreshold, ui.aa);
    ui.aa->setModel(model);
    ui.aa->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(portfolios.info(m_portfolioID).aaSort, ui.aaSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioAcct()
{
    int currentDate = dateDropDownDate(ui.accountsDateDropDown);
    QAbstractItemModel *oldModel = ui.accounts->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(currentDate);

    QList<baseRow*> rows;
    if (ui.accountsShowBlank->isChecked()) // insert blank account
         portfolios.insert(m_portfolioID, account());

    foreach(const account &acct, portfolios.acct(m_portfolioID))
        rows.append(new acctRow(info, m_calculations.acctValues(currentDate, acct.id), acct, portfolios.info(m_portfolioID).acctSort));

    portfolios.remove(m_portfolioID, account()); // remove blank account

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainAcctModel *model = new mainAcctModel(rows, m_settings.viewableColumns.value(settings::columns_Acct), info, ui.accounts);
    ui.accounts->setModel(model);
    ui.accounts->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(portfolios.info(m_portfolioID).acctSort, ui.accountsSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioPerformance()
{
    QAbstractItemModel *oldModel = ui.performance->model();
    mainPerformanceModel *model = new mainPerformanceModel(portfolios.nav(m_portfolioID), ui.performanceSortDesc->isChecked(), portfolios.info(m_portfolioID).startValue, ui.performance);
    ui.performance->setModel(model);
    delete oldModel;
}

void frmMain::resetPortfolioCorrelation()
{
    QAbstractItemModel *oldModel = ui.correlations->model();
    int startDate = dateDropDownDate(ui.correlationsStartDateDropDown);
    int endDate = dateDropDownDate(ui.correlationsEndDateDropDown);

    mainCorrelationModel::correlationList correlations;
    foreach(const security &s, portfolios.securities(m_portfolioID))
        if (ui.correlationsShowHidden->isChecked() || !s.hide)
            correlations.insert(s.symbol, QHash<QString, double>());

    QStringList symbols = correlations.keys();
    correlations.insert(portfolios.info(m_portfolioID).description, QHash<QString, double>());
    symbols.insert(0, portfolios.info(m_portfolioID).description);
    
    int count = symbols.count();
    for(int i = 0; i < count; ++i)
    {
        QString security1 = symbols.at(i);
        QHash<QString, double> &list = correlations[security1];
        securityPrices security1history;

        if (i == 0) // always current portfolio
            security1history.prices = portfolios.nav(m_portfolioID).navHistory();
        else
            security1history = prices::instance().history(security1);

        for (int x = i + 1; x < count; ++x)
            list.insert(symbols.at(x), calculations::correlation(security1history, prices::instance().history(symbols.at(x)), startDate, endDate));
    }

    mainCorrelationModel *model = new mainCorrelationModel(correlations, symbols, ui.correlations);
    ui.correlations->setModel(model);
    ui.correlations->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.correlations->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    delete oldModel;
}

void frmMain::resetPortfolioChart()
{
    const QMap<int, double> nav = portfolios.nav(m_portfolioID).navHistory();
    ui.chart->setTitle(portfolios.info(m_portfolioID).description);

    QwtPlotCurve *newLine = new QwtPlotCurve();
    QPen p(Qt::red); p.setWidth(3);
    newLine->setPen(p);

    m_chartInfo.setCurve(newLine);

    int startDate = dateDropDownDate(ui.chartStartDateDropDown);
    int endDate = dateDropDownDate(ui.chartEndDateDropDown);
    double startValue = -1;
    for(QMap<int, double>::const_iterator i = nav.lowerBound(startDate); i != nav.constEnd(); ++i)
    {
        if (i.key() > endDate)
            break;

        if (startValue == -1)
            startValue = i.value();

        m_chartInfo.append(i.key(), (i.value() / startValue) - 1);
    }

    if (m_chartInfo.count() != 0)
    {
        m_chartInfo.attach(ui.chart);
        ui.chart->setAxisScale(QwtPlot::xBottom, m_chartInfo.firstX(), m_chartInfo.lastX(), 0);
    }
    else
        ui.chart->setAxisScale(QwtPlot::xBottom, portfolios.startDate(m_portfolioID), portfolios.startDate(m_portfolioID), 0);

    ui.chart->setAxisAutoScale(QwtPlot::yLeft);
    ui.chart->replot();
    ui.chartZoomer->setZoomBase();
}

void frmMain::resetPortfolioStat()
{
    int startDate = dateDropDownDate(ui.statStartDateDropDown);
    int endDate = dateDropDownDate(ui.statEndDateDropDown);
    int previousDay = currentDateOrPrevious(startDate - 1);

    QAbstractItemModel *oldModel = ui.stat->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(endDate);
    statisticInfo s(m_portfolioID, info, startDate, previousDay);

    QList<QString> statisticValues;
    foreach(const int &i, m_settings.viewableColumns.value(settings::columns_Stat))
        statisticValues.append(statistic::calculate((statistic::stat)i, s));

    mainStatisticModel *model = new mainStatisticModel(statisticValues, m_settings.viewableColumns.value(settings::columns_Stat), ui.stat);
    ui.stat->setModel(model);
    ui.stat->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.stat->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    delete oldModel;
}

void frmMain::resetPortfolioSettings()
{
    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, QDate::fromJulianDay(portfolios.startDate(m_portfolioID)).toString(Qt::SystemLocaleShortDate)));
    portfolioInfo info = portfolios.info(m_portfolioID);
    ui.holdingsShowHidden->setChecked(info.holdingsShowHidden);
    ui.performanceSortDesc->setChecked(info.navSortDesc);
    ui.aaShowBlank->setChecked(info.aaShowBlank);
    ui.correlationsShowHidden->setChecked(info.correlationShowHidden);
    ui.accountsShowBlank->setChecked(info.acctShowBlank);
}

void frmMain::savePortfolio()
{
    if (!portfolios.exists(m_portfolioID))
        return;

    portfolioInfo info = portfolios.info(m_portfolioID);
    info.holdingsShowHidden = ui.holdingsShowHidden->isChecked();
    info.navSortDesc = ui.performanceSortDesc->isChecked();
    info.aaShowBlank = ui.aaShowBlank->isChecked();
    info.correlationShowHidden = ui.correlationsShowHidden->isChecked();
    info.acctShowBlank = ui.accountsShowBlank->isChecked();

    portfolios.update(info);
}

void frmMain::addPortfolio()
{
    frmPortfolio f(portfolioInfo(), m_settings.dataStartDate, this);
    if (f.exec())
    {
        portfolioInfo p = f.getReturnValues();

        portfolios.insert(p);
        resetPortfolioDropDown(p.id);
        loadPortfolio();
    };
}

void frmMain::editPortfolio()
{
    if (!portfolios.exists(m_portfolioID))
        return;

    portfolioInfo origInfo = portfolios.info(m_portfolioID);
    frmPortfolio f(origInfo, m_settings.dataStartDate, this);
    if (f.exec())
    {
        portfolioInfo info = f.getReturnValues();
        bool reCalcChange = info.startValue != origInfo.startValue
                            || info.startDate != origInfo.startDate
                            || info.dividends != origInfo.dividends;

        if (!reCalcChange && info.description != origInfo.description)
            resetPortfolioChart();

        portfolios.update(info);

        resetSecurityRelatedTabs(reCalcChange ? 0 : -1);
        resetPortfolioDropDown(m_portfolioID);
    }
}

void frmMain::deletePortfolio()
{
    if (!portfolios.exists(m_portfolioID))
        return;

    if (QMessageBox::question(this, "Delete Portfolio?", QString("Are you sure you want to delete %1?").arg(portfolios.info(m_portfolioID).description),
        QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    portfolios.remove(m_portfolioID);
    m_portfolioID = -1;

    deleteUnusedSymbols();

    int row = ui.mainPortfolioCombo->currentIndex(); // select another portfolio
    if (portfolios.count() == row)
        row--;

    resetPortfolioDropDown(-1);
    if (!portfolios.isEmpty())
        ui.mainPortfolioCombo->setCurrentIndex(row);

    if (portfolios.invalidNAVDates())
        beginNAV();
}

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString(VERSIONTEXT) + "</h2>"
        "<p>Copyright &copy; 2009"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}

void frmMain::resetSecurityRelatedTabs(const int &minDate)
{
    if (minDate != -1)
    {
        beginNAV(m_portfolioID, minDate);
        return;
    }

    m_calculations.clearCache();
    resetPortfolioHoldings();
    resetPortfolioAA();
    resetPortfolioAcct();
    resetPortfolioStat();
    resetPortfolioCorrelation();
}

void frmMain::addSecurity()
{
    int minDate = -1;
    int resultcode;
    bool change = false;
    bool showUpdatePrices = false;

    do
    {
        frmSecurity f(m_portfolioID, security(), this);
        resultcode = f.exec();
        if (resultcode >= QDialog::Accepted)
        {
            change = true;
            security s = f.getReturnValuesSecurity();
            portfolios.insert(m_portfolioID, s);
            minDate = securityMinDate(minDate, f.getReturnValuesMinDate());

            if (!s.cashAccount && !prices::instance().symbols().contains(s.symbol))
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

    resetSecurityRelatedTabs(minDate);
}

void frmMain::editSecurity()
{
    bool change = false;
    int minDate = -1;
    foreach(baseRow *row, static_cast<mainHoldingsModel*>(ui.holdings->model())->selectedItems())
    {
        int securityID = row->values.at(holdingsRow::row_ID).toInt();
        frmSecurity f(m_portfolioID, portfolios.securities(m_portfolioID, securityID), this);
        if (f.exec())
        {
            change = true;
            portfolios.insert(m_portfolioID, f.getReturnValuesSecurity());
            minDate = securityMinDate(minDate, f.getReturnValuesMinDate());
        }
    }

    if (!change)
        return;

    resetSecurityRelatedTabs(minDate);
}

void frmMain::deleteSecurity()
{
    QStringList securities = selectedRows(holdingsRow::row_Symbol, static_cast<mpiViewModelBase*>(ui.holdings->model()));
    if (securities.isEmpty())
        return;

    if (QMessageBox::question(this, "Delete securities", QString("Are you sure you want to delete the following securities: %1?").arg(securities.join(", ")),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    int minDate = -1;
    foreach(baseRow *row, static_cast<mpiViewModelBase*>(ui.holdings->model())->selectedItems())
    {
        security s = portfolios.securities(m_portfolioID, row->values.at(holdingsRow::row_ID).toInt());
        minDate = securityMinDate(minDate, s.firstTradeDate());
        portfolios.remove(m_portfolioID, s);
    }

    deleteUnusedSymbols();
    if (portfolios.invalidNAVDates())
        beginNAV();
    else
        resetSecurityRelatedTabs(minDate);
}

void frmMain::deleteUnusedSymbols()
{
    prices::instance().remove(functions::inLeftNotRight(prices::instance().symbols(), portfolios.symbols()));
    resetLastDate();
}

void frmMain::options()
{
    frmOptions f(m_settings, this);
    if (f.exec())
    {
        if (f.getReturnValues().dataStartDate == m_settings.dataStartDate)
        {
            m_settings = f.getReturnValues();
            return;
        }

        m_settings = f.getReturnValues();
        prices::instance().remove(prices::instance().symbols());
        foreach(portfolioInfo info, portfolios.info())
        {
            if (info.startDate < m_settings.dataStartDate)
            {
                info.startDate = m_settings.dataStartDate;
                portfolios.update(info);
            }
            portfolios.removeNAV(info.id);
            portfolios.removeExecutedTrades(info.id);
        }

        if (QMessageBox::question(this, "Update Prices", "Would you like to update prices from the new data start date?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            beginUpdate();
        else
            beginNAV();
    }
}

void frmMain::addAA()
{
    frmAAEdit f(m_portfolioID, this);
    if (f.exec())
    {
        portfolios.insert(m_portfolioID, f.getReturnValues());
        resetSecurityRelatedTabs(-1);
    }
}

int frmMain::securityMinDate(int currentMinDate, const int &firstTradeDate)
{
    if (firstTradeDate != -1 && (firstTradeDate < currentMinDate || currentMinDate == -1))
        currentMinDate = firstTradeDate;
    return currentMinDate;
}

int frmMain::aaMinDate(const int &aaID, int currentMinDate)
{
    foreach(const security &s, portfolios.securities(m_portfolioID))
        if(s.aa.contains(aaID))
            foreach(const trade &t, s.trades)
                if (t.type == trade::tradeType_AA)
                {
                    currentMinDate = securityMinDate(currentMinDate, s.firstTradeDate());
                    break;
                }

    return currentMinDate;
}

void frmMain::editAA()
{    
    bool change = false;
    int minDate = -1;
    foreach(baseRow *row, static_cast<mainAAModel*>(ui.aa->model())->selectedItems())
    {
        int aaID = row->values.at(aaRow::row_ID).toInt();
        if (aaID == -1)
            continue;

        frmAAEdit f(m_portfolioID, this, portfolios.aa(m_portfolioID, aaID));
        if (f.exec())
        {
            change = true;
            portfolios.insert(m_portfolioID, f.getReturnValues());
            minDate = aaMinDate(aaID, minDate);
        }
    }

    if (!change)
        return;

    resetSecurityRelatedTabs(minDate);
}

QStringList frmMain::selectedRows(const int &column, mpiViewModelBase *model)
{
    QStringList returnList;
    foreach(baseRow *row, model->selectedItems())
        returnList.append(row->values.at(column).toString());
    return returnList;
}

void frmMain::deleteAA()
{
    QStringList selectedAA = selectedRows(aaRow::row_Description, static_cast<mpiViewModelBase*>(ui.aa->model()));
    if (selectedAA.isEmpty())
        return;

    if (QMessageBox::question(this, "Delete asset allocation", QString("Are you sure you want to delete the following asset allocations: %1?").arg(selectedAA.join(", ")),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    int minDate = -1;
    foreach(baseRow *row, static_cast<mpiViewModelBase*>(ui.aa->model())->selectedItems())
    {
        assetAllocation aa = portfolios.aa(m_portfolioID, row->values.at(aaRow::row_ID).toInt());
        minDate = aaMinDate(aa.id, minDate);
        portfolios.remove(m_portfolioID, aa);
    }

    resetSecurityRelatedTabs(minDate);
}

void frmMain::addAcct()
{
    frmAcctEdit f(m_portfolioID, this);
    if(f.exec())
    {
        account acct = f.getReturnValues();
        portfolios.insert(m_portfolioID, acct);

        resetSecurityRelatedTabs(-1);
    }
}

void frmMain::editAcct()
{
    bool change = false;
    foreach(baseRow *row, static_cast<mainAcctModel*>(ui.accounts->model())->selectedItems())
    {
        int accountID = row->values.at(acctRow::row_ID).toInt();
        if (accountID == -1)
            continue;

        frmAcctEdit f(m_portfolioID, this, portfolios.acct(m_portfolioID, accountID));
        if (f.exec())
        {
            change = true;
            portfolios.insert(m_portfolioID, f.getReturnValues());
        }
    }

    if (!change)
        return;

    resetSecurityRelatedTabs(-1);
}

void frmMain::deleteAcct()
{
    QStringList selectedAcct = selectedRows(acctRow::row_Description, static_cast<mpiViewModelBase*>(ui.accounts->model()));
    if (selectedAcct.isEmpty())
        return;

    if (QMessageBox::question(this, "Delete account", QString("Are you sure you want to delete the following accounts: %1?").arg(selectedAcct.join(", ")),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    foreach(baseRow *row, static_cast<mainAcctModel*>(ui.accounts->model())->selectedItems())
    {
        account acct = portfolios.acct(m_portfolioID, row->values.at(acctRow::row_ID).toInt());
        portfolios.remove(m_portfolioID, acct);
    }

    resetSecurityRelatedTabs(-1);
}

void frmMain::beginUpdate()
{
    disableItems(true);

    if (!updatePrices::isInternetConnection())
    {
        QMessageBox::critical(this, "Update Error", "Cannot contact Yahoo! Finance, please check your internet connection.");
        disableItems(false);
        return;
    }

    m_calculationInProgress = true;
    statusUpdate("Updating Prices...");
    ui.stbProgress->setMaximum(0);

    QFutureWatcher<void> w;
    QEventLoop q;

    connect(&w, SIGNAL(finished()), &q, SLOT(quit()));
    updatePrices updateThread(m_settings);
    QFuture<updatePricesReturnValue> future = QtConcurrent::run(&updateThread, &updatePrices::run);
    w.setFuture(future);

    q.exec();

    if (future.result().earliestUpdate <= QDate::currentDate().toJulianDay())
        beginNAV(-1, future.result().earliestUpdate);
    else
        if (finishThread() && !future.result().updateFailures.isEmpty())
            QMessageBox::information(this,
                "Update Error", "The following securities were not updated (Yahoo! Finance may not yet have today's price):\n\n" +
                future.result().updateFailures.join(", "));
}

void frmMain::beginNAV(const int &portfolioID, const int &minDate)
{
    m_calculationInProgress = true;
    disableItems(true);
    statusUpdate("Calculating Portfolios...");
    ui.stbProgress->setMaximum(0);

    QFutureWatcher<void> w;
    QEventLoop q;

    connect(&w, SIGNAL(finished()), &q, SLOT(quit()));
    nav navThread(minDate, portfolioID);
    QFuture<void> future = QtConcurrent::run(&navThread, &nav::run);

    w.setFuture(future);
    q.exec();
    finishThread();
}

bool frmMain::finishThread()
{
    m_calculationInProgress = false;
    if (this->windowTitle().contains("task"))
    {
        this->close();
        return false;
    }

    loadPortfolio();
    disableItems(false);
    statusUpdate("");
    ui.stbProgress->setMaximum(100);
    ui.stbProgress->setValue(0); 
    return true;
}

void frmMain::statusUpdate(const QString &message)
{
    ui.stbStatus->setText(QString("Status: ").append(message));
}

int frmMain::currentDateOrPrevious(int date)
{
    const QList<int> dates = prices::instance().dates();
    QList<int>::const_iterator place = qLowerBound(dates, date);
    if (*place != date && place != dates.constBegin())
        return *(place - 1);
    else
        return *place;
}

int frmMain::dateDropDownDate(QDateEdit *dateDropDown)
{
    int currentDate = qMax(currentDateOrPrevious(dateDropDown->date().toJulianDay()), portfolios.startDate(m_portfolioID));
    dateDropDown->blockSignals(true);
    dateDropDown->setDate(QDate::fromJulianDay(currentDate));
    dateDropDown->blockSignals(false);
    return currentDate;
}

bool frmMain::modifyColumns(const int &columnID, const QMap<int, QString> &fieldNames, const QString &title)
{
    frmColumns f(m_settings.viewableColumns.value(columnID), fieldNames, title, QDialog::Rejected, this);
    if (f.exec())
    {
        m_settings.viewableColumns[columnID] = f.getReturnValues();
        settings::saveColumns(columnID, m_settings.viewableColumns[columnID]);
        return true;
    }
    return false;
}

void frmMain::holdingsModifyColumns()
{
    if (modifyColumns(settings::columns_Holdings, holdingsRow::fieldNames(), "Modify Columns"))
        resetPortfolioHoldings();
}

void frmMain::aaModifyColumns()
{
    if (modifyColumns(settings::columns_AA, aaRow::fieldNames(), "Modify Columns"))
        resetPortfolioAA();
}

void frmMain::acctModifyColumns()
{
    if (modifyColumns(settings::columns_Acct, acctRow::fieldNames(), "Modify Columns"))
        resetPortfolioHoldings();
}

void frmMain::editStat()
{
    if (modifyColumns(settings::columns_Stat, statistic::statisticList(), "Modify Statistics"))
        resetPortfolioStat();
}

void frmMain::sortDropDownChange(int columnID, QString *sortString, const QMap<int, QString> &fieldNames)
{
    if (columnID == -1)
    {
        sortString->clear();
        return;
    }

    if (columnID != -2)
    {
        *sortString = QString::number(columnID);
        return;
    }

    frmSort f(*sortString, fieldNames, this);

    if (f.exec())
        *sortString = f.getReturnValues();
}

void frmMain::holdingsSortChanged(int index)
{
    portfolioInfo info = portfolios.info(m_portfolioID);
    sortDropDownChange(ui.holdingsSortCombo->itemData(index).toInt(), &info.holdingsSort, holdingsRow::fieldNames());
    portfolios.update(info);
    resetPortfolioHoldings();
}

void frmMain::aaSortChanged(int index)
{
    portfolioInfo info = portfolios.info(m_portfolioID);
    sortDropDownChange(ui.aaSortCombo->itemData(index).toInt(), &info.aaSort, aaRow::fieldNames());
    portfolios.update(info);
    resetPortfolioAA();
}

void frmMain::acctSortChanged(int index)
{
    portfolioInfo info = portfolios.info(m_portfolioID);
    sortDropDownChange(ui.accountsSortCombo->itemData(index).toInt(), &info.acctSort, acctRow::fieldNames());
    portfolios.update(info);
    resetPortfolioAcct();
}

void frmMain::import()
{

//    struct portfolioMapping
//    {
//        int portfolioID;
//        int id;
//
//        portfolioMapping(const int &p_portfolioID, const int &p_id): portfolioID(p_portfolioID), id(p_id) {}
//    };

//    QList<portfolioMapping> selectedSecurities;
//    QList<portfolioMapping> selectedAccounts;
//    QList<portfolioMapping> selectedAA;
//
//    QMap<int, portfolioMapping> securitiesMapping;
//    QMap<int, QString> securities;
//    int i = 0;
//    foreach(portfolio *p, m_portfolios)
//        foreach(const security s, p->data.securities)
//        {
//            securitiesMapping.insert(i, portfolioMapping(p->info.id, s.id));
//            securities.insert(i, QString("%1: %2").arg(p->info.description, s.symbol));
//            ++i;
//        }
//
//    frmColumns f(QList<int>(), securities, "Import Securities", QDialog::Accepted, this);
//    if (f.exec())
//    {
//        foreach(const int &x, f.getReturnValues())
//            selectedSecurities.append(securitiesMapping.value(x));
//    }
//    else
//        return;
//
//
//    QMap<int, portfolioMapping> accountMapping;
//    QMap<int, QString> accounts;
//    QList<int> accountsToInclude;
//    i = 0;

//    foreach(const portfolioMapping &x, selectedSecurities)
//    {
//        int acctToAdd = m_portfolios.value(x.portfolioID)->data.securities.value(x).account;
//        if (acctToAdd != -1)
//            if (!accountsToInclude.contains(acctToAdd))
//                accountsToInclude.append(acctToAdd);
//    }
//
//    foreach(portfolio *p, m_portfolios)
//        foreach(const account acct, p->data.acct)
//        {
//            accountMapping.insert(i, acct.id);
//            accounts.insert(i, QString("%1: %2").arg(p->info.description, acct.description));
//            if (accountsToInclude.contains(acct.id))
//                selectedAccounts.append(i);
//            ++i;
//        }
//
//    frmColumns f2(selectedAccounts, accounts, "Import Accounts", QDialog::Accepted, this);
//    if (f2.exec())
//    {
//        selectedAccounts.clear();
//        foreach(int x, f2.getReturnValues())
//            selectedAccounts.append(accountMapping.value(x));
//    }
//    else
//        return;
//
//    QMap<int, int> aaMapping;
//    QMap<int, QString> assetAllocations;
//    QList<int> aaToInclude;
//    i = 0;
//
//    foreach(const int &x, selectedSecurities)
//        foreach(portfolio *p, m_portfolios)
//            if (p->data.securities.contains(x))
//                foreach(const int &z, p->data.securities.value(x).aa.keys())
//                    if (!aaToInclude.contains(z))
//                        aaToInclude.append(z);
//
//    foreach(portfolio *p, m_portfolios)
//        foreach(const assetAllocation aa, p->data.aa)
//        {
//            aaMapping.insert(i, aa.id);
//            assetAllocations.insert(i, QString("%1: %2").arg(p->info.description, aa.description));
//            if (aaToInclude.contains(aa.id))
//                selectedAA.append(i);
//            ++i;
//        }
//
//    frmColumns f3(selectedAA, assetAllocations, "Import Asset Allocations", QDialog::Accepted, this);
//    if (f3.exec())
//    {
//        selectedAA.clear();
//        foreach(int x, f3.getReturnValues())
//            selectedAA.append(aaMapping.value(x));
//    }
//    else
//        return;
//
//    securitiesMapping.clear();
//    aaMapping.clear();
//    accountMapping.clear();
//
//    foreach(const int &x, selectedAccounts)

}
