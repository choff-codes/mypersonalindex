#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QtGui>
#include "frmPortfolio_UI.h"
#include "queries.h"
#include "globals.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    const globals::portfolio& getReturnValues() const { return m_portfolio; }

    frmPortfolio(const globals::portfolio& p, const int &dataStartDate, const queries &sql, QWidget *parent = 0);

private:

    frmPortfolio_UI ui;
    globals::portfolio m_portfolio;
    globals::portfolio m_portfolioOriginal;
    const queries &m_sql;

    void loadPortfolioAttributes();
    bool getErrors();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
