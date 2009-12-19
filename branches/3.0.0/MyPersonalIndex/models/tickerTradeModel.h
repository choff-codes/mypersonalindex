#ifndef TICKERTRADEMODEL_H
#define TICKERTRADEMODEL_H

#include "globals.h"
#include "functions.h"
#include "mpiEditModelBase.h"
#include "frmTrade.h"

class tickerTradeModel : public mpiEditModelBase<globals::dynamicTrade, frmTrade>
{
    Q_OBJECT

public:
    tickerTradeModel(const QList<globals::dynamicTrade> &values, const QMap<int, globals::security> &cashAccounts, const int &cols, QTableView *parent = 0, QDialog *dialog = 0):
        mpiEditModelBase<globals::dynamicTrade, frmTrade>(values, cols, parent, dialog), m_cashAccounts(cashAccounts) { }

private:
    const QMap<int, globals::security> &m_cashAccounts;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QString internalCopy(const globals::dynamicTrade &item);
    globals::dynamicTrade internalPaste(const QStringList &value, bool *ok);

public slots:
    void addNew() { addItem(); autoResize(); }
    void editSelected() { editItems(); autoResize(); }
    void deleteSelected() { removeItems(); autoResize(); }
    void copy() { beginCopy(); }
    void paste() { beginPaste(); }
    void autoResize();

signals:
    void saveItem(globals::dynamicTrade *trade);
    void deleteItem(const globals::dynamicTrade& trade);
};

#endif // TICKERTRADEMODEL_H
