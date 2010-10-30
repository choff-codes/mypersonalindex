#include "frmMain.h"
#include "frmMain_UI.h"
#include <QCoreApplication>
#include <QtConcurrentMap>
#include <QFutureWatcher>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "frmEdit.h"
#include "settingsFactory.h"
#include "portfolioFactory.h"
#include "priceFactory.h"
#include "portfolioAttributes.h"
#include "updatePrices.h"
#include "tradeDateCalendar.h"

frmMain::frmMain(QWidget *parent):
    QMainWindow(parent),
    ui(new frmMain_UI),
    m_currentPortfolio(0)
{
    ui->setupUI(this);
    connectSlots();
    loadSettings();
    updateRecentFileActions();
    setCurrentFile("");
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::connectSlots()
{
    connect(ui->helpAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->fileOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->fileSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->fileSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->fileNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->fileExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->portfolioAdd, SIGNAL(triggered()), this, SLOT(addPortfolio()));
    connect(ui->portfolioEdit, SIGNAL(triggered()), this, SLOT(editPortfolio()));
    connect(ui->portfolioDelete, SIGNAL(triggered()), this, SLOT(deletePortfolio()));
    connect(ui->portfolioDropDownCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(portfolioChange(int)));
    connect(ui->importYahoo, SIGNAL(triggered()), this, SLOT(importYahoo()));
}

void frmMain::loadSettings()
{
    m_settings = settingsFactory().getSettings();
    if (m_settings.windowState != Qt::WindowActive)
    {
        resize(m_settings.windowSize);
        move(m_settings.windowLocation);
        if (m_settings.windowState != Qt::WindowNoState)
            this->setWindowState(this->windowState() | m_settings.windowState);
    }
}

void frmMain::importYahoo()
{
    if (!updatePrices::isInternetConnection())
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), "Cannot contact Yahoo! Finance, please check your internet connection.");
        return;
    }

    QFutureWatcher<void> w;
    QEventLoop q;

    int beginDate = QDate::currentDate().toJulianDay() + 1;
    QHash<QString, historicalPrices> updates;
    foreach(const portfolio &p, m_portfolios)
    {
        foreach(const QString symbol, p.symbols())
            if (!updates.contains(symbol))
                updates.insert(symbol, priceFactory::getPrices(symbol));
        beginDate = qMin(beginDate, p.attributes().startDate);
    }

    updatePricesOptions o(beginDate, tradeDateCalendar::endDate(), m_settings.splits);

    connect(&w, SIGNAL(finished()), &q, SLOT(quit()));
    QFuture<updatePricesReturnValue> future = QtConcurrent::mapped(updates.values(), updatePrices(o));
    w.setFuture(future);

    q.exec();

    int earliestUpdate = QDate::currentDate().toJulianDay() + 1;
    QStringList failures;
    foreach(const updatePricesReturnValue &result, future)
    {
        if (result.date == UNASSIGNED)
            failures.append(result.symbol);
        else
            earliestUpdate = qMin(earliestUpdate, result.date);
    }

    if (earliestUpdate <= o.endDate)
        setWindowModified(true);

    if (!failures.isEmpty())
        QMessageBox::information(this, QCoreApplication::applicationName(), "The following securities were not updated (Yahoo! Finance may not yet have today's price):\n\n" +
            failures.join(", "));
}


void frmMain::newFile()
{
    if (!maybeSave())
        return;

    m_portfolios.clear();
    priceFactory::close();
    setCurrentFile("");
}

bool frmMain::maybeSave()
{
    if (!isWindowModified())
        return true;

    QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(), "The document has been modified.\n\n"
        "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     if (ret == QMessageBox::Save)
        return save();

     if (ret == QMessageBox::Cancel)
         return false;

     return true;
}

bool frmMain::save()
{
    if (windowFilePath().isEmpty())
        return saveAs();

    return saveFile(windowFilePath());
}

bool frmMain::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As...", QString(), "My Personal Index File (*.mpi);;All Files (*)");

    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool frmMain::saveFile(const QString &filePath_)
{
    if (windowFilePath() != filePath_)
    {
        if (QFile::exists(filePath_) && !QFile::remove(filePath_)) {
            QMessageBox::warning(this, QCoreApplication::applicationName(), QString("Could not overwrite the existing file %1!").arg(filePath_));
            return false;
        }

        if (windowFilePath().isEmpty()) // new file
        {
            if (!QFile::copy("MPI.sqlite", filePath_))
            {
                QMessageBox::warning(this, QCoreApplication::applicationName(), QString("Could not save to %1!").arg(filePath_));
                return false;
            }
        }
        else
        {
            if (!QFile::copy(windowFilePath(), filePath_))
            {
                QMessageBox::warning(this, QCoreApplication::applicationName(),
                    QString("Could not save to %1 OR the original file was deleted at %2!").arg(filePath_, windowFilePath()));
                return false;
            }
        }
    }

    queries file(filePath_);

    if (file.getDatabaseVersion() == UNASSIGNED)
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return false;
    }

    int currentID = m_currentPortfolio ? m_currentPortfolio->attributes().id : UNASSIGNED;
    QList<portfolio> portfolioList = m_portfolios.values();
    m_portfolios.clear();
    for(QList<portfolio>::iterator i = portfolioList.begin(); i != portfolioList.end(); ++i)
    {
        int id = i->attributes().id;
        i->save(file);
        m_portfolios.insert(i->attributes().id, *i);
        if (id == currentID)
            currentID = i->attributes().id;
    }
    priceFactory::save(file);
    setCurrentFile(filePath_);
    m_settings.addRecentFile(filePath_);
    updateRecentFileActions();
    refreshPortfolioCmb(currentID == UNASSIGNED ? -1 : currentID);
    return true;
}

void frmMain::open()
{
    if(!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, "Open file...", QString(), "My Personal Index File (*.mpi);;All Files (*)");

    if (filePath.isEmpty())
        return;

    loadFile(filePath);
}

void frmMain::loadFile(const QString &filePath_)
{
    queries file(filePath_);

    if (file.getDatabaseVersion() == UNASSIGNED)
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return;
    }

    ui->portfolioDropDownCmb->clear();

    priceFactory::close();
    priceFactory::open(file);
    m_portfolios = portfolioFactory(file).getPortfolios(true);
    refreshPortfolioCmb();
    setCurrentFile(filePath_);
    m_settings.addRecentFile(filePath_);
    updateRecentFileActions();
}

void frmMain::refreshPortfolioCmb(int id)
{
    ui->portfolioDropDownCmb->blockSignals(true);
    foreach(const portfolio &p, m_portfolios)
    {
        if (p.attributes().deleted)
            continue;

        ui->portfolioDropDownCmb->addItem(p.attributes().description, p.attributes().id);
    }

    int index = ui->portfolioDropDownCmb->findData(id);
    if (index == -1 && ui->portfolioDropDownCmb->count() != 0)
        index = 0;

    if (index != -1)
        ui->portfolioDropDownCmb->setCurrentIndex(index);

    portfolioChange(ui->portfolioDropDownCmb->currentIndex());
    ui->portfolioDropDownCmb->blockSignals(false);
}

void frmMain::setCurrentFile(const QString &filePath_)
{
    setWindowFilePath(filePath_);
    setWindowModified(false);
    if (filePath_.isEmpty())
        setWindowTitle(QString("untitled.mpi[*] - %1").arg(QCoreApplication::applicationName()));
    else
        setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(filePath_).fileName(), QCoreApplication::applicationName()));
}

void frmMain::closeEvent(QCloseEvent *event_)
{
    if (maybeSave())
    {
        event_->accept();
        saveSettings();
    }
    else
        event_->ignore();
}

void frmMain::saveSettings()
{
    m_settings.windowSize = size();
    m_settings.windowLocation = pos();
    m_settings.windowState = isMaximized() ? Qt::WindowMaximized : isMinimized() ? Qt::WindowMinimized : Qt::WindowNoState;
    m_settings.save();
}

void frmMain::updateRecentFileActions()
{
    ui->fileRecent->clear();
    foreach(const QString &s, m_settings.recentFiles())
    {
        QAction *action = ui->fileRecent->addAction(s);
        connect(action, SIGNAL(triggered()), this, SLOT(recentFileSelected()));
    }
}

void frmMain::recentFileSelected()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action && maybeSave())
        loadFile(action->text());
}

void frmMain::addPortfolio()
{
    portfolio p(portfolio::getOpenIdentity());
    frmEdit f(p, this);
    f.exec();
    if (p == f.getPortfolio())
        return;

    setWindowModified(true);
    portfolio newPortfolio = f.getPortfolio();
    m_portfolios.insert(newPortfolio.attributes().id, newPortfolio);
    ui->portfolioDropDownCmb->addItem(newPortfolio.attributes().description, newPortfolio.attributes().id);
    ui->portfolioDropDownCmb->setCurrentIndex(ui->portfolioDropDownCmb->count() - 1);
}

void frmMain::editPortfolio()
{
    if (!m_currentPortfolio)
        return;

    frmEdit f(*m_currentPortfolio, this);
    f.exec();
    if (*m_currentPortfolio == f.getPortfolio())
        return;

    setWindowModified(true);;
    m_portfolios[m_currentPortfolio->attributes().id] = f.getPortfolio();;
    m_currentPortfolio = &m_portfolios[m_currentPortfolio->attributes().id];
    ui->portfolioDropDownCmb->setItemText(ui->portfolioDropDownCmb->currentIndex(), m_currentPortfolio->attributes().description);
}

void frmMain::deletePortfolio()
{
    if (!m_currentPortfolio)
        return;

    if (QMessageBox::question(this, QCoreApplication::applicationName(), QString("Are you sure you want to delete portfolio %1?")
        .arg(m_currentPortfolio->attributes().description), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    setWindowModified(true);
    m_currentPortfolio->attributes().deleted = true;
    ui->portfolioDropDownCmb->removeItem(ui->portfolioDropDownCmb->currentIndex());
}

void frmMain::portfolioChange(int currentIndex_)
{
    if (currentIndex_ == -1)
        m_currentPortfolio = 0;
    else
        m_currentPortfolio = &m_portfolios[ui->portfolioDropDownCmb->itemData(currentIndex_).toInt()];
}

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString::number(APP_VERSION / 100.0) + "</h2>"
        "<p>Copyright &copy; 2010"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}
