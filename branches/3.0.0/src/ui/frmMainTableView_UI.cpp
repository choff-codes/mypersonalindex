#include "frmMainTableView_UI.h"
#include <QHeaderView>

void frmMainTableView_UI::setupUI(const QMap<int, QString> &columnNames_, bool hasRowLabels_, QWidget *parent_)
{
    widget = new QWidget(parent_);

    layout = new QVBoxLayout(widget);
    layout->setSpacing(0);
    layout->setMargin(1);

    frmMainToolbar_UI::setupUI(columnNames_, widget);

    tableSplitter = new QSplitter(widget);

    table = new mpiTableView(hasRowLabels_, widget);
    tableCopy = new QShortcut(Qt::CTRL + Qt::Key_C, table);
    if (!hasRowLabels_)
        table->verticalHeader()->hide();
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setHighlightSections(false);
    table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tableSplitter->addWidget(table);
    tableSplitter->setStretchFactor(0, 3);

    layout->addWidget(toolbar);
    layout->addWidget(tableSplitter);
}
