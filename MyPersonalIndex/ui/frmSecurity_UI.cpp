#include "frmSecurity_UI.h"
#include <QtGui>


void frmSecurity_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    mainLayout = new QGridLayout(dialog);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

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
    chkReinvest = new QCheckBox("Reinvest Dividends", gpSecurity);
    securityLayout->setWidget(3, QFormLayout::LabelRole, new QLabel(gpSecurity));
    securityLayout->setWidget(3, QFormLayout::FieldRole, chkReinvest);
    chkCash = new QCheckBox("Cash Account", gpSecurity);
    securityLayout->setWidget(4, QFormLayout::LabelRole, new QLabel(gpSecurity));
    securityLayout->setWidget(4, QFormLayout::FieldRole, chkCash);
    chkInclude = new QCheckBox("Include in Calculations", gpSecurity);
    securityLayout->setWidget(5, QFormLayout::LabelRole, new QLabel(gpSecurity));
    securityLayout->setWidget(5, QFormLayout::FieldRole, chkInclude);
    chkHide = new QCheckBox("Hide in Portfolio", gpSecurity);
    securityLayout->setWidget(6, QFormLayout::LabelRole, new QLabel(gpSecurity));
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
    trades->setContextMenuPolicy(Qt::CustomContextMenu);

    tradesLayoutButtons = new QHBoxLayout();
    btnTradesAdd = new QPushButton("Add New", gpTrades);
    btnTradesEdit = new QPushButton("Edit", gpTrades);
    btnTradesDelete = new QPushButton("Delete", gpTrades);
    tradesLayoutButtons->addWidget(btnTradesAdd);
    tradesLayoutButtons->addWidget(btnTradesEdit);
    tradesLayoutButtons->addWidget(btnTradesDelete);
    tradesLayout->addWidget(trades, 1);
    tradesLayout->addLayout(tradesLayoutButtons);

    tradesPopup = new QMenu(trades);
    tradesCopy = new QAction("Copy", tradesPopup);
    tradesCopy->setShortcut(Qt::CTRL + Qt::Key_C);
    tradesCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, trades);
    tradesPopup->addAction(tradesCopy);
    tradesPaste = new QAction("Paste", tradesPopup);
    tradesPaste->setShortcut(Qt::CTRL + Qt::Key_V);
    tradesPopup->addAction(tradesPaste);
    tradesPasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, trades);

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    btnAddAnother = new QPushButton("Add Another", dialog);
    btnOkCancel->addButton(btnAddAnother, QDialogButtonBox::ActionRole);

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
    history->setSelectionMode(QAbstractItemView::ExtendedSelection);
    history->setSelectionBehavior(QAbstractItemView::SelectRows);
    history->setAlternatingRowColors(true);
    history->horizontalHeader()->setHighlightSections(false);
    history->verticalHeader()->setDefaultSectionSize(fntHeight);
    history->horizontalHeader()->setFixedHeight(fntHeight);
    history->horizontalHeader()->setStyleSheet("QHeaderView::section {border: none;}");
    history->verticalHeader()->hide();
    historicalLayout->addWidget(history);

    mainLayout->addLayout(topSectionLayout, 0, 0);
    mainLayout->addWidget(gpTrades, 1, 0);
    mainLayout->addWidget(btnOkCancel, 2, 0);
    mainLayout->addWidget(gpHistorical, 0, 1, 2, 1);

    symbol->setBuddy(txtSymbol);
    acct->setBuddy(cmbAcct);
    expense->setBuddy(sbExpense);
}
