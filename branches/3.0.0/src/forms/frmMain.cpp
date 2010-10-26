#include "frmMain.h"
#include "frmMain_UI.h"
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "frmEdit.h"
#include "settingsFactory.h"
#include "portfolioFactory.h"
#include "priceFactory.h"

frmMain::frmMain(QWidget *parent):
    QMainWindow(parent),
    ui(new frmMain_UI)
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
    connect(ui->portfolioEdit, SIGNAL(triggered()), this, SLOT(showPortfolioEdit()));
    connect(ui->helpAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->fileOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->fileSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->fileSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->fileNew, SIGNAL(triggered()), this, SLOT(newFile()));
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

void frmMain::showPortfolioEdit()
{
    frmEdit form(portfolio(), this);
    form.exec();
}

void frmMain::newFile()
{
    if (maybeSave())
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
    if (windowFilePath().isEmpty())
    {
        if (!QFile::copy("MPI.sqlite", filePath_))
        {
            QMessageBox::warning(this, QCoreApplication::applicationName(), QString("Could not save to %1!").arg(filePath_));
            return false;
        }
    }
    else
    {
        if (windowFilePath() != filePath_)
            if (!QFile::copy(windowFilePath(), filePath_))
            {
                QMessageBox::warning(this, QCoreApplication::applicationName(),
                    QString("Could not save to %1 OR the original file was deleted at %2!").arg(filePath_, windowFilePath()));
                return false;
            }
    }

    queries file(filePath_);

    if (file.getDatabaseVersion() == UNASSIGNED)
    {
        QMessageBox::critical(this, QCoreApplication::applicationName(), QString("%1 is not a valid My Personal Index file!").arg(filePath_));
        return false;
    }

    foreach(portfolio p, m_portfolios)
        p.save(file);

    priceFactory::save(file);
    setCurrentFile(filePath_);
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

    priceFactory::close();
    m_portfolios = portfolioFactory(file).getPortfolios(true);
    setCurrentFile(filePath_);
    m_settings.addRecentFile(filePath_);
    updateRecentFileActions();
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

void frmMain::closeEvent(QCloseEvent */*event_*/)
{
    saveSettings();
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

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString::number(APP_VERSION / 100.0) + "</h2>"
        "<p>Copyright &copy; 2010"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}
