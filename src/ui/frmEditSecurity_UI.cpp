#include "frmEditSecurity_UI.h"

void frmEditSecurity_UI::setupUI(QWidget *parent_)
{
    layout = new QHBoxLayout(parent_);

    layoutLeft = new QGridLayout();
    list = new QListView(parent_);
    list->setContextMenuPolicy(Qt::CustomContextMenu);
    list->setEnabled(false);
    addBtn = new QPushButton("Add", parent_);
    deleteBtn = new QPushButton("Delete", parent_);

    layoutLeft->addWidget(list, 0, 0, 1, 2);
    layoutLeft->addWidget(addBtn, 1, 0, 1, 1);
    layoutLeft->addWidget(deleteBtn, 1, 1, 1, 1);

    layout->addLayout(layoutLeft);

    widgetRight = new QWidget(parent_);
    layoutRight = new QVBoxLayout(widgetRight);
    groupBox = new QGroupBox("Security", widgetRight);
    layoutForm = new QFormLayout(groupBox);

    QString symbolTool = "The symbol of the security. By default, data will download from Yahoo! Finance.\n\nThe following special symbols will download from TSP:\nF FUND\nC FUND\nS FUND\nI FUND\nL INCOME\nL 2010\nL 2020\nL 2030\nL 2040\nL 2050";
    QString notesTool = "Personal notes about thse security.";
    QString accountTool = "The account you purchased the security.";
    QString expenseTool = "Expense ratio of a mutual fund, ETF, etc.";
    QString cashTool = "If this is a cash security (money market, CD, etc.) check this. The price will be fixed at $1.";
    QString reinvestTool = "Purchase additional shares with each dividend distributed by the security.";
    QString navTool = "Does a dividend decrease the price of the security? For example, GE decreases by the value of the\ndividend on ex-date so this should be checked. Some fixed income funds may need to uncheck this.";
    QString includeTool = "Easily exclude the security from the portfolio without deleting it.";
    QString hideTool = "Do not display on the security view.";


    symbol = new QLabel("Symbol:", groupBox);
    symbol->setToolTip(symbolTool);
    symbolTxt = new QLineEdit(groupBox);
    symbolTxt->setToolTip(symbolTool);
    layoutForm->setWidget(0, QFormLayout::LabelRole, symbol);
    layoutForm->setWidget(0, QFormLayout::FieldRole, symbolTxt);
    notes = new QLabel("Notes:", groupBox);
    notes->setToolTip(notesTool);
    notesTxt = new QTextEdit(groupBox);
    notesTxt->setToolTip(notesTool);
    notesTxt->setAcceptRichText(false);
    layoutForm->setWidget(1, QFormLayout::LabelRole, notes);
    layoutForm->setWidget(1, QFormLayout::FieldRole, notesTxt);
    acct = new QLabel("Account:", groupBox);
    acct->setToolTip(accountTool);
    acctCmb = new QComboBox(groupBox);
    acctCmb->setToolTip(accountTool);
    layoutForm->setWidget(2, QFormLayout::LabelRole, acct);
    layoutForm->setWidget(2, QFormLayout::FieldRole, acctCmb);
    expense = new QLabel("Expense:", groupBox);
    expense->setToolTip(expenseTool);
    expenseLayout = new QHBoxLayout();
    expenseSpinBox = new QDoubleSpinBox(groupBox);
    expenseSpinBox->setToolTip(expenseTool);
    expenseSpinBox->setMinimum(0);
    expenseSpinBox->setSingleStep(0.01);
    expenseSpinBox->setValue(0);
    expenseSpinBox->setSpecialValueText("None");
    expenseSpinBox->setSuffix("%");

    expenseBtnClear = new QToolButton(groupBox);
    expenseBtnClear->setText("Clear");
    expenseLayout->addWidget(expenseSpinBox, 1);
    expenseLayout->addWidget(expenseBtnClear);
    layoutForm->setWidget(3, QFormLayout::LabelRole, expense);
    layoutForm->setLayout(3, QFormLayout::FieldRole, expenseLayout);
    cashChk = new QCheckBox("Cash Equivalent", groupBox);
    cashChk->setToolTip(cashTool);
    layoutForm->setWidget(4, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(4, QFormLayout::FieldRole, cashChk);
    dividendReinvestmentChk = new QCheckBox("Reinvest Dividends", groupBox);
    dividendReinvestmentChk->setToolTip(reinvestTool);
    layoutForm->setWidget(5, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(5, QFormLayout::FieldRole, dividendReinvestmentChk);
    dividendNAVAdjustmentChk = new QCheckBox("Add Dividend To Return", groupBox);
    dividendNAVAdjustmentChk->setToolTip(navTool);
    layoutForm->setWidget(6, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(6, QFormLayout::FieldRole, dividendNAVAdjustmentChk);
    includeChk = new QCheckBox("Include in Calculations", groupBox);
    includeChk->setToolTip(includeTool);
    layoutForm->setWidget(7, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(7, QFormLayout::FieldRole, includeChk);
    hideChk = new QCheckBox("Hide in Portfolio", groupBox);
    hideChk->setToolTip(hideTool);
    layoutForm->setWidget(8, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(8, QFormLayout::FieldRole, hideChk);

    aaGroupBox = new QGroupBox("Asset Allocation (Double click on % to edit)", widgetRight);
    aaLayout = new QVBoxLayout(aaGroupBox);
    aa = new QTableView(aaGroupBox);
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->hide();
    aa->verticalHeader()->hide();

    aaLayoutBottom = new QGridLayout();
    aaCmb = new QComboBox(aaGroupBox);
    aaAddBtn = new QPushButton("Add Asset Class", aaGroupBox);
    aaDeleteBtn = new QPushButton("Remove Selected", aaGroupBox);
    aaLayoutBottom->addWidget(aaCmb, 0, 0);
    aaLayoutBottom->addWidget(aaAddBtn, 0, 1);
    aaLayoutBottom->addWidget(aaDeleteBtn, 1, 0, 1, 2);
    aaLayoutBottom->setColumnStretch(0, 2);

    aaLayout->addWidget(aa);
    aaLayout->addLayout(aaLayoutBottom, 1);

    layoutRight->addWidget(groupBox);
    layoutRight->addWidget(aaGroupBox);

    layout->addWidget(widgetRight);
    widgetRight->setEnabled(false);

    copyPastePopup = new QMenu(parent_);
    copyAction = new QAction("Copy", copyPastePopup);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    copyPastePopup->addAction(copyAction);
    pasteAction = new QAction("Paste", copyPastePopup);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    copyPastePopup->addAction(pasteAction);

    copyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, list);
    pasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, list);

    symbol->setBuddy(symbolTxt);
    notes->setBuddy(notesTxt);
    acct->setBuddy(acctCmb);
    expense->setBuddy(expenseSpinBox);
}
