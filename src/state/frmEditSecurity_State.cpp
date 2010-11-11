#include "frmEditSecurity_State.h"
#include <QMessageBox>
#include "frmEditSecurity_UI.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "objectKeyEditModel.h"
#include "securityAAModel.h"
#include "portfolioAttributes.h"

frmEditSecurity_State::frmEditSecurity_State(portfolio portfolio_, QWidget *parent_):
    frmEditStateMap(portfolio_, parent_),
    ui(new frmEditSecurity_UI),
    m_currentItem(0),
    m_model(new objectKeyEditModel(mapToList(portfolio_.securities())))
{
    ui->setupUI(parent_);
    ui->list->setModel(m_model);

    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->expenseBtnClear, SIGNAL(clicked()), this, SLOT(resetExpenseRatio()));
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->copyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->pasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->aaAddBtn, SIGNAL(clicked()), this, SLOT(addAA()));
    connect(ui->aaDeleteBtn, SIGNAL(clicked()), this, SLOT(deleteAA()));
}

frmEditSecurity_State::~frmEditSecurity_State()
{
    delete m_model;
    delete ui;
}

void frmEditSecurity_State::enter()
{
    ui->list->setEnabled(true);

    ui->aaCmb->clear();
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, assetAllocation>::const_iterator i = m_portfolio.assetAllocations().begin(); i != m_portfolio.assetAllocations().end(); ++i)
    {
        if (i.value().deleted)
            continue;
        ui->aaCmb->addItem(i.value().displayText(), i.value().id);
    }
    ui->aaCmb->model()->sort(0);
    if (ui->aaCmb->count() != 0)
        ui->aaCmb->setCurrentIndex(0);

    ui->acctCmb->clear();
    ui->acctCmb->addItem("", UNASSIGNED);
    // don't use foreach, weird stuff happens with the existing references
    for(QMap<int, account>::const_iterator i = m_portfolio.accounts().begin(); i != m_portfolio.accounts().end(); ++i)
    {
        if (i.value().deleted)
            continue;
        ui->acctCmb->addItem(i.value().displayText(), i.value().id);
    }
    ui->acctCmb->model()->sort(0);
}

void frmEditSecurity_State::leave()
{
    save();
    ui->list->setEnabled(false);
}

void frmEditSecurity_State::listChange(const QModelIndex &current_, const QModelIndex&)
{
    m_currentItem = static_cast<security*>(m_model->get(current_.row()));

    if (ui->aaCmb->count() != 0)
        ui->aaCmb->setCurrentIndex(0);

    load();
}

void frmEditSecurity_State::resetExpenseRatio()
{
    ui->expenseSpinBox->setValue(0);
}

void frmEditSecurity_State::addAA()
{
    if (ui->aaCmb->currentIndex() == -1)
        return;

    static_cast<securityAAModel*>(ui->aa->model())->addNew(ui->aaCmb->itemData(ui->aaCmb->currentIndex()).toInt());
}

void frmEditSecurity_State::deleteAA()
{
    static_cast<securityAAModel*>(ui->aa->model())->deleteSelected(ui->aa->selectionModel());
}

void frmEditSecurity_State::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

void frmEditSecurity_State::save()
{
    if (!m_currentItem)
        return;

    m_model->refresh(m_model->find(m_currentItem));

    m_currentItem->description = ui->symbolTxt->text();
    m_currentItem->account = ui->acctCmb->itemData(ui->acctCmb->currentIndex()).toInt();
    m_currentItem->expenseRatio = ui->expenseSpinBox->value();
    m_currentItem->cashAccount = ui->cashChk->isChecked();
    m_currentItem->dividendNAVAdjustment = ui->dividendNAVAdjustmentChk->isChecked();
    m_currentItem->dividendReinvestment = ui->dividendReinvestmentChk->isChecked();
    m_currentItem->hide = ui->hideChk->isChecked();
    m_currentItem->note = ui->notesTxt->toPlainText();
    m_currentItem->includeInCalc = ui->includeChk->isChecked();
    m_currentItem->targets = static_cast<securityAAModel*>(ui->aa->model())->getTargets();
}

bool frmEditSecurity_State::validate()
{
    return validateMap(m_portfolio.securities());
}

void frmEditSecurity_State::validationError(objectKey* key_, const QString &errorMessage_)
{
    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Security validation error", errorMessage_);
    ui->list->setCurrentIndex(m_model->find(key_));
}

void frmEditSecurity_State::add()
{
    int identity = portfolio::getOpenIdentity();
    objectKey *key = &m_portfolio.securities().insert(identity, security(identity, m_portfolio.attributes().id)).value();
    m_model->insert(key);
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditSecurity_State::load()
{
    ui->widgetRight->setEnabled(m_currentItem);
    if (!m_currentItem)
        return;

    ui->layoutRight->setEnabled(true);
    ui->symbolTxt->setText(m_currentItem->description);
    ui->acctCmb->setCurrentIndex(ui->acctCmb->findData(m_currentItem->account));
    ui->expenseSpinBox->setValue(m_currentItem->expenseRatio);
    ui->cashChk->setChecked(m_currentItem->cashAccount);
    ui->dividendNAVAdjustmentChk->setChecked(m_currentItem->dividendNAVAdjustment);
    ui->dividendReinvestmentChk->setChecked(m_currentItem->dividendReinvestment);
    ui->hideChk->setChecked(m_currentItem->hide);
    ui->includeChk->setChecked(m_currentItem->includeInCalc);
    ui->notesTxt->setPlainText(m_currentItem->note);

    QAbstractItemModel *model = ui->aa->model();
    ui->aa->setModel(new securityAAModel(m_currentItem->targets, m_portfolio.assetAllocations(), ui->aa));
    delete model;
}

void frmEditSecurity_State::remove()
{
    if (!m_currentItem)
        return;

    security *sec = m_currentItem;
    m_model->remove(m_currentItem);
    m_portfolio.securities()[sec->id].deleted = true;
}

bool frmEditSecurity_State::internalCopy(QDataStream &stream_) const
{
    if (!m_currentItem)
        return false;

    stream_ << *m_currentItem;
    return true;
}

void frmEditSecurity_State::internalPaste(QDataStream &stream_)
{
    if (!m_currentItem)
        return;

    stream_ >> *m_currentItem;
}
