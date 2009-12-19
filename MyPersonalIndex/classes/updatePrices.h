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
    updatePrices(const QMap<int, globals::myPersonalIndex*> &data, QList<int> &dates, const globals::settings &settings, QObject *parent = 0):
        QThread(parent), m_sql(new queries("update")), m_data(data), m_dates(dates), m_downloadSplits(settings.splits), m_dataStartDate(settings.dataStartDate) { }

    ~updatePrices() { delete m_sql; }
    static bool isInternetConnection();

signals:
    void updateFinished(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    const QMap<int, globals::myPersonalIndex*> &m_data;
    QList<int> &m_dates;
    bool m_downloadSplits;
    int m_dataStartDate;
    QStringList m_updateFailures;
    QVariantList m_pricesDate, m_pricesTicker, m_pricesPrice;
    QVariantList m_divDate, m_divTicker, m_divAmount;
    QVariantList m_splitDate, m_splitTicker, m_splitRatio;
    NAV *m_nav;

    void run();
    QString getCSVAddress(const QString &ticker, const QDate &begin, const QDate &end, const QString &type);
    QString getSplitAddress(const QString &ticker);
    QList<QByteArray>* downloadFile(const QUrl&);
    void getUpdateInfo(QMap<QString, globals::updateInfo> &tickers);
    bool getPrices(const QString &ticker, const int &minDate, int &earliestUpdate);
    void getDividends(const QString&ticker, const int &minDate, int &earliestUpdate);
    void getSplits(const QString &ticker, const int &minDate, int &earliestUpdate);
    void insertUpdates();

private slots:
    void calcuationFinished();
};

#endif // UPDATEPRICES_H
