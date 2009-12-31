#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QtGui>
#include "frmPortfolio_UI.h"
#include "queries.h"
#include "portfolio.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    const portfolioInfo& getReturnValues() const { return m_portfolio; }

    frmPortfolio(const portfolioInfo& p, const int &dataStartDate, const queries &sql, QWidget *parent = 0);

private:

    frmPortfolio_UI ui;
    portfolioInfo m_portfolio;
    portfolioInfo m_portfolioOriginal;
    const queries &m_sql;

    void loadPortfolioAttributes();
    bool getErrors();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
