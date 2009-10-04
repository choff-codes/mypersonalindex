#include "frmTableViewBase_UI.h"

void frmTableViewBase_UI::setupUI(QDialog* dialog, const QString &groupBoxText, const bool &showRightSideButtons)
{
    dialog->setWindowFlags(dialog->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    btnOkCancel = new QDialogButtonBox(dialog);
    btnOkCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mainLayout = new QVBoxLayout(dialog);
    gpMain = new QGroupBox(groupBoxText, dialog);
    groupLayout = new QGridLayout(gpMain);

    table = new QTableView(gpMain);
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setHighlightSections(false);
    int fntHeight = table->fontMetrics().height() + 2; // add small buffer
    table->verticalHeader()->setDefaultSectionSize(fntHeight);
    table->horizontalHeader()->setFixedHeight(fntHeight);
    table->horizontalHeader()->setStyleSheet("QHeaderView::section {border: none;}");
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    groupLayout->addWidget(table, 0, 0);

    if (showRightSideButtons)
    {
        groupLayoutRight = new QVBoxLayout();
        btnSelectAll = new QPushButton("Select All", gpMain);
        btnClear = new QPushButton("Clear", gpMain);
        btnMoveUp = new QPushButton("Move Up", gpMain);
        btnMoveDown = new QPushButton("Move Down", gpMain);
        groupLayoutRight->insertStretch(0);
        groupLayoutRight->addWidget(btnSelectAll);
        groupLayoutRight->addWidget(btnClear);
        groupLayoutRight->addWidget(btnMoveUp);
        groupLayoutRight->addWidget(btnMoveDown);
        groupLayoutRight->addStretch();
        groupLayout->addLayout(groupLayoutRight, 0, 1);
    }

    groupLayoutBottom = new QHBoxLayout();
    btnAdd = new QPushButton("Add New...", gpMain);
    btnEdit = new QPushButton("Edit...", gpMain);
    btnDelete = new QPushButton("Delete", gpMain);
    groupLayoutBottom->addWidget(btnAdd);
    groupLayoutBottom->addWidget(btnEdit);
    groupLayoutBottom->addWidget(btnDelete);
    btnPaste = new QPushButton("Paste", gpMain);
    groupLayoutBottom->addWidget(btnPaste);

    groupLayout->addLayout(groupLayoutBottom, 1, 0);

    mainLayout->addWidget(gpMain);
    mainLayout->addWidget(btnOkCancel);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
