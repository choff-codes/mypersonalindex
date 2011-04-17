#include "frmEdit.h"
#include "frmEdit_UI.h"
#include "frmEditAA_State.h"
#include "frmEditAcct_State.h"
#include "frmEditSecurity_State.h"
#include "frmEditTrade_State.h"
#include "frmEditPortfolio_State.h"


frmEdit::frmEdit(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent):
    QDialog(parent),
    ui(new frmEdit_UI),
    m_portfolioToReturn(portfolio_),
    m_portfolio(portfolio_),
    m_identitiesToReturn(identities_),
    m_identities(identities_),
    m_currentTab(tab_portfolio)
{
    m_portfolio.detach();
    m_identities.detach();

    ui->setupUI(this);

    m_tabs.insert(tab_portfolio, new frmEditPortfolio_State(m_portfolio, m_identities, ui->portfolioTab));
    m_tabs.insert(tab_account, new frmEditAcct_State(m_portfolio, m_identities, ui->acctTab));
    m_tabs.insert(tab_assetAllocation, new frmEditAA_State(m_portfolio, m_identities, ui->aaTab));
    m_tabs.insert(tab_security, new frmEditSecurity_State(m_portfolio, m_identities, ui->securityTab));
    m_tabs.insert(tab_trade, new frmEditTrade_State(m_portfolio, m_identities, ui->tradeTab));

    connectSlots();
    m_tabs.value(tab_portfolio)->enter();
}

frmEdit::~frmEdit()
{
    qDeleteAll(m_tabs);
    delete ui;
}

void frmEdit::connectSlots()
{
    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okCancelBtn->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

void frmEdit::accept()
{
    m_tabs.value(m_currentTab)->save();

    if (!validate())
        return;

    m_portfolioToReturn = m_portfolio;
    m_identitiesToReturn = m_identities;
    QDialog::accept();
}

void frmEdit::apply()
{
    m_tabs.value(m_currentTab)->save();

    if (!validate())
        return;

    m_portfolioToReturn = m_portfolio;
    m_portfolioToReturn.detach();
    m_identitiesToReturn = m_identities;
    m_identitiesToReturn.detach();
}

void frmEdit::tabChange(int currentIndex_)
{
    m_tabs.value(m_currentTab)->save();
    m_tabs.value(m_currentTab)->leave();
    m_currentTab = (tab)currentIndex_;
    m_tabs.value(m_currentTab)->enter();
}

bool frmEdit::validate()
{
    for(QMap<tab, frmEditState*>::const_iterator i = m_tabs.constBegin(); i != m_tabs.constEnd(); ++i)
        if (!i.value()->validate())
        {
            ui->tabs->setCurrentIndex(i.key());
            return false;
        }

    return true;
}
