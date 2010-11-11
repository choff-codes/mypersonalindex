#include "frmMainTableView_UI.h"
#include <QHeaderView>

void frmMainTableView_UI::setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_)
{
    widget = new QWidget(parent_);

    layout = new QVBoxLayout(widget);
    layout->setSpacing(0);
    layout->setMargin(1);

    toolbar = new QToolBar(widget);

    toolbarDateBegin = new QLabel(" Begin Date: ", toolbar);
    toolbarDateBeginEdit = new mpiDateEdit(toolbar);
    toolbarDateEnd = new QLabel(" End Date: ", toolbar);
    toolbarDateEndEdit = new mpiDateEdit(toolbar);
    toolbarSort = new QLabel(" Sort By: ", toolbar);
    toolbarSortCmb = new QComboBox(toolbar);
    toolbarSortCmb->setMinimumContentsLength(15);
    toolbarSortCmb->addItem("(None)", -1);
    for (QMap<int, QString>::const_iterator i = columnNames_.constBegin(); i != columnNames_.constEnd(); ++i)
            toolbarSortCmb->addItem(i.value(), i.key());
    toolbarSortCmb->addItem("Custom...", -2);

    toolbarReorder = new QAction("Modify Columns...", toolbar);
    toolbarReorder->setIconText("Modify Columns...");
    toolbarExport = new QAction("Export...", toolbar);
    toolbarExport->setIconText("Export...");

    toolbar->addWidget(toolbarDateBegin);
    toolbar->addWidget(toolbarDateBeginEdit);
    toolbar->addWidget(toolbarDateEnd);
    toolbar->addWidget(toolbarDateEndEdit);
    toolbar->addSeparator();
    toolbar->addWidget(toolbarSort);
    toolbar->addWidget(toolbarSortCmb);
    toolbar->addSeparator();
    toolbar->addAction(toolbarReorder);
    toolbar->addSeparator();
    toolbar->addAction(toolbarExport);

    table = new mpiTableView(false, widget);
    tableCopy = new QShortcut(Qt::CTRL + Qt::Key_C, table);
    table->verticalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setHighlightSections(false);
    table->verticalHeader()->setDefaultSectionSize(table->fontMetrics().height() + 2);
    table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    layout->addWidget(toolbar);
    layout->addWidget(table);
}
