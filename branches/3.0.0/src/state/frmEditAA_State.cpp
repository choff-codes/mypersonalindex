#include "frmEditAA_State.h"
#include <QMessageBox>
#include "frmEditAA_UI.h"
#include "assetAllocation.h"
#include "security.h"
#include "assetAllocationTarget.h"
#include "objectKeyEditModel.h"

frmEditAA_State::frmEditAA_State(const portfolio &portfolio_, const fileStateIdentity &identities_, QWidget *parent_):
    frmEditStateMap(portfolio_, identities_, parent_),
    ui(new frmEditAA_UI),
    m_currentItem(0),
    m_model(new objectKeyEditModel(mapToList(portfolio_.assetAllocations())))
{
    ui->setupUI(parent_);
    ui->list->setModel(m_model);

    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->targetBtnClear, SIGNAL(clicked()), this, SLOT(resetTarget()));
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->copyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->pasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->descTxt, SIGNAL(textChanged(QString)), this, SLOT(save()));
}

frmEditAA_State::~frmEditAA_State()
{
    delete m_model;
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

void frmEditAA_State::listChange(const QModelIndex &current_, const QModelIndex&)
{
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

    m_currentItem->setDescription(ui->descTxt->text());
    m_currentItem->setRebalanceBand(ui->rebalanceBandSpinBox->value() / 100);
    m_currentItem->setTarget(ui->targetSpinBox->value() / 100);
    m_currentItem->setThreshold((assetAllocation::thresholdMethod)ui->thresholdCmb->itemData(ui->thresholdCmb->currentIndex()).toInt());
    m_currentItem->setHidden(ui->hideChk->isChecked());

    m_model->refresh(m_model->find(m_currentItem));
}

bool frmEditAA_State::validate()
{
    return validateMap(m_portfolio.assetAllocations());
}

void frmEditAA_State::validationError(const objectKeyBase &key_, const QString &errorMessage_)
{
    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Asset Allocation validation error", errorMessage_);
    ui->list->setCurrentIndex(m_model->find(&key_));
}

void frmEditAA_State::add()
{
    assetAllocation aa;
    aa.setID(m_identities.nextIdentity(objectType_AA));
    aa.setParent(m_portfolio.id());
    m_portfolio.assetAllocations().insert(aa.id(), aa);
    m_model->insert(new assetAllocation(aa));
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditAA_State::load()
{
    ui->groupBox->setEnabled(m_currentItem);
    if (!m_currentItem)
        return;

    ui->descTxt->blockSignals(true);
    ui->descTxt->setText(m_currentItem->description());
    ui->descTxt->blockSignals(false);
    ui->rebalanceBandSpinBox->setValue(m_currentItem->rebalanceBand() * 100);
    ui->targetSpinBox->setValue(m_currentItem->target() * 100);
    ui->thresholdCmb->setCurrentIndex(ui->thresholdCmb->findData(m_currentItem->threshold()));
    ui->hideChk->setChecked(m_currentItem->hidden());
}

void frmEditAA_State::remove()
{
    if (!m_currentItem)
        return;

    foreach(security s, m_portfolio.securities())
        if (s.targets().contains(m_currentItem->id()))
        {
            if (QMessageBox::question(static_cast<QWidget*>(this->parent()), "Delete?",
                QString("The asset class %1 is used is one or more securities, are you sure you want to delete?").arg(m_currentItem->displayText()),
                QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
                    return;

            break;
        }

    m_portfolio.assetAllocations().remove(m_currentItem->id());
    foreach(security s, m_portfolio.securities())
        s.targets().remove(m_currentItem->id());
    m_model->remove(m_currentItem);
}

bool frmEditAA_State::internalCopy(QDataStream &stream_) const
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
