#include "frmEditPortfolio_UI.h"

void frmEditPortfolio_UI::setupUI(QWidget *parent_)
{
    layout = new QHBoxLayout(parent_);
    groupBox = new QGroupBox("Portfolio", parent_);
    layout->addWidget(groupBox);

    layoutPortfolio = new QFormLayout(groupBox);
    layoutPortfolio->setVerticalSpacing(8);

    desc = new QLabel("&Description:", parent_);
    layoutPortfolio->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent_);
    layoutPortfolio->setWidget(0, QFormLayout::FieldRole, descTxt);

    startDate = new QLabel("&Start Date:", parent_);
    layoutPortfolio->setWidget(1, QFormLayout::LabelRole, startDate);

    startDateDateEdit = new mpiDateEdit(parent_);
    layoutPortfolio->setWidget(1, QFormLayout::FieldRole, startDateDateEdit);

    desc->setBuddy(descTxt);
    startDate->setBuddy(startDateDateEdit);
}
