#include "frmMain.h"
#include "frmPortfolio.h"
#include "frmSecurity.h"
#include "frmOptions.h"
#include "frmAAEdit.h"
#include "frmAcctEdit.h"
#include "frmColumns.h"
#include "frmSort.h"
#include "mainPerformanceModel.h"
#include "mainCorrelationModel.h"
#include "mainStatisticModel.h"

frmMain::frmMain(QWidget *parent): QMainWindow(parent), m_currentPortfolio(0), m_updateThread(0), m_navThread(0)
{
    ui.setupUI(this);

    m_portfolios = portfolio::loadPortfolios();
    m_settings = settings::loadSettings();

    if (m_settings.state != Qt::WindowActive)
    {
        move(m_settings.windowLocation);
        resize(m_settings.windowSize);
        if (m_settings.state != Qt::WindowNoState)
            this->setWindowState(this->windowState() | m_settings.state);
    }
    else
        functions::showWelcomeMessage(this); // first time being run

    resetSortDropDowns();
    resetLastDate();
    resetPortfolioDropDown(m_settings.lastPortfolio.isNull() ? -1 : m_settings.lastPortfolio.toInt());
    loadPortfolio();
    connectSlots();
}

frmMain::~frmMain()
{
    qDeleteAll(m_portfolios);
    delete ui.holdings->model();
    delete ui.aa->model();
    delete ui.accounts->model();
    delete ui.correlations->model();
    delete ui.performance->model();
}

void frmMain::closeEvent(QCloseEvent *event)
{
    if (m_updateThread || m_navThread)
    {
        this->setWindowTitle(QString("%1 - %2").arg(ui.WINDOW_TITLE, ui.BUSY));
        event->ignore();
        return;
    }

    savePortfolio();
    savePortfolios();
    saveSettings();
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

    connect(ui.holdingsAdd, SIGNAL(triggered()), this, SLOT(addSecurity()));
    connect(ui.holdingsEdit, SIGNAL(triggered()), this, SLOT(editSecurity()));
    connect(ui.holdingsDelete, SIGNAL(triggered()), this, SLOT(deleteSecurity()));
    connect(ui.holdings, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editSecurity()));
    connect(ui.holdingsDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioHoldings()));
    connect(ui.holdingsShowHidden, SIGNAL(changed()), this, SLOT(resetPortfolioHoldings()));
    connect(ui.holdingsReorderColumns, SIGNAL(triggered()), this, SLOT(holdingsModifyColumns()));
    connect(ui.holdingsSortCombo, SIGNAL(activated(int)), this, SLOT(holdingsSortChanged(int)));
    connect(ui.holdingsExport, SIGNAL(triggered()), this, SLOT(holdingsExport()));

    connect(ui.performanceSortDesc, SIGNAL(triggered()), this, SLOT(resetPortfolioPerformance()));
    connect(ui.performanceExport, SIGNAL(triggered()), this, SLOT(performanceExport()));

    connect(ui.chartEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioChart()));
    connect(ui.chartStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioChart()));
    connect(ui.chartExport, SIGNAL(triggered()), this, SLOT(chartExport()));

    connect(ui.aaAdd, SIGNAL(triggered()), this, SLOT(addAA()));
    connect(ui.aaEdit, SIGNAL(triggered()), this, SLOT(editAA()));
    connect(ui.aa, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editAA()));
    connect(ui.aaDelete, SIGNAL(triggered()), this, SLOT(deleteAA()));
    connect(ui.aaDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioAA()));
    connect(ui.aaShowBlank, SIGNAL(changed()), this, SLOT(resetPortfolioAA()));
    connect(ui.aaReorderColumns, SIGNAL(triggered()), this, SLOT(aaModifyColumns()));
    connect(ui.aaSortCombo, SIGNAL(activated(int)), this, SLOT(aaSortChanged(int)));
    connect(ui.aaExport, SIGNAL(triggered()), this, SLOT(aaExport()));

    connect(ui.accountsAdd, SIGNAL(triggered()), this, SLOT(addAcct()));
    connect(ui.accountsEdit, SIGNAL(triggered()), this, SLOT(editAcct()));
    connect(ui.accounts, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editAcct()));
    connect(ui.accountsDelete, SIGNAL(triggered()), this, SLOT(deleteAcct()));
    connect(ui.accountsDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioAcct()));
    connect(ui.accountsShowBlank, SIGNAL(changed()), this, SLOT(resetPortfolioAcct()));
    connect(ui.accountsReorderColumns, SIGNAL(triggered()), this, SLOT(acctModifyColumns()));
    connect(ui.accountsSortCombo, SIGNAL(activated(int)), this, SLOT(acctSortChanged(int)));
    connect(ui.accountsExport, SIGNAL(triggered()), this, SLOT(acctExport()));

    connect(ui.statEdit, SIGNAL(triggered()), this, SLOT(editStat()));
    connect(ui.statStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioStat()));
    connect(ui.statEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioStat()));
    connect(ui.statExport, SIGNAL(triggered()), this, SLOT(statExport()));

    connect(ui.correlationsShowHidden, SIGNAL(triggered()), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsStartDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsEndDateDropDown, SIGNAL(dateChanged(QDate)), this, SLOT(resetPortfolioCorrelation()));
    connect(ui.correlationsExport, SIGNAL(triggered()), this, SLOT(correlationExport()));
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
    dropDown->addItem("", -1);

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
    if (m_currentPortfolio)
        portfolio = m_currentPortfolio->info.id;

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

    foreach(const portfolio *p, m_portfolios)
        ui.mainPortfolioCombo->addItem(p->info.description, p->info.id);

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
}

void frmMain::loadPortfolio()
{
    if (m_portfolios.isEmpty()) // no portfolios to load
    {
        ui.tab->setDisabled(true);
        return;
    }

    savePortfolio();
    ui.tab->setDisabled(false);

    m_currentPortfolio = m_portfolios.value(ui.mainPortfolioCombo->itemData(ui.mainPortfolioCombo->currentIndex()).toInt());

    QTime t;
    t.start();

    m_calculations.setPortfolio(m_currentPortfolio);
    resetPortfolioSettings();
    resetCalendars();
    resetPortfolioHoldings();
    resetPortfolioPerformance();
    resetPortfolioChart();
    resetPortfolioAA();
    resetPortfolioAcct();
    resetPortfolioCorrelation();
    resetPortfolioStat();

    qDebug("Time elapsed: %d ms (frmMain)", t.elapsed());
}

void frmMain::resetCalendars()
{
    int date = prices::instance().lastDate();
    if (date == 0)
        date = m_settings.dataStartDate;

    int start = m_currentPortfolio->info.startDate;
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
    foreach(const security &s, m_currentPortfolio->data.securities)
        if (ui.holdingsShowHidden->isChecked() || !s.hide)
            rows.append(new holdingsRow(s, info, m_currentPortfolio->data.acct, m_currentPortfolio->data.aa, m_currentPortfolio->info.holdingsSort));

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainHoldingsModel *model = new mainHoldingsModel(rows, m_settings.viewableColumns.value(settings::columns_Holdings), info, ui.holdings);
    ui.holdings->setModel(model);
    ui.holdings->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(m_currentPortfolio->info.holdingsSort, ui.holdingsSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioAA()
{
    int currentDate = dateDropDownDate(ui.aaDateDropDown);
    QAbstractItemModel *oldModel = ui.aa->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(currentDate);

    QList<baseRow*> rows;
    if (ui.aaShowBlank->isChecked()) // insert blank aa
        m_currentPortfolio->data.aa.insert(-1, assetAllocation("Blank"));

    foreach(const assetAllocation &aa, m_currentPortfolio->data.aa)
        rows.append(new aaRow(info, m_calculations.aaValues(currentDate, aa.id), m_currentPortfolio->info.aaThresholdMethod, aa, m_currentPortfolio->info.aaSort));

    m_currentPortfolio->data.aa.remove(-1); // remove blank aa

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainAAModel *model = new mainAAModel(rows, m_settings.viewableColumns.value(settings::columns_AA), info, m_currentPortfolio->info.aaThreshold, ui.aa);
    ui.aa->setModel(model);
    ui.aa->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(m_currentPortfolio->info.aaSort, ui.aaSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioAcct()
{
    int currentDate = dateDropDownDate(ui.accountsDateDropDown);
    QAbstractItemModel *oldModel = ui.accounts->model();
    dailyInfoPortfolio *info = m_calculations.portfolioValues(currentDate);

    QList<baseRow*> rows;
    if (ui.accountsShowBlank->isChecked()) // insert blank acct
        m_currentPortfolio->data.acct.insert(-1, account("(Blank)"));

    foreach(const account &acct, m_currentPortfolio->data.acct)
        rows.append(new acctRow(info, m_calculations.acctValues(currentDate, acct.id), acct, m_currentPortfolio->info.acctSort));

    m_currentPortfolio->data.acct.remove(-1); // remove blank aa

    qStableSort(rows.begin(), rows.end(), baseRow::baseRowSort);

    mainAcctModel *model = new mainAcctModel(rows, m_settings.viewableColumns.value(settings::columns_Acct), info, ui.accounts);
    ui.accounts->setModel(model);
    ui.accounts->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setSortDropDown(m_currentPortfolio->info.acctSort, ui.accountsSortCombo);
    delete oldModel;
}

void frmMain::resetPortfolioPerformance()
{
    QAbstractItemModel *oldModel = ui.performance->model();
    mainPerformanceModel *model = new mainPerformanceModel(m_currentPortfolio->data.nav, ui.performanceSortDesc->isChecked(), m_currentPortfolio->info.startValue, ui.performance);
    ui.performance->setModel(model);
    delete oldModel;
}

void frmMain::resetPortfolioCorrelation()
{
    QAbstractItemModel *oldModel = ui.correlations->model();
    int startDate = dateDropDownDate(ui.correlationsStartDateDropDown);
    int endDate = dateDropDownDate(ui.correlationsEndDateDropDown);

    mainCorrelationModel::correlationList correlations;
    foreach(const security &s, m_currentPortfolio->data.securities)
        if (ui.correlationsShowHidden->isChecked() || !s.hide)
            correlations.insert(s.symbol, QHash<QString, double>());

    QStringList symbols = correlations.keys();
    correlations.insert(QString(m_currentPortfolio->info.description).append(mainCorrelationModel::portfolioIndicator), QHash<QString, double>());
    symbols.insert(0, QString(m_currentPortfolio->info.description).append(mainCorrelationModel::portfolioIndicator));
    
    int count = symbols.count();
    for(int i = 0; i < count; ++i)
    {
        QString security1 = symbols.at(i);
        QHash<QString, double> &list = correlations[security1];
        securityPrices security1history;

        if (i == 0) // always current portfolio
            security1history.prices = m_currentPortfolio->data.nav.navHistory();
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
    const QMap<int, double> nav = m_currentPortfolio->data.nav.navHistory();
    ui.chart->setTitle(m_currentPortfolio->info.description);

    QwtPlotCurve *newLine = new QwtPlotCurve();
    newLine->setCurveAttribute(QwtPlotCurve::Fitted, true);
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

        m_chartInfo.append(i.key(), (i.value() / startValue * 100) - 100);
    }

    if (m_chartInfo.count() != 0)
    {
        m_chartInfo.attach(ui.chart);
        ui.chart->setAxisScale(QwtPlot::xBottom, m_chartInfo.firstX(), m_chartInfo.lastX(), 0);
    }
    else
        ui.chart->setAxisScale(QwtPlot::xBottom, m_currentPortfolio->info.startDate, m_currentPortfolio->info.startDate, 0);

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
    statisticInfo s(m_currentPortfolio, info, startDate, previousDay);

    QList<QString> statisticValues;
    foreach(const int &i, m_currentPortfolio->data.stats)
        statisticValues.append(statistic::calculate((statistic::stat)i, s));

    mainStatisticModel *model = new mainStatisticModel(statisticValues, m_currentPortfolio->data.stats, ui.stat);
    ui.stat->setModel(model);
    ui.stat->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui.stat->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    delete oldModel;
}

void frmMain::resetPortfolioSettings()
{
    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, QDate::fromJulianDay(m_currentPortfolio->info.startDate).toString(Qt::SystemLocaleShortDate)));
    ui.holdingsShowHidden->setChecked(m_currentPortfolio->info.holdingsShowHidden);
    ui.performanceSortDesc->setChecked(m_currentPortfolio->info.navSortDesc);
    ui.aaShowBlank->setChecked(m_currentPortfolio->info.aaShowBlank);
    ui.correlationsShowHidden->setChecked(m_currentPortfolio->info.correlationShowHidden);
    ui.accountsShowBlank->setChecked(m_currentPortfolio->info.acctShowBlank);
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
    foreach(portfolio *p, m_portfolios)
        p->info.save();
}

void frmMain::addPortfolio()
{
    frmPortfolio f(portfolioInfo(), m_settings.dataStartDate, this);
    if (f.exec())
    {
        portfolioInfo p = f.getReturnValues();

        m_portfolios.insert(p.id, new portfolio(p));
        resetPortfolioDropDown(p.id);
        loadPortfolio();
    };
}

void frmMain::editPortfolio()
{
    if (!m_currentPortfolio)
        return;

    frmPortfolio f(m_currentPortfolio->info, m_settings.dataStartDate, this);
    if (f.exec())
    {
        portfolioInfo info = f.getReturnValues();
        bool reCalcChange = info.startValue != m_currentPortfolio->info.startValue
                            || info.startDate != m_currentPortfolio->info.startDate
                            || info.dividends != m_currentPortfolio->info.dividends;

        if (!reCalcChange && info.description != m_currentPortfolio->info.description)
            resetPortfolioChart();

        m_currentPortfolio->info = info;

        resetSecurityRelatedTabs(reCalcChange ? 0 : -1);
        resetPortfolioDropDown(m_currentPortfolio->info.id);
    }
}

void frmMain::deletePortfolio()
{
    if (!m_currentPortfolio)
        return;

    if (QMessageBox::question(this, "Delete Portfolio?", QString("Are you sure you want to delete %1?").arg(m_currentPortfolio->info.description),
        QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    m_portfolios.remove(m_currentPortfolio->info.id);
    m_currentPortfolio->remove();
    delete m_currentPortfolio;
    m_currentPortfolio = 0;

    deleteUnusedSymbols();

    int row = ui.mainPortfolioCombo->currentIndex(); // select another portfolio
    if (m_portfolios.count() == row)
        row--;

    resetPortfolioDropDown(-1);
    if (!m_portfolios.isEmpty())
        ui.mainPortfolioCombo->setCurrentIndex(row);

    if (invalidNAVDates())
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
        beginNAV(m_currentPortfolio->info.id, minDate);
        return;
    }

    m_calculations.clearCache();
    resetPortfolioHoldings();
    resetPortfolioAA();
    resetPortfolioAcct();
    resetPortfolioStat();
}

void frmMain::addSecurity()
{
    int minDate = -1;
    int resultcode;
    bool change = false;
    bool showUpdatePrices = false;

    do
    {
        frmSecurity f(m_currentPortfolio->info.id, m_portfolios, security(), this);
        resultcode = f.exec();
        if (resultcode >= QDialog::Accepted)
        {
            change = true;
            security s = f.getReturnValuesSecurity();
            m_currentPortfolio->data.securities[s.id] = s;
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
        frmSecurity f(m_currentPortfolio->info.id, m_portfolios, m_currentPortfolio->data.securities.value(securityID), this);
        if (f.exec())
        {
            change = true;
            m_currentPortfolio->data.securities[securityID] = f.getReturnValuesSecurity();
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
        security s = m_currentPortfolio->data.securities.value(row->values.at(holdingsRow::row_ID).toInt());
        minDate = securityMinDate(minDate, s.firstTradeDate());
        s.remove();
        m_currentPortfolio->data.securities.remove(s.id);
    }

    deleteUnusedSymbols();
    if (invalidNAVDates())
        beginNAV();
    else
        resetSecurityRelatedTabs(minDate);
}

void frmMain::deleteUnusedSymbols()
{
    QStringList symbols;
    foreach(portfolio* p, m_portfolios)
        foreach(const security &s, p->data.securities)
            symbols.append(s.symbol);
    symbols.removeDuplicates();

    prices::instance().remove(functions::exceptLeft(prices::instance().symbols(), symbols));
    resetLastDate();
}

bool frmMain::invalidNAVDates()
{
    int firstDate = prices::instance().firstDate();
    if (firstDate == 0)
        return true;

    foreach(portfolio *p, m_portfolios)
        if (!p->data.nav.isEmpty() && p->data.nav.firstDate() < firstDate)
            return true;

    return false;
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
        foreach(portfolio *p, m_portfolios)
        {
            if (p->info.startDate < m_settings.dataStartDate)
            {
                p->info.startDate = m_settings.dataStartDate;
                p->info.save();
            }
            p->data.nav.remove(p->info.id);
            p->data.executedTrades.remove(p->info.id);
        }

        if (QMessageBox::question(this, "Update Prices", "Would you like to update prices from the new data start date?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            beginUpdate();
        else
            beginNAV();
    }
}

void frmMain::addAA()
{
    frmAAEdit f(m_currentPortfolio->info.id, this);
    if (f.exec())
    {
        assetAllocation aa = f.getReturnValues();
        m_currentPortfolio->data.aa[aa.id] = aa;

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
    foreach(const security &s, m_currentPortfolio->data.securities)
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
        frmAAEdit f(m_currentPortfolio->info.id, this, m_currentPortfolio->data.aa.value(aaID));
        if (f.exec())
        {
            change = true;
            m_currentPortfolio->data.aa[aaID] = f.getReturnValues();
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
        assetAllocation aa = m_currentPortfolio->data.aa.value(row->values.at(aaRow::row_ID).toInt());
        for(QMap<int, security>::iterator i = m_currentPortfolio->data.securities.begin(); i != m_currentPortfolio->data.securities.end(); ++i)
            i->removeAATarget(aa.id);
        minDate = aaMinDate(aa.id, minDate);
        aa.remove();
        m_currentPortfolio->data.aa.remove(aa.id);
    }

    resetSecurityRelatedTabs(minDate);
}

void frmMain::addAcct()
{
    frmAcctEdit f(m_currentPortfolio->info.id, this);
    if(f.exec())
    {
        account acct = f.getReturnValues();
        m_currentPortfolio->data.acct[acct.id] = acct;

         resetSecurityRelatedTabs(-1);
    }
}

void frmMain::editAcct()
{
    bool change = false;
    foreach(baseRow *row, static_cast<mainAcctModel*>(ui.accounts->model())->selectedItems())
    {
        int accountID = row->values.at(acctRow::row_ID).toInt();
        frmAcctEdit f(m_currentPortfolio->info.id, this, m_currentPortfolio->data.acct.value(accountID));
        if (f.exec())
        {
            change = true;
            m_currentPortfolio->data.acct[accountID] = f.getReturnValues();
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
        account acct = m_currentPortfolio->data.acct.value(row->values.at(acctRow::row_ID).toInt());
        for(QMap<int, security>::iterator i = m_currentPortfolio->data.securities.begin(); i != m_currentPortfolio->data.securities.end(); ++i)
            i->removeAccount(acct.id, m_currentPortfolio->info.id);
        acct.remove();
        m_currentPortfolio->data.acct.remove(acct.id);
    }

    resetSecurityRelatedTabs(-1);
}

void frmMain::editStat()
{
    frmColumns f(m_currentPortfolio->data.stats, statistic::statisticList(), this);
    if (f.exec())
    {
        m_currentPortfolio->data.stats = f.getReturnValues();
        statistic::saveSelectedStats(m_currentPortfolio->info.id, m_currentPortfolio->data.stats);
        resetPortfolioStat();
    }
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

    ui.stbProgress->setMaximum(0);
    m_updateThread = new updatePrices(m_portfolios, m_settings, this);
    connect(m_updateThread, SIGNAL(updateFinished(QStringList)), this, SLOT(finishUpdate(QStringList)), Qt::QueuedConnection);
    connect(m_updateThread, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdate(QString)), Qt::QueuedConnection);
    m_updateThread->start();
}

void frmMain::finishUpdate(const QStringList &invalidSecurities)
{
    ui.stbProgress->setMaximum(100);
    ui.stbProgress->setValue(0);
    statusUpdate("");

    if (!invalidSecurities.isEmpty())
        QMessageBox::information(this,
            "Update Error", "The following securities were not updated (Yahoo! Finance may not yet have today's price):\n\n" +
            invalidSecurities.join(", "));

    m_updateThread->quit();
    m_updateThread->wait();
    m_updateThread->disconnect();
    delete m_updateThread;
    m_updateThread = 0;

    if (this->windowTitle().contains("task"))
    {
        this->close();
        return;
    }

    loadPortfolio();
    disableItems(false);
}

void frmMain::beginNAV(const int &portfolioID, const int &minDate)
{
    disableItems(true);
    ui.stbProgress->setMaximum(0);
    m_navThread = new nav(m_portfolios, minDate, this, portfolioID);
    connect(m_navThread, SIGNAL(calculationFinished()), this, SLOT(finishNAV()), Qt::QueuedConnection);
    connect(m_navThread, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdate(QString)), Qt::QueuedConnection);
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

    if (this->windowTitle().contains("task"))
    {
        this->close();
        return;
    }

    loadPortfolio();
    disableItems(false);
}

void frmMain::statusUpdate(const QString &message)
{
    ui.stbStatus->setText(QString("Status: ").append(message));
}

int frmMain::currentDateOrPrevious(int date)
{
    QList<int> dates = prices::instance().dates();
    QList<int>::const_iterator place = qLowerBound(dates, date);
    if (*place != date && place != dates.constBegin())
        return *(place - 1);
    else
        return *place;
}

int frmMain::dateDropDownDate(QDateEdit *dateDropDown)
{
    int currentDate = qMax(currentDateOrPrevious(dateDropDown->date().toJulianDay()), m_currentPortfolio->info.startDate);
    dateDropDown->blockSignals(true);
    dateDropDown->setDate(QDate::fromJulianDay(currentDate));
    dateDropDown->blockSignals(false);
    return currentDate;
}

bool frmMain::modifyColumns(const int &columnID, const QMap<int, QString> &fieldNames)
{
    frmColumns f(m_settings.viewableColumns.value(columnID), fieldNames, this);
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
    if (modifyColumns(settings::columns_Holdings, holdingsRow::fieldNames()))
        resetPortfolioHoldings();
}

void frmMain::aaModifyColumns()
{
    if (modifyColumns(settings::columns_AA, aaRow::fieldNames()))
        resetPortfolioAA();
}

void frmMain::acctModifyColumns()
{
    if (modifyColumns(settings::columns_Acct, acctRow::fieldNames()))
        resetPortfolioHoldings();
}

void frmMain::sortDropDownChange(int columnID, QString &sortString, const QMap<int, QString> &fieldNames)
{
    if (columnID == -1)
    {
        sortString.clear();
        return;
    }

    if (columnID != -2)
    {
        sortString = QString::number(columnID);
        return;
    }

    frmSort f(sortString, fieldNames, this);

    if (f.exec())
        sortString = f.getReturnValues();

    setSortDropDown(m_currentPortfolio->info.holdingsSort, ui.holdingsSortCombo);
}

void frmMain::holdingsSortChanged(int index)
{
    sortDropDownChange(ui.holdingsSortCombo->itemData(index).toInt(), m_currentPortfolio->info.holdingsSort, holdingsRow::fieldNames());
    resetPortfolioHoldings();
}

void frmMain::aaSortChanged(int index)
{
    sortDropDownChange(ui.aaSortCombo->itemData(index).toInt(), m_currentPortfolio->info.aaSort, aaRow::fieldNames());
    resetPortfolioAA();
}

void frmMain::acctSortChanged(int index)
{
    sortDropDownChange(ui.accountsSortCombo->itemData(index).toInt(), m_currentPortfolio->info.acctSort, acctRow::fieldNames());
    resetPortfolioAcct();
}
