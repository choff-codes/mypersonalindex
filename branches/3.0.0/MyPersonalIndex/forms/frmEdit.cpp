#include "frmEdit.h"

frmEdit::frmEdit(portfolio portfolio_, QWidget *parent):
    QDialog(parent),
    m_originalPortfolio(portfolio_),
    m_portfolio(portfolio_),
    m_currentTab(tab_portfolio)
{
    m_portfolio.detach();

    ui.setupUI(this);
    ui.acctList->setModel(new objectKeyEditModel(mapToList(m_portfolio.accounts()), ui.acctList));
    ui.aaList->setModel(new objectKeyEditModel(mapToList(m_portfolio.assetAllocations()), ui.aaList));
    ui.securityList->setModel(new objectKeyEditModel(mapToList(m_portfolio.securities()), ui.securityList));

    connectSlots();
}

void frmEdit::connectSlots()
{
    connect(ui.acctAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.aaAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.securityAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.tradeAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

void frmEdit::accept()
{
    if (validate())
        QDialog::accept();
}

void frmEdit::tabChange(int currentIndex_)
{
    m_currentTab = (tab)currentIndex_;
}

void frmEdit::savePortfolio()
{
    m_portfolio.attributes().description = ui.portfolioForm.descTxt->text();
    m_portfolio.attributes().startDate = ui.portfolioForm.startDateDateEdit->date().toJulianDay();
    m_portfolio.attributes().startValue = ui.portfolioForm.startValueTxt->text().toInt();
}

void frmEdit::loadPortfolio()
{
    ui.portfolioForm.descTxt->setText(m_portfolio.attributes().description);
    ui.portfolioForm.startDateDateEdit->setDate(QDate::fromJulianDay(m_portfolio.attributes().startDate));
    ui.portfolioForm.startValueTxt->setText(QString::number(m_portfolio.attributes().startValue));
}

void frmEdit::saveAccount()
{
    account *acct = static_cast<account*>(m_currentItem);
    acct->description = ui.acctForm.descTxt->text();
    acct->taxRate = ui.acctForm.taxRateSpinBox->value();
    acct->taxDeferred = ui.acctForm.taxDeferredChk->isChecked();
    acct->costBasis = (account::costBasisMethod)ui.acctForm.costBasisCmb->itemData(ui.acctForm.costBasisCmb->currentIndex()).toInt();
}

void frmEdit::loadAccount()
{
    account *acct = static_cast<account*>(m_currentItem);
    ui.acctForm.descTxt->setText(acct->description);
    ui.acctForm.taxRateSpinBox->setValue(acct->taxRate);
    ui.acctForm.taxDeferredChk->setChecked(acct->taxDeferred);
    ui.acctForm.costBasisCmb->setCurrentIndex(ui.acctForm.costBasisCmb->findData(acct->costBasis));
}

void frmEdit::saveAssetAllocation()
{
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    aa->description = ui.aaForm.desc->text();
    aa->rebalanceBand = ui.aaForm.rebalanceBandSpinBox->value();
    aa->target = ui.aaForm.targetSpinBox->value();
    aa->threshold = (assetAllocation::thresholdMethod)ui.aaForm.thresholdCmb->currentIndex();
}

void frmEdit::loadAssetAllocation()
{
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    ui.aaForm.desc->setText(aa->description);
    ui.aaForm.rebalanceBandSpinBox->setValue(aa->rebalanceBand);
    ui.aaForm.targetSpinBox->setValue(aa->target);
    ui.aaForm.thresholdCmb->setCurrentIndex(ui.aaForm.thresholdCmb->findData(aa->threshold));
}

void frmEdit::saveSecurity()
{

}

void frmEdit::loadSecurity()
{

}

void frmEdit::saveTrade()
{

}

void frmEdit::loadTrade()
{

}

void frmEdit::save()
{
    switch(m_currentTab)
    {
        case tab_portfolio:
            savePortfolio();
            break;
        case tab_account:
            saveAccount();
            break;
        case tab_assetAllocation:
            saveAssetAllocation();
            break;
        case tab_security:
            saveSecurity();
            break;
        case tab_trade:
            saveTrade();
            break;
    }
}

void frmEdit::load()
{
    switch(m_currentTab)
    {
        case tab_portfolio:
            loadPortfolio();
            break;
        case tab_account:
            loadAccount();
            break;
        case tab_assetAllocation:
            loadAssetAllocation();
            break;
        case tab_security:
            loadSecurity();
            break;
        case tab_trade:
            loadTrade();
            break;
    }
}

void frmEdit::add()
{
    objectKeyEditModel* model = currentModel();
    QListView *listView = currentListView();
    if (!model || !listView)
        return;

    objectKey *key;
    int identity = portfolio::getOpenIdentity();

    switch(m_currentTab)
    {
        case tab_account:
            m_portfolio.accounts().insert(identity, account(identity, m_portfolio.attributes().id));
            key = &m_portfolio.accounts()[identity];
            break;
        case tab_assetAllocation:
            m_portfolio.assetAllocations().insert(identity, assetAllocation(identity, m_portfolio.attributes().id));
            key = &m_portfolio.assetAllocations()[identity];
            break;
        case tab_security:
            m_portfolio.securities().insert(identity, security(identity, m_portfolio.attributes().id));
            key = &m_portfolio.securities()[identity];
            break;
        case tab_trade:
            if (ui.tradeFilterCmb->currentIndex() == -1)
                break;
            m_portfolio.securities()[currentTradeSecurityID()].trades.insert(identity, trade(identity, currentTradeSecurityID()));
            key = &m_portfolio.securities()[currentTradeSecurityID()].trades[identity];
            break;
    }
    model->insert(key);
    listView->setCurrentIndex(model->index(model->rowCount(QModelIndex()) - 1));
}

objectKeyEditModel* frmEdit::currentModel()
{
    switch(m_currentTab)
    {
        case tab_account:
            return static_cast<objectKeyEditModel*>(ui.acctList->model());
        case tab_assetAllocation:
            return static_cast<objectKeyEditModel*>(ui.aaList->model());
        case tab_security:
            return static_cast<objectKeyEditModel*>(ui.securityList->model());
        case tab_trade:
            return static_cast<objectKeyEditModel*>(ui.tradeList->model());
        default:
            return 0;
    }
}

QListView* frmEdit::currentListView()
{
    switch(m_currentTab)
    {
        case tab_account:
            return ui.acctList;
        case tab_assetAllocation:
            return ui.aaList;
        case tab_security:
            return ui.securityList;
        case tab_trade:
            return ui.tradeList;
        default:
            return 0;
    }
}

bool frmEdit::validate()
{
    QString message = m_portfolio.attributes().validate();
    if (!message.isEmpty())
    {
        QMessageBox::critical(this, "Portfolio validation error", message);
        ui.tabs->setCurrentIndex(tab_portfolio);
        return false;
    }

    if (!validateObjectKeys(m_portfolio.accounts(), tab_account, "Account validation error"))
        return false;

    if (!validateObjectKeys(m_portfolio.assetAllocations(), tab_assetAllocation, "Asset class validation error"))
        return false;

    if (!validateObjectKeys(m_portfolio.securities(), tab_security, "Security validation error"))
        return false;

    if (!validateTrades())
        return false;

    return true;
}

bool frmEdit::validateTrades()
{
    for(QMap<int, security>::const_iterator i = m_portfolio.securities().constBegin(); i != m_portfolio.securities().constEnd(); ++i)
        for(QMap<int, trade>::const_iterator x = i.value().trades.constBegin(); x != i.value().trades.constEnd(); ++x)
        {
            QString error = x.value().validate();
            if (error.isEmpty())
                continue;

            QMessageBox::critical(this, "Trade validation error", error);
            ui.tabs->setCurrentIndex(tab_trade);
            ui.tradeFilterCmb->setCurrentIndex(ui.tradeFilterCmb->findData(i.value().id));

            objectKeyEditModel* model = currentModel();
            QListView *listView = currentListView();
            if (!model || !listView)
                return false;

            listView->setCurrentIndex(model->find(&i.value()));
            return false;
        }
    return true;
}
