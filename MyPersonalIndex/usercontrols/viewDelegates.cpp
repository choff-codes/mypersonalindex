#include "viewDelegates.h"

QWidget* spinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
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

void spinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor);
    s->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}

void spinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid())
        return;

    QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor);
    model->setData(index, s->value(), Qt::EditRole);
}

void spinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}
