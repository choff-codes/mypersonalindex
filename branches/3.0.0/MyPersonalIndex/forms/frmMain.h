#ifndef FRMMAIN_H
#define FRMMAIN_H

#define VERSION 300 // UPDATE EACH RELEASE
#define VERSIONTEXT "3.0.0" // UPDATE EACH RELEASE

#include <QtGui>
#include "frmMain_UI.h"
#include "queries.h"
#include "globals.h"
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

private slots:
    void dateChanged(QDate);
    void addPortfolio();
    void editPortfolio();
    void deletePortfolio();
    void loadPortfolio();
    void loadPortfolioHoldings();
    void about();
    void addTicker();
    void options();
    void aa();
    void acct();
    void stat();
    void beginUpdate();
    void finishUpdate(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FRMMAIN_H


class holdingsModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    holdingsModel(QSqlQuery *query, QObject *parent = 0): QSqlQueryModel(parent), m_query(query)
    {
        setQuery(*m_query);
    }

    ~holdingsModel() { delete m_query; }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (index.column() == queries::getPortfolioHoldings_Symbol)
                return QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable;

        return QSqlQueryModel::flags(index);
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            //if (index.column() == 1)
            //    return functions::doubleToPercentage(m_list.at(index.row()).second);

            return QSqlQueryModel::data(index, role);
        }

        if (role == Qt::CheckStateRole && index.column() == queries::getPortfolioHoldings_Symbol)
        {
            m_query->seek(index.row());
            return m_query->value(queries::getPortfolioHoldings_Active).toInt() == 1 ? Qt::Checked : Qt::Unchecked;
        }

        if (role == Qt::TextColorRole)
        {
            if (index.column() == queries::getPortfolioHoldings_Shares)
                return qVariantFromValue(QColor(Qt::red));
        }

        return QVariant();
    }

public slots:

signals:

private:
    QSqlQuery *m_query;
};
