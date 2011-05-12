#include "frmEditAcct_UI.h"
#include "account.h"

void frmEditAcct_UI::setupUI(QWidget *parent_)
{
    layout = new QHBoxLayout(parent_);

    layoutLeft = new QGridLayout();
    list = new QListView(parent_);
    list->setContextMenuPolicy(Qt::CustomContextMenu);
    list->setEnabled(false);
    addBtn = new QPushButton("Add", parent_);
    deleteBtn = new QPushButton("Delete", parent_);

    layoutLeft->addWidget(list, 0, 0, 1, 2);
    layoutLeft->addWidget(addBtn, 1, 0, 1, 1);
    layoutLeft->addWidget(deleteBtn, 1, 1, 1, 1);

    layout->addLayout(layoutLeft);

    groupBox = new QGroupBox("Account", parent_);
    groupBox->setEnabled(false);
    layout->addWidget(groupBox);

    layoutForm = new QFormLayout(groupBox);

    QString descTool = "A simple identifier for the account.";
    QString taxRateTool = "Estimated tax rate for this account when a gain is realized.";
    QString costBasisTool = "Calculation method for determining the cost basis of current shares.";
    QString taxDeferredTool = "A tax deferred account like a traditional 401(k)/IRA has the entire balance taxed when withdrawn.";
    QString hideTool = "Do not display on the account view.";

    desc = new QLabel("Description:", parent_);
    desc->setToolTip(descTool);
    layoutForm->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent_);
    descTxt->setToolTip(descTool);
    layoutForm->setWidget(0, QFormLayout::FieldRole, descTxt);

    taxRate = new QLabel("Tax Rate:", parent_);
    taxRate->setToolTip(taxRateTool);
    layoutForm->setWidget(1, QFormLayout::LabelRole, taxRate);

    taxRateLayout = new QHBoxLayout();
    taxRateSpinBox = new QDoubleSpinBox(parent_);
    taxRateSpinBox->setToolTip(taxRateTool);
    taxRateSpinBox->setDecimals(2);
    taxRateSpinBox->setMinimum(0);
    taxRateSpinBox->setMaximum(100);
    taxRateSpinBox->setSingleStep(1);
    taxRateSpinBox->setSuffix("%");
    taxRateSpinBox->setSpecialValueText("None");
    taxRateSpinBox->setValue(0);
    taxRateBtnClear = new QToolButton(parent_);
    taxRateBtnClear->setText("Clear");

    taxRateLayout->addWidget(taxRateSpinBox, 1);
    taxRateLayout->addWidget(taxRateBtnClear);
    layoutForm->setLayout(1, QFormLayout::FieldRole, taxRateLayout);

    costBasis = new QLabel("Cost Basis Method:", parent_);
    costBasis->setToolTip(costBasisTool);
    layoutForm->setWidget(2, QFormLayout::LabelRole, costBasis);

    costBasisCmb = new QComboBox(parent_);
    costBasisCmb->setToolTip(costBasisTool);
    costBasisCmb->addItem("FIFO", account::costBasisMethod_FIFO);
    costBasisCmb->addItem("LIFO", account::costBasisMethod_LIFO);
    costBasisCmb->addItem("HIFO", account::costBasisMethod_HIFO);
    costBasisCmb->addItem("Average", account::costBasisMethod_AVG);
    layoutForm->setWidget(2, QFormLayout::FieldRole, costBasisCmb);

    // no label
    layoutForm->setWidget(3, QFormLayout::LabelRole, new QLabel(parent_));
    taxDeferredChk = new QCheckBox("Tax Deferred", parent_);
    taxDeferredChk->setToolTip(taxDeferredTool);
    layoutForm->setWidget(3, QFormLayout::FieldRole, taxDeferredChk);

    // no label
    layoutForm->setWidget(4, QFormLayout::LabelRole, new QLabel(parent_));
    hideChk = new QCheckBox("Hide in Portfolio", parent_);
    hideChk->setToolTip(hideTool);
    layoutForm->setWidget(4, QFormLayout::FieldRole, hideChk);

    copyPastePopup = new QMenu(parent_);
    copyAction = new QAction("Copy", copyPastePopup);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    copyPastePopup->addAction(copyAction);
    pasteAction = new QAction("Paste", copyPastePopup);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    copyPastePopup->addAction(pasteAction);

    copyShortcut = new QShortcut(Qt::CTRL + Qt::Key_C, list);
    pasteShortcut = new QShortcut(Qt::CTRL + Qt::Key_V, list);

    desc->setBuddy(descTxt);
    taxRate->setBuddy(taxRateSpinBox);
    costBasis->setBuddy(costBasisCmb);
}

