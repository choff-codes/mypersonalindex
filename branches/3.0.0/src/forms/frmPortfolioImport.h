#ifndef FRMPORTFOLIOIMPORT_H
#define FRMPORTFOLIOIMPORT_H

#include <QDialog>
#include <QMap>
#include "portfolio.h"
#include "fileStateIdentity.h"

class QTreeWidgetItem;
class frmPortfolioImport_UI;
class frmPortfolioImport : public QDialog
{
    Q_OBJECT

public:
    frmPortfolioImport(const portfolio &portfolio_, const QMap<int, portfolio> portfolios_, const fileStateIdentity &identities_, QWidget *parent_ = 0);
    ~frmPortfolioImport();

    portfolio getPortfolio() { return m_portfolio; }

private slots:
    void indexChange(int index_);
    void accept();

private:
    frmPortfolioImport_UI *ui;
    portfolio m_portfolio;
    QMap<int, portfolio> m_portfolios;
    fileStateIdentity m_identities;

    QTreeWidgetItem* createTreeItem(int type_, const QString &description_);
};

#endif // FRMPORTFOLIOIMPORT_H
