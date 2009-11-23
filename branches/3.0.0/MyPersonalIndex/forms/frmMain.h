#ifndef FRMMAIN_H
#define FRMMAIN_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QtGui>
#include <QtSql>
#include "frmMain_UI.h"
#include "globals.h"
#include "functions.h"
#include "queries.h"
#include "updatePrices.h"

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain(QWidget *parent = 0);
    ~frmMain() { delete sql; qDeleteAll(m_portfolios); }

    bool databaseExists() { return sql; }

private:
    frmMain_UI ui;
    queries *sql;
    QMap<int, globals::myPersonalIndex*> m_portfolios;
    globals::myPersonalIndex *m_currentPortfolio;
    int m_lastDate;
    globals::settings m_settings;
    QList<int> m_dates;
    QMap<int, globals::statistic> m_statistics;
    updatePrices *m_updateThread;

    void connectSlots();
    void loadSettings();
    void loadSettingsColumns();
    void resetLastDate();
    void checkVersion();
    void saveSettings();
    void loadPortfolioDropDown(const int &portfolioID);
    void loadPortfolios();
    void loadPortfoliosInfo();
    void loadPortfoliosTickers();
    void loadPortfoliosTickersAA();
    void loadPortfoliosTickersTrades();
    void loadPortfoliosAA();
    void loadPortfoliosAcct();
    void loadPortfoliosStat();
    void loadDates();
    void loadStats();
    void loadPortfolioSettings();
    void savePortfolio();
    void savePortfolios();
    void disableItems(bool disabled);
    int getCurrentDateOrPrevious(int date);
    int getDateDropDownDate(QDateEdit *dateDropDown);
    void loadSortDropDown(const QList<QString> &fieldNames, QComboBox *dropDown);

private slots:
    void dateChanged(QDate);
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void loadPortfolioHoldings(const QDate&);
    void about();
    void addTicker();
    void editTicker();
    void options();
    void aa();
    void acct();
    void stat();
    void beginUpdate();
    void finishUpdate(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);
    void holdingsShowHiddenToggle() { loadPortfolioHoldings(QDate()); }
    void holdingsModifyColumns();
    void sortDropDownChange(int index);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FRMMAIN_H


class holdingsModel: public QAbstractTableModel
{
    Q_OBJECT

public:

    holdingsModel(QSqlQuery *query, QList<int> viewableColumns, QTableView *parent = 0): QAbstractTableModel(parent), m_parent(parent), m_query(query), m_viewableColumns(viewableColumns)
    {
        m_rowCount = 0;
        if (!m_query)
            return;

        do
        {
            ++m_rowCount;
        }
        while (m_query->next());
        insertRows(0, m_rowCount);
    }

    ~holdingsModel() { delete m_query; }

    QList<QString> fieldNames(const bool &removeActiveColumn = false)
    {
        QList<QString> names;
        QSqlRecord record = m_query->record();

        for (int i = 0; i < record.count(); ++i)
            names.append(record.fieldName(i));

        names.removeAt(names.indexOf("ID"));
        if (removeActiveColumn)
            names.removeAt(names.indexOf("Active"));

        return names;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (m_viewableColumns.at(index.column()) == queries::getPortfolioHoldings_Symbol)
                return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
        return QAbstractTableModel::flags(index);
    }

    int rowCount(const QModelIndex&) const
    {
        return m_rowCount;
    }

    int columnCount (const QModelIndex&) const
    {
        return m_viewableColumns.count();
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        int column = m_viewableColumns.at(index.column());

        if (role == Qt::DisplayRole)
        {
            m_query->seek(index.row());
            return m_query->value(column);
        }

        if (role == Qt::CheckStateRole && column == queries::getPortfolioHoldings_Symbol)
        {
            m_query->seek(index.row());
            return m_query->value(queries::getPortfolioHoldings_Active).toInt() == 1 ? Qt::Checked : Qt::Unchecked;
        }

        if (role == Qt::TextColorRole)
        {
            if (column == queries::getPortfolioHoldings_Shares)
                return qVariantFromValue(QColor(Qt::red));
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return QVariant();

        if (section >= m_viewableColumns.count())
            return QVariant();

        return m_query->record().fieldName(m_viewableColumns.at(section));
    }

    QList<int> selectedItems()
    {
        QList<int> items;

        QModelIndexList model = m_parent->selectionModel()->selectedRows();
        if (model.count() == 0)
            return items;

        foreach(const QModelIndex &q, model)
        {
            m_query->seek(q.row());
            items.append(m_query->value(queries::getPortfolioHoldings_ID).toInt());
        }

        return items;
    }

public slots:

private:
    QTableView *m_parent;
    QSqlQuery *m_query;
    QList<int> m_viewableColumns;
    int m_rowCount;
};
