#include "frmMain.h"
#include <QtGui>
#include <QtSql>
#include "mainQueries.h"
#include "frmPortfolio.h"
#include "frmTicker.h"

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
        {
            QString welcomeMessage = "Welcome to My Personal Index!\n\nThere is no documentation yet, but I recommend starting in the following way:\n\n1. Set the start date under options (on the top toolbar).\n2. Add a new Portfolio\n3. Set your asset allocation \n4. Set your accounts\n5. Add holdings\n6. Add relevant portfolio statistics\n7. Update prices!\n\nThis text has been copied to the clipboard for your convenience.";
            QApplication::clipboard()->setText(welcomeMessage);
            QMessageBox::information(this, "My Personal Index", welcomeMessage);
        }
        if (!q->value(mainQueries::getSettings_LastPortfolio).isNull())
            mpi.currentPortfolio.id = q->value(mainQueries::getSettings_LastPortfolio).toInt();
    }
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

    foreach(const globals::portfolio &p, mpi.portfolios)
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
    if (mpi.portfolios.count() == 0) // no portfolios to load
        disableItems(true);
    else
    {
        if (this->isVisible())
            savePortfolio(); // save currently loaded portfolio except on initial load
        disableItems(false);

        mpi.currentPortfolio = mpi.portfolios.value(ui.mainPortfolioCombo->itemData(ui.mainPortfolioCombo->currentIndex()).toInt());

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
    ui.performanceSortDesc->setChecked(mpi.currentPortfolio.navSort);
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
        p.navSort = q->value(mainQueries::getPortfolioAttributes_NAVSort).toBool();
        p.aaShowBlank = q->value(mainQueries::getPortfolioAttributes_AAShowBlank).toBool();
        p.correlationShowHidden = q->value(mainQueries::getPortfolioAttributes_CorrelationShowHidden).toBool();
        p.acctShowBlank = q->value(mainQueries::getPortfolioAttributes_AcctShowBlank).toBool();
        p.holdingsSort = q->value(mainQueries::getPortfolioAttributes_HoldingsSort).toString();
        p.aaSort = q->value(mainQueries::getPortfolioAttributes_AASort).toString();
        p.acctSort = q->value(mainQueries::getPortfolioAttributes_AcctSort).toString();

        mpi.portfolios.insert(p.id, p);
    }
    while(q->next());

    delete q;
}

void frmMain::savePortfolio()
{
    if (!mpi.portfolios.contains(mpi.currentPortfolio.id))
        return;

    mpi.currentPortfolio.holdingsShowHidden = ui.holdingsShowHidden->isChecked();
    mpi.currentPortfolio.navSort = ui.performanceSortDesc->isChecked();
    mpi.currentPortfolio.aaShowBlank = ui.aaShowBlank->isChecked();
    mpi.currentPortfolio.correlationShowHidden = ui.correlationsShowHidden->isChecked();
    mpi.currentPortfolio.acctShowBlank = ui.accountsShowBlank->isChecked();
    mpi.currentPortfolio.holdingsSort = mpi.holdings.sort;
    mpi.currentPortfolio.aaSort = mpi.aa.sort;
    mpi.currentPortfolio.acctSort = mpi.account.sort;

    mpi.portfolios[mpi.currentPortfolio.id] = mpi.currentPortfolio;
}

void frmMain::savePortfolios()
{
    sql->getDatabase().transaction();
    foreach(const globals::portfolio &p, mpi.portfolios)
        sql->executeNonQuery(sql->updatePortfolioAttributes(p));
    sql->getDatabase().commit();
}

void frmMain::addPortfolio()
{
    frmPortfolio f(this, mpi.settings.dataStartDate);
    if (f.exec())
    {
        globals::portfolio p = f.getReturnValues();

        mpi.portfolios.insert(p.id, p);
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
        mpi.portfolios.remove(mpi.currentPortfolio.id);
        sql->executeNonQuery(sql->deletePortfolio(mpi.currentPortfolio.id));

        int row = ui.mainPortfolioCombo->currentIndex();
        if (mpi.portfolios.count() == row)
            row--;

        loadPortfolioDropDown(-1);
        if (mpi.portfolios.count() != 0)
            ui.mainPortfolioCombo->setCurrentIndex(row);
    }

}

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString(VERSIONTEXT) + "</h2>"
               "<p>Copyright &copy; 2009"
               "<p>By Matthew Wikler"
               "<p>Create personal indexes and perform analysis to make better investing decisions."
               "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>"
    );
}

void frmMain::addTicker()
{
    frmTicker f(this);
    f.exec();
}
