#include "frmEditPortfolio_State.h"
#include <QMessageBox>
#include "frmEditPortfolio_UI.h"

frmEditPortfolio_State::frmEditPortfolio_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_):
    frmEditState(portfolio_, identities_, parent_),
    ui(new frmEditPortfolio_UI())
{
    ui->setupUI(parent_);
    load();
}

frmEditPortfolio_State::~frmEditPortfolio_State()
{
    delete ui;
}

void frmEditPortfolio_State::save()
{
    m_portfolio.setDescription(ui->descTxt->text());
    m_portfolio.setStartDate(ui->startDateDateEdit->date().toJulianDay());
}

void frmEditPortfolio_State::load()
{
    ui->descTxt->setText(m_portfolio.description());
    ui->startDateDateEdit->setDate(QDate::fromJulianDay(m_portfolio.startDate()));
}

bool frmEditPortfolio_State::validate()
{
    QString validation = m_portfolio.validate();
    if (validation.isEmpty())
        return true;

    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Portfolio validation error", validation);
    return false;
}
