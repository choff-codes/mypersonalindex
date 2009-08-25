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
    this->setWindowTitle(QString("%1 Properties").arg(name.isEmpty() ? "New Portfolio" : name));
    ui.dateStartDate->setMinimumDate(dataStartDate);

    if (portfolio != -1)
        loadPortfolioAttributes();

    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));

    ui.txtDesc->setFocus();
    ui.txtDesc->selectAll();
}

void frmPortfolio::loadPortfolioAttributes()
{
    QSqlQuery *q = sql.executeResultSet(sql.getPortfolioAttributes(m_id));
    if (!q)
        return;

    // set fields and m_returnValues to keep a record of original values
    ui.txtDesc->setText(m_returnValues.name = q->value(portfolioQueries::getPortfolioAttributes_Name).toString());
    ui.chkIncludeDiv->setChecked(m_returnValues.dividends = q->value(portfolioQueries::getPortfolioAttributes_Dividends).toBool());
    ui.txtStartValue->setText(QString::number(m_returnValues.navStart = q->value(portfolioQueries::getPortfolioAttributes_NAVStartValue).toDouble()));
    ui.sbAAThreshold->setValue(m_returnValues.aaThreshold = q->value(portfolioQueries::getPortfolioAttributes_AAThreshold).toInt());
    ui.cmbCostBasis->setCurrentIndex(m_returnValues.costCalc = (globals::avgShareCalc)q->value(portfolioQueries::getPortfolioAttributes_CostCalc).toInt());
    ui.dateStartDate->setDate(m_returnValues.startDate = m_returnValues.origStartDate = QDate::fromJulianDay(q->value(portfolioQueries::getPortfolioAttributes_StartDate).toInt()));

    delete q;
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

    bool tmp; double d;
    d = ui.txtStartValue->text().toDouble(&tmp);
    if (!tmp)
    {
        QMessageBox::critical(this, "Error", "Index Start Value must be number!");
        return false;
    }

    if (d < 1 || d > 1000000)
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

    if (m_id == -1)
    {
        sql.executeNonQuery(sql.insertPortfolio(ui.txtDesc->text(), ui.chkIncludeDiv->isChecked(), ui.txtStartValue->text().toDouble(),
            ui.cmbCostBasis->currentIndex(), ui.sbAAThreshold->value(), ui.dateStartDate->date()));
        m_id = sql.executeScalar(sql.getIdentity(), -1).toInt();
    }
    else
        sql.executeNonQuery(sql.updatePortfolio(m_id, ui.txtDesc->text(), ui.chkIncludeDiv->isChecked(), ui.txtStartValue->text().toDouble(),
            ui.cmbCostBasis->currentIndex(), ui.sbAAThreshold->value(), ui.dateStartDate->date()));

    m_returnValues.id = m_id;
    m_returnValues.name = ui.txtDesc->text();
    m_returnValues.dividends = ui.chkIncludeDiv->isChecked();
    m_returnValues.aaThreshold = ui.sbAAThreshold->value();
    m_returnValues.costCalc = (globals::avgShareCalc)ui.cmbCostBasis->currentIndex();
    m_returnValues.navStart = ui.txtStartValue->text().toDouble();
    m_returnValues.startDate = ui.dateStartDate->date();

    QDialog::accept();
}
