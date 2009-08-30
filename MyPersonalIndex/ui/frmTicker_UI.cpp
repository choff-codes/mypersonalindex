#include "frmTicker_UI.h"
#include <QtGui>


void frmTicker_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    mainLayout = new QGridLayout(dialog);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    vlayoutTicker = new QVBoxLayout();
    vlayoutTicker->setSpacing(8);

    gpTicker = new QGroupBox("Ticker", dialog);
    tickerLayout = new QFormLayout(gpTicker);
    tickerLayout->setVerticalSpacing(8);

    symbol = new QLabel("&Symbol:", gpTicker);
    tickerLayout->setWidget(0, QFormLayout::LabelRole, symbol);

    symbolLayout = new QHBoxLayout();
    symbolLayout->setSpacing(4);
    txtSymbol = new QLineEdit(gpTicker);
    btnHistorical = new QPushButton("&View Historical Prices >", gpTicker);
    btnHistorical->setCheckable(true);
    btnHistorical->setMaximumHeight(txtSymbol->sizeHint().height());
    symbolLayout->addWidget(txtSymbol);
    symbolLayout->addWidget(btnHistorical);
    tickerLayout->setLayout(0, QFormLayout::FieldRole, symbolLayout);

    aa = new QLabel("&Asset Allocation:", gpTicker);
    tickerLayout->setWidget(1, QFormLayout::LabelRole, aa);

    cmbAA = new QComboBox(gpTicker);
    tickerLayout->setWidget(1, QFormLayout::FieldRole, cmbAA);

    acct = new QLabel("Acc&ount:", gpTicker);
    tickerLayout->setWidget(2, QFormLayout::LabelRole, acct);

    cmbAcct = new QComboBox(gpTicker);
    tickerLayout->setWidget(2, QFormLayout::FieldRole, cmbAcct);

    chkHide = new QCheckBox("&Hide in Portfolio", gpTicker);
    tickerLayout->setWidget(3, QFormLayout::LabelRole, chkHide);

    chkInclude = new QCheckBox("&Include in Calculations", gpTicker);
    tickerLayout->setWidget(3, QFormLayout::FieldRole, chkInclude);

    chkCash = new QCheckBox("&Cash", gpTicker);
    tickerLayout->setWidget(4, QFormLayout::LabelRole, chkCash);

    chkReinvest = new QCheckBox("&Reinvest Dividends", gpTicker);
    tickerLayout->setWidget(4, QFormLayout::FieldRole, chkReinvest);

    gpActivity = new QGroupBox("Activity", dialog);
    activityLayout = new QHBoxLayout(gpActivity);

    activity = new QTableView(gpActivity);
    activityLayout->addWidget(activity);

    vlayoutTicker->addWidget(gpTicker);
    vlayoutTicker->addWidget(gpActivity);
    mainLayout->addLayout(vlayoutTicker, 0, 0);

    okLayout = new QHBoxLayout();

    btnCustom = new QPushButton("Custom &Trades...", gpActivity);
    btnOkCancel = new QDialogButtonBox(gpActivity);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    okLayout->addWidget(btnCustom);
    okLayout->addStretch();
    okLayout->addWidget(btnOkCancel);
    mainLayout->addLayout(okLayout, 1, 0);

    gpHistorical = new QGroupBox("Historical Prices", dialog);
    gpHistorical->setHidden(true);
    vlayoutHistorical = new QVBoxLayout(gpHistorical);

    tbHistorical = new QToolBar(gpHistorical);
    cmbHistorical = new QComboBox(gpHistorical);
    cmbHistorical->addItem("Show All (Excl. Trades)");
    cmbHistorical->addItem("Show % Change");
    cmbHistorical->addItem("Show Dividends");
    cmbHistorical->addItem("Show Splits");
    cmbHistorical->addItem("Show Trades");
    tbHistorical->addWidget(cmbHistorical);
    sort = new QAction("Sort Descending", gpHistorical);
    sort->setCheckable(true);
    sort->setChecked(true);
    tbHistorical->addAction(sort);
    vlayoutHistorical->addWidget(tbHistorical);

    history = new QTableView(gpHistorical);
    vlayoutHistorical->addWidget(history);

    mainLayout->addWidget(gpHistorical, 0, 1);

    symbol->setBuddy(txtSymbol);
    aa->setBuddy(cmbAA);
    acct->setBuddy(cmbAcct);
}
