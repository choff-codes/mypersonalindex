#ifndef MPIVIEWDELEGATES_H
#define MPIVIEWDELEGATES_H

#include <QItemDelegate>

class spinBoxDelegate : public QItemDelegate
{
public:
    spinBoxDelegate(double min_, double max_, QObject *parent_):
        QItemDelegate(parent_),
        m_min(min_),
        m_max(max_)
    {}

    QWidget* createEditor(QWidget *parent_, const QStyleOptionViewItem&, const QModelIndex&) const;
    void setEditorData(QWidget *editor_, const QModelIndex &index_) const;
    void setModelData(QWidget *editor_, QAbstractItemModel *model_, const QModelIndex &index_) const;
    void updateEditorGeometry(QWidget *editor_, const QStyleOptionViewItem &option_, const QModelIndex&) const;

private:
    double m_min;
    double m_max;
};

#endif // MPIVIEWDELEGATES_H
