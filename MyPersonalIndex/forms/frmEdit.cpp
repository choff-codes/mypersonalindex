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
    loadPortfolio();
}

void frmEdit::connectSlots()
{
    connect(ui.tradeAdd, SIGNAL(clicked()), this, SLOT(add()));

    //order matters
    connect(ui.okCancelBtn, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui.okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(save()));
    connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
    connect(ui.aaList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui.aaList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui.aaAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.aaDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui.acctList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui.acctList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui.acctAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.acctDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui.securityList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui.securityList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui.securityAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.securityDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui.securityForm.aaBtnAdd, SIGNAL(clicked()), this, SLOT(securityAddAA()));
    connect(ui.securityForm.aaBtnDelete, SIGNAL(clicked()), this, SLOT(securityDeleteAA()));

    connect(ui.aaForm.targetBtnClear, SIGNAL(clicked()), this, SLOT(resetTarget()));
    connect(ui.acctForm.taxRateBtnClear, SIGNAL(clicked()), this, SLOT(resetTaxRate()));
    connect(ui.securityForm.expenseBtnClear, SIGNAL(clicked()), this, SLOT(resetExpenseRatio()));

}

void frmEdit::accept()
{
    if (validate())
        QDialog::accept();
}

void frmEdit::tabChange(int currentIndex_)
{
    m_currentTab = (tab)currentIndex_;

    if (m_currentTab == tab_portfolio)
    {
        m_currentItem = &m_portfolio.attributes();
        return;
    }
    if (m_currentTab == tab_security)
    {
        ui.securityForm.aaCmb->clear();
        foreach(const assetAllocation &aa, m_portfolio.assetAllocations())
            ui.securityForm.aaCmb->addItem(aa.description, aa.id);
        ui.securityForm.aaCmb->model()->sort(0);
        if (ui.securityForm.aaCmb->count() != 0)
            ui.securityForm.aaCmb->setCurrentIndex(0);

        ui.securityForm.acctCmb->clear();
        ui.securityForm.acctCmb->addItem("", UNASSIGNED);
        foreach(const account &acct, m_portfolio.accounts())
            ui.securityForm.acctCmb->addItem(acct.description, acct.id);
        ui.securityForm.acctCmb->model()->sort(0);
    }

    objectKeyEditModel* model = currentModel();
    QListView *listView = currentListView();
    if (!model || !listView)
    {
        m_currentItem = 0;
        return;
    }
    m_currentItem = model->get(listView->currentIndex().row());
}

void frmEdit::listChange(const QModelIndex &current_, const QModelIndex &previous_)
{
    objectKeyEditModel* model = currentModel();
    QListView *listView = currentListView();
    if (!model || !listView)
    {
        m_currentItem = 0;
        return;
    }
    model->refresh(previous_);
    m_currentItem = model->get(current_.row());
    load();
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
    if (!m_currentItem)
        return;
    account *acct = static_cast<account*>(m_currentItem);
    acct->description = ui.acctForm.descTxt->text();
    acct->taxRate = ui.acctForm.taxRateSpinBox->value();
    acct->taxDeferred = ui.acctForm.taxDeferredChk->isChecked();
    acct->costBasis = (account::costBasisMethod)ui.acctForm.costBasisCmb->itemData(ui.acctForm.costBasisCmb->currentIndex()).toInt();
}

void frmEdit::loadAccount()
{
    if (!m_currentItem)
        return;
    account *acct = static_cast<account*>(m_currentItem);
    ui.acctForm.descTxt->setText(acct->description);
    ui.acctForm.taxRateSpinBox->setValue(acct->taxRate);
    ui.acctForm.taxDeferredChk->setChecked(acct->taxDeferred);
    ui.acctForm.costBasisCmb->setCurrentIndex(ui.acctForm.costBasisCmb->findData(acct->costBasis));
}

void frmEdit::saveAssetAllocation()
{
    if (!m_currentItem)
        return;
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    aa->description = ui.aaForm.descTxt->text();
    aa->rebalanceBand = ui.aaForm.rebalanceBandSpinBox->value();
    aa->target = ui.aaForm.targetSpinBox->value();
    aa->threshold = (assetAllocation::thresholdMethod)ui.aaForm.thresholdCmb->itemData(ui.aaForm.thresholdCmb->currentIndex()).toInt();
}

void frmEdit::loadAssetAllocation()
{
    if (!m_currentItem)
        return;
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    ui.aaForm.descTxt->setText(aa->description);
    ui.aaForm.rebalanceBandSpinBox->setValue(aa->rebalanceBand);
    ui.aaForm.targetSpinBox->setValue(aa->target);
    ui.aaForm.thresholdCmb->setCurrentIndex(ui.aaForm.thresholdCmb->findData(aa->threshold));
}

void frmEdit::saveSecurity()
{
    if (!m_currentItem)
        return;
    security *sec = static_cast<security*>(m_currentItem);
    sec->description = ui.securityForm.symbolTxt->text();
    sec->account = ui.securityForm.acctCmb->itemData(ui.securityForm.acctCmb->currentIndex()).toInt();
    sec->expenseRatio = ui.securityForm.expenseSpinBox->value();
    sec->cashAccount = ui.securityForm.cashChk->isChecked();
    sec->dividendNAVAdjustment = ui.securityForm.dividendNAVAdjustmentChk->isChecked();
    sec->dividendReinvestment = ui.securityForm.dividendReinvestmentChk->isChecked();
    sec->hide = ui.securityForm.hideChk->isChecked();
    sec->note = ui.securityForm.notesTxt->toPlainText();
    sec->includeInCalc = ui.securityForm.includeChk->isChecked();
    sec->targets = static_cast<securityAAModel*>(ui.securityForm.aa->model())->getTargets();
}

void frmEdit::loadSecurity()
{
    if (!m_currentItem)
        return;
    security *sec = static_cast<security*>(m_currentItem);
    ui.securityForm.symbolTxt->setText(sec->description);
    ui.securityForm.acctCmb->setCurrentIndex(ui.securityForm.acctCmb->findData(sec->account));
    ui.securityForm.expenseSpinBox->setValue(sec->expenseRatio);
    ui.securityForm.cashChk->setChecked(sec->cashAccount);
    ui.securityForm.dividendNAVAdjustmentChk->setChecked(sec->dividendNAVAdjustment);
    ui.securityForm.dividendReinvestmentChk->setChecked(sec->dividendReinvestment);
    ui.securityForm.hideChk->setChecked(sec->hide);
    ui.securityForm.includeChk->setChecked(sec->includeInCalc);
    ui.securityForm.notesTxt->setPlainText(sec->note);

    QAbstractItemModel *model = ui.securityForm.aa->model();
    ui.securityForm.aa->setModel(new securityAAModel(sec->targets, m_portfolio.assetAllocations(), ui.securityForm.aa));
    delete model;
}

void frmEdit::saveTrade()
{
    if (!m_currentItem)
        return;
    trade *t = static_cast<trade*>(m_currentItem);
    t->action = (trade::tradeAction)ui.tradeForm.actionCmb->itemData(ui.tradeForm.actionCmb->currentIndex()).toInt();
    t->cashAccount = ui.tradeForm.cashCmb->itemData(ui.tradeForm.cashCmb->currentIndex()).toInt();
    t->commission = ui.tradeForm.commissionTxt->text().toDouble();
    t->date = ui.tradeForm.dateDateEdit->isEnabled() ? ui.tradeForm.dateDateEdit->date().toJulianDay() : 0;
    t->startDate = ui.tradeForm.startingDateEdit->isEnabled() ? ui.tradeForm.startingDateEdit->date().toJulianDay() : 0;
    t->endDate = ui.tradeForm.endingDateEdit->isEnabled() ? ui.tradeForm.endingDateEdit->date().toJulianDay() : 0;
    t->description = ui.tradeForm.noteTxt->toPlainText();
    t->frequency = (tradeDateCalendar::frequency)ui.tradeForm.freqCmb->itemData(ui.tradeForm.freqCmb->currentIndex()).toInt();
    t->price = ui.tradeForm.priceChk->isChecked() && !ui.tradeForm.priceTxt->text().isEmpty() ? ui.tradeForm.priceTxt->text().toDouble() : -1;
    t->value = ui.tradeForm.sharesTxt->text().toDouble();
}

void frmEdit::loadTrade()
{
    if (!m_currentItem)
        return;
    trade *t = static_cast<trade*>(m_currentItem);
    ui.tradeForm.actionCmb->setCurrentIndex(ui.tradeForm.actionCmb->findData(t->action));
    ui.tradeForm.cashCmb->setCurrentIndex(ui.tradeForm.cashCmb->findData(t->cashAccount));
    ui.tradeForm.commissionTxt->setText(QString::number(t->commission, 'f', 4));
    ui.tradeForm.dateDateEdit->setDate(t->date != 0 ? QDate::fromJulianDay(t->date) : QDate::currentDate());
    ui.tradeForm.startingChk->setChecked(t->startDate != 0);
    ui.tradeForm.startingDateEdit->setDate(t->startDate != 0 ? QDate::fromJulianDay(t->startDate) : QDate::currentDate());
    ui.tradeForm.endingChk->setChecked(t->endDate != 0);
    ui.tradeForm.endingDateEdit->setDate(t->endDate != 0 ? QDate::fromJulianDay(t->endDate) : QDate::currentDate());
    ui.tradeForm.noteTxt->setPlainText(t->description);
    ui.tradeForm.freqCmb->setCurrentIndex(ui.tradeForm.freqCmb->findData(t->frequency));
    ui.tradeForm.priceChk->setChecked(functions::massage(t->price) < 0);
    ui.tradeForm.priceTxt->setText(QString::number(t->price, 'f', 4));
    ui.tradeForm.sharesTxt->setText(QString::number(t->value, 'f', 4));
}

void frmEdit::save()
{
    if (!m_currentItem)
        return;

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
            ui.acctFormWidget->setEnabled(m_currentItem);
            loadAccount();
            break;
        case tab_assetAllocation:
            ui.aaFormWidget->setEnabled(m_currentItem);
            loadAssetAllocation();
            break;
        case tab_security:
            ui.securityFormWidget->setEnabled(m_currentItem);
            loadSecurity();
            break;
        case tab_trade:
            ui.tradeFormWidget->setEnabled(m_currentItem);
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
        default:
            return;
    }
    model->insert(key);
    listView->setCurrentIndex(model->index(model->rowCount(QModelIndex()) - 1));
}

void frmEdit::remove()
{
    objectKeyEditModel* model = currentModel();
    QListView *listView = currentListView();
    if (!model || !listView || !m_currentItem)
        return;

    objectKey *key = m_currentItem;

    switch(m_currentTab)
    {
        case tab_account:
            model->remove(key);
            m_portfolio.accounts().remove(key->id);
            break;
        case tab_assetAllocation:
            model->remove(key);
            m_portfolio.assetAllocations().remove(key->id);
            break;
        case tab_security:
            model->remove(key);
            m_portfolio.securities().remove(key->id);
            break;
        case tab_trade:
            model->remove(key);
            m_portfolio.securities()[key->parent].trades.remove(key->id);
            break;
        default:
            return;
    }
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
    for(QMap<int, security>::iterator i = m_portfolio.securities().begin(); i != m_portfolio.securities().end(); ++i)
        for(QMap<int, trade>::iterator x = i.value().trades.begin(); x != i.value().trades.end(); ++x)
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

void frmEdit::securityAddAA()
{
    if (ui.securityForm.aaCmb->currentIndex() == -1)
        return;

    static_cast<securityAAModel*>(ui.securityForm.aa->model())->addNew(ui.securityForm.aaCmb->itemData(ui.securityForm.aaCmb->currentIndex()).toInt());
}

void frmEdit::securityDeleteAA()
{
    static_cast<securityAAModel*>(ui.securityForm.aa->model())->deleteSelected(ui.securityForm.aa->selectionModel());
}
