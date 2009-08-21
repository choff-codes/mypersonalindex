#include "frmMain.h"
#include <QtGui>
#include <QtSql>
#include "queries.h"


frmMain::frmMain(QWidget *parent) : QMainWindow(parent)
{    
    QString location = queries::getDatabaseLocation();
    if (!QFile::exists(location))
        if (!QDir().mkpath(QFileInfo(location).dir().absolutePath()) || !QFile::copy(QCoreApplication::applicationDirPath().append("/MPI.sqlite"), location))
            QMessageBox::critical(this, "Error", "Cannot write to the user settings folder!", QMessageBox::Ok);

    sql = new queries();
    if (!sql->isOpen())
    {
        QMessageBox::critical(this, "Error", "Cannot read user settings folder!", QMessageBox::Ok);
        this->close();
        return;
    }

    ui.setupUI(this);


    loadSettings();
    loadPortfolioDropDown();
    loadPortfolio();
    connectSlots();

    //updatePrices u(sql);
    //bool temp = u.isInternetConnection();
    //u.getSplits("VTI", QDate(2008, 1, 1));
}

void frmMain::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();

    //event->ignore();
}

void frmMain::connectSlots()
{
    connectDateButton(ui.holdingsDateDropDown, mpi.lastDate);
    connectDateButton(ui.statStartDateDropDown, mpi.lastDate);
    connectDateButton(ui.statEndDateDropDown, mpi.lastDate);
    connectDateButton(ui.chartStartDateDropDown, mpi.lastDate);
    connectDateButton(ui.chartEndDateDropDown, mpi.lastDate);
    connectDateButton(ui.correlationsStartDateDropDown, mpi.lastDate);
    connectDateButton(ui.correlationsEndDateDropDown, mpi.lastDate);
    connectDateButton(ui.accountsDateDropDown, mpi.lastDate);
    connectDateButton(ui.aaDateDropDown, mpi.lastDate);
    connect(ui.DateCalendar, SIGNAL(clicked(QDate)), this, SLOT(dateChanged(QDate)));
}

void frmMain::connectDateButton(mpiToolButton *t, const QDate &date)
{
    t->setDate(date);
    setDateDropDownText(t);
    connect(t, SIGNAL(pressDown()), this, SLOT(dateButtonPressed()));
}

void frmMain::dateChanged(QDate d)
{
    if (!ui.DateMenu->isVisible())
        return;

    ui.DateMenu->hide();

    if (!m_mpiButtonPressed || !d.isValid())
        return;

    m_mpiButtonPressed->setDate(d);
    setDateDropDownText(m_mpiButtonPressed);
}

void frmMain::setDateDropDownText(mpiToolButton *t)
{
    if (!t)
        return;

    QString str;
    switch(t->type())
    {
        case mpiToolButton::startDate:
            str = ui.START_DATE;
            break;
        case mpiToolButton::endDate:
            str = ui.END_DATE;
            break;
        default:
            str = ui.DATE;
    }
    t->setText(str.append(t->date().toString(Qt::SystemLocaleShortDate)).append(" "));
}

void frmMain::dateButtonPressed()
{
    m_mpiButtonPressed = static_cast<mpiToolButton*>(sender());
    ui.DateCalendar->setSelectedDate(m_mpiButtonPressed->date());
}

void frmMain::loadSettings()
{
    checkVersion();

    QSqlQuery *q = sql->executeResultSet(sql->getSettings());
    if (q)
    {
        int x = q->value(queries::getSettings_DataStartDate).toInt();
        mpi.settings.dataStartDate = QDate::fromJulianDay(x);
        mpi.settings.splits = q->value(queries::getSettings_Splits).toBool();
        if (!q->value(queries::getSettings_WindowState).isNull())
        {
            resize(QSize(q->value(queries::getSettings_WindowWidth).toInt(),
                         q->value(queries::getSettings_WindowHeight).toInt()));
            move(QPoint(q->value(queries::getSettings_WindowX).toInt(),
                        q->value(queries::getSettings_WindowY).toInt()));
            int state = q->value(queries::getSettings_WindowState).toInt();
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
        if (!q->value(queries::getSettings_LastPortfolio).isNull())
            mpi.portfolio.id = q->value(queries::getSettings_LastPortfolio).toInt();
        else
            mpi.portfolio.id = -1;
    }

    resetLastDate();
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
    QVariant portfolio(QVariant::Int);
    if (savePortfolio())
        portfolio = mpi.portfolio.id;
    sql->executeNonQuery(sql->updateSettings(portfolio, this->size(), this->pos(), this->isMaximized() ? 1 : this->isMinimized() ? 2 : 0));
}

void frmMain::loadPortfolioDropDown()
{
    //ui.mainPortfolioCombo->blockSignals(true);
    QSqlQueryModel *dataset = sql->executeDataSet(sql->getPortfolios());
    if (dataset)
    {
        ui.mainPortfolioCombo->setModel(dataset);
        if (dataset->rowCount() != 0)
        {
            QModelIndexList result = dataset->match(dataset->index(0, 1), Qt::EditRole, mpi.portfolio.id, 1, Qt::MatchExactly);
            int row = result.value(0).row();
            if (row == -1)
                ui.mainPortfolioCombo->setCurrentIndex(0);
            else
                ui.mainPortfolioCombo->setCurrentIndex(row);
        }
    }
    //ui.mainPortfolioCombo->blockSignals(false);
}

void frmMain::disableItems(bool disabled)
{
    disabled = !disabled;

    // todo
}

void frmMain::loadPortfolio()
{
    if (ui.mainPortfolioCombo->count() == 0) // no portfolios to load
        disableItems(true);
    else
    {
        if (this->isVisible())
            savePortfolio(); // save currently loaded portfolio
        disableItems(false);

        QAbstractItemModel *dataset = ui.mainPortfolioCombo->model();
        int row = ui.mainPortfolioCombo->currentIndex();
        mpi.portfolio.id = dataset->data(dataset->index(row, 1)).toInt();
        mpi.portfolio.name = dataset->data(dataset->index(row, 0)).toString();

        if (!loadPortfolioSettings())
        {
            QMessageBox::information(this, "Error!", "Portfolio appears to be deleted. Please restart.");
            disableItems(true);
            return;
        }

        // resetCalendars();
        // todo
    }
}

bool frmMain::loadPortfolioSettings()
{
    QSqlQuery *q = sql->executeResultSet(sql->getPortfolioAttributes(mpi.portfolio.id));
    if (!q)
        return false;

    // set start date equal to earliest data day possible (may be after start date)

    //CheckPortfolioStartDate(rs.GetDateTime((int)MainQueries.eGetPortfolioAttributes.StartDate));
    // todo ^
    mpi.portfolio.startDate = QDate::fromJulianDay(q->value(queries::getPortfolioAttributes_StartDate).toInt());
    ui.stbStartDate->setText(QString(" %1%2 ").arg(ui.INDEX_START_TEXT, mpi.portfolio.startDate.toString(Qt::SystemLocaleShortDate)));
    mpi.portfolio.dividends = q->value(queries::getPortfolioAttributes_Dividends).toBool();
    mpi.portfolio.costCalc = (globals::avgShareCalc)q->value(queries::getPortfolioAttributes_CostCalc).toInt();
    mpi.portfolio.navStart = q->value(queries::getPortfolioAttributes_NAVStartValue).toDouble();
    mpi.portfolio.aaThreshold = q->value(queries::getPortfolioAttributes_AAThreshold).toInt();
    ui.holdingsShowHidden->setChecked(q->value(queries::getPortfolioAttributes_HoldingsShowHidden).toBool());
    ui.performanceSortDesc->setChecked(q->value(queries::getPortfolioAttributes_NAVSort).toBool());
    ui.aaShowBlank->setChecked(q->value(queries::getPortfolioAttributes_AAShowBlank).toBool());
    ui.correlationsShowHidden->setChecked(q->value(queries::getPortfolioAttributes_CorrelationShowHidden).toBool());
    ui.accountsShowBlank->setChecked(q->value(queries::getPortfolioAttributes_AcctShowBlank).toBool());

    mpi.holdings.sort = q->value(queries::getPortfolioAttributes_HoldingsSort).toString();
    // todo
    //ResetSortDropDown(cmbHoldingsSortBy, MPI.Holdings.Sort, new EventHandler(cmbHoldingsSortBy_SelectedIndexChanged));

    mpi.aa.sort = q->value(queries::getPortfolioAttributes_AASort).toString();
    // todo
    //ResetSortDropDown(cmbAASortBy, MPI.AA.Sort, new EventHandler(cmbAASortBy_SelectedIndexChanged));

    mpi.account.sort = q->value(queries::getPortfolioAttributes_AcctSort).toString();
    // todo
    //ResetSortDropDown(cmbAcctSortBy, MPI.Account.Sort, new EventHandler(cmbAcctSortBy_SelectedIndexChanged));

    delete q;
    return true;
}

bool frmMain::savePortfolio()
{
    if (!sql->executeScalar(sql->getPortfolioExists(mpi.portfolio.id)).isValid())
        return false;

    sql->executeNonQuery(sql->updatePortfolioAttributes(mpi.portfolio.id, ui.holdingsShowHidden->isChecked(),
        ui.performanceSortDesc->isChecked(), ui.aaShowBlank->isChecked(), mpi.holdings.sort, mpi.aa.sort,
        ui.correlationsShowHidden->isChecked(), ui.accountsShowBlank->isChecked(), mpi.account.sort));
    return true;
}
