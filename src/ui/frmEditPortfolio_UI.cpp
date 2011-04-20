#include "frmEditPortfolio_UI.h"

void frmEditPortfolio_UI::setupUI(QWidget *parent_)
{
    layout = new QHBoxLayout(parent_);
    groupBox = new QGroupBox("Portfolio", parent_);
    layout->addWidget(groupBox);

    layoutPortfolio = new QFormLayout(groupBox);
    layoutPortfolio->setVerticalSpacing(8);

    QString descTool = "A simple identifier for the portfolio.";
    QString startDateTool = "The date you opened the portfolio or when a theoretical portfolio should start calculating.";

    desc = new QLabel("Description:", parent_);
    desc->setToolTip(descTool);
    layoutPortfolio->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent_);
    descTxt->setToolTip(descTool);
    layoutPortfolio->setWidget(0, QFormLayout::FieldRole, descTxt);

    startDate = new QLabel("Inception Date:", parent_);
    startDate->setToolTip(startDateTool);
    layoutPortfolio->setWidget(1, QFormLayout::LabelRole, startDate);

    startDateDateEdit = new mpiDateEdit(parent_);
    startDateDateEdit->setToolTip(startDateTool);
    layoutPortfolio->setWidget(1, QFormLayout::FieldRole, startDateDateEdit);

    desc->setBuddy(descTxt);
    startDate->setBuddy(startDateDateEdit);
}
