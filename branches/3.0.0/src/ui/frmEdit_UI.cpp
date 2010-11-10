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
    acctList->setContextMenuPolicy(Qt::CustomContextMenu);
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
    tabs->addTab(aaTab, "Asset Allocation");

    securityTab = new QWidget(dialog);
    securityLayout = new QHBoxLayout(securityTab);
    securityLayoutLeft = new QGridLayout();
    securityList = new QListView(securityTab);
    securityList->setContextMenuPolicy(Qt::CustomContextMenu);
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
    tradeList->setContextMenuPolicy(Qt::CustomContextMenu);
    tradeFilter = new QLabel("Choose Security:", tradeTab);
    tradeFilterCmb = new QComboBox(tradeTab);
    tradeAdd = new QPushButton("Add", tradeTab);
    tradeDelete = new QPushButton("Delete", tradeTab);

    tradeLayoutTop->addWidget(tradeList, 0, 0, 1, 5);
    tradeLayoutTop->addWidget(tradeFilter, 1, 0, 1, 1);
    tradeLayoutTop->addWidget(tradeFilterCmb, 1, 1, 1, 2);
    tradeLayoutTop->addWidget(tradeAdd, 1, 3, 1, 1);
    tradeLayoutTop->addWidget(tradeDelete, 1, 4, 1, 1);

    tradeFormWidget = new QWidget(tradeTab);
    tradeFormWidget->setEnabled(false);

    tradeLayout->addLayout(tradeLayoutTop, 2);
    tradeLayout->addWidget(tradeFormWidget, 1);
    tradeForm.setupUI(tradeFormWidget);

    tabs->addTab(tradeTab, "Trades");
    tabs->setCurrentIndex(0);

    copyPastePopup = new QMenu(dialog);
    copyAction = new QAction("Copy", copyPastePopup);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    copyPastePopup->addAction(copyAction);
    pasteAction = new QAction("Paste", copyPastePopup);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    copyPastePopup->addAction(pasteAction);

    acctCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, acctList);
    acctPasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, acctList);
    securityCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, securityList);
    securityPasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, securityList);
    tradeCopyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, tradeList);
    tradePasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, tradeList);
}
