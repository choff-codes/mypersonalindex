#include "frmEdit_UI.h"

void frmEdit_UI::setupUI(QDialog* dialog) {
    dialog->setWindowFlags((dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint)) | Qt::WindowMinMaxButtonsHint);
    dialog->setWindowTitle("Edit Portfolio");

    layout = new QVBoxLayout(dialog);

    tabs = new QTabWidget(dialog);

    okCancelBtn = new QDialogButtonBox(dialog);
    okCancelBtn->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    layout->addWidget(tabs);
    layout->addWidget(okCancelBtn);

    portfolioTab = new QWidget(dialog);
    portfolioLayout = new QHBoxLayout(portfolioTab);
    portfolioFormWidget = new QWidget(portfolioTab);

    portfolioLayout->addWidget(portfolioFormWidget);
    portfolioForm.setupUI(portfolioFormWidget);

    tabs->addTab(portfolioTab, "Portfolio");

    acctTab = new QWidget(dialog);
    acctLayout = new QHBoxLayout(acctTab);
    acctLayoutLeft = new QGridLayout();
    acctList = new QListView(acctTab);
    acctAdd = new QPushButton("Add", acctTab);
    acctDelete = new QPushButton("Delete", acctTab);
    acctFormWidget = new QWidget(acctTab);
    acctFormWidget->setEnabled(false);

    acctLayoutLeft->addWidget(acctList, 0, 0, 1, 2);
    acctLayoutLeft->addWidget(acctAdd, 1, 0, 1, 1);
    acctLayoutLeft->addWidget(acctDelete, 1, 1, 1, 1);

    acctLayout->addLayout(acctLayoutLeft);
    acctLayout->addWidget(acctFormWidget);
    acctForm.setupUI(acctFormWidget);

    tabs->addTab(acctTab, "Accounts");

    aaTab = new QWidget(dialog);
    aaLayout = new QHBoxLayout(aaTab);
    aaLayoutLeft = new QGridLayout();
    aaList = new QListView(aaTab);
    aaAdd = new QPushButton("Add", aaTab);
    aaDelete = new QPushButton("Delete", aaTab);
    aaFormWidget = new QWidget(aaTab);
    aaFormWidget->setEnabled(false);

    aaLayoutLeft->addWidget(aaList, 0, 0, 1, 2);
    aaLayoutLeft->addWidget(aaAdd, 1, 0, 1, 1);
    aaLayoutLeft->addWidget(aaDelete, 1, 1, 1, 1);

    aaLayout->addLayout(aaLayoutLeft);
    aaLayout->addWidget(aaFormWidget);
    aaForm.setupUI(aaFormWidget);

    tabs->addTab(aaTab, "Asset Allocation");

    securityTab = new QWidget(dialog);
    securityLayout = new QHBoxLayout(securityTab);
    securityLayoutLeft = new QGridLayout();
    securityList = new QListView(securityTab);
    securityAdd = new QPushButton("Add", securityTab);
    securityDelete = new QPushButton("Delete", securityTab);
    securityFormWidget = new QWidget(securityTab);
    securityFormWidget->setEnabled(false);

    securityLayoutLeft->addWidget(securityList, 0, 0, 1, 2);
    securityLayoutLeft->addWidget(securityAdd, 1, 0, 1, 1);
    securityLayoutLeft->addWidget(securityDelete, 1, 1, 1, 1);

    securityLayout->addLayout(securityLayoutLeft);
    securityLayout->addWidget(securityFormWidget);
    securityForm.setupUI(securityFormWidget);

    tabs->addTab(securityTab, "Securites");

    tradeTab = new QWidget(dialog);
    tradeLayout = new QVBoxLayout(tradeTab);
    tradeLayoutTop = new QGridLayout();

    tradeList = new QListView(tradeTab);
    tradeFilter = new QLabel("Choose Security:", tradeTab);
    tradeFilterCmb = new QComboBox(tradeTab);
    tradeAdd = new QPushButton("Add", tradeTab);
    tradeDelete = new QPushButton("Delete", tradeTab);

    tradeLayoutTop->addWidget(tradeList, 0, 0, 1, 4);
    tradeLayoutTop->addWidget(tradeFilter, 1, 0, 1, 1);
    tradeLayoutTop->addWidget(tradeFilterCmb, 1, 1, 1, 1);
    tradeLayoutTop->addWidget(tradeAdd, 1, 2, 1, 1);
    tradeLayoutTop->addWidget(tradeDelete, 1, 3, 1, 1);


    tradeFormWidget = new QWidget(tradeTab);
    tradeFormWidget->setEnabled(false);

    tradeLayout->addLayout(tradeLayoutTop, 2);
    tradeLayout->addWidget(tradeFormWidget, 1);
    tradeForm.setupUI(tradeFormWidget);

    tabs->addTab(tradeTab, "Trades");
    tabs->setCurrentIndex(0);
}
