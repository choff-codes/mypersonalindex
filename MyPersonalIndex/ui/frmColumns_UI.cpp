#include "frmColumns_UI.h"

void frmColumns_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    gpMain = new QGroupBox("Add/Remove", dialog);
    groupLayout = new QHBoxLayout(gpMain);

    removedItems = new QListWidget(gpMain);
    removedItems->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupLayout->addWidget(removedItems);

    moveLayout = new QVBoxLayout();
    btnAdd = new QToolButton(gpMain);
    btnAdd->setArrowType(Qt::RightArrow);
    btnRemove = new QToolButton(gpMain);
    btnRemove->setArrowType(Qt::LeftArrow);
    moveLayout->addStretch();
    moveLayout->addWidget(btnAdd);
    moveLayout->addWidget(btnRemove);
    moveLayout->addStretch();
    groupLayout->addLayout(moveLayout);

    addedItems = new QListWidget(gpMain);
    addedItems->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupLayout->addWidget(addedItems);

    reorderLayout = new QVBoxLayout();
    btnMoveUp = new QToolButton(gpMain);
    btnMoveUp->setArrowType(Qt::UpArrow);
    btnMoveDown = new QToolButton(gpMain);
    btnMoveDown->setArrowType(Qt::DownArrow);
    reorderLayout->addStretch();
    reorderLayout->addWidget(btnMoveUp);
    reorderLayout->addWidget(btnMoveDown);
    reorderLayout->addStretch();
    groupLayout->addLayout(reorderLayout);

    mainLayout->addWidget(gpMain);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
