#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QtGui>
#include "frmPortfolio_UI.h"
#include "portfolioQueries.h"
#include "globals.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    const globals::portfolio& getReturnValues() const { return m_portfolio; }

    frmPortfolio(QWidget *parent = 0, const QDate &dataStartDate = QDate(), const globals::portfolio& p = globals::portfolio());

private:

    frmPortfolio_UI ui;
    portfolioQueries sql;
    globals::portfolio m_portfolio;

    void loadPortfolioAttributes();
    bool getErrors();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
