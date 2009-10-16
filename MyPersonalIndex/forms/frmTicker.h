#ifndef FRMTICKER_H
#define FRMTICKER_H

#include <QtGui>
#include "frmTicker_UI.h"
#include "globals.h"
#include "queries.h"
#include "mpiModelBase.h"
#include "frmTrade.h"
#include "functions.h"

class tickerAAModel;
class tickerTradeModel;
class frmTicker : public QDialog
{
    Q_OBJECT

public:

    const globals::security& getReturnValues() const { return m_security; }
    const int &getTickerID() const { return m_security.id; }
    const QMap<int, globals::security>* getCashAccounts() const { return &m_data->tickers; }

    frmTicker(QWidget *parent = 0, queries *sql = 0, const int &portfolioID = 0, const globals::portfolioData *data = 0, const globals::security& security = globals::security());

private:

    frmTicker_UI ui;
    queries *m_sql;
    int m_portfolioID;
    const globals::portfolioData *m_data;
    globals::security m_security;
    globals::security m_securityOriginal;
    tickerTradeModel *m_modelTrade;
    tickerAAModel *m_modelAA;

    void connectSlots();
    void loadSecurity();
    void loadDropDowns();
    void installAAModel();

private slots:
    void accept();
    void resetExpense();
    void addAA();
    void updateAAPercentage();
    void saveItem(globals::dynamicTrade *trade);
    void deleteItem(const globals::dynamicTrade &trade);
};

class tickerAAModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    QList<globals::intdoublePair> getList() { return m_list; }

    tickerAAModel(const QList<globals::intdoublePair> &values, const QMap<int, globals::assetAllocation> *aaValues = 0, const int &cols = 0, QTableView *parent = 0):
            QAbstractTableModel(parent), m_aaValues(aaValues), m_parent(parent), m_columns(cols), m_list(values) {}

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        if (index.column() == 1)
                flags = flags | Qt::ItemIsEditable;

        return flags;
    }

    int rowCount(const QModelIndex&) const
    {
        return m_list.count();
    }

    int columnCount (const QModelIndex&) const
    {
        return m_columns;
    }

    double totalPercentage()
    {
        double total = 0;

        foreach(const globals::intdoublePair &pair, m_list)
            total += pair.value;

        return total;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= m_list.size())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            if (index.column() == 0 && m_aaValues)
                return m_aaValues->value(m_list.at(index.row()).key).description;

            if (index.column() == 1)
                return functions::doubleToPercentage(m_list.at(index.row()).value);
        }

        if (role == Qt::EditRole)
        {
            if (index.column() == 0 && m_aaValues)
                return m_aaValues->value(m_list.at(index.row()).key).description;

            if (index.column() == 1)
                return m_list.at(index.row()).value;
        }

        return QVariant();
    }

    QVariant headerData (int, Qt::Orientation, int) const
    {
        return QVariant();
    }

    bool setData (const QModelIndex &index, const QVariant &value, int role)
    {
        if (index.isValid() && index.column() == 1 && role == Qt::EditRole)
        {
            m_list[index.row()].value = value.toDouble();
            emit updateHeader();
            return true;
        }

        return false;
    }

public slots:
    void addNew(const int &id)
    {
        beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
        double total = totalPercentage();
        m_list.append(globals::intdoublePair(id, total >= 100 ? 0 : 100 - total));
        endInsertRows();
        emit updateHeader();
    }

    void deleteSelected()
    {
        QList<int> indexes;
        foreach(const QModelIndex &q, m_parent->selectionModel()->selectedRows())
            indexes.append(q.row());
        qSort(indexes);

        if(indexes.count() == 0)
            return;

        for(int i = indexes.count() - 1; i >= 0; --i)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_list.removeAt(indexes.at(i));
            endRemoveRows();
        }
        emit updateHeader();
    }

signals:
    void updateHeader();

private:
    const QMap<int, globals::assetAllocation> *m_aaValues;
    QTableView *m_parent;
    int m_columns;
    QList<globals::intdoublePair> m_list;
};

class tickerTradeModel : public mpiModelBase<globals::dynamicTrade, frmTrade>
{
    Q_OBJECT

public:
    tickerTradeModel(const QList<globals::dynamicTrade> &values, const QMap<int, globals::security> *cashAccounts = 0, const int &cols = 0, QTableView *parent = 0, QDialog *dialog = 0):
            mpiModelBase<globals::dynamicTrade, frmTrade>(values, cols, parent, dialog), m_cashAccounts(cashAccounts) { }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= m_list.size())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            globals::dynamicTrade trade = m_list.at(index.row());
            if (index.column() == 0)
            {
                return trade.tradeTypeToString();
            }
            if (index.column() == 1)
            {
                return trade.valueToString();
            }
            if (index.column() == 2)
            {
                return trade.price < 0 ? "Prev Close" : functions::doubleToCurrency(trade.price);
            }
            if (index.column() == 3)
            {
                return trade.commission < 0 ? "" : functions::doubleToCurrency(trade.commission);
            }
            if (index.column() == 4)
            {
                return m_cashAccounts && m_cashAccounts->contains(trade.cashAccount) ? m_cashAccounts->value(trade.cashAccount).symbol : "";
            }
            if (index.column() == 5)
            {
                return trade.frequencyToString();
            }
            if (index.column() == 6)
            {
                return trade.dateToString();
            }
            if (index.column() == 7)
            {
                return trade.startDate.isValid() ? trade.startDate.toString(Qt::SystemLocaleShortDate) : "";
            }
            if (index.column() == 8)
            {
                return trade.endDate.isValid() ? trade.endDate.toString(Qt::SystemLocaleShortDate) : "";
            }
        }

        return QVariant();
    }

    QVariant headerData (int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        if (section == 0)
            return "Type";

        if (section == 1)
            return "Value";

        if (section == 2)
            return "Price";

        if (section == 3)
            return "Comm.";

        if (section == 4)
            return "$ Acct";

        if (section == 5)
            return "Freq.";

        if (section == 6)
            return "Date";

        if (section == 7)
            return "Start";

        if (section == 8)
            return "End";

        return QVariant();
    }

public slots:
    inline void addNew() { addItem(); autoResize(); }
    inline void editSelected() { editItems(); autoResize(); }
    inline void deleteSelected() { removeItems(); autoResize(); }

signals:
    void saveItem(globals::dynamicTrade *trade);
    void deleteItem(const globals::dynamicTrade& trade);

private:
    const QMap<int, globals::security> *m_cashAccounts;

    void autoResize()
    {
        if (m_list.count() == 0)
            m_parent->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        else
            m_parent->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    }
};

#endif // FRMTICKER_H
