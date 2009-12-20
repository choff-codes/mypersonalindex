#include <QtGui>
#include "frmPortfolio.h"
#include "globals.h"

frmPortfolio::frmPortfolio(const globals::portfolio& p, const int &dataStartDate, const queries &sql, QWidget *parent): QDialog(parent), m_portfolio(p), m_portfolioOriginal(p), m_sql(sql)
{
    ui.setupUI(this);    
    this->setWindowTitle(QString("%1 Properties").arg(m_portfolio.description.isEmpty() ? "New Portfolio" : m_portfolio.description));
    if (dataStartDate != 0)
    {
        QDate d = QDate::fromJulianDay(dataStartDate);
        ui.dateStartDate->setMinimumDate(d);
        ui.dateStartDate->setDate(d);
    }

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));

    loadPortfolioAttributes();
    ui.txtDesc->setFocus();
    ui.txtDesc->selectAll();
}

void frmPortfolio::loadPortfolioAttributes()
{
    ui.txtDesc->setText(m_portfolio.description);
    ui.chkIncludeDiv->setChecked(m_portfolio.dividends);
    ui.txtStartValue->setText(QString::number(m_portfolio.startValue));
    ui.sbAAThreshold->setValue(m_portfolio.aaThreshold);
    ui.cmbAAThresholdValue->setCurrentIndex((int)m_portfolio.aaThresholdMethod);
    ui.cmbCostBasis->setCurrentIndex((int)m_portfolio.costCalc);
    if (m_portfolio.id != -1)
        ui.dateStartDate->setDate(QDate::fromJulianDay(m_portfolio.origStartDate));
}

bool frmPortfolio::getErrors()
{
    if (ui.txtDesc->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Set a name before saving!");
        return false;
    }

    if (ui.txtStartValue->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be number!");
        return false;
    }

    bool tmp; int i;
    i = ui.txtStartValue->text().toInt(&tmp);
    if (!tmp)
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be number!");
        return false;
    }

    if (i < 1 || i > 1000000)
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be between 1 and 1000000!");
        return false;
    }

    return true;
}

void frmPortfolio::accept()
{
    if (!getErrors())
        return;

    m_portfolio.description = ui.txtDesc->text();
    m_portfolio.dividends = ui.chkIncludeDiv->isChecked();
    m_portfolio.aaThreshold = ui.sbAAThreshold->value();
    m_portfolio.aaThresholdMethod = (globals::thesholdMethod)ui.cmbAAThresholdValue->currentIndex();
    m_portfolio.costCalc = (globals::avgShareCalc)ui.cmbCostBasis->currentIndex();
    m_portfolio.startValue = ui.txtStartValue->text().toInt();
    m_portfolio.origStartDate = ui.dateStartDate->date().toJulianDay();
    m_portfolio.startDate = m_portfolio.origStartDate;

    if (m_portfolio == m_portfolioOriginal)
    {
        QDialog::reject();
        return;
    }

    m_sql.executeNonQuery(queries::updatePortfolio(m_portfolio));
    if (m_portfolio.id == -1)
        m_portfolio.id = m_sql.getIdentity();
    QDialog::accept();
}
