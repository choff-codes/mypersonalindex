#include "frmTrade.h"
#include "frmTrade_UI.h"
#include "frmSecurity.h"
#include <QtGui>

frmTrade::frmTrade(const QMap<int, security> &cashAccounts, QWidget *parent, const trade &trade): QDialog(parent), m_trade(trade)
{
    ui.setupUI(this);
    this->setWindowTitle("Edit Trade");
    ui.cmbCash->addItem("(None)", -1);

    foreach(const security &sec, cashAccounts)
        if (sec.cashAccount) // && sec.id != id)
            ui.cmbCash->addItem(sec.symbol, sec.id);

    connectSlots();
    loadTrade();
}

void frmTrade::connectSlots()
{
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.chkStarting, SIGNAL(toggled(bool)), ui.deStarting, SLOT(setEnabled(bool)));
    connect(ui.chkEnding, SIGNAL(toggled(bool)), ui.deEnding, SLOT(setEnabled(bool)));
    connect(ui.chkPrice, SIGNAL(toggled(bool)), this, SLOT(togglePrice(bool)));
    connect(ui.cmbFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(freqChange(int)));
    connect(ui.cmbType, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChange(int)));
}

void frmTrade::loadTrade()
{
    ui.cmbType->setCurrentIndex((int)m_trade.type);
    if (m_trade.value >= 0)
        ui.txtShares->setText(QString::number(m_trade.value, 'f', 4));
    if (m_trade.price >= 0)
    {
        ui.chkPrice->setChecked(true);
        ui.txtPrice->setText(QString::number(m_trade.price, 'f', 4));
    }
    if (m_trade.commission >= 0)
        ui.txtCommission->setText(QString::number(m_trade.commission, 'f', 4));
    if (m_trade.cashAccount > 0)
        for(int i = 0; i < ui.cmbCash->count(); ++i)
            if (ui.cmbCash->itemData(i).toInt() == m_trade.cashAccount)
            {
                ui.cmbCash->setCurrentIndex(i);
                break;
            }
    ui.cmbFreq->setCurrentIndex((int)m_trade.frequency);
    if (m_trade.date != 0)
        ui.deDate->setDate(QDate::fromJulianDay(m_trade.date));
    if (m_trade.startDate  != 0)
    {
        ui.chkStarting->setChecked(true);
        ui.deStarting->setDate(QDate::fromJulianDay(m_trade.startDate));
    }
    if (m_trade.endDate  != 0)
    {
        ui.chkEnding->setChecked(true);
        ui.deEnding->setDate(QDate::fromJulianDay(m_trade.endDate));
    }
}

void frmTrade::togglePrice(bool checked)
{
    ui.txtPrice->setEnabled(checked);
    if (!checked)
    {
        m_oldPrice = ui.txtPrice->text();
        ui.txtPrice->setText("Previous Close");
    }
    else
        ui.txtPrice->setText(m_oldPrice);
}

void frmTrade::accept()
{
    m_trade.type = (trade::tradeType)ui.cmbType->currentIndex();
    m_trade.value = ui.txtShares->text().toDouble();
    m_trade.price = ui.chkPrice->isChecked() && !ui.txtPrice->text().isEmpty() ? ui.txtPrice->text().toDouble() : -1;
    m_trade.commission = !ui.txtCommission->text().isEmpty() ? ui.txtCommission->text().toDouble() : -1;
    m_trade.cashAccount = ui.cmbCash->itemData(ui.cmbCash->currentIndex()).toInt();
    m_trade.frequency = (trade::tradeFreq)ui.cmbFreq->currentIndex();
    m_trade.date = ui.deDate->isEnabled() ? ui.deDate->date().toJulianDay() : 0;
    m_trade.startDate = ui.deStarting->isEnabled() ? ui.deStarting->date().toJulianDay() : 0;
    m_trade.endDate = ui.deEnding->isEnabled() ? ui.deEnding->date().toJulianDay() : 0;

    QDialog::accept();
}

void frmTrade::freqChange(int index)
{
    ui.deDate->setEnabled(true);
    switch ((trade::tradeFreq)index)
    {
        case trade::tradeFreq_Daily:
            ui.deDate->setDisabled(true);
            break;
        case trade::tradeFreq_Monthly:
            ui.deDate->setDisplayFormat("dd");
            ui.deDate->setMinimumDate(QDate(2009, 1, 1));
            ui.deDate->setMaximumDate(QDate(2009, 1, 31));
            ui.deDate->setCalendarPopup(false);
            ui.deDate->setDate(QDate(2009, 1, 1));
            break;
        case trade::tradeFreq_Once:
            ui.deDate->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
            ui.deDate->clearMinimumDate();
            ui.deDate->clearMaximumDate();
            ui.deDate->setCalendarPopup(true);
            ui.deDate->setDate(QDate::currentDate());
            break;
        case trade::tradeFreq_Weekly:
            ui.deDate->setDisplayFormat("dddd");
            ui.deDate->setMinimumDate(QDate(2009, 1, 5));
            ui.deDate->setMaximumDate(QDate(2009, 1, 9));
            ui.deDate->setCalendarPopup(false);
            ui.deDate->setDate(QDate(2009, 1, 5));
            break;
        case trade::tradeFreq_Yearly:
            ui.deDate->setDisplayFormat("dd MMM");
            ui.deDate->setMinimumDate(QDate(2009, 1, 1));
            ui.deDate->setMaximumDate(QDate(2009, 12, 31));
            ui.deDate->setCalendarPopup(false);
            ui.deDate->setDate(QDate(2009, 1, 1));
            break;
        case trade::tradeFreq_Count:
            break;
    }
}

void frmTrade::typeChange(int index)
{
    switch ((trade::tradeType)index)
    {
        case trade::tradeType_Purchase:
            ui.shares->setText("Shares:");
            break;
        case trade::tradeType_Sale:
            ui.shares->setText("Shares:");
            break;
        case trade::tradeType_DivReinvest:
            ui.shares->setText("Shares:");
            break;
        case trade::tradeType_Interest:
            ui.shares->setText("Amount ($):");
            break;
        case trade::tradeType_FixedPurchase:
        case trade::tradeType_FixedSale:
            ui.shares->setText("Amount ($):");
            break;
        case trade::tradeType_TotalValue:
            ui.shares->setText("% of Value:");
            break;
        case trade::tradeType_AA:
            ui.shares->setText("% of Target:");
            break;
        case trade::tradeType_Count:
            break;
    }
}
