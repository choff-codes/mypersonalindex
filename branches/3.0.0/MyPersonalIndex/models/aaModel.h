#ifndef AAMODEL_H
#define AAMODEL_H

#include "functions.h"
#include "mpiEditModelBase.h"
#include "frmAAEdit.h"

class aaModel : public mpiEditModelBase<assetAllocation, frmAAEdit>
{
    Q_OBJECT

public:
    aaModel(const QList<assetAllocation> &values, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
            mpiEditModelBase<assetAllocation, frmAAEdit>(values, cols, parent, dialog) { }

private:
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QString internalCopy(const assetAllocation &item);
    assetAllocation internalPaste(const QStringList &value, bool *ok);

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }
    inline void copy() { beginCopy(); }
    inline void paste() { beginPaste(); }

signals:
    void saveItem(assetAllocation *aa);
    void deleteItem(const assetAllocation& aa);
};

#endif // AAMODEL_H
