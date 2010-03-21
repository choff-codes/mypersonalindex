#include "frmColumns.h"

frmColumns::frmColumns(const QList<int> &selected, const QMap<int, QString> &values, const QString &title,
    const QDialog::DialogCode &resultNoChange, QWidget *parent): QDialog(parent), m_selected(selected), m_values(values),
    m_resultNoChange(resultNoChange)
{
    ui.setupUI(this);
    this->setWindowTitle(title);

    for(QMap<int, QString>::const_iterator i = m_values.constBegin(); i != m_values.constEnd(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(i.value(), ui.removedItems);
        item->setData(Qt::UserRole, i.key());
    }

    foreach(const int &columnID, m_selected)
        for(int i = 0; i < ui.removedItems->count(); ++i)
            if (ui.removedItems->item(i)->data(Qt::UserRole).toInt() == columnID)
            {
                QListWidgetItem *item = ui.removedItems->item(i);
                QListWidgetItem *addedItem = new QListWidgetItem(item->text(), ui.addedItems);
                addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
                ui.addedItems->addItem(addedItem);
                delete item;
                break;
            }

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui.btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui.btnMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(ui.btnMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
}

void frmColumns::accept()
{
    QList<int> selected;
    for(int i = 0; i < ui.addedItems->count(); ++i)
        selected.append(ui.addedItems->item(i)->data(Qt::UserRole).toInt());

    if (selected == m_selected)
    {
        QDialog::done(m_resultNoChange);
        return;
    }

    m_selected = selected;
    QDialog::accept();
}

void frmColumns::switchSelected(QListWidget *from, QListWidget* to)
{
    QList<QListWidgetItem*> toAdd = from->selectedItems();
    if (toAdd.isEmpty())
        return;

    to->clearSelection();
    foreach(QListWidgetItem* item, toAdd)
    {
        QListWidgetItem *addedItem = new QListWidgetItem(item->text(), to);
        addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
        to->addItem(addedItem);
        delete item;
        to->setCurrentItem(addedItem);
    }
    to->setFocus();
}

void frmColumns::moveDown()
{
    QList<int> toMove;
    foreach(QListWidgetItem* item, ui.addedItems->selectedItems())
        toMove.append(ui.addedItems->row(item));

    if (toMove.isEmpty())
        return;

    qSort(toMove);
    ui.addedItems->clearSelection();
    int x = ui.addedItems->count() - 1;

    for(int i = toMove.count() - 1; i >= 0; --i, --x)
    {
        int row = toMove.at(i);
        int spacesToMove = row == x ? 0 : 1;

        QListWidgetItem* moving = ui.addedItems->takeItem(row);
        ui.addedItems->insertItem(row + spacesToMove, moving);
        ui.addedItems->setCurrentRow(row + spacesToMove);
    }
}

void frmColumns::moveUp()
{
    QList<int> toMove;
    foreach(QListWidgetItem* item, ui.addedItems->selectedItems())
        toMove.append(ui.addedItems->row(item));

    if (toMove.isEmpty())
        return;

    qSort(toMove);
    ui.addedItems->clearSelection();
    int x = 0;

    for(int i = 0; i < toMove.count(); ++i, ++x)
    {
        int row = toMove.at(i);
        int spacesToMove = row == x ? 0 : -1;

        QListWidgetItem* moving = ui.addedItems->takeItem(row);
        ui.addedItems->insertItem(row + spacesToMove, moving);
        ui.addedItems->setCurrentRow(row + spacesToMove);
    }
}
