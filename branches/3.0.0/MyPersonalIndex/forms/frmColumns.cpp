#include "frmColumns.h"

frmColumns::frmColumns(const QList<int> &selectedColumns, const QMap<int, QString> &columns, QWidget *parent):
        QDialog(parent), m_selectedColumns(selectedColumns), m_columns(columns)
{
    ui.setupUI(this);

    for(QMap<int, QString>::const_iterator i = m_columns.constBegin(); i != m_columns.constEnd(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(i.value(), ui.removedColumns);
        item->setData(Qt::UserRole, i.key());
    }

    foreach(const int &columnID, m_selectedColumns)
    {
        for(int i = 0; i < ui.removedColumns->count(); ++i)
            if (ui.removedColumns->item(i)->data(Qt::UserRole).toInt() == columnID)
            {
                QListWidgetItem *item = ui.removedColumns->item(i);
                QListWidgetItem *addedItem = new QListWidgetItem(item->text(), ui.addedColumns);
                addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
                ui.addedColumns->addItem(addedItem);
                delete item;
                break;
            }
    }

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addColumn()));
    connect(ui.btnRemove, SIGNAL(clicked()), this, SLOT(removeColumn()));
    connect(ui.btnMoveUp, SIGNAL(clicked()), this, SLOT(moveColumnUp()));
    connect(ui.btnMoveDown, SIGNAL(clicked()), this, SLOT(moveColumnDown()));
}

void frmColumns::accept()
{
    QList<int> selected;
    for(int i = 0; i < ui.addedColumns->count(); ++i)
        selected.append(ui.addedColumns->item(i)->data(Qt::UserRole).toInt());

    if (selected == m_selectedColumns)
    {
       QDialog::reject();
       return;
   }

    m_selectedColumns = selected;

    QDialog::accept();
}

void frmColumns::addColumn()
{
    if (ui.removedColumns->currentIndex().row() < 0)
        return;

    QListWidgetItem *item = ui.removedColumns->item(ui.removedColumns->currentIndex().row());
    QListWidgetItem *addedItem = new QListWidgetItem(item->text(), ui.addedColumns);
    addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
    ui.addedColumns->addItem(addedItem);
    delete item;
    ui.addedColumns->setCurrentItem(addedItem);
}

void frmColumns::removeColumn()
{
    if (ui.addedColumns->currentIndex().row() < 0)
        return;

    QListWidgetItem *item = ui.addedColumns->item(ui.addedColumns->currentIndex().row());
    QListWidgetItem *addedItem = new QListWidgetItem(item->text(), ui.removedColumns);
    addedItem->setData(Qt::UserRole, item->data(Qt::UserRole));
    ui.removedColumns->addItem(addedItem);
    delete item;
    ui.removedColumns->setCurrentItem(addedItem);
}

void frmColumns::moveColumnDown()
{
    int i = ui.addedColumns->currentIndex().row();

    if (i < 0 || i == ui.addedColumns->count() - 1)
        return;

    QListWidgetItem *item = ui.addedColumns->takeItem(i);
    ui.addedColumns->insertItem(i + 1, item);
    ui.addedColumns->setCurrentItem(item);
}

void frmColumns::moveColumnUp()
{
    int i = ui.addedColumns->currentIndex().row();

    if (i < 1)
        return;

    QListWidgetItem *item = ui.addedColumns->takeItem(i);
    ui.addedColumns->insertItem(i - 1, item);
    ui.addedColumns->setCurrentItem(item);
}
