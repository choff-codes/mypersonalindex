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
#include "calculatorTrade.h"

#ifdef CLOCKTIME
#include <QTime>
#endif

frmMain::frmMain(QWidget *parent):
    QMainWindow(parent),
    ui(new frmMain_UI),
    m_currentPortfolio(0)
{
    ui->setupUI(this);
    connectSlots();
    loadSettings();
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

void frmMain::newFile()
{
    if (!maybeSave())
        return;

    m_portfolios.clear();
    refreshPortfolioCmb();
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
    if (!prepareFileForSave(filePath_))
        return false;

    queries file(filePath_);
    if (!file.isValid())
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return false;
    }
#ifdef CLOCKTIME
    QTime t;
    t.start();
#endif
    file.beginTransaction();

    int currentID = m_currentPortfolio ? m_currentPortfolio->attributes().id : UNASSIGNED; // track current portfolio
    m_portfolios = portfolio::save(m_portfolios, file, &currentID);
    refreshPortfolioCmb(currentID == UNASSIGNED ? -1 : currentID);

    file.commit();
#ifdef CLOCKTIME
    qDebug("Time elapsed (save porfolios): %d ms", t.elapsed());
#endif
    file.beginTransaction();

    priceFactory::save(file);

    file.commit();
#ifdef CLOCKTIME
    qDebug("Time elapsed (save prices): %d ms", t.elapsed());
#endif

#ifdef CLOCKTIME
    qDebug("Time elapsed (save): %d ms", t.elapsed());
#endif

    setCurrentFile(filePath_);
    return true;
}

bool frmMain::prepareFileForSave(const QString &filePath_)
{
    if (windowFilePath() == filePath_)
        return true;

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
    if (!file.isValid())
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return;
    }

    priceFactory::close();
    priceFactory::open(file);

    m_portfolios = portfolioFactory(file).getPortfolios(true);
    refreshPortfolioCmb();

    setCurrentFile(filePath_);
}

void frmMain::setCurrentFile(const QString &filePath_)
{
    setWindowFilePath(filePath_);
    setWindowModified(false);
    if (filePath_.isEmpty())
        setWindowTitle(QString("untitled.mpi[*] - %1").arg(QCoreApplication::applicationName()));
    else
        setWindowTitle(QString("%1[*] - %2").arg(QFileInfo(filePath_).fileName(), QCoreApplication::applicationName()));

    updateRecentFileActions(filePath_);
}

void frmMain::refreshPortfolioCmb(int id_)
{
    ui->portfolioDropDownCmb->blockSignals(true);
    ui->portfolioDropDownCmb->clear();
    foreach(const portfolio &p, m_portfolios)
    {
        if (p.attributes().deleted)
            continue;

        ui->portfolioDropDownCmb->addItem(p.attributes().description, p.attributes().id);
    }

    int index = ui->portfolioDropDownCmb->findData(id_);
    if (index == -1 && ui->portfolioDropDownCmb->count() != 0)
        index = 0;

    if (index != -1)
        ui->portfolioDropDownCmb->setCurrentIndex(index);

    portfolioChange(ui->portfolioDropDownCmb->currentIndex());
    ui->portfolioDropDownCmb->blockSignals(false);
}

void frmMain::closeEvent(QCloseEvent *event_)
{
    if (!maybeSave())
    {
        event_->ignore();
        return;
    }

    event_->accept();
    saveSettings();
}

void frmMain::saveSettings()
{
    m_settings.windowSize = size();
    m_settings.windowLocation = pos();
    m_settings.windowState = isMaximized() ? Qt::WindowMaximized : isMinimized() ? Qt::WindowMinimized : Qt::WindowNoState;
    m_settings.save();
}

void frmMain::updateRecentFileActions(const QString &newFilePath_)
{
    if (!newFilePath_.isEmpty())
        m_settings.addRecentFile(newFilePath_);

    ui->fileRecent->clear();
    foreach(const QString &s, m_settings.recentFiles())
        connect(ui->fileRecent->addAction(s), SIGNAL(triggered()), this, SLOT(recentFileSelected()));
}

void frmMain::recentFileSelected()
{
    QAction *action = qobject_cast<QAction*>(sender());

    if (!action || action->text() == windowFilePath())
        return;

    if (maybeSave())
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
    recalculateTrades(*m_currentPortfolio, 0);
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
    ui->portfolioDelete->setDisabled(currentIndex_ == -1);
    ui->portfolioEdit->setDisabled(currentIndex_ == -1);
    ui->portfolioDropDownCmb->setDisabled(ui->portfolioDropDownCmb->count() == 0);
    m_currentPortfolio = currentIndex_ == -1 ? 0 : &m_portfolios[ui->portfolioDropDownCmb->itemData(currentIndex_).toInt()];
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

    int beginDate = tradeDateCalendar::endDate() + 1;
    foreach(const portfolio &p, m_portfolios)
        beginDate = qMin(beginDate, p.attributes().startDate);

    QList<historicalPrices> prices;
    foreach(const QString &s, portfolio::symbols(m_portfolios))
        prices.append(priceFactory::getPrices(s));

    updatePricesOptions options(beginDate, tradeDateCalendar::endDate(), m_settings.splits);

    QFutureWatcher<updatePricesReturnValue> watcher;
    QEventLoop eventLoop;
    connect(&watcher, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    watcher.setFuture(QtConcurrent::mapped(prices, updatePrices(options)));

    eventLoop.exec();

    int earliestUpdate = options.endDate + 1;
    QStringList failures;
    foreach(const updatePricesReturnValue &result, watcher.future())
        if (result.date == UNASSIGNED)
            failures.append(result.symbol);
        else
            earliestUpdate = qMin(earliestUpdate, result.date);

    if (earliestUpdate <= options.endDate)
        setWindowModified(true);

    if (!failures.isEmpty())
        QMessageBox::information(this, QCoreApplication::applicationName(), "The following securities were not updated (Yahoo! Finance may not yet have today's price):\n\n" +
            failures.join(", "));
}

void frmMain::recalculateTrades(const portfolio &portfolio_, int beginDate_)
{
    recalculateTrades(QList<portfolio>() << portfolio_, beginDate_);
}

void frmMain::recalculateTrades(int beginDate_)
{
    recalculateTrades(m_portfolios.values(), beginDate_);
}

void frmMain::recalculateTrades(QList<portfolio> portfolios_, int beginDate_)
{
    QFutureWatcher<void> watcher;
    QEventLoop eventLoop;
    connect(&watcher, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    watcher.setFuture(QtConcurrent::map(portfolios_, calculatorTrade(beginDate_)));

    eventLoop.exec();
}
