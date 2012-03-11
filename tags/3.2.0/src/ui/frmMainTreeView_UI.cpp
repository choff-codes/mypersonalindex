#include "frmMainTreeView_UI.h"
#include <QHeaderView>

void frmMainTreeView_UI::setupUI(QWidget *parent_)
{
    widget = new QWidget(parent_);

    layout = new QVBoxLayout(widget);
    layout->setSpacing(0);
    layout->setMargin(1);

    frmMainToolbar_UI::setupUI(QMap<int, QString>(), widget);

    tree = new QTreeView(widget);
    tree->header()->hide();

    layout->addWidget(toolbar);
    layout->addWidget(tree);
}
