#include <QtGui>
#include "queries.h"
#include "globals.h"
#include "frmTicker.h"
#include "frmTrade.h"
#include "viewDelegates.h"

frmTicker::frmTicker(QWidget *parent, queries *sql, const int &portfolioID, const globals::portfolioData *data, const globals::security &security):
        QDialog(parent), m_sql(sql), m_portfolioID(portfolioID), m_data(data), m_security(security), m_securityOriginal(security)
{
    if(!m_sql || !m_sql->isOpen() || !data)
    {
        reject();
        return;
    }

    ui.setupUI(this);
    this->setWindowTitle(QString("%1 Properties").arg(security.id == -1 ? "New Ticker" : m_security.symbol));

    //ui.cmbAcct->model()->sort(0);

    loadDropDowns();
    loadSecurity();
    connectSlots();
}

void frmTicker::loadDropDowns()
{
    foreach(const globals::assetAllocation &value, m_data->aa)
        ui.cmbAA->addItem(value.description, value.id);

    ui.cmbAcct->addItem("(None)", -1);
    foreach(const globals::account &value, m_data->acct)
        ui.cmbAcct->addItem(value.description, value.id);
}

void frmTicker::connectSlots()
{
    connect(ui.btnHistorical, SIGNAL(toggled(bool)), ui.gpHistorical, SLOT(setVisible(bool)));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.btnTradesAdd, SIGNAL(clicked()), m_modelTrade, SLOT(addNew()));
    connect(ui.btnTradesEdit, SIGNAL(clicked()), m_modelTrade, SLOT(editSelected()));
    connect(ui.trades, SIGNAL(doubleClicked(QModelIndex)), m_modelTrade, SLOT(editSelected()));
    connect(ui.btnTradesDelete, SIGNAL(clicked()), m_modelTrade, SLOT(deleteSelected()));
    connect(ui.tradesCopy, SIGNAL(triggered()), m_modelTrade, SLOT(copy()));
    connect(ui.tradesCopyShortcut, SIGNAL(activated()), m_modelTrade, SLOT(copy()));
    connect(ui.tradesPaste, SIGNAL(triggered()), m_modelTrade, SLOT(paste()));
    connect(ui.tradesPasteShortcut, SIGNAL(activated()), m_modelTrade, SLOT(paste()));
    connect(ui.trades, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui.btnExpenseClear, SIGNAL(clicked()), this, SLOT(resetExpense()));
    connect(ui.btnAAAdd, SIGNAL(clicked()), this, SLOT(addAA()));
    connect(ui.btnAADelete, SIGNAL(clicked()), m_modelAA, SLOT(deleteSelected()));
    connect(m_modelAA, SIGNAL(updateHeader()), this, SLOT(updateAAPercentage()));
    connect(m_modelTrade, SIGNAL(saveItem(globals::dynamicTrade*)), this, SLOT(saveItem(globals::dynamicTrade*)));
    connect(m_modelTrade, SIGNAL(deleteItem(globals::dynamicTrade)), this, SLOT(deleteItem(globals::dynamicTrade)));
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

    m_modelAA = new tickerAAModel(m_security.aa, &m_data->aa, 2, ui.aa);
    installAAModel();

    m_modelTrade = new tickerTradeModel(m_security.trades.values(), &m_data->tickers, 9, ui.trades, this);
    ui.trades->setModel(m_modelTrade);
}

void frmTicker::updateAAPercentage()
{
    ui.gpAA->setTitle(QString("Asset Allocation (%L1%)").arg(m_modelAA->totalPercentage(), 0, 'f', 2));
}


void frmTicker::addAA()
{
    if (ui.cmbAA->currentIndex() == -1)
        return;

    m_modelAA->addNew(ui.cmbAA->itemData(ui.cmbAA->currentIndex()).toInt());
}

void frmTicker::installAAModel()
{
    QDoubleSpinBox sbTmp(this);
    sbTmp.setSuffix("%");
    sbTmp.setMaximum(100);
    sbTmp.setDecimals(2);
    ui.aa->verticalHeader()->setDefaultSectionSize(sbTmp.sizeHint().height());

    ui.aa->setModel(m_modelAA);
    spinBoxDelegate *s = new spinBoxDelegate(0, 100, this);
    ui.aa->setItemDelegateForColumn(1, s);

    ui.aa->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui.aa->setColumnWidth(1, sbTmp.sizeHint().width());
}

void frmTicker::accept()
{
    m_security.symbol = ui.txtSymbol->text();
    m_security.account = ui.cmbAcct->itemData(ui.cmbAcct->currentIndex()).toInt();
    m_security.expense = ui.sbExpense->value();
    m_security.divReinvest = ui.chkReinvest->isChecked();
    m_security.cashAccount = ui.chkCash->isChecked();
    m_security.includeInCalc = ui.chkInclude->isChecked();
    m_security.hide = ui.chkHide->isChecked();
    m_security.trades = m_modelTrade->saveList(m_securityOriginal.trades);
    m_security.aa = m_modelAA->getList();

    if (m_security == m_securityOriginal)
    {
        QDialog::reject();
        return;
    }

    m_sql->executeNonQuery(m_sql->updateSecurity(m_portfolioID, m_security));
    if (m_security.id == -1)
        m_security.id = m_sql->executeScalar(m_sql->getIdentity()).toInt();

    if(m_security.aa == m_securityOriginal.aa)
    {
        QDialog::accept();
        return;
    }

    QVariantList tickerID, aaID, percent;

    QList<QPair<int, double> >::const_iterator i;
    for (i = m_security.aa.constBegin(); i != m_security.aa.constEnd(); ++i)
    {
        tickerID.append(m_security.id);
        aaID.append((*i).first);
        percent.append((*i).second);
    }

    QMap<QString, QVariantList> tableValues;
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_TickerID), tickerID);
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_AAID), aaID);
    tableValues.insert(queries::tickersAAColumns.at(queries::tickersAAColumns_Percent), percent);

    m_sql->deleteTickerItems(queries::table_TickersAA, m_security.id);
    if (tickerID.count() != 0)
        m_sql->executeTableUpdate(queries::table_TickersAA, tableValues);

    QDialog::accept();
}

void frmTicker::resetExpense()
{
    ui.sbExpense->setValue(-1);
}

void frmTicker::saveItem(globals::dynamicTrade *trade)
{
    m_sql->executeNonQuery(m_sql->updateSecurityTrade(m_security.id, (*trade)));
    if (trade->id == -1)
        trade->id = m_sql->executeScalar(m_sql->getIdentity()).toInt();
}

void frmTicker::deleteItem(const globals::dynamicTrade &trade)
{
    m_sql->executeNonQuery(m_sql->deleteItem(queries::table_TickersTrades, trade.id));
}

void frmTicker::customContextMenuRequested(const QPoint&)
{
    ui.tradesPopup->popup(QCursor::pos());
}
