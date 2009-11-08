#ifndef VIEWDELEGATES_H
#define VIEWDELEGATES_H

#include <QtGui>
#include "functions.h"
#include "globals.h"

//class dateEditDelegate : public QItemDelegate
//{
//public:
//    dateEditDelegate(QObject *parent): QItemDelegate(parent) {}
//
//    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
//    {
//        QDateEdit *dateWidget = functions::createDateEdit(parent);
//        dateWidget->installEventFilter(const_cast<dateEditDelegate*>(this));
//        return dateWidget;
//    }
//
//    void setEditorData(QWidget *editor, const QModelIndex &index) const
//    {
//        if (!index.isValid())
//            return;
//
//        QDateEdit *dateWidget = static_cast<QDateEdit*>(editor);
//        dateWidget->setDate(QDate::fromString(index.model()->data(index, Qt::DisplayRole).toString(), globals::shortDateFormat));
//    }
//
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//    {
//        if(!index.isValid())
//            return;
//
//        QDateEdit *dateWidget = static_cast<QDateEdit*>(editor);
//        model->setData(index, dateWidget->date().toJulianDay());
//    }
//
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
//    {
//        editor->setGeometry(option.rect);
//    }
//
//};

class spinBoxDelegate : public QItemDelegate
{
public:
    spinBoxDelegate(const double &min, const double &max, QObject *parent): QItemDelegate(parent), m_min(min), m_max(max) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
    {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setDecimals(2);
        editor->setMinimum(m_min);
        editor->setMaximum(m_max);
        editor->setSingleStep(1);
        editor->setSuffix("%");
        editor->installEventFilter(const_cast<spinBoxDelegate*>(this));
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        if (!index.isValid())
            return;

        QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor);
        s->setValue(index.model()->data(index, Qt::EditRole).toDouble());
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if(!index.isValid())
            return;

        QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor);
        model->setData(index, s->value(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
    {
        editor->setGeometry(option.rect);
    }

private:
    double m_min, m_max;
};

#endif // VIEWDELEGATES_H
