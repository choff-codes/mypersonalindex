#ifndef SECURITYAAMODEL_H
#define SECURITYAAMODEL_H

#include <QAbstractTableModel>
#include <QTableView>
#include <QMap>
#include "functions.h"
#include "security.h"
#include "assetAllocation.h"

class securityAAModel: public QAbstractTableModel
{
    Q_OBJECT

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

    double totalAssignedPercentage() const { return m_target.totalAssignedPercentage(); }
    void addNew(int id_);
    void deleteSelected(QItemSelectionModel selection_);

private:
    assetAllocationTarget m_target;
    QList<int> m_keys;
    QMap<int, assetAllocation> m_descriptions;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex&) const { return m_keys.count(); }
    int columnCount (const QModelIndex&) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int, Qt::Orientation, int) const { return QVariant(); }
    bool setData(const QModelIndex &index, const QVariant &value, int role);

signals:
    void updateHeader();
};

#endif // SECURITYAAMODEL_H
