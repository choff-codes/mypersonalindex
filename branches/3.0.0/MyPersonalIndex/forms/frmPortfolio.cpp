#include <QtGui>
#include "frmPortfolio.h"

frmPortfolio::frmPortfolio(const portfolioAttributes& p, const int &dataStartDate, QWidget *parent): QDialog(parent), m_portfolio(p), m_portfolioOriginal(p)
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
    ui.cmbCostBasis->setCurrentIndex(ui.cmbCostBasis->findData(m_portfolio.costBasis));
    ui.dateStartDate->setDate(QDate::fromJulianDay(m_portfolio.startDate));
}

bool frmPortfolio::hasValidationErrors()
{
    if (ui.txtDesc->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Set a name before saving!");
        return true;
    }

    if (ui.txtStartValue->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be number!");
        return true;
    }

    bool tmp; int i;
    i = ui.txtStartValue->text().toInt(&tmp);
    if (!tmp)
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be number!");
        return true;
    }

    if (i < 1 || i > 1000000)
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be between 1 and 1000000!");
        return true;
    }

    return false;
}

void frmPortfolio::accept()
{
    if (hasValidationErrors())
        return;

    m_portfolio.description = ui.txtDesc->text();
    m_portfolio.dividends = ui.chkIncludeDiv->isChecked();
    m_portfolio.aaThreshold = ui.sbAAThreshold->value();
    m_portfolio.aaThresholdMethod = (portfolioAttributes::thesholdMethod)ui.cmbAAThresholdValue->currentIndex();
    m_portfolio.costBasis = (account::costBasisType)ui.cmbCostBasis->itemData(ui.cmbCostBasis->currentIndex(), Qt::UserRole).toInt(); // ignore none
    m_portfolio.startValue = ui.txtStartValue->text().toInt();
    m_portfolio.startDate = ui.dateStartDate->date().toJulianDay();

    if (m_portfolio == m_portfolioOriginal)
    {
        QDialog::reject();
        return;
    }

    m_portfolio.save();
    QDialog::accept();
}
