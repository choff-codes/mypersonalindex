#include "mpiViewDelegates.h"
#include <QDoubleSpinBox>

QWidget* spinBoxDelegate::createEditor(QWidget *parent_, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent_);
    editor->setDecimals(2);
    editor->setMinimum(m_min);
    editor->setMaximum(m_max);
    editor->setSingleStep(1);
    editor->setSuffix("%");
    editor->installEventFilter(const_cast<spinBoxDelegate*>(this));
    return editor;
}

void spinBoxDelegate::setEditorData(QWidget *editor_, const QModelIndex &index_) const
{
    if (!index_.isValid())
        return;

    QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor_);
    s->setValue(index_.model()->data(index_, Qt::EditRole).toDouble());
}

void spinBoxDelegate::setModelData(QWidget *editor_, QAbstractItemModel *model_, const QModelIndex &index_) const
{
    if(!index_.isValid())
        return;

    QDoubleSpinBox *s = static_cast<QDoubleSpinBox*>(editor_);
    model_->setData(index_, s->value(), Qt::EditRole);
}

void spinBoxDelegate::updateEditorGeometry(QWidget *editor_, const QStyleOptionViewItem &option_, const QModelIndex&) const
{
    editor_->setGeometry(option_.rect);
}
