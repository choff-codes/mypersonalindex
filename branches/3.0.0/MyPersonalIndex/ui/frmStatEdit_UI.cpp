#include "frmStatEdit_UI.h"

void frmStatEdit_UI::setupUI(QDialog *dialog)
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

    sql = new QLabel("SQL:", dialog);
    formLayout->setWidget(1, QFormLayout::LabelRole, sql);

    txtSql = new QPlainTextEdit(dialog);
    formLayout->setWidget(1, QFormLayout::FieldRole, txtSql);

    format = new QLabel("Format:", dialog);
    formLayout->setWidget(2, QFormLayout::LabelRole, format);

    cmbFormat = new QComboBox(dialog);
    cmbFormat->addItem("Currency");
    cmbFormat->addItem("Percentage");
    cmbFormat->addItem("Decimal");
    cmbFormat->addItem("Integer");
    cmbFormat->addItem("Short Date");
    cmbFormat->addItem("Long Date");
    cmbFormat->addItem("None");
    formLayout->setWidget(2, QFormLayout::FieldRole, cmbFormat);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
