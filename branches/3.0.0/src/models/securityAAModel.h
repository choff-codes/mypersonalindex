#ifndef SECURITYAAMODEL_H
#define SECURITYAAMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include "assetAllocationTarget.h"
#include "assetAllocation.h"

class securityAAModel: public QAbstractTableModel
{
public:
    assetAllocationTarget getTargets() const { return m_target; }

    securityAAModel(assetAllocationTarget target_, const QMap<int, assetAllocation> &descriptions_, QObject *parent_ = 0):
        QAbstractTableModel(parent_),
        m_target(target_),
        m_keys(target_.keys()),
        m_descriptions(descriptions_)
    {
        insertRows(0, m_keys.count());
    }

    void addNew(int id_);
    void deleteSelected(const QModelIndexList &selection_);

private:
    assetAllocationTarget m_target;
    QList<int> m_keys;
    QMap<int, assetAllocation> m_descriptions;

    Qt::ItemFlags flags(const QModelIndex &index_) const;
    int rowCount(const QModelIndex&) const { return m_keys.count(); }
    int columnCount (const QModelIndex&) const { return 2; }

    void insertUnassigned();
    void removeUnassigned();

    QVariant data(const QModelIndex &index_, int role_) const;
    QVariant headerData(int, Qt::Orientation, int) const { return QVariant(); }
    bool setData(const QModelIndex &index_, const QVariant &value_, int role_);
};

#endif // SECURITYAAMODEL_H
