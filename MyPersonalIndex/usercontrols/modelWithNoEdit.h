#ifndef MODELWITHNOEDIT_H
#define MODELWITHNOEDIT_H

#include <QStandardItemModel>

class modelWithNoEdit: public QStandardItemModel
{
public:

    modelWithNoEdit(const int &rows = 0, const int &cols = 0, QWidget *parent = 0): QStandardItemModel(rows, cols, parent) {}

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        return QStandardItemModel::flags(index) & (~Qt::ItemIsEditable);
    }
};

#endif // MODELWITHNOEDIT_H
