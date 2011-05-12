#ifndef MAINTRADEMODEL_H
#define MAINTRADEMODEL_H

#include <QAbstractItemModel>
#include "security.h"

class portfolio;
class mainTradeModel: public QAbstractItemModel
{
public:
    mainTradeModel(const portfolio &portfolio_, int beginDate_, int endDate_, QObject *parent_ = 0);

    int rowCount(const QModelIndex&) const;
    int columnCount (const QModelIndex&) const;

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int, Qt::Orientation, int) const { return QVariant(); }
    Qt::ItemFlags flags(const QModelIndex &index_) const;

    QModelIndex index(int row_, int column_, const QModelIndex &parent_) const;
    QModelIndex parent(const QModelIndex &child_) const;

private:
    QList<security> m_securities;
    int m_beginDate;
    int m_endDate;

    static bool securitySort(const security &row1_, const security &row2_);
};

#endif // MAINTRADEMODEL_H
