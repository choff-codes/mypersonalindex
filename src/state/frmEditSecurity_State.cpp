#include "frmEditSecurity_State.h"
#include <QMessageBox>
#include "frmEditSecurity_UI.h"
#include "security.h"
#include "assetAllocation.h"
#include "account.h"
#include "objectKeyEditModel.h"
#include "securityAAModel.h"

frmEditSecurity_State::frmEditSecurity_State(const portfolio &portfolio_, QWidget *parent_):
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
    connect(ui->symbolTxt, SIGNAL(textChanged(QString)), this, SLOT(save()));
}

frmEditSecurity_State::~frmEditSecurity_State()
{
    delete m_model;
    delete ui;
}

void frmEditSecurity_State::enter()
{
    ui->aaCmb->clear();
    foreach(const assetAllocation &aa, m_portfolio.assetAllocations())
        ui->aaCmb->addItem(aa.displayText(), aa.id());

    ui->aaCmb->model()->sort(0);
    if (ui->aaCmb->count() != 0)
        ui->aaCmb->setCurrentIndex(0);

    ui->acctCmb->clear();
    ui->acctCmb->addItem("", UNASSIGNED);
    foreach(const account &acct, m_portfolio.accounts())
        ui->acctCmb->addItem(acct.displayText(), acct.id());

    ui->acctCmb->model()->sort(0);

    load(); // this could be modified by the AA and Acct tab, so reload the current item
    ui->list->setEnabled(true);
}

void frmEditSecurity_State::leave()
{
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
    static_cast<securityAAModel*>(ui->aa->model())->deleteSelected(ui->aa->selectionModel()->selectedRows());
}

void frmEditSecurity_State::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

void frmEditSecurity_State::save()
{
    if (!m_currentItem)
        return;

    m_currentItem->setDescription(ui->symbolTxt->text());
    m_currentItem->setAccount(ui->acctCmb->itemData(ui->acctCmb->currentIndex()).toInt());
    m_currentItem->setExpenseRatio(ui->expenseSpinBox->value() / 100);
    m_currentItem->setCashAccount(ui->cashChk->isChecked());
    m_currentItem->setDividendNAVAdjustment(ui->dividendNAVAdjustmentChk->isChecked());
    m_currentItem->setDividendReinvestment(ui->dividendReinvestmentChk->isChecked());
    m_currentItem->setHidden(ui->hideChk->isChecked());
    m_currentItem->setNote(ui->notesTxt->toPlainText());
    m_currentItem->setIncludeInCalc(ui->includeChk->isChecked());
    m_currentItem->targets() = static_cast<securityAAModel*>(ui->aa->model())->getTargets();

    m_model->refresh(m_model->find(m_currentItem));
}

bool frmEditSecurity_State::validate()
{
    return validateMap(m_portfolio.securities());
}

void frmEditSecurity_State::validationError(const objectKeyBase &key_, const QString &errorMessage_)
{
    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Security validation error", errorMessage_);
    ui->list->setCurrentIndex(m_model->find(&key_));
}

void frmEditSecurity_State::add()
{
    security s;
    s.setNewIdentity();
    s.setParent(m_portfolio.id());
    m_portfolio.securities().insert(s.id(), s);
    m_model->insert(new security(s));
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditSecurity_State::load()
{
    ui->widgetRight->setEnabled(m_currentItem);
    if (!m_currentItem)
        return;

    ui->symbolTxt->blockSignals(true);
    ui->symbolTxt->setText(m_currentItem->description());
    ui->symbolTxt->blockSignals(false);
    ui->acctCmb->setCurrentIndex(ui->acctCmb->findData(m_currentItem->account()));
    ui->expenseSpinBox->setValue(m_currentItem->expenseRatio() * 100);
    ui->cashChk->setChecked(m_currentItem->cashAccount());
    ui->dividendNAVAdjustmentChk->setChecked(m_currentItem->dividendNAVAdjustment());
    ui->dividendReinvestmentChk->setChecked(m_currentItem->dividendReinvestment());
    ui->hideChk->setChecked(m_currentItem->hidden());
    ui->includeChk->setChecked(m_currentItem->includeInCalc());
    ui->notesTxt->setPlainText(m_currentItem->note());

    QAbstractItemModel *model = ui->aa->model();
    ui->aa->setModel(new securityAAModel(m_currentItem->targets(), m_portfolio.assetAllocations(), ui->aa));
    delete model;
}

void frmEditSecurity_State::remove()
{
    if (!m_currentItem)
        return;

    m_portfolio.securities().remove(m_currentItem->id());
    m_model->remove(m_currentItem);
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
