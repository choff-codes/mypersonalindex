#include "frmMain.h"
#include <QtGui>
#include <QtSql>
#include "mainQueries.h"
#include "frmPortfolio.h"
#include "functions.h"
#include "frmTicker.h"
#include "frmOptions.h"
#include "frmAA.h"
#include "frmAcct.h"

frmMain::frmMain(QWidget *parent) : QMainWindow(parent), m_currentPortfolio(0)
{
    QString location = mainQueries::getDatabaseLocation();
    if (!QFile::exists(location))
        if (!QDir().mkpath(QFileInfo(location).dir().absolutePath()) ||
            !QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), location))
        {
            QMessageBox::critical(this, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);
        }

    sql = new mainQueries();
    if (!sql->isOpen())
    {
        QMessageBox::critical(this, "Error", "Cannot read user settings folder!", QMessageBox::Ok);
        this->close();
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
    savePortfolio();
    saveSettings();
    event->accept();

    //event->ignore();
}

void frmMain::connectSlots()
{
//    connectDateButton(ui.holdingsDateDropDown, mpi.lastDate);
//    connectDateButton(ui.statStartDateDropDown, mpi.lastDate);
//    connectDateButton(ui.statEndDateDropDown, mpi.lastDate);
//    connectDateButton(ui.chartStartDateDropDown, mpi.lastDate);
//    connectDateButton(ui.chartEndDateDropDown, mpi.lastDate);
//    connectDateButton(ui.correlationsStartDateDropDown, mpi.lastDate);
//    connectDateButton(ui.correlationsEndDateDropDown, mpi.lastDate);
//    connectDateButton(ui.accountsDateDropDown, mpi.lastDate);
//    connectDateButton(ui.aaDateDropDown, mpi.lastDate);
    connect(ui.mainAdd, SIGNAL(triggered()), this, SLOT(addPortfolio()));
    connect(ui.mainEdit, SIGNAL(triggered()), this, SLOT(editPortfolio()));
    connect(ui.mainDelete, SIGNAL(triggered()), this, SLOT(deletePortfolio()));
    connect(ui.mainAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui.holdingsAdd, SIGNAL(triggered()), this, SLOT(addTicker()));
    connect(ui.mainOptions, SIGNAL(triggered()), this, SLOT(options()));
    connect(ui.aaEdit, SIGNAL(triggered()), this, SLOT(aa()));
    connect(ui.accountsEdit, SIGNAL(triggered()), this, SLOT(acct()));

    connect(ui.mainPortfolioCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPortfolio()));
}

void frmMain::dateChanged(QDate)
{

}

void frmMain::loadSettings()
{
    checkVersion();
    resetLastDate();
    loadStats();
    loadDates();

    QSqlQuery *q = sql->executeResultSet(sql->getSettings());
    if (q)
    {
        int x = q->value(mainQueries::getSettings_DataStartDate).toInt();
        m_settings.dataStartDate = QDate::fromJulianDay(x);
        m_settings.splits = q->value(mainQueries::getSettings_Splits).toBool();
        m_settings.version = q->value(mainQueries::getSettings_Version).toInt();
        if (!q->value(mainQueries::getSettings_WindowState).isNull())
        {
            resize(QSize(q->value(mainQueries::getSettings_WindowWidth).toInt(),
                         q->value(mainQueries::getSettings_WindowHeight).toInt()));
            move(QPoint(q->value(mainQueries::getSettings_WindowX).toInt(),
                        q->value(mainQueries::getSettings_WindowY).toInt()));
            int state = q->value(mainQueries::getSettings_WindowState).toInt();
            if (state)
            {
                Qt::WindowState w = state == 1 ? Qt::WindowMaximized : Qt::WindowMinimized;
                this->setWindowState(this->windowState() | w);
            }
        }
        else
            functions::showWelcomeMessage(this);

        if (!q->value(mainQueries::getSettings_LastPortfolio).isNull())
            m_currentPortfolio = m_portfolios.value(q->value(mainQueries::getSettings_LastPortfolio).toInt());
    }

    delete q;
}

void frmMain::loadStats()
{
    // to do
}

void frmMain::loadDates()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSettings());
    if (q)
    {
        do
        {
            m_dates.append(q->value(mainQueries::getDates_Date).toInt());
        }
        while (q->next());
    }

    delete q;
}

void frmMain::resetLastDate()
{
    m_lastDate = QDate::fromJulianDay(sql->executeScalar(sql->getLastDate(), m_settings.dataStartDate.toJulianDay()).toInt());
    ui.stbLastUpdated->setText(QString(" %1%2 ").arg(ui.LAST_UPDATED_TEXT,
                m_lastDate == m_settings.dataStartDate ?
                "Never" :
                m_lastDate.toString(Qt::SystemLocaleShortDate)));
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
    sql->executeNonQuery(sql->updateSettings(portfolio, this->size(), this->pos(), this->isMaximized() ? 1 : this->isMinimized() ? 2 : 0));
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
    if (m_portfolios.count() == 0) // no portfolios to load
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
        // resetCalendars();
        // todo
    }
}

void frmMain::loadPortfolioSettings()
{
    // set start date equal to earliest data day possible (may be after start date)
    //CheckPortfolioStartDate(rs.GetDateTime((int)MainQueries.eGetPortfolioAttributes.StartDate));
    // todo ^

    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, m_currentPortfolio->info.startDate.toString(Qt::SystemLocaleShortDate)));
    ui.holdingsShowHidden->setChecked(m_currentPortfolio->info.holdingsShowHidden);
    ui.performanceSortDesc->setChecked(m_currentPortfolio->info.navSortDesc);
    ui.aaShowBlank->setChecked(m_currentPortfolio->info.aaShowBlank);
    ui.correlationsShowHidden->setChecked(m_currentPortfolio->info.correlationShowHidden);
    ui.accountsShowBlank->setChecked(m_currentPortfolio->info.acctShowBlank);

    // todo
    //ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));
    // todo
    //ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));
    //mpi.account.sort = q->value(mainQueries::getPortfolioAttributes_AcctSort).toString();
}

void frmMain::loadPortfolios()
{
    loadPortfoliosInfo();
    loadPortfoliosAA();
}

void frmMain::loadPortfoliosInfo()
{
    QSqlQuery *q = sql->executeResultSet(sql->getPortfolioAttributes());

    if (!q)
        return;

    do
    {
        globals::portfolio p;

        p.id = q->value(mainQueries::getPortfolioAttributes_PortfolioID).toInt();
        p.description = q->value(mainQueries::getPortfolioAttributes_Description).toString();
        p.origStartDate = QDate::fromJulianDay(q->value(mainQueries::getPortfolioAttributes_StartDate).toInt());
        p.startDate = p.origStartDate;
        p.dividends = q->value(mainQueries::getPortfolioAttributes_Dividends).toBool();
        p.costCalc = (globals::avgShareCalc)q->value(mainQueries::getPortfolioAttributes_CostCalc).toInt();
        p.startValue = q->value(mainQueries::getPortfolioAttributes_StartValue).toInt();
        p.aaThreshold = q->value(mainQueries::getPortfolioAttributes_AAThreshold).toInt();
        p.aaThresholdMethod = (globals::thesholdMethod)q->value(mainQueries::getPortfolioAttributes_AAThresholdMethod).toInt();
        p.holdingsShowHidden = q->value(mainQueries::getPortfolioAttributes_HoldingsShowHidden).toBool();
        p.navSortDesc = q->value(mainQueries::getPortfolioAttributes_NAVSortDesc).toBool();
        p.aaShowBlank = q->value(mainQueries::getPortfolioAttributes_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(mainQueries::getPortfolioAttributes_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(mainQueries::getPortfolioAttributes_AcctShowBlank).toBool();
        p.holdingsSort = q->value(mainQueries::getPortfolioAttributes_HoldingsSort).toString();
        p.aaSort = q->value(mainQueries::getPortfolioAttributes_AASort).toString();
        p.acctSort = q->value(mainQueries::getPortfolioAttributes_AcctSort).toString();

        m_portfolios.insert(p.id, new globals::myPersonalIndex(p));
    }
    while(q->next());

    delete q;
}

void frmMain::loadPortfoliosTickers()
{
}

void frmMain::loadPortfoliosTickersAA()
{
}

void frmMain::loadPortfoliosTickersTrades()
{
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

        aa.id = q->value(mainQueries::getAA_ID).toInt();
        aa.description = q->value(mainQueries::getAA_Description).toString();
        if (!q->value(mainQueries::getAA_Target).isNull())
            aa.target = q->value(mainQueries::getAA_Target).toDouble();

        int portfolioID = q->value(mainQueries::getAA_PortfolioID).toInt();
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
}

void frmMain::loadPortfoliosStat()
{
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
        sql->executeNonQuery(sql->updatePortfolioAttributes(p->info));
    sql->getDatabase().commit();
}

void frmMain::addPortfolio()
{
    frmPortfolio f(this, m_settings.dataStartDate);
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

    frmPortfolio f(this,  m_settings.dataStartDate, m_currentPortfolio->info);
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
        sql->executeNonQuery(sql->deletePortfolio(i));

        int row = ui.mainPortfolioCombo->currentIndex();
        if (m_portfolios.count() == row)
            row--;

        loadPortfolioDropDown(-1);
        if (m_portfolios.count() != 0)
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

void frmMain::addTicker()
{
    frmTicker f(this, &m_currentPortfolio->data.aa, &m_currentPortfolio->data.acct);
    f.exec();
}

void frmMain::options()
{
    frmOptions f(this);
    f.exec();
}

void frmMain::aa()
{
    frmAA f(m_currentPortfolio->info.id, this, m_currentPortfolio->data.aa);
    if (f.exec())
    {
        m_currentPortfolio->data.aa = f.getReturnValues();
    }

}

void frmMain::acct()
{
    frmAcct f(m_currentPortfolio->info.id, this, m_currentPortfolio->data.acct);
    if (f.exec())
    {
        m_currentPortfolio->data.acct = f.getReturnValues();
    }

}
