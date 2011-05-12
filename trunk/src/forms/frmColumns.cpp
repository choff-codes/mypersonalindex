#include "frmColumns.h"
#include "frmColumns_UI.h"

frmColumns::frmColumns(const QList<int> &selectedItems_, const QMap<int, QString> &items_, QWidget *parent_):
    QDialog(parent_),
    ui(new frmColumns_UI),
    m_selectedItems(selectedItems_),
    m_items(items_)
{
    ui->setupUI(this);

    // since selectedItems can be unsorted, this is not efficient, but the lists are never large so far
    for(QMap<int, QString>::const_iterator i = m_items.constBegin(); i != m_items.constEnd(); ++i)
    {
        if (selectedItems_.contains(i.key()))
            continue;

        QListWidgetItem *item = new QListWidgetItem(i.value(), ui->removedItemsList);
        item->setData(Qt::UserRole, i.key());
    }

    foreach(int itemID, m_selectedItems)
    {
        if (!items_.contains(itemID))
            continue;

        QListWidgetItem *item = new QListWidgetItem(items_.value(itemID), ui->addedItemsList);
        item->setData(Qt::UserRole, itemID);
    }

    connect(ui->okCancelBtn, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->okCancelBtn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui->moveBtnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->moveBtnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->reorderBtnMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui->reorderBtnMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
}

frmColumns::~frmColumns()
{
    delete ui;
}

void frmColumns::remove()
{
    switchSelected(ui->addedItemsList, ui->removedItemsList);
}

void frmColumns::add()
{
    switchSelected(ui->removedItemsList, ui->addedItemsList);
}

void frmColumns::accept()
{
    QList<int> selected;
    for(int i = 0; i < ui->addedItemsList->count(); ++i)
        selected.append(ui->addedItemsList->item(i)->data(Qt::UserRole).toInt());

    if (selected == m_selectedItems)
    {
        QDialog::reject();
        return;
    }

    m_selectedItems = selected;
    QDialog::accept();
}

void frmColumns::switchSelected(QListWidget *from, QListWidget* to)
{
    QList<QListWidgetItem*> itemsToAdd = from->selectedItems();
    if (itemsToAdd.isEmpty())
        return;

    to->clearSelection();
    foreach(QListWidgetItem* item, itemsToAdd)
    {
        QListWidgetItem *addedItem = new QListWidgetItem(item->text(), to);
        addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
        to->addItem(addedItem);
        delete item;
        to->setCurrentItem(addedItem);
    }
    to->setFocus();
}

void frmColumns::move(direction direction_) {
    QList<int> itemsToMove;
    foreach(QListWidgetItem* item, ui->addedItemsList->selectedItems())
        itemsToMove.append(ui->addedItemsList->row(item));

    if (itemsToMove.isEmpty())
        return;

    qSort(itemsToMove);
    ui->addedItemsList->clearSelection();

    // Increments from 0 to the end of the list widget when moving down
    // Increments from the end of the list widget to 0 when moving up
    // This prevents items from "hopping" over one another in the simple case, for example,
    // that the first two items are selected and move up is called. The second item
    // will not move in this case.
    int listCounter =
        direction_ == direction_up ?
            0 :
            ui->addedItemsList->count() - 1;

    // 1 when moving up, since the loop goes through the items in sorted order (by row index)
    // -1 when moving down, since the loops goes through the items in descending order
    int increment = direction_ * -1;

    // value of i to break out of for loop, when moving up it's the end of the
    // and moving down the beginning of the list (loop in reverse order)
    int breakCondition =
            direction_ == direction_up ?
                itemsToMove.count() :
                -1;

    // start value of i, beginning of the list for move up, end of list for move down
    int i =
            direction_ == direction_up ?
                0 :
                itemsToMove.count() - 1;

    while (i != breakCondition)
    {
        int row = itemsToMove.at(i);
        if (row == listCounter) // occurs when there is a selected item directly above, so don't hop it
        {
            ui->addedItemsList->setCurrentRow(row); // select the row again, but don't need to move
            continue;
        }

        QListWidgetItem* moving = ui->addedItemsList->takeItem(row);
        ui->addedItemsList->insertItem(row + direction_, moving);
        ui->addedItemsList->setCurrentRow(row + direction_);

        i += increment;
        listCounter += increment;
    }
}
