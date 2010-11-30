#ifndef FRMMAINTABLEVIEWTREE_UI_H
#define FRMMAINTABLEVIEWTREE_UI_H

#include <QTreeWidget>
#include "frmMainTableView_UI.h"

class frmMainTableViewTree_UI: public frmMainTableView_UI
{
public:
    QTreeWidget *tree;

    virtual ~frmMainTableViewTree_UI() {}

    virtual void setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_ = 0);
};

#endif // FRMMAINTABLEVIEWTREE_UI_H
