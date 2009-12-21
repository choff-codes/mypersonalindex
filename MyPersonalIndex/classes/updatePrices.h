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
    updatePrices(const QMap<int, globals::myPersonalIndex*> &data, const globals::settings &settings, QObject *parent = 0):
        QThread(parent), m_sql(new queries("update")), m_data(data), m_downloadSplits(settings.splits), m_dataStartDate(settings.dataStartDate - 6 /* need a couple days before */) { }

    ~updatePrices() { delete m_sql; QSqlDatabase::removeDatabase("update"); }
    static bool isInternetConnection();

signals:
    void updateFinished(const QStringList &invalidTickers);
    void statusUpdate(const QString &message);

private:
    queries *m_sql;
    const QMap<int, globals::myPersonalIndex*> &m_data;
    bool m_downloadSplits;
    int m_dataStartDate;
    QStringList m_updateFailures;
    QVariantList m_pricesDate, m_pricesTicker, m_pricesPrice;
    QVariantList m_divDate, m_divTicker, m_divAmount;
    QVariantList m_splitDate, m_splitTicker, m_splitRatio;
    nav *m_nav;

    struct updateInfo
    {
        QString ticker;
        int lastPrice;
        int lastDividend;
        int lastSplit;

        updateInfo() {}
        updateInfo(const QString &p_ticker, const int &minDate): ticker(p_ticker), lastPrice(minDate), lastDividend(minDate), lastSplit(minDate) {}
    };

    void run();
    QString getCSVAddress(const QString &ticker, const QDate &begin, const QDate &end, const QString &type);
    QString getSplitAddress(const QString &ticker);
    QList<QByteArray>* downloadFile(const QUrl&);
    void getUpdateInfo(QMap<QString, updateInfo> &tickers);
    bool getPrices(const QString &ticker, const int &minDate, int &earliestUpdate);
    void getDividends(const QString&ticker, const int &minDate, int &earliestUpdate);
    void getSplits(const QString &ticker, const int &minDate, int &earliestUpdate);
    void updateMissingPrices();
    void insertUpdatesToObject();
    void insertUpdates();

private slots:
    void calcuationFinished();
};

#endif // UPDATEPRICES_H
