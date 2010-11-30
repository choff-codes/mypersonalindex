#include "frmMainTableViewTree_UI.h"
#include <QHeaderView>

void frmMainTableViewTree_UI::setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_)
{
    frmMainTableView_UI::setupUI(columnNames_, parent_);
    tree = new QTreeWidget(widget);
    tree->header()->setVisible(false);
    layoutTable->insertWidget(0, tree, 1);
}
