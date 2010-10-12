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
    connect(ui.tradeAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.tradeDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui.tradeFilterCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeSecurityFilterChange()));
    connect(ui.tradeForm.startingChk, SIGNAL(toggled(bool)), ui.tradeForm.startingDateEdit, SLOT(setEnabled(bool)));
    connect(ui.tradeForm.endingChk, SIGNAL(toggled(bool)), ui.tradeForm.endingDateEdit, SLOT(setEnabled(bool)));
    connect(ui.tradeForm.freqCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeFrequencyChange(int)));
    connect(ui.tradeForm.actionCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeActionChange(int)));
    connect(ui.tradeForm.priceChk, SIGNAL(toggled(bool)), this, SLOT(tradePriceChange(bool)));

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

    switch(m_currentTab)
    {
        case tab_portfolio:
            m_currentItem = &m_portfolio.attributes();
            return;
        case tab_security:
            populateSecurityTab();
            break;
        case tab_trade:
            populateTradeTab();
            break;
        default:
            break;
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
    t->value = ui.tradeForm.shares->text().toDouble();
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
    if (functions::massage(t->price) >= 0)
    {
        ui.tradeForm.priceTxt->setText(QString::number(t->price, 'f', 4));
        ui.tradeForm.priceChk->setChecked(true);
    }
    else
        ui.tradeForm.priceChk->setChecked(false);
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

            listView->setCurrentIndex(model->find(&x.value()));
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

void frmEdit::populateSecurityTab()
{
    ui.securityForm.aaCmb->clear();
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, assetAllocation>::const_iterator i = m_portfolio.assetAllocations().begin(); i != m_portfolio.assetAllocations().end(); ++i)
        ui.securityForm.aaCmb->addItem(i.value().displayText(), i.value().id);
    ui.securityForm.aaCmb->model()->sort(0);
    if (ui.securityForm.aaCmb->count() != 0)
        ui.securityForm.aaCmb->setCurrentIndex(0);

    ui.securityForm.acctCmb->clear();
    ui.securityForm.acctCmb->addItem("", UNASSIGNED);
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, account>::const_iterator i = m_portfolio.accounts().begin(); i != m_portfolio.accounts().end(); ++i)
        ui.securityForm.acctCmb->addItem(i.value().displayText(), i.value().id);
    ui.securityForm.acctCmb->model()->sort(0);
}

void frmEdit::populateTradeTab()
{
    int currentSecurityFilter = currentTradeSecurityID();
    ui.tradeFilterCmb->blockSignals(true);

    ui.tradeFilterCmb->clear();
    ui.tradeForm.cashCmb->clear();

    ui.tradeForm.cashCmb->addItem("", UNASSIGNED);

    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, security>::const_iterator i = m_portfolio.securities().begin(); i != m_portfolio.securities().end(); ++i)
    {
        QString item = functions::formatForComboBox(i.value().displayText(), i.value().note);
        ui.tradeFilterCmb->addItem(item, i.value().id);
        ui.tradeForm.cashCmb->addItem(item, i.value().id);
    }
    ui.tradeFilterCmb->model()->sort(0);
    ui.tradeForm.cashCmb->model()->sort(0);

    int selectRow = ui.tradeFilterCmb->findData(currentSecurityFilter);
    if (selectRow != -1)
        ui.tradeFilterCmb->setCurrentIndex(selectRow == -1 ? 0 : selectRow);
    else
    {
        if (ui.tradeFilterCmb->count() != 0)
            ui.tradeFilterCmb->setCurrentIndex(0);
        tradeSecurityFilterChange();
    }
    ui.tradeFilterCmb->blockSignals(false);
}

void frmEdit::tradeSecurityFilterChange()
{
    int securityID = currentTradeSecurityID();
    if (securityID == -1)
    {
        QAbstractItemModel *model = currentModel();
        ui.tradeList->setModel(0);
        delete model;
        ui.tradeFormWidget->setEnabled(false);
        return;
    }

    QAbstractItemModel *model = currentModel();
    ui.tradeList->setModel(new objectKeyEditModel(mapToList(m_portfolio.securities()[securityID].trades), ui.tradeList));
    connect(ui.tradeList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui.tradeList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    delete model;

    if (currentModel()->rowCount(QModelIndex()) > 0)
        ui.tradeList->setCurrentIndex(currentModel()->index(0, 0));
}

void frmEdit::tradeFrequencyChange(int index_)
{
    ui.tradeForm.dateDateEdit->setEnabled(true);
    switch ((tradeDateCalendar::frequency)index_)
    {
        case tradeDateCalendar::frequency_Daily:
            ui.tradeForm.dateDateEdit->setDisabled(true);
            break;
        case tradeDateCalendar::frequency_Monthly:
            ui.tradeForm.dateDateEdit->setDisplayFormat("dd");
            ui.tradeForm.dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            ui.tradeForm.dateDateEdit->setMaximumDate(QDate(2009, 1, 31));
            ui.tradeForm.dateDateEdit->setCalendarPopup(false);
            ui.tradeForm.dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
        case tradeDateCalendar::frequency_Once:
            ui.tradeForm.dateDateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
            ui.tradeForm.dateDateEdit->clearMinimumDate();
            ui.tradeForm.dateDateEdit->clearMaximumDate();
            ui.tradeForm.dateDateEdit->setCalendarPopup(true);
            ui.tradeForm.dateDateEdit->setDate(QDate::currentDate());
            break;
        case tradeDateCalendar::frequency_Weekly:
            ui.tradeForm.dateDateEdit->setDisplayFormat("dddd");
            ui.tradeForm.dateDateEdit->setMinimumDate(QDate(2009, 1, 5));
            ui.tradeForm.dateDateEdit->setMaximumDate(QDate(2009, 1, 9));
            ui.tradeForm.dateDateEdit->setCalendarPopup(false);
            ui.tradeForm.dateDateEdit->setDate(QDate(2009, 1, 5));
            break;
        case tradeDateCalendar::frequency_Yearly:
            ui.tradeForm.dateDateEdit->setDisplayFormat("dd MMM");
            ui.tradeForm.dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            ui.tradeForm.dateDateEdit->setMaximumDate(QDate(2009, 12, 31));
            ui.tradeForm.dateDateEdit->setCalendarPopup(false);
            ui.tradeForm.dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
    }
}

void frmEdit::tradeActionChange(int index_)
{
    switch ((trade::tradeAction)index_)
    {
        case trade::tradeAction_Purchase:
            ui.tradeForm.shares->setText("Shares:");
            break;
        case trade::tradeAction_Sale:
            ui.tradeForm.shares->setText("Shares:");
            break;
        case trade::tradeAction_DivReinvest:
            ui.tradeForm.shares->setText("Shares:");
            break;
        case trade::tradeAction_Interest:
            ui.tradeForm.shares->setText("Amount ($):");
            break;
        case trade::tradeAction_FixedPurchase:
        case trade::tradeAction_FixedSale:
            ui.tradeForm.shares->setText("Amount ($):");
            break;
        case trade::tradeAction_Value:
            ui.tradeForm.shares->setText("% of Value:");
            break;
        case trade::tradeAction_InterestPercent:
            ui.tradeForm.shares->setText("Rate (%):");
            break;
        case trade::tradeAction_TotalValue:
            ui.tradeForm.shares->setText("% of Total");
            break;
        case trade::tradeAction_AA:
            ui.tradeForm.shares->setText("% of Target:");
            break;
        case trade::tradeAction_DivReinvestAuto:
            break;
    }
}

void frmEdit::tradePriceChange(bool checked_)
{
    ui.tradeForm.priceTxt->setEnabled(checked_);
    if (!checked_)
        ui.tradeForm.priceTxt->setText("Previous Close");
    else
        ui.tradeForm.priceTxt->setText("0.0000");
}

