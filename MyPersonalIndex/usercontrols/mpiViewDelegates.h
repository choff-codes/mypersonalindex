#ifndef MPIVIEWDELEGATES_H
#define MPIVIEWDELEGATES_H

#include <QtGui>

class spinBoxDelegate : public QItemDelegate
{
public:
    spinBoxDelegate(const double &min, const double &max, QObject *parent): QItemDelegate(parent), m_min(min), m_max(max) {}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const;

private:
    double m_min, m_max;
};

//class checkBoxDelegate : public QItemDelegate
//{
//public:
//    checkBoxDelegate(QObject *parent): QItemDelegate(parent) {}
//
//    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
//    {
//        QCheckBox *editor = new QCheckBox(parent);
//        editor->installEventFilter(const_cast<checkBoxDelegate*>(this));
//        return editor;
//    }
//
//    void setEditorData(QWidget *editor, const QModelIndex &index) const
//    {
//        if (!index.isValid())
//            return;
//
//        QCheckBox *c = static_cast<QCheckBox*>(editor);
//        c->setChecked(index.model()->data(index).toInt() == 1);
//    }
//
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//    {
//        if(!index.isValid())
//            return;
//
//        QCheckBox *c = static_cast<QCheckBox*>(editor);
//        model->setData(index, c->isChecked() ? 1 : 0);
//    }
//
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
//    {
//        editor->setGeometry(option.rect);
//    }
//};

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

#endif // MPIVIEWDELEGATES_H
