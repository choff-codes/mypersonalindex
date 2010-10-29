#include "frmEdit.h"
#include "frmEdit_UI.h"
#include <QMessageBox>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include "portfolioAttributes.h"
#include "securityAAModel.h"
#include "security.h"
#include "functions.h"
#include "account.h"

const int frmEdit::m_magicNumber = rand();

frmEdit::frmEdit(portfolio portfolio_, QWidget *parent):
    QDialog(parent),
    ui(new frmEdit_UI),
    m_portfolioToReturn(portfolio_),
    m_portfolio(portfolio_),
    m_currentTab(tab_portfolio)
{
    m_portfolio.detach();

    ui->setupUI(this);
    ui->acctList->setModel(new objectKeyEditModel(mapToList(m_portfolio.accounts()), ui->acctList));
    ui->aaList->setModel(new objectKeyEditModel(mapToList(m_portfolio.assetAllocations()), ui->aaList));
    ui->securityList->setModel(new objectKeyEditModel(mapToList(m_portfolio.securities()), ui->securityList));

    connectSlots();
    loadPortfolio();
    tabChange(tab_portfolio);
}

frmEdit::~frmEdit()
{
    delete ui;
}

void frmEdit::connectSlots()
{
    //order matters!!
    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(save()));
    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okCancelBtn->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->okCancelBtn->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(save()));
    connect(ui->tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
    connect(ui->aaList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->aaList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->aaAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->aaDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->acctList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->acctList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->acctAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->acctDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->securityList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->securityList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->securityAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->securityDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->securityForm.aaBtnAdd, SIGNAL(clicked()), this, SLOT(securityAddAA()));
    connect(ui->securityForm.aaBtnDelete, SIGNAL(clicked()), this, SLOT(securityDeleteAA()));
    connect(ui->tradeAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->tradeDelete, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->tradeFilterCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
    connect(ui->tradeFilterCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeSecurityFilterChange()));
    connect(ui->tradeForm.startingChk, SIGNAL(toggled(bool)), ui->tradeForm.startingDateEdit, SLOT(setEnabled(bool)));
    connect(ui->tradeForm.endingChk, SIGNAL(toggled(bool)), ui->tradeForm.endingDateEdit, SLOT(setEnabled(bool)));
    connect(ui->tradeForm.freqCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeFrequencyChange(int)));
    connect(ui->tradeForm.actionCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeActionChange(int)));
    connect(ui->tradeForm.priceChk, SIGNAL(toggled(bool)), this, SLOT(tradePriceChange(bool)));

    connect(ui->aaForm.targetBtnClear, SIGNAL(clicked()), this, SLOT(resetTarget()));
    connect(ui->acctForm.taxRateBtnClear, SIGNAL(clicked()), this, SLOT(resetTaxRate()));
    connect(ui->securityForm.expenseBtnClear, SIGNAL(clicked()), this, SLOT(resetExpenseRatio()));

    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->acctList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->aaList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->securityList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->tradeList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->acctCopyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->acctPasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->aaCopyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->aaPasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->securityCopyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->securityPasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->tradeCopyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->tradePasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
}

void frmEdit::accept()
{
    if (!validate())
        return;

    m_portfolioToReturn = m_portfolio;
    QDialog::accept();
}

void frmEdit::apply()
{
    if (!validate())
        return;

    m_portfolioToReturn = m_portfolio;
    m_portfolio.detach();
}

void frmEdit::tabChange(int currentIndex_)
{
    m_currentTab = (tab)currentIndex_;
    // prevent signals from firing on focus
    ui->aaList->setDisabled(true);
    ui->acctList->setDisabled(true);
    ui->securityList->setDisabled(true);
    ui->tradeList->setDisabled(true);

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
        case tab_account:
        case tab_assetAllocation:
            break;
    }

    if (!isValidCurrentModel())
        return;

    currentListView()->setEnabled(true);
    m_currentItem = currentModel()->get(currentListView()->currentIndex().row());
}

void frmEdit::listChange(const QModelIndex &current_, const QModelIndex &previous_)
{
    if (!isValidCurrentModel())
        return;

    currentModel()->refresh(previous_);
    m_currentItem = currentModel()->get(current_.row());
    load();
}

void frmEdit::savePortfolio()
{
    m_portfolio.attributes().description = ui->portfolioForm.descTxt->text();
    m_portfolio.attributes().startDate = ui->portfolioForm.startDateDateEdit->date().toJulianDay();
    m_portfolio.attributes().startValue = ui->portfolioForm.startValueTxt->text().toInt();
}

void frmEdit::loadPortfolio()
{
    ui->portfolioForm.descTxt->setText(m_portfolio.attributes().description);
    ui->portfolioForm.startDateDateEdit->setDate(QDate::fromJulianDay(m_portfolio.attributes().startDate));
    ui->portfolioForm.startValueTxt->setText(QString::number(m_portfolio.attributes().startValue));
}

void frmEdit::saveAccount()
{
    if (!m_currentItem)
        return;
    account *acct = static_cast<account*>(m_currentItem);
    acct->description = ui->acctForm.descTxt->text();
    acct->taxRate = ui->acctForm.taxRateSpinBox->value();
    acct->taxDeferred = ui->acctForm.taxDeferredChk->isChecked();
    acct->costBasis = (account::costBasisMethod)ui->acctForm.costBasisCmb->itemData(ui->acctForm.costBasisCmb->currentIndex()).toInt();
}

void frmEdit::loadAccount()
{
    if (!m_currentItem)
        return;
    account *acct = static_cast<account*>(m_currentItem);
    ui->acctForm.descTxt->setText(acct->description);
    ui->acctForm.taxRateSpinBox->setValue(acct->taxRate);
    ui->acctForm.taxDeferredChk->setChecked(acct->taxDeferred);
    ui->acctForm.costBasisCmb->setCurrentIndex(ui->acctForm.costBasisCmb->findData(acct->costBasis));
}

void frmEdit::saveAssetAllocation()
{
    if (!m_currentItem)
        return;
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    aa->description = ui->aaForm.descTxt->text();
    aa->rebalanceBand = ui->aaForm.rebalanceBandSpinBox->value();
    aa->target = ui->aaForm.targetSpinBox->value();
    aa->threshold = (assetAllocation::thresholdMethod)ui->aaForm.thresholdCmb->itemData(ui->aaForm.thresholdCmb->currentIndex()).toInt();
}

void frmEdit::loadAssetAllocation()
{
    if (!m_currentItem)
        return;
    assetAllocation *aa = static_cast<assetAllocation*>(m_currentItem);
    ui->aaForm.descTxt->setText(aa->description);
    ui->aaForm.rebalanceBandSpinBox->setValue(aa->rebalanceBand);
    ui->aaForm.targetSpinBox->setValue(aa->target);
    ui->aaForm.thresholdCmb->setCurrentIndex(ui->aaForm.thresholdCmb->findData(aa->threshold));
}

void frmEdit::saveSecurity()
{
    if (!m_currentItem)
        return;
    security *sec = static_cast<security*>(m_currentItem);
    sec->description = ui->securityForm.symbolTxt->text();
    sec->account = ui->securityForm.acctCmb->itemData(ui->securityForm.acctCmb->currentIndex()).toInt();
    sec->expenseRatio = ui->securityForm.expenseSpinBox->value();
    sec->cashAccount = ui->securityForm.cashChk->isChecked();
    sec->dividendNAVAdjustment = ui->securityForm.dividendNAVAdjustmentChk->isChecked();
    sec->dividendReinvestment = ui->securityForm.dividendReinvestmentChk->isChecked();
    sec->hide = ui->securityForm.hideChk->isChecked();
    sec->note = ui->securityForm.notesTxt->toPlainText();
    sec->includeInCalc = ui->securityForm.includeChk->isChecked();
    sec->targets = static_cast<securityAAModel*>(ui->securityForm.aa->model())->getTargets();
}

void frmEdit::loadSecurity()
{
    if (!m_currentItem)
        return;
    security *sec = static_cast<security*>(m_currentItem);
    ui->securityForm.symbolTxt->setText(sec->description);
    ui->securityForm.acctCmb->setCurrentIndex(ui->securityForm.acctCmb->findData(sec->account));
    ui->securityForm.expenseSpinBox->setValue(sec->expenseRatio);
    ui->securityForm.cashChk->setChecked(sec->cashAccount);
    ui->securityForm.dividendNAVAdjustmentChk->setChecked(sec->dividendNAVAdjustment);
    ui->securityForm.dividendReinvestmentChk->setChecked(sec->dividendReinvestment);
    ui->securityForm.hideChk->setChecked(sec->hide);
    ui->securityForm.includeChk->setChecked(sec->includeInCalc);
    ui->securityForm.notesTxt->setPlainText(sec->note);

    QAbstractItemModel *model = ui->securityForm.aa->model();
    ui->securityForm.aa->setModel(new securityAAModel(sec->targets, m_portfolio.assetAllocations(), ui->securityForm.aa));
    delete model;
}

void frmEdit::saveTrade()
{
    if (!m_currentItem)
        return;
    trade *t = static_cast<trade*>(m_currentItem);
    t->action = (trade::tradeAction)ui->tradeForm.actionCmb->itemData(ui->tradeForm.actionCmb->currentIndex()).toInt();
    t->cashAccount = ui->tradeForm.cashCmb->itemData(ui->tradeForm.cashCmb->currentIndex()).toInt();
    t->commission = ui->tradeForm.commissionTxt->text().toDouble();
    t->date = ui->tradeForm.dateDateEdit->isEnabled() ? ui->tradeForm.dateDateEdit->date().toJulianDay() : 0;
    t->startDate = ui->tradeForm.startingDateEdit->isEnabled() ? ui->tradeForm.startingDateEdit->date().toJulianDay() : 0;
    t->endDate = ui->tradeForm.endingDateEdit->isEnabled() ? ui->tradeForm.endingDateEdit->date().toJulianDay() : 0;
    t->description = ui->tradeForm.noteTxt->toPlainText();
    t->frequency = (tradeDateCalendar::frequency)ui->tradeForm.freqCmb->itemData(ui->tradeForm.freqCmb->currentIndex()).toInt();
    t->price = ui->tradeForm.priceChk->isChecked() && !ui->tradeForm.priceTxt->text().isEmpty() ? ui->tradeForm.priceTxt->text().toDouble() : -1;
    t->value = ui->tradeForm.sharesTxt->text().toDouble();
}

void frmEdit::loadTrade()
{
    if (!m_currentItem)
        return;
    trade *t = static_cast<trade*>(m_currentItem);
    ui->tradeForm.actionCmb->setCurrentIndex(ui->tradeForm.actionCmb->findData(t->action));
    ui->tradeForm.cashCmb->setCurrentIndex(ui->tradeForm.cashCmb->findData(t->cashAccount));
    ui->tradeForm.commissionTxt->setText(QString::number(t->commission, 'f', 4));
    ui->tradeForm.dateDateEdit->setDate(t->date != 0 ? QDate::fromJulianDay(t->date) : QDate::currentDate());
    ui->tradeForm.startingChk->setChecked(t->startDate != 0);
    ui->tradeForm.startingDateEdit->setDate(t->startDate != 0 ? QDate::fromJulianDay(t->startDate) : QDate::currentDate());
    ui->tradeForm.endingChk->setChecked(t->endDate != 0);
    ui->tradeForm.endingDateEdit->setDate(t->endDate != 0 ? QDate::fromJulianDay(t->endDate) : QDate::currentDate());
    ui->tradeForm.noteTxt->setPlainText(t->description);
    ui->tradeForm.freqCmb->setCurrentIndex(ui->tradeForm.freqCmb->findData(t->frequency));
    if (functions::massage(t->price) >= 0)
    {
        ui->tradeForm.priceChk->setChecked(true);
        ui->tradeForm.priceTxt->setText(QString::number(t->price, 'f', 4));
    }
    else
        ui->tradeForm.priceChk->setChecked(false);
    ui->tradeForm.sharesTxt->setText(QString::number(t->value, 'f', 4));
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
            ui->acctFormWidget->setEnabled(m_currentItem);
            loadAccount();
            break;
        case tab_assetAllocation:
            ui->aaFormWidget->setEnabled(m_currentItem);
            loadAssetAllocation();
            break;
        case tab_security:
            ui->securityFormWidget->setEnabled(m_currentItem);
            loadSecurity();
            break;
        case tab_trade:
            ui->tradeFormWidget->setEnabled(m_currentItem);
            loadTrade();
            break;
    }
}

void frmEdit::add()
{
    if (!isValidCurrentModel())
        return;

    objectKey *key;
    int identity = portfolio::getOpenIdentity();

    switch(m_currentTab)
    {
        case tab_account:
            key = &m_portfolio.accounts().insert(identity, account(identity, m_portfolio.attributes().id)).value();
            break;
        case tab_assetAllocation:
            key = &m_portfolio.assetAllocations().insert(identity, assetAllocation(identity, m_portfolio.attributes().id)).value();
            break;
        case tab_security:
            key = &m_portfolio.securities().insert(identity, security(identity, m_portfolio.attributes().id)).value();
            break;
        case tab_trade:
            if (ui->tradeFilterCmb->currentIndex() == -1)
                return;
            key = &m_portfolio.securities()[currentTradeSecurityID()].trades.insert(identity, trade(identity, currentTradeSecurityID())).value();
            break;
        case tab_portfolio:
            return;
    }
    currentModel()->insert(key);
    currentListView()->setCurrentIndex(currentModel()->index(currentModel()->rowCount(QModelIndex()) - 1));
}

void frmEdit::remove()
{
    if (!isValidCurrentModel() || !m_currentItem)
        return;

    objectKey *key = m_currentItem;

    currentModel()->remove(key);
    switch(m_currentTab)
    {
        case tab_account:
            m_portfolio.accounts()[key->id].deleted = true;
            break;
        case tab_assetAllocation:
            m_portfolio.assetAllocations()[key->id].deleted = true;
            break;
        case tab_security:
            m_portfolio.securities()[key->id].deleted = true;
            break;
        case tab_trade:
            m_portfolio.securities()[key->parent].trades[key->id].deleted = true;
            break;
        case tab_portfolio:
            break;
    }
}

objectKeyEditModel* frmEdit::currentModel()
{
    switch(m_currentTab)
    {
        case tab_account:
            return static_cast<objectKeyEditModel*>(ui->acctList->model());
        case tab_assetAllocation:
            return static_cast<objectKeyEditModel*>(ui->aaList->model());
        case tab_security:
            return static_cast<objectKeyEditModel*>(ui->securityList->model());
        case tab_trade:
            return static_cast<objectKeyEditModel*>(ui->tradeList->model());
        case tab_portfolio:
            return 0;
    }
    return 0;
}

QListView* frmEdit::currentListView()
{
    switch(m_currentTab)
    {
        case tab_account:
            return ui->acctList;
        case tab_assetAllocation:
            return ui->aaList;
        case tab_security:
            return ui->securityList;
        case tab_trade:
            return ui->tradeList;
        case tab_portfolio:
            return 0;
    }
    return 0;
}

bool frmEdit::isValidCurrentModel()
{
    if (!currentModel() || !currentListView())
    {
        m_currentItem = 0;
        return false;
    }
    return true;
}

void frmEdit::securityAddAA()
{
    if (ui->securityForm.aaCmb->currentIndex() == -1)
        return;

    static_cast<securityAAModel*>(ui->securityForm.aa->model())->addNew(ui->securityForm.aaCmb->itemData(ui->securityForm.aaCmb->currentIndex()).toInt());
}

void frmEdit::securityDeleteAA()
{
    static_cast<securityAAModel*>(ui->securityForm.aa->model())->deleteSelected(ui->securityForm.aa->selectionModel());
}

void frmEdit::populateSecurityTab()
{
    ui->securityForm.aaCmb->clear();
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, assetAllocation>::const_iterator i = m_portfolio.assetAllocations().begin(); i != m_portfolio.assetAllocations().end(); ++i)
    {
        if (i.value().deleted)
            continue;
        ui->securityForm.aaCmb->addItem(i.value().displayText(), i.value().id);
    }
    ui->securityForm.aaCmb->model()->sort(0);
    if (ui->securityForm.aaCmb->count() != 0)
        ui->securityForm.aaCmb->setCurrentIndex(0);

    ui->securityForm.acctCmb->clear();
    ui->securityForm.acctCmb->addItem("", UNASSIGNED);
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, account>::const_iterator i = m_portfolio.accounts().begin(); i != m_portfolio.accounts().end(); ++i)
    {
        if (i.value().deleted)
            continue;
        ui->securityForm.acctCmb->addItem(i.value().displayText(), i.value().id);
    }
    ui->securityForm.acctCmb->model()->sort(0);
}

void frmEdit::populateTradeTab()
{
    int currentSecurityFilter = currentTradeSecurityID();
    ui->tradeFilterCmb->blockSignals(true);
    ui->tradeFilterCmb->clear();
    ui->tradeForm.cashCmb->clear();

    ui->tradeForm.cashCmb->addItem("", UNASSIGNED);

    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, security>::const_iterator i = m_portfolio.securities().begin(); i != m_portfolio.securities().end(); ++i)
    {
        if (i.value().deleted)
            continue;

        QString item = functions::formatForComboBox(i.value().displayText(), i.value().note);
        ui->tradeFilterCmb->addItem(item, i.value().id);
        ui->tradeForm.cashCmb->addItem(item, i.value().id);
    }
    ui->tradeFilterCmb->model()->sort(0);
    ui->tradeForm.cashCmb->model()->sort(0);

    int selectRow = ui->tradeFilterCmb->findData(currentSecurityFilter);
    if (selectRow != -1)
        ui->tradeFilterCmb->setCurrentIndex(selectRow);
    else
        tradeSecurityFilterChange();

    ui->tradeFilterCmb->blockSignals(false);
}

void frmEdit::tradeSecurityFilterChange()
{
    int securityID = currentTradeSecurityID();
    if (securityID == -1)
    {
        delete currentModel();
        ui->tradeFormWidget->setEnabled(false);
        return;
    }

    QAbstractItemModel *model = currentModel();
    ui->tradeList->setModel(new objectKeyEditModel(mapToList(m_portfolio.securities()[securityID].trades), ui->tradeList));
    delete model;

    if (currentModel()->rowCount(QModelIndex()) > 0)
    {
        ui->tradeList->setCurrentIndex(currentModel()->index(0, 0));
        listChange(ui->tradeList->model()->index(0, 0), QModelIndex());
    }

    connect(ui->tradeList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->tradeList->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
}

void frmEdit::copy()
{
    save();

    if (!m_currentItem)
        return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QMimeData *mimeData = new QMimeData();

    stream << m_magicNumber << m_portfolio.attributes().id;
    switch(m_currentTab)
    {
        case tab_account:
            stream << *(static_cast<account*>(m_currentItem));
            mimeData->setData("application/mypersonalindex-account", data);
            break;
        case tab_assetAllocation:
            stream << *(static_cast<assetAllocation*>(m_currentItem));
            mimeData->setData("application/mypersonalindex-assetallocation", data);
            break;
        case tab_security:
            stream << *(static_cast<security*>(m_currentItem));
            mimeData->setData("application/mypersonalindex-security", data);
            break;
        case tab_trade:
            stream << *(static_cast<trade*>(m_currentItem));
            mimeData->setData("application/mypersonalindex-trade", data);
            break;
        case tab_portfolio:
           return;
    }
    QApplication::clipboard()->setMimeData(mimeData);
}

void frmEdit::paste()
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    QByteArray data;
    QDataStream stream(&data, QIODevice::ReadOnly);

    switch(m_currentTab)
    {
        case tab_account:
            data = mimeData->data("application/mypersonalindex-account");
            break;
        case tab_assetAllocation:
            data = mimeData->data("application/mypersonalindex-assetallocation");
            break;
        case tab_security:
            data = mimeData->data("application/mypersonalindex-security");
            break;
        case tab_trade:
            data = mimeData->data("application/mypersonalindex-trade");
            break;
        case tab_portfolio:
            break;
    }

    if (data.isEmpty())
        return;

    int magicNumber, portfolioID;
    stream >> magicNumber;
    stream >> portfolioID;

    if (magicNumber != m_magicNumber || portfolioID != m_portfolio.attributes().id)
        return;

    add();

    if (!m_currentItem)
        return;

    switch(m_currentTab)
    {
        case tab_account:
            stream >> *(static_cast<account*>(m_currentItem));
            break;
        case tab_assetAllocation:
            stream >> *(static_cast<assetAllocation*>(m_currentItem));
            break;
        case tab_security:
            stream >> *(static_cast<security*>(m_currentItem));
            break;
        case tab_trade:
            stream >> *(static_cast<trade*>(m_currentItem));
            break;
        case tab_portfolio:
            break;
    }

    load();
}

void frmEdit::resetTaxRate()
{
    ui->acctForm.taxRateSpinBox->setValue(0);
}

void frmEdit::resetTarget()
{
    ui->aaForm.targetSpinBox->setValue(0);
}

void frmEdit::resetExpenseRatio()
{
    ui->securityForm.expenseSpinBox->setValue(0);
}

void frmEdit::tradeActionChange(int index_)
{
    ui->tradeForm.tradeActionChange(index_);
}

void frmEdit::tradeFrequencyChange(int index_)
{
    ui->tradeForm.tradeFrequencyChange(index_);
}

void frmEdit::tradePriceChange(bool checked_)
{
    ui->tradeForm.tradePriceChange(checked_);
}

void frmEdit::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

int frmEdit::currentTradeSecurityID()
{
    return ui->tradeFilterCmb->currentIndex()== -1 ? -1 :ui->tradeFilterCmb->itemData(ui->tradeFilterCmb->currentIndex()).toInt();
}

template <class T>
QList<objectKey*> frmEdit::mapToList(QMap<int, T> &map_)
{
    QList<objectKey*> list;
    if (map_.isEmpty())
        return list;

    typename QMap<int, T>::iterator i = map_.end();
    do
    {
        --i;
        if (i.value().deleted)
            continue;

        if (i.value().id < 0)
            list.append(&i.value());
        else
            list.prepend(&i.value());

    } while (i != map_.begin());

    return list;
}

// implementations
template QList<objectKey*> frmEdit::mapToList(QMap<int, account> &map_);
template QList<objectKey*> frmEdit::mapToList(QMap<int, assetAllocation> &map_);
template QList<objectKey*> frmEdit::mapToList(QMap<int, security> &map_);
template QList<objectKey*> frmEdit::mapToList(QMap<int, trade> &map_);

bool frmEdit::validate()
{
    QString message = m_portfolio.attributes().validate();
    if (!message.isEmpty())
    {
        QMessageBox::critical(this, "Portfolio validation error", message);
        ui->tabs->setCurrentIndex(tab_portfolio);
        return false;
    }

    if (!validateObjectKeys(m_portfolio.accounts(), tab_account, "Account validation error"))
        return false;

    if (!validateObjectKeys(m_portfolio.assetAllocations(), tab_assetAllocation, "Asset class validation error"))
        return false;

    if (!validateObjectKeys(m_portfolio.securities(), tab_security, "Security validation error"))
        return false;

    for(QMap<int, security>::iterator i = m_portfolio.securities().begin(); i != m_portfolio.securities().end(); ++i)
    {
        if (i.value().deleted)
            continue;

        if (!validateObjectKeys(i.value().trades, tab_trade, "Trade validation error"))
            return false;
    }

    return true;
}

template <class T>
bool frmEdit::validateObjectKeys(QMap<int, T> &map_, tab tab_, const QString &title_)
{
    for(typename QMap<int, T>::iterator i = map_.begin(); i != map_.end(); ++i)
    {
        if (i.value().deleted)
            continue;

        QString error = i.value().validate();
        if (error.isEmpty())
            continue;

        QMessageBox::critical(this, title_, error);
        ui->tabs->setCurrentIndex(tab_);

        //special case
        if (tab_ == tab_trade)
            ui->tradeFilterCmb->setCurrentIndex(ui->tradeFilterCmb->findData(i.value().parent));

        if (!isValidCurrentModel())
            return false;

        currentListView()->setCurrentIndex(currentModel()->find(&i.value()));
        return false;
    }
    return true;
}

template bool frmEdit::validateObjectKeys(QMap<int, account> &map_, tab tab_, const QString &title_);
template bool frmEdit::validateObjectKeys(QMap<int, assetAllocation> &map_, tab tab_, const QString &title_);
template bool frmEdit::validateObjectKeys(QMap<int, security> &map_, tab tab_, const QString &title_);
template bool frmEdit::validateObjectKeys(QMap<int, trade> &map_, tab tab_, const QString &title_);
