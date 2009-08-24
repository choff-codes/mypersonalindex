#ifndef FRMPORTFOLIO_H
#define FRMPORTFOLIO_H

#include <QDialog>
#include <QtGui>
#include "frmPortfolio_UI.h"
#include "portfolioQueries.h"

class frmPortfolio : public QDialog
{
    Q_OBJECT

public:

    frmPortfolio(QWidget *parent = 0, const QDate &dataStartDate = QDate(), const int &portfolio = -1, const QString &name = "");

private:

    frmPortfolio_UI ui;
    portfolioQueries sql;
    int m_id;

    void loadPortfolioAttributes();

private slots:
    void accept();
};

#endif // FRMPORTFOLIO_H
