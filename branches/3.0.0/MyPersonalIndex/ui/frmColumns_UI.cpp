#include "frmColumns_UI.h"

void frmColumns_UI::setupUI(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    gpMain = new QGroupBox("Add/Remove", dialog);
    groupLayout = new QHBoxLayout(gpMain);

    removedColumns = new QListWidget(gpMain);
    removedColumns->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupLayout->addWidget(removedColumns);

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

    addedColumns = new QListWidget(gpMain);
    addedColumns->setSelectionMode(QAbstractItemView::ExtendedSelection);
    groupLayout->addWidget(addedColumns);

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
