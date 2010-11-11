#include "frmEditPortfolio_State.h"
#include <QMessageBox>
#include "frmEditPortfolio_UI.h"
#include "portfolioAttributes.h"

frmEditPortfolio_State::frmEditPortfolio_State(portfolio portfolio_, QWidget *parent_):
    frmEditState(portfolio_, parent_),
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
    m_portfolio.attributes().description = ui->descTxt->text();
    m_portfolio.attributes().startDate = ui->startDateDateEdit->date().toJulianDay();
    m_portfolio.attributes().startValue = ui->startValueTxt->text().toInt();
}

void frmEditPortfolio_State::load()
{
    ui->descTxt->setText(m_portfolio.attributes().description);
    ui->startDateDateEdit->setDate(QDate::fromJulianDay(m_portfolio.attributes().startDate));
    ui->startValueTxt->setText(QString::number(m_portfolio.attributes().startValue));
}

bool frmEditPortfolio_State::validate()
{
    QString validation = m_portfolio.attributes().validate();
    if (validation.isEmpty())
        return true;

    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Portfolio validation error", validation);
    return false;
}
