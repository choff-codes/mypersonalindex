#include "frmMain.h"

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

