#include "frmPortfolio_UI.h"

void frmPortfolio_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(15);

    layout = new QFormLayout();
    layout->setVerticalSpacing(8);

    desc = new QLabel("&Description:", dialog);
    layout->setWidget(0, QFormLayout::LabelRole, desc);

    txtDesc = new QLineEdit(dialog);
    layout->setWidget(0, QFormLayout::FieldRole, txtDesc);

    startValue = new QLabel("&Index Start Value:", dialog);
    layout->setWidget(1, QFormLayout::LabelRole, startValue);

    txtStartValue = new QLineEdit(dialog);
    ivalidator = new mpiIntValidator(1, 1000000, dialog);
    txtStartValue->setValidator(ivalidator);
    layout->setWidget(1, QFormLayout::FieldRole, txtStartValue);

    aaThreshold = new QLabel("&AA Threshold:", dialog);
    layout->setWidget(2, QFormLayout::LabelRole, aaThreshold);

    sbAAThreshold = new QSpinBox(dialog);
    sbAAThreshold->setMaximum(100);
    sbAAThreshold->setSuffix("%");
    layout->setWidget(2, QFormLayout::FieldRole, sbAAThreshold);

    aaThresholdValue = new QLabel("&Threshold Method:", dialog);
    layout->setWidget(3, QFormLayout::LabelRole, aaThresholdValue);

    cmbAAThresholdValue = new QComboBox(dialog);
    cmbAAThresholdValue->addItem("Portfolio Value");
    cmbAAThresholdValue->addItem("AA Value");
    layout->setWidget(3, QFormLayout::FieldRole, cmbAAThresholdValue);

    costBasis = new QLabel("&Cost Basis Method:", dialog);
    layout->setWidget(4, QFormLayout::LabelRole, costBasis);

    cmbCostBasis = new QComboBox(dialog);
    cmbCostBasis->addItem("FIFO");
    cmbCostBasis->addItem("LIFO");
    cmbCostBasis->addItem("Average");
    layout->setWidget(4, QFormLayout::FieldRole, cmbCostBasis);

    startDate = new QLabel("&Start Date:", dialog);
    layout->setWidget(5, QFormLayout::LabelRole, startDate);

    dateStartDate = new mpiDateEdit(dialog);
    layout->setWidget(5, QFormLayout::FieldRole, dateStartDate);

    QLabel *tmp = new QLabel(dialog);  // hack needed for correct resizing
    layout->setWidget(6, QFormLayout::LabelRole, tmp);

    chkIncludeDiv = new QCheckBox("I&nclude Dividends", dialog);
    layout->setWidget(6, QFormLayout::FieldRole, chkIncludeDiv);

    mainLayout->addLayout(layout);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    desc->setBuddy(txtDesc);
    startValue->setBuddy(txtStartValue);
    aaThreshold->setBuddy(sbAAThreshold);
    aaThresholdValue->setBuddy(cmbAAThresholdValue);
    costBasis->setBuddy(cmbCostBasis);
    startDate->setBuddy(dateStartDate);
}
