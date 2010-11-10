#include "frmEditAA_State.h"
#include <QMessageBox>
#include "frmEditAA_UI.h"
#include "assetAllocation.h"
#include "objectKeyEditModel.h"
#include "portfolioAttributes.h"

frmEditAA_State::frmEditAA_State(portfolio portfolio_, QWidget *parent_):
    QObject(parent_),
    frmEditStateMap(portfolio_),
    ui(new frmEditAA_UI),
    m_model(new objectKeyEditModel(mapToList(portfolio_.assetAllocations())))
{
    ui->setupUI(parent_);
    m_model->setParent(ui->list);
    ui->list->setModel(m_model);

    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->targetBtnClear, SIGNAL(clicked()), this, SLOT(resetTarget()));
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->copyShortcut, SIGNAL(activated()), this, SLOT(copySlot()));
    connect(ui->pasteShortcut, SIGNAL(activated()), this, SLOT(pasteSlot()));
}

frmEditAA_State::~frmEditAA_State()
{
    delete ui;
}

void frmEditAA_State::enter()
{
    ui->list->setEnabled(true);
}

void frmEditAA_State::leave()
{
    ui->list->setEnabled(false);
}

void frmEditAA_State::listChange(const QModelIndex &current_, const QModelIndex &previous_)
{
    m_model->refresh(previous_);
    m_currentItem = static_cast<assetAllocation*>(m_model->get(current_.row()));
    load();
}

void frmEditAA_State::resetTarget()
{
    ui->targetSpinBox->setValue(0);
}

void frmEditAA_State::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

void frmEditAA_State::save()
{
    if (!m_currentItem)
        return;
    m_currentItem->description = ui->descTxt->text();
    m_currentItem->rebalanceBand = ui->rebalanceBandSpinBox->value() / 100;
    m_currentItem->target = ui->targetSpinBox->value() / 100;
    m_currentItem->threshold = (assetAllocation::thresholdMethod)ui->thresholdCmb->itemData(ui->thresholdCmb->currentIndex()).toInt();
    m_currentItem->hide = ui->hideChk->isChecked();
}

bool frmEditAA_State::validate()
{
    return validateMap(m_portfolio.assetAllocations());
}

void frmEditAA_State::validationError(objectKey* key_, const QString &errorMessage_)
{
    QMessageBox::critical(0, "Asset Allocation validation error", errorMessage_);
    ui->list->setCurrentIndex(m_model->find(key_));
}

void frmEditAA_State::add()
{
    int identity = portfolio::getOpenIdentity();
    objectKey *key = &m_portfolio.assetAllocations().insert(identity, assetAllocation(identity, m_portfolio.attributes().id)).value();
    m_model->insert(key);
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditAA_State::load()
{
    if (!m_currentItem)
        return;
    ui->descTxt->setText(m_currentItem->description);
    ui->rebalanceBandSpinBox->setValue(m_currentItem->rebalanceBand * 100);
    ui->targetSpinBox->setValue(m_currentItem->target * 100);
    ui->thresholdCmb->setCurrentIndex(ui->thresholdCmb->findData(m_currentItem->threshold));
    ui->hideChk->setChecked(m_currentItem->hide);
}

void frmEditAA_State::remove()
{
    if (!m_currentItem)
        return;

    assetAllocation *aa = m_currentItem;
    m_model->remove(m_currentItem);
    m_portfolio.assetAllocations()[aa->id].deleted = true;
}

bool frmEditAA_State::internalCopy(QDataStream &stream_)
{
    if (!m_currentItem)
        return false;

    stream_ << *m_currentItem;
    return true;
}

void frmEditAA_State::internalPaste(QDataStream &stream_)
{
    if (!m_currentItem)
        return;

    stream_ >> *m_currentItem;
}
