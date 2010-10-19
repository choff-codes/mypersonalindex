#include "frmMain.h"

frmMain::frmMain(QWidget *parent): QMainWindow(parent)
{
    ui.setupUI(this);
    connectSlots();
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
