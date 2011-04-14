#ifndef FRMPRICEIMPORT_H
#define FRMPRICEIMPORT_H

#include <QDialog>
#include "historicalPrices.h"

class QFile;
class frmPriceImport_UI;
class frmPriceImport : public QDialog
{
    Q_OBJECT

public:
    frmPriceImport(const historicalPricesMap &pricesMap_, QWidget *parent_ = 0);
    ~frmPriceImport();

    historicalPricesMap getHistoricalPricesMap() { return m_pricesMap; }

private slots:
    void accept();
    void moveDown();
    void moveUp();
    void browse();

private:
    frmPriceImport_UI *ui;
    historicalPricesMap m_pricesMap;

    bool mergePrices(const historicalPricesMap &newPriceMap_, bool dryRun_);
    bool duplicatePriceCheck(const QString &symbol_, historicalPrices::type type_, const QMap<int, double> &values_, bool &askAgain);
    bool validateFilePath();
    bool validateRow(int row_, const QStringList line_, const QHash<int, int> &columns_, const QString &dateFormat_);
    QString selectedDelimiter();
    QString selectedDateFormat();
    QHash<int, int> selectedColumnOrder();
};

#endif // FRMPRICEIMPORT_H
