#include "frmEditTrade_State.h"
#include <QMessageBox>
#include "frmEditTrade_UI.h"
#include "trade.h"
#include "security.h"
#include "functions.h"
#include "objectKeyEditModel.h"

frmEditTrade_State::frmEditTrade_State(const portfolio &portfolio_, QWidget *parent_):
    frmEditStateMap(portfolio_, parent_),
    ui(new frmEditTrade_UI),
    m_currentItem(0),
    m_model(0)
{
    ui->setupUI(parent_);

    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->copyShortcut, SIGNAL(activated()), this, SLOT(copy()));
    connect(ui->pasteShortcut, SIGNAL(activated()), this, SLOT(paste()));
    connect(ui->copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->filterCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
    connect(ui->filterCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(securityFilterChange(int)));
    connect(ui->startingChk, SIGNAL(toggled(bool)), ui->startingDateEdit, SLOT(setEnabled(bool)));
    connect(ui->endingChk, SIGNAL(toggled(bool)), ui->endingDateEdit, SLOT(setEnabled(bool)));
    connect(ui->freqCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeFrequencyChange(int)));
    connect(ui->actionCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(tradeActionChange(int)));
    connect(ui->priceChk, SIGNAL(toggled(bool)), this, SLOT(tradePriceChange(bool)));
}

frmEditTrade_State::~frmEditTrade_State()
{
    delete m_model;
    delete ui;
}

void frmEditTrade_State::enter()
{
    ui->list->setEnabled(true);

    int currentSecurityFilter = ui->filterCmb->currentIndex() == -1 ? -1 : ui->filterCmb->itemData(ui->filterCmb->currentIndex()).toInt();
    ui->filterCmb->blockSignals(true);
    ui->filterCmb->clear();
    ui->cashCmb->clear();

    ui->cashCmb->addItem("", UNASSIGNED);

    foreach(const security &s, m_portfolio.securities())
    {
        if (s.deleted())
            continue;

        QString item = functions::join(s.displayText(), functions::fitString(functions::removeNewLines(s.note()), 20), " | ");
        ui->filterCmb->addItem(item, s.id());
        ui->cashCmb->addItem(item, s.id());
    }
    ui->filterCmb->model()->sort(0);
    ui->cashCmb->model()->sort(0);

    int selectRow = ui->filterCmb->findData(currentSecurityFilter);
    if (selectRow != -1)
    {
        ui->filterCmb->setCurrentIndex(selectRow);
    }
    else if (ui->filterCmb->count() != 0)
    {
        ui->filterCmb->setCurrentIndex(0);
        securityFilterChange(0);
    }
    else
        securityFilterChange(-1);

    ui->filterCmb->blockSignals(false);
}

void frmEditTrade_State::leave()
{
    save();
    ui->list->setEnabled(false);
}

void frmEditTrade_State::listChange(const QModelIndex &current_, const QModelIndex&)
{
    m_currentItem = static_cast<trade*>(m_model->get(current_.row()));
    load();
}

void frmEditTrade_State::securityFilterChange(int index_)
{
    m_currentItem = 0;

    int securityID = index_ == -1 ? UNASSIGNED : ui->filterCmb->itemData(index_).toInt();
    if (securityID == UNASSIGNED)
    {
        delete m_model;
        m_model = 0;
        ui->layoutBottom->setEnabled(false);
        return;
    }

    QAbstractItemModel *model = m_model;
    m_model = new objectKeyEditModel(mapToList(m_portfolio.securities()[securityID].trades()), ui->list);
    ui->list->setModel(m_model);
    delete model;

    if (m_model->rowCount(QModelIndex()) > 0)
    {
        ui->list->setCurrentIndex(m_model->index(0, 0));
        listChange(ui->list->model()->index(0, 0), QModelIndex());
    }

    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(save()));
    connect(ui->list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(listChange(QModelIndex,QModelIndex)));
}

void frmEditTrade_State::customContextMenuRequested(const QPoint&)
{
    ui->copyPastePopup->popup(QCursor::pos());
}

void frmEditTrade_State::save()
{
    if (!m_currentItem)
        return;

    m_currentItem->setAction((trade::tradeAction)ui->actionCmb->itemData(ui->actionCmb->currentIndex()).toInt());
    m_currentItem->setCashAccount(ui->cashCmb->itemData(ui->cashCmb->currentIndex()).toInt());
    m_currentItem->setCommission(ui->commissionTxt->text().toDouble());
    m_currentItem->setDate(ui->dateDateEdit->isEnabled() ? ui->dateDateEdit->date().toJulianDay() : 0);
    m_currentItem->setStartDate(ui->startingDateEdit->isEnabled() ? ui->startingDateEdit->date().toJulianDay() : 0);
    m_currentItem->setEndDate(ui->endingDateEdit->isEnabled() ? ui->endingDateEdit->date().toJulianDay() : 0);
    m_currentItem->setDescription(ui->noteTxt->toPlainText());
    m_currentItem->setFrequency((tradeDateCalendar::frequency)ui->freqCmb->itemData(ui->freqCmb->currentIndex()).toInt());
    m_currentItem->setPrice(ui->priceChk->isChecked() && !ui->priceTxt->text().isEmpty() ? ui->priceTxt->text().toDouble() : -1);
    m_currentItem->setValue(ui->sharesTxt->text().toDouble());

    m_model->refresh(m_model->find(m_currentItem));
}

bool frmEditTrade_State::validate()
{
    foreach(const security &s, m_portfolio.securities())
    {
        if (s.deleted())
            continue;

        if (!validateMap(s.trades()))
            return false;
    }

    return true;
}

void frmEditTrade_State::validationError(const objectKeyBase &key_, const QString &errorMessage_)
{
    QMessageBox::critical(static_cast<QWidget*>(this->parent()), "Trade validation error", errorMessage_);
    ui->filterCmb->setCurrentIndex(ui->filterCmb->findData(key_.parent()));
    ui->list->setCurrentIndex(m_model->find(&key_));
}

void frmEditTrade_State::add()
{
    if (ui->filterCmb->currentIndex() == -1)
        return;

    trade t(portfolio::getOpenIdentity(), ui->filterCmb->itemData(ui->filterCmb->currentIndex()).toInt());
    m_portfolio.securities()[t.parent()].trades().insert(t.id(), t);
    m_model->insert(new trade(t));
    ui->list->setCurrentIndex(m_model->index(m_model->rowCount(QModelIndex()) - 1));
}

void frmEditTrade_State::load()
{
    ui->widgetBottom->setEnabled(m_currentItem);
    if (!m_currentItem)
        return;

    ui->actionCmb->setCurrentIndex(ui->actionCmb->findData(m_currentItem->action()));
    ui->cashCmb->setCurrentIndex(ui->cashCmb->findData(m_currentItem->cashAccount()));
    ui->commissionTxt->setText(QString::number(m_currentItem->commission(), 'f', 4));
    ui->dateDateEdit->setDate(m_currentItem->date() != 0 ? QDate::fromJulianDay(m_currentItem->date()) : QDate::currentDate());
    ui->startingChk->setChecked(m_currentItem->startDate() != 0);
    ui->startingDateEdit->setDate(m_currentItem->startDate() != 0 ? QDate::fromJulianDay(m_currentItem->startDate()) : QDate::currentDate());
    ui->endingChk->setChecked(m_currentItem->endDate() != 0);
    ui->endingDateEdit->setDate(m_currentItem->endDate() != 0 ? QDate::fromJulianDay(m_currentItem->endDate()) : QDate::currentDate());
    ui->noteTxt->setPlainText(m_currentItem->description());
    ui->freqCmb->setCurrentIndex(ui->freqCmb->findData(m_currentItem->frequency()));
    if (functions::massage(m_currentItem->price()) >= 0)
    {
        ui->priceChk->setChecked(true);
        ui->priceTxt->setText(QString::number(m_currentItem->price(), 'f', 4));
    }
    else
        ui->priceChk->setChecked(false);
    ui->sharesTxt->setText(QString::number(m_currentItem->value(), 'f', 4));
}

void frmEditTrade_State::remove()
{
    if (!m_currentItem)
        return;

    m_portfolio.securities()[m_currentItem->parent()].trades()[m_currentItem->id()].setDeleted(true);
    m_model->remove(m_currentItem);
}

bool frmEditTrade_State::internalCopy(QDataStream &stream_) const
{
    if (!m_currentItem)
        return false;

    stream_ << *m_currentItem;
    return true;
}

void frmEditTrade_State::internalPaste(QDataStream &stream_)
{
    if (!m_currentItem)
        return;

    stream_ >> *m_currentItem;
}

void frmEditTrade_State::tradeFrequencyChange(int index_)
{
    ui->dateDateEdit->setEnabled(true);
    switch ((tradeDateCalendar::frequency)ui->freqCmb->itemData(index_).toInt())
    {
        case tradeDateCalendar::frequency_Daily:
            ui->dateDateEdit->setDisabled(true);
            break;
        case tradeDateCalendar::frequency_Monthly:
            ui->dateDateEdit->setDisplayFormat("dd");
            ui->dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            ui->dateDateEdit->setMaximumDate(QDate(2009, 1, 31));
            ui->dateDateEdit->setCalendarPopup(false);
            ui->dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
        case tradeDateCalendar::frequency_Once:
            ui->dateDateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
            ui->dateDateEdit->clearMinimumDate();
            ui->dateDateEdit->clearMaximumDate();
            ui->dateDateEdit->setCalendarPopup(true);
            ui->dateDateEdit->setDate(QDate::currentDate());
            break;
        case tradeDateCalendar::frequency_Weekly:
            ui->dateDateEdit->setDisplayFormat("dddd");
            ui->dateDateEdit->setMinimumDate(QDate(2009, 1, 5));
            ui->dateDateEdit->setMaximumDate(QDate(2009, 1, 9));
            ui->dateDateEdit->setCalendarPopup(false);
            ui->dateDateEdit->setDate(QDate(2009, 1, 5));
            break;
        case tradeDateCalendar::frequency_Yearly:
            ui->dateDateEdit->setDisplayFormat("dd MMM");
            ui->dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            ui->dateDateEdit->setMaximumDate(QDate(2009, 12, 31));
            ui->dateDateEdit->setCalendarPopup(false);
            ui->dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
    }
}

void frmEditTrade_State::tradeActionChange(int index_)
{
    switch ((trade::tradeAction)ui->actionCmb->itemData(index_).toInt())
    {
        case trade::tradeAction_Purchase:
        case trade::tradeAction_Sell:
        case trade::tradeAction_ReinvestDividends:
            ui->shares->setText("Shares:");
            break;
        case trade::tradeAction_ReceiveInterest:
        case trade::tradeAction_PurchaseFixedAmount:
        case trade::tradeAction_SellFixedAmount:
            ui->shares->setText("Amount ($):");
            break;
        case trade::tradeAction_PurchasePercentOfSecurityValue:
            ui->shares->setText("% of Value:");
            break;
        case trade::tradeAction_ReceiveInterestPercent:
            ui->shares->setText("Rate (%):");
            break;
        case trade::tradeAction_PurchasePercentOfPortfolioValue:
            ui->shares->setText("% of Total");
            break;
        case trade::tradeAction_PurchasePercentOfAATarget:
            ui->shares->setText("% of Target:");
            break;
        case trade::tradeAction_ReinvestDividendsAuto:
            break;
    }
}

void frmEditTrade_State::tradePriceChange(bool checked_)
{
    ui->priceTxt->setEnabled(checked_);
    ui->priceTxt->setText(checked_ ? "0.0000" : "Previous Close");
}
