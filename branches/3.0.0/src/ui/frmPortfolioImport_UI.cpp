#include "frmPortfolioImport_UI.h"
#include <QDialog>
#include <QHeaderView>

void frmPortfolioImport_UI::setupUI(QDialog *dialog_)
{
    dialog_->setWindowFlags((dialog_->windowFlags() & (~Qt::WindowContextHelpButtonHint)) | Qt::WindowMinMaxButtonsHint);
    dialog_->setWindowTitle("Import Portfolio");

    layout = new QGridLayout(dialog_);

    portfolio = new QLabel("Portfolio:", dialog_);
    portfolioCmb = new QComboBox(dialog_);
    tree = new QTreeWidget(dialog_);
    tree->header()->hide();
    portfolioNewRadio = new QRadioButton("Create as New Portfolio", dialog_);
    portfolioNewRadio->setChecked(true);
    description = new QLabel("Description:", dialog_);
    descriptionTxt = new QLineEdit(dialog_);
    portfolioExistingRadio = new QRadioButton("Import into Current Portfolio", dialog_);
    okCancelBtn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, dialog_);

    layout->addWidget(portfolio, 0, 0);
    layout->addWidget(portfolioCmb, 0, 1);
    layout->addWidget(tree, 1, 0, 1, 2);
    layout->addWidget(portfolioNewRadio, 2, 0, 1, 2);
    layout->addWidget(description, 3, 0);
    layout->addWidget(descriptionTxt, 3, 1);
    layout->addWidget(portfolioExistingRadio, 4, 0, 1, 2);
    layout->addWidget(okCancelBtn, 5, 0, 1, 2);

    description->setBuddy(descriptionTxt);
}
