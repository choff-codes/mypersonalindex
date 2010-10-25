#include "frmMain.h"
#include "frmMain_UI.h"
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
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::connectSlots()
{
    connect(ui->portfolioEdit, SIGNAL(triggered()), this, SLOT(showPortfolioEdit()));
    connect(ui->helpAbout, SIGNAL(triggered()), this, SLOT(about()));
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

void frmMain::open()
{
    filePath = QFileDialog::getOpenFileName(this, "Open file...", QString(), "My Personal Index File (*.mpi)");

    if (filePath.isEmpty())
        return;

    queries file(filePath);

    if (file.getDatabaseVersion() == UNASSIGNED)
        return;

    priceFactory::close(m_fileLocation);
    m_portfolios = portfolioFactory(file).getPortfolios(true);
    m_fileLocation = filePath;
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

void frmMain::about()
{
    QMessageBox::about(this, "About My Personal Index", "<h2>My Personal Index " + QString::number(APP_VERSION / 100.0) + "</h2>"
        "<p>Copyright &copy; 2010"
        "<p>By Matthew Wikler"
        "<p>Create personal indexes and perform analysis to make better investing decisions."
        "<br><a href='http://code.google.com/p/mypersonalindex/'>http://code.google.com/p/mypersonalindex/</a></p>");
}
