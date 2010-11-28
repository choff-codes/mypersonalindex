#include "frmMainToolbar_UI.h"

void frmMainToolbar_UI::setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_)
{
    toolbar = new QToolBar(parent_);

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
    if (!columnNames_.isEmpty())
    {
        toolbar->addSeparator();
        toolbar->addWidget(toolbarSort);
        toolbar->addWidget(toolbarSortCmb);
        toolbar->addSeparator();
        toolbar->addAction(toolbarReorder);
    }
    toolbar->addSeparator();
    toolbar->addAction(toolbarExport);
}
