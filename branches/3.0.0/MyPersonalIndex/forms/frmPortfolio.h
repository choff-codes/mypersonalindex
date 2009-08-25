#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QDialog>
#include <QtGui>
#include "frmPortfolio_UI.h"
#include "portfolioQueries.h"
#include "globals.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    struct portfolioReturn
    {
        int id;
        QString name;
        bool dividends;
        double navStart;
        int aaThreshold;
        QDate startDate;
        QDate origStartDate;
        globals::avgShareCalc costCalc;
    };

    const portfolioReturn& getReturnValues() const { return m_returnValues; }

    frmPortfolio(QWidget *parent = 0, const QDate &dataStartDate = QDate(), const int &portfolio = -1, const QString &name = "");

private:

    frmPortfolio_UI ui;
    portfolioQueries sql;
    portfolioReturn m_returnValues;
    int m_id;

    void loadPortfolioAttributes();
    bool getErrors();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
