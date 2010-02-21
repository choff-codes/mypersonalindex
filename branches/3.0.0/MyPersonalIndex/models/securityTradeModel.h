#ifndef SECURITYTRADEMODEL_H
#define SECURITYTRADEMODEL_H

#include <QtGui>
#include "functions.h"
#include "frmTrade.h"
#include "security.h"

class securityTradeModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    securityTradeModel(const QList<trade> &trades, const QMap<int, security> &cashAccounts, QTableView *parent = 0, QDialog *dialog = 0):
        QAbstractTableModel(parent), m_parent(parent), m_dialog(dialog), m_trades(trades), m_cashAccounts(cashAccounts) { }

    QMap<int, trade> saveList(const QMap<int, trade> &originalValues, const int &parentID);

public slots:
    void addNew();
    void editSelected();
    void deleteSelected();
    void copy();
    void paste();
    void autoResize();

private:
    QTableView *m_parent;
    QDialog *m_dialog;
    QList<trade> m_trades;
    const QMap<int, security> m_cashAccounts;

    const static int m_columns = 9;

    int rowCount(const QModelIndex&) const { return m_trades.count(); }
    int columnCount (const QModelIndex&) const { return m_columns; }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    QString internalCopy(const trade &item);
    trade internalPaste(const QStringList &value, bool *ok);
    void selectItem(const QModelIndex &index);
    QList<int> getSelectedRows() const;
};

#endif // SECURITYTRADEMODEL_H
