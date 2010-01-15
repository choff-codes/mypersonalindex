#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include "updateInfo.h"
#include "queries.h"
#include "nav.h"
#include "portfolio.h"
#include "settings.h"
#include <QtNetwork>

class updatePrices: public QThread
{
    Q_OBJECT

public:
    updatePrices(const QMap<int, portfolio*> &data, const settings &settings, QObject *parent = 0):
        QThread(parent), m_data(data), m_downloadSplits(settings.splits), m_dataStartDate(settings.dataStartDate - 6 /* need a couple days before */) { }

    static bool isInternetConnection();

signals:
    void updateFinished(const QStringList &invalidSecurities);
    void statusUpdate(const QString &message);

private:
    const QMap<int, portfolio*> &m_data;
    bool m_downloadSplits;
    int m_dataStartDate;
    QStringList m_updateFailures;
    QVariantList m_pricesDate, m_pricesSymbol, m_pricesPrice;
    QVariantList m_divDate, m_divSymbol, m_divAmount;
    QVariantList m_splitDate, m_splitSymbol, m_splitRatio;
    nav *m_nav;

    static const char stockPrices = 'd';
    static const char stockDividends = 'v';

    void run();
    QString getCSVAddress(const QString &symbol, const QDate &begin, const QDate &end, const QString &type);
    QString getSplitAddress(const QString &symbol);
    QList<QByteArray>* downloadFile(const QUrl&);
    void getUpdateInfo(QMap<QString, updateInfo> &securities);
    bool getPrices(const QString &symbol, const int &minDate, int &earliestUpdate);
    void getDividends(const QString&symbol, const int &minDate, int &earliestUpdate);
    void getSplits(const QString &symbol, const int &minDate, int &earliestUpdate);
    void updateMissingPrices();
    void insertUpdatesToObject();
    void insertUpdates();

private slots:
    void calcuationFinished();
};

#endif // UPDATEPRICES_H
