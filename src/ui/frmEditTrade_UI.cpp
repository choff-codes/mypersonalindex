#include "frmEditTrade_UI.h"

void frmEditTrade_UI::setupUI(QWidget *parent)
{
    layout = new QHBoxLayout(parent);

    groupBoxTrade = new QGroupBox("Trade", parent);
    tradeLayout = new QFormLayout(groupBoxTrade);

    sharePriceValidator = new mpiDoubleValidator(-100000000, 100000000, 4, parent);

    action = new QLabel("&Action:", groupBoxTrade);
    actionCmb = new QComboBox(groupBoxTrade);

    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_Purchase), trade::tradeAction_Purchase);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchaseFixedAmount), trade::tradeAction_PurchaseFixedAmount);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_Sell), trade::tradeAction_Sell);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_SellFixedAmount), trade::tradeAction_SellFixedAmount);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReinvestDividends), trade::tradeAction_ReinvestDividends);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReceiveInterest), trade::tradeAction_ReceiveInterest);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReceiveInterestPercent), trade::tradeAction_ReceiveInterestPercent);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfSecurityValue), trade::tradeAction_PurchasePercentOfSecurityValue);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfPortfolioValue), trade::tradeAction_PurchasePercentOfPortfolioValue);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfAATarget), trade::tradeAction_PurchasePercentOfAATarget);

    tradeLayout->setWidget(0, QFormLayout::LabelRole, action);
    tradeLayout->setWidget(0, QFormLayout::FieldRole, actionCmb);
    shares = new QLabel("&Shares:", groupBoxTrade);
    sharesTxt = new QLineEdit(groupBoxTrade);
    sharesTxt->setValidator(sharePriceValidator);
    tradeLayout->setWidget(1, QFormLayout::LabelRole, shares);
    tradeLayout->setWidget(1, QFormLayout::FieldRole, sharesTxt);
    priceChk = new QCheckBox("&Price:", groupBoxTrade);
    priceTxt = new QLineEdit("Previous Close", groupBoxTrade);
    priceTxt->setEnabled(false);
    priceTxt->setValidator(sharePriceValidator);
    tradeLayout->setWidget(2, QFormLayout::LabelRole, priceChk);
    tradeLayout->setWidget(2, QFormLayout::FieldRole, priceTxt);
    commission = new QLabel("&Commission:", groupBoxTrade);
    commissionTxt = new QLineEdit(groupBoxTrade);
    commissionTxt->setValidator(sharePriceValidator);
    tradeLayout->setWidget(3, QFormLayout::LabelRole, commission);
    tradeLayout->setWidget(3, QFormLayout::FieldRole, commissionTxt);
    cash = new QLabel("T&o/From Cash:", groupBoxTrade);
    cashCmb = new QComboBox(groupBoxTrade);
    tradeLayout->setWidget(4, QFormLayout::LabelRole, cash);
    tradeLayout->setWidget(4, QFormLayout::FieldRole, cashCmb);
    note = new QLabel("&Notes:", groupBoxTrade);
    noteTxt = new QTextEdit(groupBoxTrade);
    noteTxt->setAcceptRichText(false);
    tradeLayout->setWidget(5, QFormLayout::LabelRole, note);
    tradeLayout->setWidget(5, QFormLayout::FieldRole, noteTxt);

    groupBoxDate = new QGroupBox("Date", parent);
    dateLayout = new QFormLayout(groupBoxDate);

    freq = new QLabel("&Frequency:", groupBoxDate);
    freqCmb = new QComboBox(groupBoxDate);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Once), tradeDateCalendar::frequency_Once);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Daily), tradeDateCalendar::frequency_Daily);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Weekly), tradeDateCalendar::frequency_Weekly);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Monthly), tradeDateCalendar::frequency_Monthly);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Yearly), tradeDateCalendar::frequency_Yearly);

    dateLayout->setWidget(0, QFormLayout::LabelRole, freq);
    dateLayout->setWidget(0, QFormLayout::FieldRole, freqCmb);
    date = new QLabel("&Date:", groupBoxDate);
    dateDateEdit = new mpiDateEdit(groupBoxDate);
    dateLayout->setWidget(1, QFormLayout::LabelRole, date);
    dateLayout->setWidget(1, QFormLayout::FieldRole, dateDateEdit);
    startingChk = new QCheckBox("St&arting:", groupBoxDate);
    startingDateEdit = new mpiDateEdit(groupBoxDate);
    startingDateEdit->setEnabled(false);
    dateLayout->setWidget(2, QFormLayout::LabelRole, startingChk);
    dateLayout->setWidget(2, QFormLayout::FieldRole, startingDateEdit);
    endingChk = new QCheckBox("&Ending:", groupBoxDate);
    endingDateEdit = new mpiDateEdit(groupBoxDate);
    endingDateEdit->setEnabled(false);
    dateLayout->setWidget(3, QFormLayout::LabelRole, endingChk);
    dateLayout->setWidget(3, QFormLayout::FieldRole, endingDateEdit);

    layout->addWidget(groupBoxTrade, 1);
    layout->addWidget(groupBoxDate, 1);

    action->setBuddy(actionCmb);
    shares->setBuddy(sharesTxt);
    commission->setBuddy(commissionTxt);
    cash->setBuddy(cashCmb);
    freq->setBuddy(freqCmb);
    date->setBuddy(dateDateEdit);
    note->setBuddy(noteTxt);
}

void frmEditTrade_UI::tradeFrequencyChange(int index_)
{
    dateDateEdit->setEnabled(true);
    switch ((tradeDateCalendar::frequency)freqCmb->itemData(index_).toInt())
    {
        case tradeDateCalendar::frequency_Daily:
            dateDateEdit->setDisabled(true);
            break;
        case tradeDateCalendar::frequency_Monthly:
            dateDateEdit->setDisplayFormat("dd");
            dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            dateDateEdit->setMaximumDate(QDate(2009, 1, 31));
            dateDateEdit->setCalendarPopup(false);
            dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
        case tradeDateCalendar::frequency_Once:
            dateDateEdit->setDisplayFormat(QLocale::system().dateFormat(QLocale::ShortFormat));
            dateDateEdit->clearMinimumDate();
            dateDateEdit->clearMaximumDate();
            dateDateEdit->setCalendarPopup(true);
            dateDateEdit->setDate(QDate::currentDate());
            break;
        case tradeDateCalendar::frequency_Weekly:
            dateDateEdit->setDisplayFormat("dddd");
            dateDateEdit->setMinimumDate(QDate(2009, 1, 5));
            dateDateEdit->setMaximumDate(QDate(2009, 1, 9));
            dateDateEdit->setCalendarPopup(false);
            dateDateEdit->setDate(QDate(2009, 1, 5));
            break;
        case tradeDateCalendar::frequency_Yearly:
            dateDateEdit->setDisplayFormat("dd MMM");
            dateDateEdit->setMinimumDate(QDate(2009, 1, 1));
            dateDateEdit->setMaximumDate(QDate(2009, 12, 31));
            dateDateEdit->setCalendarPopup(false);
            dateDateEdit->setDate(QDate(2009, 1, 1));
            break;
    }
}

void frmEditTrade_UI::tradeActionChange(int index_)
{
    switch ((trade::tradeAction)actionCmb->itemData(index_).toInt())
    {
        case trade::tradeAction_Purchase:
        case trade::tradeAction_Sell:
        case trade::tradeAction_ReinvestDividends:
            shares->setText("Shares:");
            break;
        case trade::tradeAction_ReceiveInterest:
        case trade::tradeAction_PurchaseFixedAmount:
        case trade::tradeAction_SellFixedAmount:
            shares->setText("Amount ($):");
            break;
        case trade::tradeAction_PurchasePercentOfSecurityValue:
            shares->setText("% of Value:");
            break;
        case trade::tradeAction_ReceiveInterestPercent:
            shares->setText("Rate (%):");
            break;
        case trade::tradeAction_PurchasePercentOfPortfolioValue:
            shares->setText("% of Total");
            break;
        case trade::tradeAction_PurchasePercentOfAATarget:
            shares->setText("% of Target:");
            break;
        case trade::tradeAction_ReinvestDividendsAuto:
            break;
    }
}

void frmEditTrade_UI::tradePriceChange(bool checked_)
{
    priceTxt->setEnabled(checked_);
    priceTxt->setText(checked_ ? "0.0000" : "Previous Close");
}
