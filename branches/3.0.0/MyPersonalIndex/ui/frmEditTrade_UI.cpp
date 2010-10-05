#include "frmEditTrade_UI.h"

void frmEditTrade_UI::setupUI(QWidget *parent)
{
    layout = new QHBoxLayout(parent);

    groupBoxTrade = new QGroupBox("Trade", parent);
    tradeLayout = new QFormLayout(groupBoxTrade);

    sharePriceValidator = new mpiDoubleValidator(-100000000, 100000000, 4, parent);

    type = new QLabel("&Type:", groupBoxTrade);
    typeCmb = new QComboBox(groupBoxTrade);

    for(int i = 0; i < trade::tradeType_Count; ++i)
        typeCmb->addItem(trade::tradeTypeToString((trade::tradeType)i));

    tradeLayout->setWidget(0, QFormLayout::LabelRole, type);
    tradeLayout->setWidget(0, QFormLayout::FieldRole, typeCmb);
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

    groupBoxDate = new QGroupBox("Date", parent);
    dateLayout = new QFormLayout(groupBoxDate);

    freq = new QLabel("&Frequency:", groupBoxDate);
    freqCmb = new QComboBox(groupBoxDate);

    for(int i = 0; i < tradeDateCalendar::frequency_Count; ++i)
        freqCmb->addItem(trade::frequencyToString((tradeDateCalendar::frequency)i));

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

    type->setBuddy(typeCmb);
    shares->setBuddy(sharesTxt);
    commission->setBuddy(commissionTxt);
    cash->setBuddy(cashCmb);
    freq->setBuddy(freqCmb);
    date->setBuddy(dateDateEdit);
}
