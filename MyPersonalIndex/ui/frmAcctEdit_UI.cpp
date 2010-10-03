#include "frmAcctEdit_UI.h"

void frmAcctEdit_UI::setupUI(QWidget *parent)
{
    layout = new QFormLayout(parent);

    desc = new QLabel("&Description:", parent);
    layout->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent);
    layout->setWidget(0, QFormLayout::FieldRole, descTxt);

    taxRate = new QLabel("&Tax Rate:", parent);
    layout->setWidget(1, QFormLayout::LabelRole, taxRate);

    taxRateLayout = new QHBoxLayout();
    taxRateSpinBox = new QDoubleSpinBox(parent);
    taxRateSpinBox->setDecimals(2);
    taxRateSpinBox->setMinimum(-1);
    taxRateSpinBox->setMaximum(100);
    taxRateSpinBox->setSingleStep(1);
    taxRateSpinBox->setSuffix("%");
    taxRateSpinBox->setSpecialValueText("None");
    taxRateSpinBox->setValue(-1);
    taxRateBtnClear = new QToolButton(parent);
    taxRateBtnClear->setText("C&lear");

    taxRateLayout->addWidget(taxRateSpinBox, 1);
    taxRateLayout->addWidget(taxRateBtnClear);
    layout->setLayout(1, QFormLayout::FieldRole, taxRateLayout);

    costBasis = new QLabel("&Cost Basis Method:", parent);
    layout->setWidget(2, QFormLayout::LabelRole, costBasis);

    costBasisCmb = new QComboBox(parent);
    costBasisCmb->addItem("Portfolio Default", costBasis_None);
    costBasisCmb->addItem("FIFO", costBasis_FIFO);
    costBasisCmb->addItem("LIFO", costBasis_LIFO);
    costBasisCmb->addItem("HIFO", costBasis_HIFO);
    costBasisCmb->addItem("Average", costBasis_AVG);
    layout->setWidget(2, QFormLayout::FieldRole, costBasisCmb);

    // no label
    layout->setWidget(3, QFormLayout::LabelRole, new QLabel(parent));

    taxDeferredChk = new QCheckBox("T&ax Deferred", parent);
    layout->setWidget(3, QFormLayout::FieldRole, taxDeferredChk);

    desc->setBuddy(descTxt);
    taxRate->setBuddy(taxRateSpinBox);
    costBasis->setBuddy(costBasisCmb);
}

