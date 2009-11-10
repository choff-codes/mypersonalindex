#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include "globals.h"
#include "queries.h"
#include "NAV.h"
#include <QtNetwork>

class updatePrices: public QThread
{
    Q_OBJECT

public:
    updatePrices(QMap<int, globals::myPersonalIndex*> *data, QList<int> *dates, const bool &splits, const int &firstDate, QObject *parent = 0):
        QThread(parent), m_data(data), m_dates(dates), m_splits(splits), m_firstDate(firstDate)
    {
        m_firstDate = m_firstDate - 6;
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "update");
        db.setDatabaseName(queries::getDatabaseLocation());
        m_sql = new queries(db);
    }

    ~updatePrices() { delete m_sql; }
    static bool isInternetConnection();

signals:
    void updateFinished(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    QMap<int, globals::myPersonalIndex*> *m_data;
    QList<int> *m_dates;
    bool m_splits;
    int m_firstDate;
    QStringList m_updateFailures;
    NAV *m_nav;

    QString getCSVAddress(const QString &ticker, const QDate &begin, const QDate &end, const QString &type);
    QString getSplitAddress(const QString &ticker);
    QList<QByteArray>* downloadFile(const QUrl&);
    void getUpdateInfo(QMap<QString, globals::updateInfo> *tickers);

    void run();
    bool getPrices(const QString &ticker, const int &minDate, int &earliestUpdate);
    void getDividends(const QString&ticker, const int &minDate, int &earliestUpdate);
    void getSplits(const QString &ticker, const int &minDate, int &earliestUpdate);

private slots:
    void calcuationFinished();
};

#endif // UPDATEPRICES_H
