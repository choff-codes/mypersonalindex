#include "frmAAEdit_UI.h"

void frmAAEdit_UI::setupUI(QDialog *dialog)
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

    target = new QLabel("Target:", dialog);
    formLayout->setWidget(1, QFormLayout::LabelRole, target);

    targetLayout = new QHBoxLayout();
    sbTarget = new QDoubleSpinBox(dialog);
    sbTarget->setDecimals(2);
    sbTarget->setMinimum(-1);
    sbTarget->setMaximum(100);
    sbTarget->setSingleStep(1);
    sbTarget->setSuffix("%");
    sbTarget->setSpecialValueText("None");
    sbTarget->setValue(-1);
    btnClearTarget = new QToolButton(dialog);
    btnClearTarget->setText("Clear");

    targetLayout->addWidget(sbTarget, 1);
    targetLayout->addWidget(btnClearTarget);

    formLayout->setLayout(1, QFormLayout::FieldRole, targetLayout);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
