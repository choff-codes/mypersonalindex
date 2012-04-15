#include "frmEdit_UI.h"
#include <QDialog>

void frmEdit_UI::setupUI(QDialog* dialog_) {
    dialog_->setWindowFlags((dialog_->windowFlags() & (~Qt::WindowContextHelpButtonHint)) | Qt::WindowMinMaxButtonsHint);
    dialog_->setWindowTitle("Edit Portfolio");

    layout = new QVBoxLayout(dialog_);

    tabs = new QTabWidget(dialog_);

    okCancelBtn = new QDialogButtonBox(dialog_);
    okCancelBtn->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    layout->addWidget(tabs);
    layout->addWidget(okCancelBtn);

    portfolioTab = new QWidget(dialog_);
    tabs->addTab(portfolioTab, "Portfolio");

    acctTab = new QWidget(dialog_);
    tabs->addTab(acctTab, "Accounts");

    aaTab = new QWidget(dialog_);
    tabs->addTab(aaTab, "Asset Allocation");

    securityTab = new QWidget(dialog_);
    tabs->addTab(securityTab, "Securites");

    tradeTab = new QWidget(dialog_);
    tabs->addTab(tradeTab, "Trades");

    tabs->setCurrentIndex(0);
}
