#include "frmAcctEdit_UI.h"

void frmAcctEdit_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    formLayout = new QFormLayout();

    desc = new QLabel("&Description:", dialog);
    formLayout->setWidget(0, QFormLayout::LabelRole, desc);

    txtDesc = new QLineEdit(dialog);
    formLayout->setWidget(0, QFormLayout::FieldRole, txtDesc);

    taxrate = new QLabel("&Tax Rate:", dialog);
    formLayout->setWidget(1, QFormLayout::LabelRole, taxrate);

    taxRateLayout = new QHBoxLayout();
    sbTaxRate = new QDoubleSpinBox(dialog);
    sbTaxRate->setDecimals(2);
    sbTaxRate->setMinimum(-1);
    sbTaxRate->setMaximum(100);
    sbTaxRate->setSingleStep(1);
    sbTaxRate->setSuffix("%");
    sbTaxRate->setSpecialValueText("None");
    sbTaxRate->setValue(-1);
    btnClearTaxRate = new QToolButton(dialog);
    btnClearTaxRate->setText("C&lear");

    taxRateLayout->addWidget(sbTaxRate, 1);
    taxRateLayout->addWidget(btnClearTaxRate);
    formLayout->setLayout(1, QFormLayout::FieldRole, taxRateLayout);

    costBasis = new QLabel("&Cost Basis Method:", dialog);
    formLayout->setWidget(2, QFormLayout::LabelRole, costBasis);

    cmbCostBasis = new QComboBox(dialog);
    cmbCostBasis->addItem("Portfolio Default");
    cmbCostBasis->addItem("FIFO");
    cmbCostBasis->addItem("LIFO");
    cmbCostBasis->addItem("Average");
    formLayout->setWidget(2, QFormLayout::FieldRole, cmbCostBasis);

    formLayout->setWidget(3, QFormLayout::LabelRole, new QLabel(dialog));

    chkTaxDeferred = new QCheckBox("T&ax Deferred", dialog);
    formLayout->setWidget(3, QFormLayout::FieldRole, chkTaxDeferred);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    desc->setBuddy(txtDesc);
    taxrate->setBuddy(sbTaxRate);
    costBasis->setBuddy(cmbCostBasis);
}

