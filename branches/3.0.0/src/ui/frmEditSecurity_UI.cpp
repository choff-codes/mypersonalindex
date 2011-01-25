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

    symbol = new QLabel("&Symbol:", groupBox);
    symbolTxt = new QLineEdit(groupBox);
    layoutForm->setWidget(0, QFormLayout::LabelRole, symbol);
    layoutForm->setWidget(0, QFormLayout::FieldRole, symbolTxt);
    notes = new QLabel("&Notes:", groupBox);
    notesTxt = new QTextEdit(groupBox);
    notesTxt->setAcceptRichText(false);
    layoutForm->setWidget(1, QFormLayout::LabelRole, notes);
    layoutForm->setWidget(1, QFormLayout::FieldRole, notesTxt);
    acct = new QLabel("&Account:", groupBox);
    acctCmb = new QComboBox(groupBox);
    layoutForm->setWidget(2, QFormLayout::LabelRole, acct);
    layoutForm->setWidget(2, QFormLayout::FieldRole, acctCmb);
    expense = new QLabel("&Expense:", groupBox);
    expenseLayout = new QHBoxLayout();
    expenseSpinBox = new QDoubleSpinBox(groupBox);
    expenseSpinBox->setMinimum(0);
    expenseSpinBox->setSingleStep(0.01);
    expenseSpinBox->setValue(0);
    expenseSpinBox->setSpecialValueText("None");
    expenseSpinBox->setSuffix("%");

    expenseBtnClear = new QToolButton(groupBox);
    expenseBtnClear->setText("&Clear");
    expenseLayout->addWidget(expenseSpinBox, 1);
    expenseLayout->addWidget(expenseBtnClear);
    layoutForm->setWidget(3, QFormLayout::LabelRole, expense);
    layoutForm->setLayout(3, QFormLayout::FieldRole, expenseLayout);
    cashChk = new QCheckBox("Cash Acc&ount", groupBox);
    layoutForm->setWidget(4, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(4, QFormLayout::FieldRole, cashChk);
    dividendReinvestmentChk = new QCheckBox("&Reinvest Dividends", groupBox);
    layoutForm->setWidget(5, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(5, QFormLayout::FieldRole, dividendReinvestmentChk);
    dividendNAVAdjustmentChk = new QCheckBox("&Adjust NAV for Dividends", groupBox);
    layoutForm->setWidget(6, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(6, QFormLayout::FieldRole, dividendNAVAdjustmentChk);
    includeChk = new QCheckBox("Incl&ude in Calculations", groupBox);
    layoutForm->setWidget(7, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(7, QFormLayout::FieldRole, includeChk);
    hideChk = new QCheckBox("Hide in Port&folio", groupBox);
    layoutForm->setWidget(8, QFormLayout::LabelRole, new QLabel(groupBox));
    layoutForm->setWidget(8, QFormLayout::FieldRole, hideChk);

    aaGroupBox = new QGroupBox("Asset Allocation", widgetRight);
    aaLayout = new QVBoxLayout(aaGroupBox);
    aa = new QTableView(aaGroupBox);
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->hide();
    aa->verticalHeader()->hide();

    aaLayoutBottom = new QGridLayout();
    aaCmb = new QComboBox(aaGroupBox);
    aaAddBtn = new QPushButton("A&dd Asset Class", aaGroupBox);
    aaDeleteBtn = new QPushButton("De&lete Asset Class", aaGroupBox);
    aaLayoutBottom->addWidget(aaCmb, 0, 0, 1, 2);
    aaLayoutBottom->addWidget(aaAddBtn, 1, 0);
    aaLayoutBottom->addWidget(aaDeleteBtn, 1, 1);
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
