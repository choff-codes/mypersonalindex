#include <QDialog>
#include <QtGui>
#include "frmPortfolio.h"
#include "frmPortfolio_UI.h"

frmPortfolio::frmPortfolio(QWidget *parent, const QDate &dataStartDate, const int &portfolio, const QString &name): QDialog(parent), m_id(portfolio)
{
    if (!sql.isOpen())
    {
        this->reject();
        return;
    }

    ui.setupUI(this);
    ui.sbAAThreshold->setValue(5);
    ui.dateStartDate->setDate(QDate::currentDate());
    ui.txtDesc->setFocus();
    this->setWindowTitle(QString("%1 Properties").arg(name.isEmpty() ? "New Portfolio" : name));
    ui.dateStartDate->setMinimumDate(dataStartDate);

    if (portfolio != -1)
        loadPortfolioAttributes();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
}

void frmPortfolio::loadPortfolioAttributes()
{
    QSqlQuery *q = sql.executeResultSet(sql.getPortfolioAttributes(m_id));
    if (!q)
        return;

    ui.txtDesc->setText(q->value(portfolioQueries::getPortfolioAttributes_Name).toString());
    ui.chkIncludeDiv->setChecked(q->value(portfolioQueries::getPortfolioAttributes_Dividends).toBool());
    ui.txtStartValue->setText(QString::number(q->value(portfolioQueries::getPortfolioAttributes_NAVStartValue).toDouble()));
    ui.sbAAThreshold->setValue(q->value(portfolioQueries::getPortfolioAttributes_AAThreshold).toInt());
    ui.cmbCostBasis->setCurrentIndex(q->value(portfolioQueries::getPortfolioAttributes_CostCalc).toInt());
    ui.dateStartDate->setDate(QDate::fromJulianDay(q->value(portfolioQueries::getPortfolioAttributes_StartDate).toInt()));

    delete q;
}

void frmPortfolio::accept()
{
    if (m_id == -1)
    {
        sql.executeNonQuery(sql.insertPortfolio(ui.txtDesc->text(), ui.chkIncludeDiv->isChecked(), ui.txtStartValue->text().toDouble(),
                                                ui.cmbCostBasis->currentIndex(), ui.sbAAThreshold->value(), ui.dateStartDate->date()));
        m_id = sql.executeScalar(sql.getIdentity(), -1).toInt();
    }
    else
        sql.executeNonQuery(sql.updatePortfolio(m_id, ui.txtDesc->text(), ui.chkIncludeDiv->isChecked(), ui.txtStartValue->text().toDouble(),
                                                ui.cmbCostBasis->currentIndex(), ui.sbAAThreshold->value(), ui.dateStartDate->date()));

    QDialog::accept();
}
