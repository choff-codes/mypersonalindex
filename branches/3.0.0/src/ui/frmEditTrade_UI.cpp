#include "frmEditTrade_UI.h"
#include "trade.h"

void frmEditTrade_UI::setupUI(QWidget *parent_)
{
    layout = new QVBoxLayout(parent_);

    layoutTop = new QGridLayout();
    list = new QListView(parent_);
    list->setContextMenuPolicy(Qt::CustomContextMenu);
    list->setEnabled(false);
    filter = new QLabel("Choose Security:", parent_);
    filterCmb = new QComboBox(parent_);
    addBtn = new QPushButton("Add", parent_);
    deleteBtn = new QPushButton("Delete", parent_);

    layoutTop->addWidget(list, 0, 0, 1, 5);
    layoutTop->addWidget(filter, 1, 0, 1, 1);
    layoutTop->addWidget(filterCmb, 1, 1, 1, 2);
    layoutTop->addWidget(addBtn, 1, 3, 1, 1);
    layoutTop->addWidget(deleteBtn, 1, 4, 1, 1);

    layout->addLayout(layoutTop, 2);

    widgetBottom = new QWidget(parent_);
    layoutBottom = new QHBoxLayout(widgetBottom);

    groupBox = new QGroupBox("Trade", widgetBottom);
    layoutForm = new QFormLayout(groupBox);

    sharePriceValidator = new mpiDoubleValidator(-100000000, 100000000, 4, parent_);

    action = new QLabel("&Action:", groupBox);
    actionCmb = new QComboBox(groupBox);

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

    layoutForm->setWidget(0, QFormLayout::LabelRole, action);
    layoutForm->setWidget(0, QFormLayout::FieldRole, actionCmb);
    shares = new QLabel("&Shares:", groupBox);
    sharesTxt = new QLineEdit(groupBox);
    sharesTxt->setValidator(sharePriceValidator);
    layoutForm->setWidget(1, QFormLayout::LabelRole, shares);
    layoutForm->setWidget(1, QFormLayout::FieldRole, sharesTxt);
    priceChk = new QCheckBox("&Price:", groupBox);
    priceTxt = new QLineEdit("Previous Close", groupBox);
    priceTxt->setEnabled(false);
    priceTxt->setValidator(sharePriceValidator);
    layoutForm->setWidget(2, QFormLayout::LabelRole, priceChk);
    layoutForm->setWidget(2, QFormLayout::FieldRole, priceTxt);
    commission = new QLabel("&Commission:", groupBox);
    commissionTxt = new QLineEdit(groupBox);
    commissionTxt->setValidator(sharePriceValidator);
    layoutForm->setWidget(3, QFormLayout::LabelRole, commission);
    layoutForm->setWidget(3, QFormLayout::FieldRole, commissionTxt);
    cash = new QLabel("T&o/From Cash:", groupBox);
    cashCmb = new QComboBox(groupBox);
    layoutForm->setWidget(4, QFormLayout::LabelRole, cash);
    layoutForm->setWidget(4, QFormLayout::FieldRole, cashCmb);
    note = new QLabel("&Notes:", groupBox);
    noteTxt = new QTextEdit(groupBox);
    noteTxt->setAcceptRichText(false);
    layoutForm->setWidget(5, QFormLayout::LabelRole, note);
    layoutForm->setWidget(5, QFormLayout::FieldRole, noteTxt);

    groupBoxDate = new QGroupBox("Date", widgetBottom);
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

    layoutBottom->addWidget(groupBox, 1);
    layoutBottom->addWidget(groupBoxDate, 1);

    layout->addWidget(widgetBottom, 1);
    widgetBottom->setEnabled(false);

    copyPastePopup = new QMenu(parent_);
    copyAction = new QAction("Copy", copyPastePopup);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    copyPastePopup->addAction(copyAction);
    pasteAction = new QAction("Paste", copyPastePopup);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    copyPastePopup->addAction(pasteAction);

    copyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, list);
    pasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, list);

    action->setBuddy(actionCmb);
    shares->setBuddy(sharesTxt);
    commission->setBuddy(commissionTxt);
    cash->setBuddy(cashCmb);
    freq->setBuddy(freqCmb);
    date->setBuddy(dateDateEdit);
    note->setBuddy(noteTxt);
}
