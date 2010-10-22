#include "frmMain.h"
#include <QCloseEvent>
#include "frmEdit.h"
#include "settingsFactory.h"

frmMain::frmMain(QWidget *parent): QMainWindow(parent)
{
    ui.setupUI(this);
    connectSlots();
    m_settings = settingsFactory().getSettings();
    if (m_settings.windowState != Qt::WindowActive)
    {
        resize(m_settings.windowSize);
        move(m_settings.windowLocation);
        if (m_settings.windowState != Qt::WindowNoState)
            this->setWindowState(this->windowState() | m_settings.windowState);
    }
}

void frmMain::connectSlots()
{
    connect(ui.portfolioEdit, SIGNAL(triggered()), this, SLOT(showPortfolioEdit()));
    connect(ui.helpAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void frmMain::showPortfolioEdit()
{
    frmEdit form(portfolio(), this);
    form.exec();
}

void frmMain::closeEvent(QCloseEvent */*event_*/)
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
