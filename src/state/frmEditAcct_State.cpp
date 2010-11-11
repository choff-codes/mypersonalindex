#include "frmEditAcct_State.h"
#include <QMessageBox>
#include "frmEditAcct_UI.h"
#include "account.h"
#include "objectKeyEditModel.h"
#include "portfolioAttributes.h"

frmEditAcct_State::frmEditAcct_State(portfolio portfolio_, QWidget *parent_):
    frmEditStateMap(portfolio_, parent_),
    ui(new frmEditAcct_UI),
    m_currentItem(0),
    m_model(new objectKeyEditModel(mapToList(portfolio_.assetAllocations())))
{
    ui->setupUI(parent_);
    ui->list->setModel(m_model);

    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->taxRateBtnClear, SIGNAL(clicked()), this, SLOT(resetTaxRate()));
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->copyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->pasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
}

frmEditAcct_State::~frmEditAcct_State()
{
    delete m_model;
    delete ui;
}

void frmEditAcct_State::enter()
{
    ui->list->setEnabled(true);
}

void frmEditAcct_State::leave()
{
    save();
    ui->list->setEnabled(false);
}

void frmEditAcct_State::listChange(const QModelIndex &current_, const QModelIndex&)
{
    m_currentItem = static_cast<account*>(m_model->get(current_.row()));
    load();
}

void frmEditAcct_State::resetTaxRate()
{
    ui->taxRateSpinBox->setValue(0);
}

void frmEditAcct_State::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

void frmEditAcct_State::save()
{
    if (!m_currentItem)
        return;

    m_model->refresh(m_model->find(m_currentItem));

    m_currentItem->description = ui->descTxt->text();
    m_currentItem->taxRate = ui->taxRateSpinBox->value();
    m_currentItem->taxDeferred = ui->taxDeferredChk->isChecked();
    m_currentItem->costBasis = (account::costBasisMethod)ui->costBasisCmb->itemData(ui->costBasisCmb->currentIndex()).toInt();
    m_currentItem->hide = ui->hideChk->isChecked();
}

bool frmEditAcct_State::validate()
{
    return validateMap(m_portfolio.accounts());
}

void frmEditAcct_State::validationError(objectKey* key_, const QString &errorMessage_)
{
    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Account validation error", errorMessage_);
    ui->list->setCurrentIndex(m_model->find(key_));
}

void frmEditAcct_State::add()
{
    int identity = portfolio::getOpenIdentity();
    objectKey *key = &m_portfolio.accounts().insert(identity, account(identity, m_portfolio.attributes().id)).value();
    m_model->insert(key);
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditAcct_State::load()
{
    ui->groupBox->setEnabled(m_currentItem);
    if (!m_currentItem)
        return;

    ui->descTxt->setText(m_currentItem->description);
    ui->taxRateSpinBox->setValue(m_currentItem->taxRate);
    ui->taxDeferredChk->setChecked(m_currentItem->taxDeferred);
    ui->costBasisCmb->setCurrentIndex(ui->costBasisCmb->findData(m_currentItem->costBasis));
    ui->hideChk->setChecked(m_currentItem->hide);
}

void frmEditAcct_State::remove()
{
    if (!m_currentItem)
        return;

    account *acct = m_currentItem;
    m_model->remove(m_currentItem);
    m_portfolio.accounts()[acct->id].deleted = true;
}

bool frmEditAcct_State::internalCopy(QDataStream &stream_) const
{
    if (!m_currentItem)
        return false;

    stream_ << *m_currentItem;
    return true;
}

void frmEditAcct_State::internalPaste(QDataStream &stream_)
{
    if (!m_currentItem)
        return;

    stream_ >> *m_currentItem;
}

