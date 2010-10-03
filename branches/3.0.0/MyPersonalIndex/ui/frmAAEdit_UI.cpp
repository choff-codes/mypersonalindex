#include "frmAAEdit_UI.h"

void frmAAEdit_UI::setupUI(QWidget *parent)
{
    layout = new QFormLayout(parent);

    desc = new QLabel("&Description:", parent);
    layout->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent);
    layout->setWidget(0, QFormLayout::FieldRole, descTxt);

    target = new QLabel("&Target:", parent);
    layout->setWidget(1, QFormLayout::LabelRole, target);

    targetLayout = new QHBoxLayout();
    targetSpinBox = new QDoubleSpinBox(parent);
    targetSpinBox->setDecimals(2);
    targetSpinBox->setMinimum(-1);
    targetSpinBox->setMaximum(100);
    targetSpinBox->setSingleStep(1);
    targetSpinBox->setSuffix("%");
    targetSpinBox->setSpecialValueText("None");
    targetSpinBox->setValue(-1);
    targetBtnClear = new QToolButton(parent);
    targetBtnClear->setText("&Clear");

    targetLayout->addWidget(targetSpinBox, 1);
    targetLayout->addWidget(targetBtnClear);

    layout->setLayout(1, QFormLayout::FieldRole, targetLayout);

    desc->setBuddy(descTxt);
    target->setBuddy(targetSpinBox);
}
