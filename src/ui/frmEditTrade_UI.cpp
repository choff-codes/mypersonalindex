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
    addBtn = new QPushButton("Add Trade", parent_);
    deleteBtn = new QPushButton("Delete Trade", parent_);

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

    QString actionTool = "The intent of the trade.";
    QString priceTool = "Enter the price of the trade by checking \"Price\", otherwise you can use a downloaded\nprice from the market close or the prior date close (open price)";
    QString commissionTool = "Fee paid for this trade.";
    QString cashTool = "An opposite trade will be booked to this security automatically.";
    QString notesTool = "Personal notes about the trade.";
    QString freqTool = "Is this a one time trade? If not you can set up a recurring trade.";
    QString dateTool = "The dates which this trade occurs.";
    QString startDateTool = "When to begin for recurring trades.";
    QString endDateTool = "When to end for recurring trades.";

    action = new QLabel("Action:", groupBox);
    action->setToolTip(actionTool);
    actionCmb = new QComboBox(groupBox);
    actionCmb->setToolTip(actionTool);

    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_Purchase), trade::tradeAction_Purchase);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchaseFixedAmount), trade::tradeAction_PurchaseFixedAmount);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_Sell), trade::tradeAction_Sell);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_SellFixedAmount), trade::tradeAction_SellFixedAmount);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReinvestDividends), trade::tradeAction_ReinvestDividends);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReceiveInterest), trade::tradeAction_ReceiveInterest);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_ReceiveInterestPercent), trade::tradeAction_ReceiveInterestPercent);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfSecurityShares), trade::tradeAction_PurchasePercentOfSecurityShares);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_SellPercentOfSecurityShares), trade::tradeAction_SellPercentOfSecurityShares);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfPortfolioValue), trade::tradeAction_PurchasePercentOfPortfolioValue);
    actionCmb->addItem(trade::tradeTypeToString(trade::tradeAction_PurchasePercentOfAATarget), trade::tradeAction_PurchasePercentOfAATarget);

    layoutForm->setWidget(0, QFormLayout::LabelRole, action);
    layoutForm->setWidget(0, QFormLayout::FieldRole, actionCmb);
    shares = new QLabel("Shares:", groupBox);
    sharesSpinBox = new QDoubleSpinBox(groupBox);
    sharesSpinBox->setDecimals(4);
    sharesSpinBox->setMinimum(0);
    sharesSpinBox->setSingleStep(1);
    sharesSpinBox->setMaximum(999999999);
    sharesSpinBox->setValue(0);
    layoutForm->setWidget(1, QFormLayout::LabelRole, shares);
    layoutForm->setWidget(1, QFormLayout::FieldRole, sharesSpinBox);
    priceChk = new QCheckBox("Price:", groupBox);
    priceChk->setToolTip(priceTool);
    priceWidget = new QStackedWidget(groupBox);
    priceSpinBox = new QDoubleSpinBox(groupBox);
    priceSpinBox->setToolTip(priceTool);
    priceSpinBox->setDecimals(4);
    priceSpinBox->setMinimum(0);
    priceSpinBox->setSingleStep(1);
    priceSpinBox->setMaximum(999999999);
    priceSpinBox->setValue(0);
    priceCmb = new QComboBox(groupBox);
    priceCmb->addItem(trade::priceToString(trade::tradePriceType_CurrentClose), trade::tradePriceType_CurrentClose);
    priceCmb->addItem(trade::priceToString(trade::tradePriceType_PreviousClose), trade::tradePriceType_PreviousClose);
    priceWidget->addWidget(priceCmb);
    priceWidget->addWidget(priceSpinBox);
    layoutForm->setWidget(2, QFormLayout::LabelRole, priceChk);
    layoutForm->setWidget(2, QFormLayout::FieldRole, priceWidget);
    commission = new QLabel("Commission:", groupBox);
    commission->setToolTip(commissionTool);
    commissionSpinBox = new QDoubleSpinBox(groupBox);
    commissionSpinBox->setToolTip(commissionTool);
    commissionSpinBox->setDecimals(2);
    commissionSpinBox->setMinimum(0);
    commissionSpinBox->setSingleStep(1);
    commissionSpinBox->setMaximum(999999999);
    commissionSpinBox->setValue(0);
    layoutForm->setWidget(3, QFormLayout::LabelRole, commission);
    layoutForm->setWidget(3, QFormLayout::FieldRole, commissionSpinBox);
    cash = new QLabel("To/From Cash:", groupBox);
    cash->setToolTip(cashTool);
    cashCmb = new QComboBox(groupBox);
    cashCmb->setToolTip(cashTool);
    layoutForm->setWidget(4, QFormLayout::LabelRole, cash);
    layoutForm->setWidget(4, QFormLayout::FieldRole, cashCmb);
    note = new QLabel("Notes:", groupBox);
    note->setToolTip(notesTool);
    noteTxt = new QTextEdit(groupBox);
    noteTxt->setToolTip(notesTool);
    noteTxt->setAcceptRichText(false);
    layoutForm->setWidget(5, QFormLayout::LabelRole, note);
    layoutForm->setWidget(5, QFormLayout::FieldRole, noteTxt);

    groupBoxDate = new QGroupBox("Date", widgetBottom);
    dateLayout = new QFormLayout(groupBoxDate);

    freq = new QLabel("Frequency:", groupBoxDate);
    freq->setToolTip(freqTool);
    freqCmb = new QComboBox(groupBoxDate);
    freqCmb->setToolTip(freqTool);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Once), tradeDateCalendar::frequency_Once);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Daily), tradeDateCalendar::frequency_Daily);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Weekly), tradeDateCalendar::frequency_Weekly);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Monthly), tradeDateCalendar::frequency_Monthly);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Quarterly), tradeDateCalendar::frequency_Quarterly);
    freqCmb->addItem(trade::frequencyToString(tradeDateCalendar::frequency_Yearly), tradeDateCalendar::frequency_Yearly);

    dateLayout->setWidget(0, QFormLayout::LabelRole, freq);
    dateLayout->setWidget(0, QFormLayout::FieldRole, freqCmb);
    date = new QLabel("Date:", groupBoxDate);
    date->setToolTip(dateTool);
    dateDateEdit = new mpiDateEdit(groupBoxDate);
    dateDateEdit->setToolTip(dateTool);
    dateLayout->setWidget(1, QFormLayout::LabelRole, date);
    dateLayout->setWidget(1, QFormLayout::FieldRole, dateDateEdit);
    startingChk = new QCheckBox("Starting:", groupBoxDate);
    startingChk->setToolTip(startDateTool);
    startingDateEdit = new mpiDateEdit(groupBoxDate);
    startingDateEdit->setToolTip(startDateTool);
    startingDateEdit->setEnabled(false);
    dateLayout->setWidget(2, QFormLayout::LabelRole, startingChk);
    dateLayout->setWidget(2, QFormLayout::FieldRole, startingDateEdit);
    endingChk = new QCheckBox("Ending:", groupBoxDate);
    endingChk->setToolTip(endDateTool);
    endingDateEdit = new mpiDateEdit(groupBoxDate);
    endingDateEdit->setToolTip(endDateTool);
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
    shares->setBuddy(sharesSpinBox);
    commission->setBuddy(commissionSpinBox);
    cash->setBuddy(cashCmb);
    freq->setBuddy(freqCmb);
    date->setBuddy(dateDateEdit);
    note->setBuddy(noteTxt);
}
