#ifndef FRMMAINTABLEVIEWLIST_UI_H
#define FRMMAINTABLEVIEWLIST_UI_H

#include <QTreeWidget>
#include "frmMainTableView_UI.h"

class frmMainTableViewList_UI: public frmMainTableView_UI
{
public:
    QTreeWidget *tree;

    virtual ~frmMainTableViewList_UI() {}

    virtual void setupUI(const QMap<int, QString> &columnNames_, QWidget *parent_ = 0);
};

#endif // FRMMAINTABLEVIEWLIST_UI_H
