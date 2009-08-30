#ifndef DATEEDITDELEGATE_H
#define DATEEDITDELEGATE_H

#include <QtGui>
#include "functions.h"

class dateEditDelegate : public QItemDelegate
{
public:
    dateEditDelegate(QObject *parent): QItemDelegate(parent) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QDateEdit *dateWidget = functions::createDateEdit(parent);
        dateWidget->installEventFilter(const_cast<dateEditDelegate*>(this));
        return dateWidget;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        if (!index.isValid())
            return;
        
        QDateEdit *dateWidget = static_cast<QDateEdit*>(editor);
        dateWidget->setDate(QDate::fromString(index.model()->data(index, Qt::DisplayRole).toString(), globals::shortDateFormat));
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if(!index.isValid())
            return;

        QDateEdit *dateWidget = static_cast<QDateEdit*>(editor);
        model->setData(index, dateWidget->date().toJulianDay());
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }

};

#endif // DATEEDITDELEGATE_H
