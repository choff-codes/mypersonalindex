#include "frmSecurity_UI.h"

void frmSecurity_UI::setupUI(QWidget *parent)
{
    layout = new QVBoxLayout(parent);

    securityGroupBox = new QGroupBox("Security", parent);
    securityLayout = new QFormLayout(securityGroupBox);

    symbol = new QLabel("&Symbol:", securityGroupBox);
    symbolTxt = new QLineEdit(securityGroupBox);
    securityLayout->setWidget(0, QFormLayout::LabelRole, symbol);
    securityLayout->setWidget(0, QFormLayout::FieldRole, symbolTxt);
    acct = new QLabel("&Account:", securityGroupBox);
    acctCmb = new QComboBox(securityGroupBox);
    securityLayout->setWidget(1, QFormLayout::LabelRole, acct);
    securityLayout->setWidget(1, QFormLayout::FieldRole, acctCmb);
    expense = new QLabel("&Expense:", securityGroupBox);
    expenseLayout = new QHBoxLayout();
    expenseSpinBox = new QDoubleSpinBox(securityGroupBox);
    expenseSpinBox->setMinimum(0);
    expenseSpinBox->setSingleStep(0.01);
    expenseSpinBox->setValue(0);
    expenseSpinBox->setSpecialValueText("N/A");
    expenseSpinBox->setSuffix("%");

    expenseBtnClear = new QToolButton(securityGroupBox);
    expenseBtnClear->setText("&Clear");
    expenseLayout->addWidget(expenseSpinBox, 1);
    expenseLayout->addWidget(expenseBtnClear);
    securityLayout->setWidget(2, QFormLayout::LabelRole, expense);
    securityLayout->setLayout(2, QFormLayout::FieldRole, expenseLayout);
    reinvestChk = new QCheckBox("&Reinvest Dividends", securityGroupBox);
    securityLayout->setWidget(3, QFormLayout::LabelRole, new QLabel(securityGroupBox));
    securityLayout->setWidget(3, QFormLayout::FieldRole, reinvestChk);
    cashChk = new QCheckBox("Cash Acc&ount", securityGroupBox);
    securityLayout->setWidget(4, QFormLayout::LabelRole, new QLabel(securityGroupBox));
    securityLayout->setWidget(4, QFormLayout::FieldRole, cashChk);
    includeChk = new QCheckBox("Incl&ude in Calculations", securityGroupBox);
    securityLayout->setWidget(5, QFormLayout::LabelRole, new QLabel(securityGroupBox));
    securityLayout->setWidget(5, QFormLayout::FieldRole, includeChk);
    hideChk = new QCheckBox("Hide in Port&folio", securityGroupBox);
    securityLayout->setWidget(6, QFormLayout::LabelRole, new QLabel(securityGroupBox));
    securityLayout->setWidget(6, QFormLayout::FieldRole, hideChk);

    aaGroupBox = new QGroupBox("Asset Allocation", parent);
    aaLayout = new QVBoxLayout(aaGroupBox);
    aa = new mpiTableView(false, aaGroupBox);
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->hide();
    aa->verticalHeader()->hide();

    aaLayoutBottom = new QGridLayout();
    aaCmb = new QComboBox(aaGroupBox);
    aaBtnAdd = new QPushButton("A&dd", aaGroupBox);
    aaBtnDelete = new QPushButton("De&lete", aaGroupBox);
    aaLayoutBottom->addWidget(aaCmb, 0, 0, 1, 2);
    aaLayoutBottom->addWidget(aaBtnAdd, 1, 0);
    aaLayoutBottom->addWidget(aaBtnDelete, 1, 1);
    aaLayout->addWidget(aa);
    aaLayout->addLayout(aaLayoutBottom, 1);

    layout->addWidget(securityGroupBox);
    layout->addWidget(aaGroupBox);

    symbol->setBuddy(symbolTxt);
    acct->setBuddy(acctCmb);
    expense->setBuddy(expenseSpinBox);
}
