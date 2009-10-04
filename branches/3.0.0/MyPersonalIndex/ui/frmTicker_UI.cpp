#include "frmTicker_UI.h"
#include <QtGui>


void frmTicker_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    mainLayout = new QGridLayout(dialog);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    QLabel *tmp; // fake labels to add to form layout to get proper sizing

    topSectionLayout = new QHBoxLayout();
    gpSecurity = new QGroupBox("Security", dialog);
    securityLayout = new QFormLayout(gpSecurity);

    symbol = new QLabel("Symbol:", gpSecurity);
    symbolLayout = new QHBoxLayout();
    txtSymbol = new QLineEdit(gpSecurity);
    btnHistorical = new QToolButton(gpSecurity);
    btnHistorical->setText("History->");
    btnHistorical->setCheckable(true);
    symbolLayout->addWidget(txtSymbol, 1);
    symbolLayout->addWidget(btnHistorical);
    securityLayout->setWidget(0, QFormLayout::LabelRole, symbol);
    securityLayout->setLayout(0, QFormLayout::FieldRole, symbolLayout);
    acct = new QLabel("Account:", gpSecurity);
    cmbAcct = new QComboBox(gpSecurity);
    securityLayout->setWidget(1, QFormLayout::LabelRole, acct);
    securityLayout->setWidget(1, QFormLayout::FieldRole, cmbAcct);
    expense = new QLabel("Expense:", gpSecurity);
    expenseLayout = new QHBoxLayout();
    sbExpense = new QDoubleSpinBox(gpSecurity);
    sbExpense->setMinimum(-0.01);
    sbExpense->setSingleStep(0.01);
    sbExpense->setValue(-0.01);
    sbExpense->setSpecialValueText("N/A");
    sbExpense->setSuffix("%");

    btnExpenseClear = new QToolButton(gpSecurity);
    btnExpenseClear->setText("Clear");
    expenseLayout->addWidget(sbExpense, 1);
    expenseLayout->addWidget(btnExpenseClear);
    securityLayout->setWidget(2, QFormLayout::LabelRole, expense);
    securityLayout->setLayout(2, QFormLayout::FieldRole, expenseLayout);
    tmp = new QLabel(gpSecurity);
    chkReinvest = new QCheckBox("Reinvest Dividends", gpSecurity);
    securityLayout->setWidget(3, QFormLayout::LabelRole, tmp);
    securityLayout->setWidget(3, QFormLayout::FieldRole, chkReinvest);
    tmp = new QLabel(gpSecurity);
    chkCash = new QCheckBox("Cash Account", gpSecurity);
    securityLayout->setWidget(4, QFormLayout::LabelRole, tmp);
    securityLayout->setWidget(4, QFormLayout::FieldRole, chkCash);
    tmp = new QLabel(gpSecurity);
    chkInclude = new QCheckBox("Include in Calculations", gpSecurity);
    securityLayout->setWidget(5, QFormLayout::LabelRole, tmp);
    securityLayout->setWidget(5, QFormLayout::FieldRole, chkInclude);
    tmp = new QLabel(gpSecurity);
    chkHide = new QCheckBox("Hide in Portfolio", gpSecurity);
    securityLayout->setWidget(6, QFormLayout::LabelRole, tmp);
    securityLayout->setWidget(6, QFormLayout::FieldRole, chkHide);

    gpAA = new QGroupBox("Asset Allocation", dialog);
    aaLayout = new QVBoxLayout(gpAA);
    aa = new QTableView(gpAA);
    aa->setSelectionMode(QAbstractItemView::ExtendedSelection);
    aa->setSelectionBehavior(QAbstractItemView::SelectRows);
    aa->setAlternatingRowColors(true);
    aa->horizontalHeader()->hide();
    aa->verticalHeader()->hide();

    aaLayoutBottom = new QGridLayout();
    cmbAA = new QComboBox(gpAA);
    btnAAAdd = new QPushButton("Add", gpAA);
    btnAADelete = new QPushButton("Delete", gpAA);
    aaLayoutBottom->addWidget(cmbAA, 0, 0, 1, 2);
    aaLayoutBottom->addWidget(btnAAAdd, 1, 0);
    aaLayoutBottom->addWidget(btnAADelete, 1, 1);
    aaLayout->addWidget(aa);
    aaLayout->addLayout(aaLayoutBottom, 1);

    // make the aa tableView a bit smaller, since there shouldn't be more than 1-2 entries
    // this will make it just small enough so it matches the security layout on the left
    aa->setMaximumHeight(securityLayout->sizeHint().height() -
                         aaLayoutBottom->sizeHint().height() -
                         aaLayout->margin() * 3);  // top, in between layout, and bottom
    topSectionLayout->addWidget(gpSecurity);
    topSectionLayout->addWidget(gpAA, 1);

    gpTrades = new QGroupBox("Trades", dialog);
    tradesLayout = new QVBoxLayout(gpTrades);
    trades = new QTableView(gpTrades);
    trades->setSelectionMode(QAbstractItemView::ExtendedSelection);
    trades->setSelectionBehavior(QAbstractItemView::SelectRows);
    trades->setAlternatingRowColors(true);
    trades->horizontalHeader()->setHighlightSections(false);
    int fntHeight = trades->fontMetrics().height() + 2; // add small buffer
    trades->verticalHeader()->setDefaultSectionSize(fntHeight);
    trades->horizontalHeader()->setFixedHeight(fntHeight);
    trades->setMaximumHeight(fntHeight * 6);
    trades->horizontalHeader()->setStyleSheet("QHeaderView::section {border: none;}");
    trades->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    trades->verticalHeader()->hide();

    tradesLayoutButtons = new QHBoxLayout();
    btnTradesAdd = new QPushButton("Add New", gpTrades);
    btnTradesCopy = new QPushButton("Add Copy", gpTrades);;
    btnTradesEdit = new QPushButton("Edit", gpTrades);;
    btnTradesDelete = new QPushButton("Delete", gpTrades);;
    tradesLayoutButtons->addWidget(btnTradesAdd);
    tradesLayoutButtons->addWidget(btnTradesCopy);
    tradesLayoutButtons->addWidget(btnTradesEdit);
    tradesLayoutButtons->addWidget(btnTradesDelete);
    tradesLayout->addWidget(trades, 1);
    tradesLayout->addLayout(tradesLayoutButtons);

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gpHistorical = new QGroupBox("Historical Prices", dialog);
    gpHistorical->setHidden(true);
    historicalLayout = new QVBoxLayout(gpHistorical);

    tbHistorical = new QToolBar(gpHistorical);
    cmbHistorical = new QComboBox(gpHistorical);
    cmbHistorical->addItem("Show All (Excl. Trades)");
    cmbHistorical->addItem("Show % Change");
    cmbHistorical->addItem("Show Dividends");
    cmbHistorical->addItem("Show Splits");
    cmbHistorical->addItem("Show Trades");
    tbHistorical->addWidget(cmbHistorical);
    sortHistorical = new QAction("Sort Descending", gpHistorical);
    sortHistorical->setCheckable(true);
    sortHistorical->setChecked(true);
    tbHistorical->addAction(sortHistorical);
    historicalLayout->addWidget(tbHistorical);

    history = new QTableView(gpHistorical);
    historicalLayout->addWidget(history);

    mainLayout->addLayout(topSectionLayout, 0, 0);
    mainLayout->addWidget(gpTrades, 1, 0);
    mainLayout->addWidget(btnOkCancel, 2, 0);
    mainLayout->addWidget(gpHistorical, 0, 1, 2, 1);

    symbol->setBuddy(txtSymbol);
    acct->setBuddy(cmbAcct);
    expense->setBuddy(sbExpense);
}
