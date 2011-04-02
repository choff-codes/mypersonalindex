#include "frmMainTrade_State.h"
#include "frmMainTreeView_UI.h"

frmMainTrade_State::frmMainTrade_State(const portfolio &portfolio_, QWidget *parent_):
    frmMainState(portfolio_, parent_),
    ui(new frmMainTreeView_UI())
{
    ui->setupUI(static_cast<QWidget*>(this->parent()));
}

frmMainTrade_State::~frmMainTrade_State()
{
    delete ui;
}

QWidget* frmMainTrade_State::mainWidget()
{
    return ui->widget;
}
