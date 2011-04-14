#include "frmMain.h"
#include "frmMain_UI.h"
#include <QCoreApplication>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileInfo>
#include <QFutureWatcher>
#include "frmEdit.h"
#include "settingsFactory.h"
#include "updatePrices.h"
#include "tradeDateCalendar.h"
#include "calculatorTrade.h"
#include "security.h"
#include "historicalNAV.h"
#include "fileState.h"
#include "frmMainAA_State.h"
#include "frmMainSecurity_State.h"
#include "frmMainPerformance_State.h"
#include "frmMainAcct_State.h"
#include "frmMainCorrelation_State.h"
#include "frmMainStatistic_State.h"
#include "frmMainTrade_State.h"
#include "frmMainChart_State.h"
#include "frmPortfolioImport.h"
#include "frmPriceImport.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

frmMain::frmMain(QWidget *parent_):
    QMainWindow(parent_),
    ui(new frmMain_UI()),
    m_file(new fileState(this)),
    m_currentPortfolio(UNASSIGNED),
    m_currentTab(tab_security),
    m_futureWatcherYahoo(0),
    m_futureWatcherTrade(0)
{
    ui->setupUI(this);
    connectSlots();
    loadSettings();
    m_file->newFile();
}

frmMain::~frmMain()
{
    if (m_futureWatcherYahoo)
    {
        m_futureWatcherYahoo->cancel();
        m_futureWatcherYahoo->waitForFinished();
        delete m_futureWatcherYahoo;
    }

    if (m_futureWatcherTrade)
    {
        // cancel trade calculations? not for now.
        //m_futureWatcherTrade->cancel();
        m_futureWatcherTrade->waitForFinished();
        delete m_futureWatcherTrade;
    }

    qDeleteAll(m_tabs);

    delete ui;
    delete m_file;
}

void frmMain::connectSlots()
{
    connect(ui->helpAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->fileOpen, SIGNAL(triggered()), m_file, SLOT(open()));
    connect(ui->fileSave, SIGNAL(triggered()), m_file, SLOT(save()));
    connect(ui->fileSaveAs, SIGNAL(triggered()), m_file, SLOT(saveAs()));
    connect(ui->fileNew, SIGNAL(triggered()), m_file, SLOT(newFile()));
    connect(ui->fileExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_file, SIGNAL(fileNameChange(QString,bool)), this, SLOT(fileChange(QString,bool)));
    connect(ui->portfolioAdd, SIGNAL(triggered()), this, SLOT(addPortfolio()));
    connect(ui->portfolioEdit, SIGNAL(triggered()), this, SLOT(editPortfolio()));
    connect(ui->portfolioDelete, SIGNAL(triggered()), this, SLOT(deletePortfolio()));
    connect(ui->portfolioDropDownCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(portfolioDropDownChange(int)));
    connect(ui->priceDownload, SIGNAL(triggered()), this, SLOT(importYahoo()));
    connect(ui->viewAssetAllocation, SIGNAL(triggered()), this, SLOT(tabAA()));
    connect(ui->viewSecurities, SIGNAL(triggered()), this, SLOT(tabSecurity()));
    connect(ui->viewPerformance, SIGNAL(triggered()), this, SLOT(tabPerformance()));
    connect(ui->viewAccounts, SIGNAL(triggered()), this, SLOT(tabAccount()));
    connect(ui->viewCorrelations, SIGNAL(triggered()), this, SLOT(tabCorrelation()));
    connect(ui->viewStatistics, SIGNAL(triggered()), this, SLOT(tabStatistic()));
    connect(ui->viewCharts, SIGNAL(triggered()), this, SLOT(tabChart()));
    connect(ui->viewTrades, SIGNAL(triggered()), this, SLOT(tabTrade()));
    connect(ui->portfolioImport, SIGNAL(triggered()), this, SLOT(importPortfolio()));
    connect(ui->portfolioImportFile, SIGNAL(triggered()), this, SLOT(importPortfolio()));
    connect(ui->priceImport, SIGNAL(triggered()), this, SLOT(importPrice()));
}

void frmMain::loadSettings()
{
    m_settings = settingsFactory().getSettings();
    resize(m_settings.windowSize());

    if (m_settings.windowState() == Qt::WindowActive) // first time opening, no useful location saved yet
        return;

    move(m_settings.windowLocation());
    if (m_settings.windowState() != Qt::WindowNoState)
        this->setWindowState(this->windowState() | m_settings.windowState());
}

void frmMain::fileChange(const QString &filePath_, bool newFile_)
{
    setWindowModified(false);
    if (filePath_.isEmpty())
        setWindowTitle(QString("untitled.mpi[*] - %1").arg(QCoreApplication::applicationName()));
    else
        setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(filePath_).fileName(), QCoreApplication::applicationName()));

    updateRecentFileActions(filePath_);

    if (!newFile_)
        return;

    refreshPortfolioPrices();
    refreshPortfolioCmb(UNASSIGNED);
}

void frmMain::setCurrentPortfolio(const portfolio &portfolio_)
{
    m_currentPortfolio = portfolio_.id();
}

void frmMain::refreshPortfolioCmb(int id_)
{
    ui->portfolioDropDownCmb->blockSignals(true);
    ui->portfolioDropDownCmb->clear();
    foreach(const portfolio &p, m_file->portfolios)
        ui->portfolioDropDownCmb->addItem(p.displayText(), p.id());

    int index = ui->portfolioDropDownCmb->findData(id_);
    if (index == -1 && ui->portfolioDropDownCmb->count() != 0)
        index = 0;

    if (index != -1)
        ui->portfolioDropDownCmb->setCurrentIndex(index);

    portfolioDropDownChange(ui->portfolioDropDownCmb->currentIndex());
    ui->portfolioDropDownCmb->blockSignals(false);
}

void frmMain::closeEvent(QCloseEvent *event_)
{
    if (!m_file->maybeSave())
    {
        event_->ignore();
        return;
    }

    event_->accept();
    saveSettings();
}

void frmMain::saveSettings()
{
    m_settings.setWindowSize(size());
    m_settings.setWindowLocation(pos());
    m_settings.setWindowState(isMaximized() ? Qt::WindowMaximized : isMinimized() ? Qt::WindowMinimized : Qt::WindowNoState);
    m_settings.save();
}

void frmMain::updateRecentFileActions(const QString &newFilePath_)
{
    if (!newFilePath_.isEmpty())
        m_settings.setRecentFile(newFilePath_);

    ui->fileRecent->clear();
    foreach(const QString &s, m_settings.recentFiles())     
        connect(ui->fileRecent->addAction(s), SIGNAL(triggered()), this, SLOT(recentFileSelected()));

    int counter = 1;
    foreach(QAction *action, ui->fileRecent->actions())
        action->setShortcut(QString("Ctrl+%1").arg(QString::number(counter++)));
}

void frmMain::recentFileSelected()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action || action->text() == m_file->path())
        return;

    m_file->open(action->text());
}

void frmMain::addPortfolio()
{
    portfolio p;
    p.setNewIdentity();
    frmEdit f(p, this);
    f.exec();
    if (p == f.getPortfolio())
        return;

    addPortfolioToFile(f.getPortfolio());
}

void frmMain::editPortfolio()
{
    if (m_currentPortfolio == UNASSIGNED)
        return;

    frmEdit f(m_file->portfolios.value(m_currentPortfolio), this);
    f.exec();
    if (m_file->portfolios.value(m_currentPortfolio) == f.getPortfolio())
        return;

    editPortfolioToFile(f.getPortfolio());
}

void frmMain::deletePortfolio()
{
    if (m_currentPortfolio == UNASSIGNED)
        return;

    if (QMessageBox::question(this, QCoreApplication::applicationName(), QString("Are you sure you want to delete portfolio %1?")
        .arg(m_file->portfolios.value(m_currentPortfolio).displayText()), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    setWindowModified(true);
    m_file->modified = true;
    m_file->portfolios.remove(m_currentPortfolio);
    ui->portfolioDropDownCmb->removeItem(ui->portfolioDropDownCmb->currentIndex());
}

void frmMain::addPortfolioToFile(const portfolio &portfolio_)
{
    setWindowModified(true);
    m_file->modified = true;
    m_file->portfolios.insert(portfolio_.id(), portfolio_);
    refreshPortfolioPrices();
    ui->portfolioDropDownCmb->addItem(portfolio_.displayText(), portfolio_.id());
    ui->portfolioDropDownCmb->setCurrentIndex(ui->portfolioDropDownCmb->count() - 1);
    recalculateTrades(portfolio_);
}

void frmMain::editPortfolioToFile(const portfolio &portfolio_)
{
    setWindowModified(true);
    m_file->modified = true;
    m_file->portfolios[m_currentPortfolio] = portfolio_;
    refreshPortfolioPrices();

    ui->portfolioDropDownCmb->setItemText(ui->portfolioDropDownCmb->currentIndex(), portfolio_.displayText());
    recalculateTrades(portfolio_, 0);
}

void frmMain::refreshPortfolioPrices()
{
    foreach(const portfolio &p, m_file->portfolios)
        foreach(security s, p.securities())
            s.setHistoricalPrices(m_file->prices.getHistoricalPrice(s.description()));
}

void frmMain::portfolioDropDownChange(int currentIndex_)
{
    clearTabs();
    ui->portfolioDelete->setDisabled(currentIndex_ == -1);
    ui->portfolioEdit->setDisabled(currentIndex_ == -1);
    ui->portfolioDropDownCmb->setDisabled(ui->portfolioDropDownCmb->count() == 0);
    setCurrentPortfolio(currentIndex_ == -1 ? portfolio() : m_file->portfolios.value(ui->portfolioDropDownCmb->itemData(currentIndex_).toInt()));
    switchToTab(m_currentTab, true);
}

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString::number(APP_VERSION / 100.0) + "</h2>"
        "<p>Copyright &copy; 2010"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}

void frmMain::importYahoo()
{
    if (!updatePrices::isInternetConnection())
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), "Cannot contact Yahoo! Finance, please check your internet connection.");
        return;
    }

    QList<historicalPrices> prices;
    QMap<QString, updatePricesOptions> options;
    QMap<QString, int> symbols = portfolio::symbols(m_file->portfolios);
    if (symbols.isEmpty())
        return;

    for(QMap<QString, int>::const_iterator i = symbols.constBegin(); i != symbols.constEnd(); ++i)
    {
        prices.append(m_file->prices.getHistoricalPrice(i.key()));
        options.insert(i.key(), updatePricesOptions(i.value(), tradeDateCalendar::endDate(), m_settings.splits()));
    }

    showProgressBar("Downloading", prices.count());

    m_futureWatcherYahoo = new QFutureWatcher<int>();
    connect(m_futureWatcherYahoo, SIGNAL(finished()), this, SLOT(importYahooFinished()));
    connect(m_futureWatcherYahoo, SIGNAL(progressValueChanged(int)), ui->progressBar, SLOT(setValue(int)));
    m_futureWatcherYahoo->setFuture(QtConcurrent::mapped(prices, updatePrices(options)));
}

void frmMain::importYahooFinished()
{
    hideProgressBar();

    int earliestUpdate = tradeDateCalendar::endDate() + 1;
    foreach(int result, m_futureWatcherYahoo->future())
        if (result != -1)
            earliestUpdate = qMin(earliestUpdate, result);

    delete m_futureWatcherYahoo;
    m_futureWatcherYahoo = 0;

    if (earliestUpdate > tradeDateCalendar::endDate())
        return;

    setWindowModified(true);
    m_file->modified = true;
    recalculateTrades(m_file->portfolios.values(), earliestUpdate);
}

void frmMain::recalculateTrades(const portfolio &portfolio_, int beginDate_)
{
    recalculateTrades(QList<portfolio>() << portfolio_, beginDate_);
}

void frmMain::recalculateTrades(const QList<portfolio> &portfolios_, int beginDate_)
{
    showProgressBar("Calculating", portfolios_.count());
    m_futureWatcherTrade = new QFutureWatcher<void>();
    connect(m_futureWatcherTrade, SIGNAL(finished()), this, SLOT(recalculateTradesFinished()));
    m_futureWatcherTrade->setFuture(QtConcurrent::mapped(portfolios_, calculatorTrade(beginDate_)));
}

void frmMain::recalculateTradesFinished()
{
    delete m_futureWatcherTrade;
    m_futureWatcherTrade = 0;
    hideProgressBar();
    clearTabs();

    if (m_currentPortfolio != UNASSIGNED)
        setCurrentPortfolio(m_file->portfolios.value(m_currentPortfolio));

    switchToTab(m_currentTab, true);
}

void frmMain::showProgressBar(const QString &description_, int steps_)
{
    ui->progressBar->setMaximum(steps_);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat(QString("%1: %p%").arg(description_));
    ui->cornerWidget->setCurrentIndex(1);
}

void frmMain::hideProgressBar()
{
    ui->cornerWidget->setCurrentIndex(0);
}

void frmMain::clearTabs()
{
    foreach(frmMainState *state, m_tabs)
    {
        ui->centralWidget->removeWidget(state->mainWidget());
        delete state->mainWidget();
    }

    qDeleteAll(m_tabs);
    m_tabs.clear();
}

void frmMain::switchToTab(tab tab_, bool force_)
{
    if ((!force_ && m_currentTab == tab_) || m_currentPortfolio == UNASSIGNED )
        return;

    if (m_tabs.contains(tab_))
    {
        ui->centralWidget->setCurrentWidget(m_tabs.value(tab_)->mainWidget());
        m_currentTab = tab_;
        return;
    }

    switch (tab_)
    {
        case tab_assetAllocation:
            m_tabs.insert(tab_assetAllocation, new frmMainAA_State(m_file->portfolios.value(m_currentPortfolio), m_settings, this));
            break;
        case tab_account:
            m_tabs.insert(tab_account, new frmMainAcct_State(m_file->portfolios.value(m_currentPortfolio), m_settings, this));
            break;
        case tab_security:
            m_tabs.insert(tab_security, new frmMainSecurity_State(m_file->portfolios.value(m_currentPortfolio), m_settings, this));
            break;
        case tab_performance:
            m_tabs.insert(tab_performance, new frmMainPerformance_State(m_currentPortfolio, m_file->portfolios, m_settings, m_file->prices.getHistoricalPrices(), this));
            break;
        case tab_correlation:
            m_tabs.insert(tab_correlation, new frmMainCorrelation_State(m_currentPortfolio, m_file->portfolios, m_settings, m_file->prices.getHistoricalPrices(), this));
            break;
        case tab_statistic:
            m_tabs.insert(tab_statistic, new frmMainStatistic_State(m_currentPortfolio, m_file->portfolios, m_settings, m_file->prices.getHistoricalPrices(), this));
            break;
        case tab_chart:
            m_tabs.insert(tab_chart, new frmMainChart_State(m_currentPortfolio, m_file->portfolios, m_file->prices.getHistoricalPrices(), this));
            break;
        case tab_trade:
            m_tabs.insert(tab_trade, new frmMainTrade_State(m_file->portfolios.value(m_currentPortfolio), this));
            break;
    }

    ui->centralWidget->addWidget(m_tabs.value(tab_)->mainWidget());
    ui->centralWidget->setCurrentWidget(m_tabs.value(tab_)->mainWidget());
    m_currentTab = tab_;
}

void frmMain::importPortfolio()
{
    QMap<int, portfolio> portfolios;

    if (static_cast<QAction*>(sender()) == ui->portfolioImportFile)
    {
        fileState file(this);
        file.open(false);
        if (file.path().isEmpty())
            return;
        else
            portfolios = file.portfolios;
    }
    else
        portfolios = m_file->portfolios;

    frmPortfolioImport f(m_currentPortfolio == UNASSIGNED ? portfolio() : m_file->portfolios.value(m_currentPortfolio), portfolios, this);
    if (f.exec() != QDialog::Accepted)
        return;

    if (f.getPortfolio().id() != m_file->portfolios.value(m_currentPortfolio).id())
        addPortfolioToFile(f.getPortfolio());
    else
        editPortfolioToFile(f.getPortfolio());
}

void frmMain::importPrice()
{
    frmPriceImport f(m_file->prices, this);
    if (f.exec() != QDialog::Accepted)
        return;

    setWindowModified(true);
    m_file->modified = true;
    m_file->prices = f.getHistoricalPricesMap();
}
