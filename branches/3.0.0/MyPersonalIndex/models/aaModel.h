#ifndef AAMODEL_H
#define AAMODEL_H

#include "globals.h"
#include "functions.h"
#include "mpiEditModelBase.h"
#include "frmAAEdit.h"

class aaModel : public mpiEditModelBase<globals::assetAllocation, frmAAEdit>
{
    Q_OBJECT

public:
    aaModel(const QList<globals::assetAllocation> &values, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
            mpiEditModelBase<globals::assetAllocation, frmAAEdit>(values, cols, parent, dialog) { }

private:
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QString internalCopy(const globals::assetAllocation &item);
    globals::assetAllocation internalPaste(const QStringList &value, bool *ok);

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }
    inline void copy() { beginCopy(); }
    inline void paste() { beginPaste(); }

signals:
    void saveItem(globals::assetAllocation *aa);
    void deleteItem(const globals::assetAllocation& aa);
};

#endif // AAMODEL_H
