#include "frmMain.h"
#include <QtGui>
#include <QtSql>
#include "mainQueries.h"
#include "frmPortfolio.h"
#include "functions.h"
#include "frmTicker.h"
#include "frmOptions.h"
#include "frmAA.h"

frmMain::frmMain(QWidget *parent) : QMainWindow(parent)
{    
    QString location = mainQueries::getDatabaseLocation();
    if (!QFile::exists(location))
        if (!QDir().mkpath(QFileInfo(location).dir().absolutePath()))
            if (!QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), location))
                QMessageBox::critical(this, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);

    sql = new mainQueries();
    if (!sql->isOpen())
    {
        QMessageBox::critical(this, "Error", "Cannot read user settings folder!", QMessageBox::Ok);
        this->close();
        return;
    }

    ui.setupUI(this);

    loadSettings();
    loadPortfolios();
    loadPortfolioDropDown(mpi.currentPortfolio.id);
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

    connect(ui.mainPortfolioCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPortfolio()));
}

void frmMain::dateChanged(QDate)
{

}

void frmMain::loadSettings()
{
    checkVersion();
    resetLastDate();

    QSqlQuery *q = sql->executeResultSet(sql->getSettings());
    if (q)
    {
        int x = q->value(mainQueries::getSettings_DataStartDate).toInt();
        mpi.settings.dataStartDate = QDate::fromJulianDay(x);
        mpi.settings.splits = q->value(mainQueries::getSettings_Splits).toBool();
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
            mpi.currentPortfolio.id = q->value(mainQueries::getSettings_LastPortfolio).toInt();
    }

    delete q;
}

void frmMain::loadDates()
{
    QSqlQuery *q = sql->executeResultSet(sql->getSettings());
    if (q)
    {
        do
        {
            mpi.dates.append(q->value(mainQueries::getDates_Date).toInt());
        }
        while (q->next());
    }

    delete q;
}


void frmMain::resetLastDate()
{
    mpi.lastDate = QDate::fromJulianDay(sql->executeScalar(sql->getLastDate(), mpi.settings.dataStartDate.toJulianDay()).toInt());
    ui.stbLastUpdated->setText(QString(" %1%2 ").arg(ui.LAST_UPDATED_TEXT,
                mpi.lastDate == mpi.settings.dataStartDate ?
                "Never" :
                mpi.lastDate.toString(Qt::SystemLocaleShortDate)));
}

void frmMain::checkVersion()
{
    int databaseVersion = sql->executeScalar(sql->getVersion()).toInt();

    if (databaseVersion == VERSION)
        return;
}

void frmMain::saveSettings()
{
    savePortfolios();
    QVariant portfolio(QVariant::Int);
    if (mpi.currentPortfolio.id != -1)
        portfolio = mpi.currentPortfolio.id;
    sql->executeNonQuery(sql->updateSettings(portfolio, this->size(), this->pos(), this->isMaximized() ? 1 : this->isMinimized() ? 2 : 0));
}

void frmMain::loadPortfolioDropDown(const int &portfolioID = -1)
{
    ui.mainPortfolioCombo->blockSignals(true);
    ui.mainPortfolioCombo->setUpdatesEnabled(false);
    ui.mainPortfolioCombo->clear();

    foreach(const globals::portfolio &p, m_portfolios)
        ui.mainPortfolioCombo->addItem(p.description, p.id);

    int row = ui.mainPortfolioCombo->findData(portfolioID);
    if (row == -1)
        ui.mainPortfolioCombo->setCurrentIndex(0);
    else
        ui.mainPortfolioCombo->setCurrentIndex(row);

    ui.mainPortfolioCombo->setUpdatesEnabled(true);
    ui.mainPortfolioCombo->blockSignals(false);
}

void frmMain::disableItems(bool disabled)
{
    disabled = !disabled;

    // todo
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

        mpi.currentPortfolio = m_portfolios.value(ui.mainPortfolioCombo->itemData(ui.mainPortfolioCombo->currentIndex()).toInt());

        if (mpi.currentPortfolio.id == -1)
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

    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, mpi.currentPortfolio.startDate.toString(Qt::SystemLocaleShortDate)));
    ui.holdingsShowHidden->setChecked(mpi.currentPortfolio.holdingsShowHidden);
    ui.performanceSortDesc->setChecked(mpi.currentPortfolio.navSortDesc);
    ui.aaShowBlank->setChecked(mpi.currentPortfolio.aaShowBlank);
    ui.correlationsShowHidden->setChecked(mpi.currentPortfolio.correlationShowHidden);
    ui.accountsShowBlank->setChecked(mpi.currentPortfolio.acctShowBlank);

    // todo
    //ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));
    // todo
    //ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));
    //mpi.account.sort = q->value(mainQueries::getPortfolioAttributes_AcctSort).toString();
}

void frmMain::loadPortfolios()
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
        p.aaThresholdValue = (globals::thesholdValue)q->value(mainQueries::getPortfolioAttributes_AAThresholdValue).toInt();
        p.holdingsShowHidden = q->value(mainQueries::getPortfolioAttributes_HoldingsShowHidden).toBool();
        p.navSortDesc = q->value(mainQueries::getPortfolioAttributes_NAVSortDesc).toBool();
        p.aaShowBlank = q->value(mainQueries::getPortfolioAttributes_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(mainQueries::getPortfolioAttributes_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(mainQueries::getPortfolioAttributes_AcctShowBlank).toBool();
        p.holdingsSort = q->value(mainQueries::getPortfolioAttributes_HoldingsSort).toString();
        p.aaSort = q->value(mainQueries::getPortfolioAttributes_AASort).toString();
        p.acctSort = q->value(mainQueries::getPortfolioAttributes_AcctSort).toString();

        m_portfolios.insert(p.id, p);
    }
    while(q->next());

    delete q;
}

void frmMain::savePortfolio()
{
    if (!m_portfolios.contains(mpi.currentPortfolio.id))
        return;

    mpi.currentPortfolio.holdingsShowHidden = ui.holdingsShowHidden->isChecked();
    mpi.currentPortfolio.navSortDesc = ui.performanceSortDesc->isChecked();
    mpi.currentPortfolio.aaShowBlank = ui.aaShowBlank->isChecked();
    mpi.currentPortfolio.correlationShowHidden = ui.correlationsShowHidden->isChecked();
    mpi.currentPortfolio.acctShowBlank = ui.accountsShowBlank->isChecked();
    mpi.currentPortfolio.holdingsSort = mpi.holdings.sort;
    mpi.currentPortfolio.aaSort = mpi.aa.sort;
    mpi.currentPortfolio.acctSort = mpi.account.sort;

    m_portfolios[mpi.currentPortfolio.id] = mpi.currentPortfolio;
}

void frmMain::savePortfolios()
{
    sql->getDatabase().transaction();
    foreach(const globals::portfolio &p, m_portfolios)
        sql->executeNonQuery(sql->updatePortfolioAttributes(p));
    sql->getDatabase().commit();
}

void frmMain::addPortfolio()
{
    frmPortfolio f(this, mpi.settings.dataStartDate);
    if (f.exec())
    {
        globals::portfolio p = f.getReturnValues();

        m_portfolios.insert(p.id, p);
        loadPortfolioDropDown(p.id);
        loadPortfolio();
    };
}

void frmMain::editPortfolio()
{
    if (mpi.currentPortfolio.id == -1)
        return;

    frmPortfolio f(this,  mpi.settings.dataStartDate, mpi.currentPortfolio);
    if (f.exec())
    {
        globals::mpiPortfolio returnPortfolio = f.getReturnValues();

        // toDo

        loadPortfolioDropDown(mpi.currentPortfolio.id);
    }
}

void frmMain::deletePortfolio()
{
    if (mpi.currentPortfolio.id == -1)
        return;

    if (QMessageBox::question(this, "Delete Portfolio?", QString("Are you sure you want to delete %1?").arg(mpi.currentPortfolio.description),
            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        m_portfolios.remove(mpi.currentPortfolio.id);
        sql->executeNonQuery(sql->deletePortfolio(mpi.currentPortfolio.id));

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
    frmTicker f(this, &mpi.portfolioData.aa, &mpi.portfolioData.acct);
    f.exec();
}

void frmMain::options()
{
    frmOptions f(this);
    f.exec();
}

void frmMain::aa()
{
    frmAA f(this);
    f.exec();
}
