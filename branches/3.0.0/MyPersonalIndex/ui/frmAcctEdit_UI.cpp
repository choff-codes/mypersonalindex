#include "frmAcctEdit_UI.h"

void frmAcctEdit_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    formLayout = new QFormLayout();

    desc = new QLabel("Description:", dialog);
    formLayout->setWidget(0, QFormLayout::LabelRole, desc);

    txtDesc = new QLineEdit(dialog);
    formLayout->setWidget(0, QFormLayout::FieldRole, txtDesc);

    taxrate = new QLabel("Tax Rate:", dialog);
    formLayout->setWidget(1, QFormLayout::LabelRole, taxrate);

    sbTaxRate = new QDoubleSpinBox(dialog);
    sbTaxRate->setDecimals(2);
    sbTaxRate->setMinimum(-1);
    sbTaxRate->setMaximum(100);
    sbTaxRate->setSingleStep(1);
    sbTaxRate->setSuffix("%");
    sbTaxRate->setSpecialValueText("None");
    sbTaxRate->setValue(-1);
    formLayout->setWidget(1, QFormLayout::FieldRole, sbTaxRate);

    formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(dialog));

    chkTaxDeferred = new QCheckBox("Tax Deferred", dialog);
    formLayout->setWidget(2, QFormLayout::FieldRole, chkTaxDeferred);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}

