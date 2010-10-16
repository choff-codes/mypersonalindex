#include "frmEditAcct_UI.h"

void frmEditAcct_UI::setupUI(QWidget *parent)
{
    layout = new QHBoxLayout(parent);
    groupBox = new QGroupBox("Account", parent);
    layout->addWidget(groupBox);

    layoutAcct = new QFormLayout(groupBox);

    desc = new QLabel("&Description:", parent);
    layoutAcct->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent);
    layoutAcct->setWidget(0, QFormLayout::FieldRole, descTxt);

    taxRate = new QLabel("&Tax Rate:", parent);
    layoutAcct->setWidget(1, QFormLayout::LabelRole, taxRate);

    taxRateLayout = new QHBoxLayout();
    taxRateSpinBox = new QDoubleSpinBox(parent);
    taxRateSpinBox->setDecimals(2);
    taxRateSpinBox->setMinimum(0);
    taxRateSpinBox->setMaximum(100);
    taxRateSpinBox->setSingleStep(1);
    taxRateSpinBox->setSuffix("%");
    taxRateSpinBox->setSpecialValueText("None");
    taxRateSpinBox->setValue(0);
    taxRateBtnClear = new QToolButton(parent);
    taxRateBtnClear->setText("C&lear");

    taxRateLayout->addWidget(taxRateSpinBox, 1);
    taxRateLayout->addWidget(taxRateBtnClear);
    layoutAcct->setLayout(1, QFormLayout::FieldRole, taxRateLayout);

    costBasis = new QLabel("&Cost Basis Method:", parent);
    layoutAcct->setWidget(2, QFormLayout::LabelRole, costBasis);

    costBasisCmb = new QComboBox(parent);
    costBasisCmb->addItem("FIFO", account::costBasisMethod_FIFO);
    costBasisCmb->addItem("LIFO", account::costBasisMethod_LIFO);
    costBasisCmb->addItem("HIFO", account::costBasisMethod_HIFO);
    costBasisCmb->addItem("Average", account::costBasisMethod_AVG);
    layoutAcct->setWidget(2, QFormLayout::FieldRole, costBasisCmb);

    // no label
    layoutAcct->setWidget(3, QFormLayout::LabelRole, new QLabel(parent));

    taxDeferredChk = new QCheckBox("T&ax Deferred", parent);
    layoutAcct->setWidget(3, QFormLayout::FieldRole, taxDeferredChk);

    desc->setBuddy(descTxt);
    taxRate->setBuddy(taxRateSpinBox);
    costBasis->setBuddy(costBasisCmb);
}

