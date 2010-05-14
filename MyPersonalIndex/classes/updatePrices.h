#ifndef UPDATEPRICES_H
#define UPDATEPRICES_H

#include <QMap>
#include <QString>
#include <QDate>
#include <QtNetwork>
#include "updateInfo.h"
#include "queries.h"
#include "portfolio.h"
#include "settings.h"

struct updatePricesReturnValue
{
    QStringList updateFailures;
    int earliestUpdate;
};

class updatePrices
{
public:
    updatePrices(const settings &settings): m_downloadSplits(settings.splits), m_dataStartDate(settings.dataStartDate - 6 /* need a couple days before */) { }

    static bool isInternetConnection();
    updatePricesReturnValue run();

private:
    bool m_downloadSplits;
    int m_dataStartDate;
    QVariantList m_pricesDate, m_pricesSymbol, m_pricesPrice;
    QVariantList m_divDate, m_divSymbol, m_divAmount;
    QVariantList m_splitDate, m_splitSymbol, m_splitRatio;

    static const char stockPrices = 'd';
    static const char stockDividends = 'v';

    QMap<QString, updateInfo> getUpdateInfo() const;
    bool getPrices(const QString &symbol, const int &minDate, int *earliestUpdate);
    void getDividends(const QString &symbol, const int &minDate, int *earliestUpdate);
    void getSplits(const QString &symbol, const int &minDate, int *earliestUpdate);
    void updateMissingPrices();
    void insertUpdates();

    QList<QByteArray> downloadFile(const QUrl &url, const bool &splitOnLineBreak = true);
    static QString getCSVAddress(const QString &symbol, const QDate &begin, const QDate &end, const QString &type);
    static QString getSplitAddress(const QString &symbol);
};

#endif // UPDATEPRICES_H
