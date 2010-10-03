#include "frmPortfolio_UI.h"

void frmPortfolio_UI::setupUI(QWidget *parent)
{
    layout = new QFormLayout(parent);
    layout->setVerticalSpacing(8);

    desc = new QLabel("&Description:", parent);
    layout->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent);
    layout->setWidget(0, QFormLayout::FieldRole, descTxt);

    startValue = new QLabel("&Index Start Value:", parent);
    layout->setWidget(1, QFormLayout::LabelRole, startValue);

    startValueTxt = new QLineEdit(parent);
    startValueValidator = new mpiIntValidator(1, 1000000, parent);
    startValueTxt->setValidator(startValueValidator);
    layout->setWidget(1, QFormLayout::FieldRole, startValueTxt);

    aaThreshold = new QLabel("&AA Threshold:", parent);
    layout->setWidget(2, QFormLayout::LabelRole, aaThreshold);

    aaThresholdSpinBox = new QSpinBox(parent);
    aaThresholdSpinBox->setMaximum(100);
    aaThresholdSpinBox->setSuffix("%");
    layout->setWidget(2, QFormLayout::FieldRole, aaThresholdSpinBox);

    aaThresholdValue = new QLabel("&Threshold Method:", parent);
    layout->setWidget(3, QFormLayout::LabelRole, aaThresholdValue);

    aaThresholdValueCmb = new QComboBox(parent);
    aaThresholdValueCmb->addItem("Portfolio Value");
    aaThresholdValueCmb->addItem("AA Value");
    layout->setWidget(3, QFormLayout::FieldRole, aaThresholdValueCmb);

    costBasis = new QLabel("&Cost Basis Method:", parent);
    layout->setWidget(4, QFormLayout::LabelRole, costBasis);

    costBasisCmb = new QComboBox(parent);
    costBasisCmb->addItem("FIFO", costBasis_FIFO);
    costBasisCmb->addItem("LIFO", costBasis_LIFO);
    costBasisCmb->addItem("HIFO", costBasis_HIFO);
    costBasisCmb->addItem("Average", costBasis_AVG);
    layout->setWidget(4, QFormLayout::FieldRole, costBasisCmb);

    startDate = new QLabel("&Start Date:", parent);
    layout->setWidget(5, QFormLayout::LabelRole, startDate);

    startDateDateEdit = new mpiDateEdit(parent);
    layout->setWidget(5, QFormLayout::FieldRole, startDateDateEdit);

    // no text label
    layout->setWidget(6, QFormLayout::LabelRole, new QLabel(parent));

    includeDivChk = new QCheckBox("I&nclude Dividends", parent);
    layout->setWidget(6, QFormLayout::FieldRole, includeDivChk);

    desc->setBuddy(descTxt);
    startValue->setBuddy(startValueTxt);
    aaThreshold->setBuddy(aaThresholdSpinBox);
    aaThresholdValue->setBuddy(aaThresholdValueCmb);
    costBasis->setBuddy(costBasisCmb);
    startDate->setBuddy(startDateDateEdit);
}
