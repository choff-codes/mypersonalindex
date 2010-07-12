#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QObject>
#include "frmPortfolio_UI.h"
#include "portfolio.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    const portfolioAttributes& getReturnValues() const { return m_portfolio; }

    frmPortfolio(const portfolioAttributes& p, const int &dataStartDate, QWidget *parent = 0);

private:

    frmPortfolio_UI ui;
    portfolioAttributes m_portfolio;
    portfolioAttributes m_portfolioOriginal;

    void loadPortfolioAttributes();
    bool hasValidationErrors();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
