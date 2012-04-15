#include "frmMainTableViewTree_UI.h"
#include <QHeaderView>

void frmMainTableViewTree_UI::setupUI(const QMap<int, QString> &columnNames_, bool hasRowLabels_, QWidget *parent_)
{
    frmMainTableView_UI::setupUI(columnNames_, hasRowLabels_, parent_);
    treeWidget = new QWidget(widget);
    treeLayout = new QVBoxLayout(treeWidget);
    treeLayout->setMargin(0);
    treeLayout->setSpacing(0);
    treeCmb = new QComboBox(widget);
    tree = new QTreeWidget(widget);
    tree->header()->setVisible(false);
    treeAllPortfolios = new QPushButton("All Portfolios", widget);
    treeClearBtn = new QPushButton("Reset", widget);
    treeLayout->addWidget(treeCmb);
    treeLayout->addWidget(tree);
    treeLayout->addWidget(treeAllPortfolios);
    treeLayout->addWidget(treeClearBtn);
    tableSplitter->insertWidget(0, treeWidget);
    tableSplitter->setStretchFactor(0, 1);
}
