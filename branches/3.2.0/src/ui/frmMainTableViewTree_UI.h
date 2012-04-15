#ifndef FRMMAINTABLEVIEWTREE_UI_H
#define FRMMAINTABLEVIEWTREE_UI_H

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include "frmMainTableView_UI.h"

class frmMainTableViewTree_UI: public frmMainTableView_UI
{
public:
    QWidget *treeWidget;
    QVBoxLayout *treeLayout;
    QComboBox *treeCmb;
    QTreeWidget *tree;
    QPushButton *treeAllPortfolios;
    QPushButton *treeClearBtn;

    virtual ~frmMainTableViewTree_UI() {}

    virtual void setupUI(const QMap<int, QString> &columnNames_, bool hasRowLabels_, QWidget *parent_ = 0);
};

#endif // FRMMAINTABLEVIEWTREE_UI_H
