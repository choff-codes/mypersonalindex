#include <QtGui>
#include "queries.h"
#include "globals.h"
#include "frmTicker.h"
#include "frmTrade.h"

frmTicker::frmTicker(QWidget *parent, queries *sql, QMap<int, globals::assetAllocation> *aa, QMap<int, globals::account> *acct, const globals::security &security):
        QDialog(parent), m_sql(sql), m_security(security)
{
    if(!m_sql || !m_sql->isOpen())
    {
        reject();
        return;
    }

    ui.setupUI(this);
    this->setWindowTitle(QString("%1 Properties").arg(security.id == -1 ? "New Ticker" : m_security.symbol));

    if (aa)
        foreach(const globals::assetAllocation &value, (*aa))
            ui.cmbAA->addItem(value.description, value.id);

    ui.cmbAcct->addItem("(None)", -1);
    if (acct)
        foreach(const globals::account &value, (*acct))
            ui.cmbAcct->addItem(value.description, value.id);
            //ui.cmbAcct->model()->sort(0);

    loadSecurity();
    connectSlots();

//    dateEditDelegate *dateEdit = new dateEditDelegate(ui.activity);
//    ui.activity->setItemDelegateForColumn(queries::trades_Date, dateEdit);
//    ui.activity->resizeRowsToContents();
//    ui.activity->openPersistentEditor(dataset->index(0, queries::trades_Date));
}

void frmTicker::connectSlots()
{
    connect(ui.btnHistorical, SIGNAL(toggled(bool)), ui.gpHistorical, SLOT(setVisible(bool)));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.btnTradesAdd, SIGNAL(clicked()), this, SLOT(addTrade()));
    connect(ui.btnExpenseClear, SIGNAL(clicked()), this, SLOT(resetExpense()));
}

void frmTicker::loadSecurity()
{
    ui.txtSymbol->setText(m_security.symbol);
    ui.cmbAcct->setCurrentIndex(ui.cmbAcct->findData(m_security.account));
    ui.sbExpense->setValue(m_security.expense);
    ui.chkReinvest->setChecked(m_security.divReinvest);
    ui.chkHide->setChecked(m_security.hide);
    ui.chkCash->setChecked(m_security.cashAccount);
    ui.chkInclude->setChecked(m_security.includeInCalc);
    ui.btnHistorical->setDisabled(m_security.id == -1);
}

void frmTicker::accept()
{
    QDialog::accept();
}

void frmTicker::addTrade()
{
    frmTrade f(this);
    if (f.exec())
    {

    }
}

void frmTicker::resetExpense()
{
    ui.sbExpense->setValue(-1);
}
