#include "frmColumns_UI.h"

void frmColumns_UI::setupUI(QDialog *dialog_)
{
    dialog_->setWindowFlags(dialog_->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    dialog_->setWindowTitle("Add/Remove");

    okCancelBtn = new QDialogButtonBox(dialog_);
    okCancelBtn->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    layout = new QVBoxLayout(dialog_);
    groupBox = new QGroupBox("Add/Remove", dialog_);
    groupBoxLayout = new QHBoxLayout(groupBox);

    removedItemsList = new QListWidget(groupBox);
    removedItemsList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupBoxLayout->addWidget(removedItemsList);

    moveLayout = new QVBoxLayout();
    moveBtnAdd = new QToolButton(groupBox);
    moveBtnAdd->setArrowType(Qt::RightArrow);
    moveBtnRemove = new QToolButton(groupBox);
    moveBtnRemove->setArrowType(Qt::LeftArrow);
    moveLayout->addStretch();
    moveLayout->addWidget(moveBtnAdd);
    moveLayout->addWidget(moveBtnRemove);
    moveLayout->addStretch();
    groupBoxLayout->addLayout(moveLayout);

    addedItemsList = new QListWidget(groupBox);
    addedItemsList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupBoxLayout->addWidget(addedItemsList);

    reorderLayout = new QVBoxLayout();
    reorderBtnMoveUp = new QToolButton(groupBox);
    reorderBtnMoveUp->setArrowType(Qt::UpArrow);
    reorderBtnMoveDown = new QToolButton(groupBox);
    reorderBtnMoveDown->setArrowType(Qt::DownArrow);
    reorderLayout->addStretch();
    reorderLayout->addWidget(reorderBtnMoveUp);
    reorderLayout->addWidget(reorderBtnMoveDown);
    reorderLayout->addStretch();
    groupBoxLayout->addLayout(reorderLayout);

    layout->addWidget(groupBox);
    layout->addWidget(okCancelBtn);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}
