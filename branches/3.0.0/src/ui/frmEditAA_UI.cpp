#include "frmEditAA_UI.h"
#include "assetAllocation.h"

void frmEditAA_UI::setupUI(QWidget *parent_)
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

    groupBox = new QGroupBox("Asset Class", parent_);
    groupBox->setEnabled(false);
    layout->addWidget(groupBox);

    layoutForm = new QFormLayout(groupBox);

    QString descTool = "A simple identifier for the asset class.";
    QString targetTool = "What percent should this asset class make up of the total portfolio?";
    QString rebalanceBandTool = "What percent outside of the target should trigger a rebalance?";
    QString rebalanceDenominatorTool = "A rebalance denominator is how the rebalance band above is calculated.\nPortfolio value will use the total portfolio value. Asset class value uses\nthe total portfolio value multipled by the asset class target. Portfolio value\n will trigger a rebalance less often.";
    QString hideTool = "Do not display on the asset allocation view.";

    desc = new QLabel("Description:", parent_);
    desc->setToolTip(descTool);
    layoutForm->setWidget(0, QFormLayout::LabelRole, desc);

    descTxt = new QLineEdit(parent_);
    descTxt->setToolTip(descTool);
    layoutForm->setWidget(0, QFormLayout::FieldRole, descTxt);

    target = new QLabel("Target:", parent_);
    target->setToolTip(targetTool);
    layoutForm->setWidget(1, QFormLayout::LabelRole, target);

    targetLayout = new QHBoxLayout();
    targetSpinBox = new QDoubleSpinBox(parent_);
    targetSpinBox->setToolTip(targetTool);
    targetSpinBox->setDecimals(2);
    targetSpinBox->setMinimum(0);
    targetSpinBox->setMaximum(100);
    targetSpinBox->setSingleStep(1);
    targetSpinBox->setSuffix("%");
    targetSpinBox->setSpecialValueText("None");
    targetSpinBox->setValue(0);
    targetBtnClear = new QToolButton(parent_);
    targetBtnClear->setText("Clear");

    targetLayout->addWidget(targetSpinBox, 1);
    targetLayout->addWidget(targetBtnClear);

    layoutForm->setLayout(1, QFormLayout::FieldRole, targetLayout);

    rebalanceBand = new QLabel("Rebalance Band:", parent_);
    rebalanceBand->setToolTip(rebalanceBandTool);
    layoutForm->setWidget(2, QFormLayout::LabelRole, rebalanceBand);

    rebalanceBandSpinBox = new QDoubleSpinBox(parent_);
    rebalanceBandSpinBox->setToolTip(rebalanceBandTool);
    rebalanceBandSpinBox->setDecimals(2);
    rebalanceBandSpinBox->setMinimum(0);
    rebalanceBandSpinBox->setMaximum(100);
    rebalanceBandSpinBox->setSingleStep(1);
    rebalanceBandSpinBox->setSuffix("%");
    rebalanceBandSpinBox->setSpecialValueText("None");
    rebalanceBandSpinBox->setValue(0);
    layoutForm->setWidget(2, QFormLayout::FieldRole, rebalanceBandSpinBox);

    threshold = new QLabel("Rebalance Denominator:", parent_);
    threshold->setToolTip(rebalanceDenominatorTool);
    layoutForm->setWidget(3, QFormLayout::LabelRole, threshold);

    thresholdCmb = new QComboBox(parent_);
    thresholdCmb->setToolTip(rebalanceDenominatorTool);
    thresholdCmb->addItem("Portfolio Value", assetAllocation::thresholdMethod_Portfolio);
    thresholdCmb->addItem("Asset Class Value", assetAllocation::thresholdMethod_AA);
    layoutForm->setWidget(3, QFormLayout::FieldRole, thresholdCmb);

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
    target->setBuddy(targetSpinBox);
    rebalanceBand->setBuddy(rebalanceBandSpinBox);
    threshold->setBuddy(thresholdCmb);
}
