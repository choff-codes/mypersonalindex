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
    connectSlots();

    loadSettings();

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
    connectDateButton(ui.holdingsDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.statStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.statEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.chartStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.chartEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.correlationsStartDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.correlationsEndDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.accountsDateDropDown, QDate(2009, 8, 9));
    connectDateButton(ui.aaDateDropDown, QDate(2009, 8, 9));
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
//    int? Portfolio = null;
//    if (SavePortfolio())
//        Portfolio = MPI.Portfolio.ID;
    sql->executeNonQuery(sql->updateSettings(QVariant(QVariant::Int), this->size(), this->pos(), this->isMaximized() ? 1 : this->isMinimized() ? 2 : 0));
}

