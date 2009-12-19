#ifndef STATMODEL_H
#define STATMODEL_H

#include "globals.h"
#include "functions.h"
#include "mpiEditModelBase.h"
#include "frmStatEdit.h"

class statModel : public mpiEditModelBase<globals::statistic, frmStatEdit>
{
    Q_OBJECT

public:
    statModel(const QList<globals::statistic> &values, const QList<int> &selected, const int &cols, QTableView *parent = 0, QDialog *dialog = 0);

    QList<int> getSelected();

private:
    QMap<int, bool> m_selected;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    bool setData (const QModelIndex &index, const QVariant &value, int role);

    // not used
    QString internalCopy(const globals::statistic&) { return QString(); }
    // not used
    globals::statistic internalPaste(const QStringList&, bool*) {  return globals::statistic(); }

public slots:
    inline void addNew() { addItem(); }
    inline void editSelected() { editItems(); }
    inline void deleteSelected() { removeItems(); }
    inline void moveSelectedUp() { moveUp(); }
    inline void moveSelectedDown() { moveDown(); }
    void selectAll();
    void clearAll();
    void itemDeleted(const QModelIndex&, int start, int end);

signals:
    void saveItem(globals::statistic *stat);
    void deleteItem(const globals::statistic& stat);
};

#endif // STATMODEL_H
