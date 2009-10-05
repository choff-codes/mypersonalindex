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

    // ui.activity->openPersistentEditor(dataset->index(0, queries::trades_Date));
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
    connect(ui.btnTradesAdd, SIGNAL(clicked()), this, SLOT(addTrade()));
    connect(ui.btnTradesEdit, SIGNAL(clicked()), this, SLOT(editTrade()));
    connect(ui.btnTradesDelete, SIGNAL(clicked()), this, SLOT(deleteTrades()));
    connect(ui.trades, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editTrade()));
    connect(ui.btnExpenseClear, SIGNAL(clicked()), this, SLOT(resetExpense()));
    connect(ui.btnAAAdd, SIGNAL(clicked()), this, SLOT(addAA()));
    connect(ui.btnAADelete, SIGNAL(clicked()), this, SLOT(deleteAA()));
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

    m_modelAA = new tickerAAModel(0 , 2, this);
    foreach(const globals::intdoublePair &value, m_security.aa)
        updateAAList(value);

    installAAModel();

    m_modelTrade  = new modelWithNoEdit(0, 9, this);
    foreach(const globals::dynamicTrade &trade, m_security.trades)
        updateTradeList(trade);

    installTradeModel();
}

void frmTicker::aaListChange(QStandardItem* item)
{
    if (item && item->column() != 1) // wait until percentage column changes
        return;

    if (item) // delete passes null pointer
    {
        QString value = item->data(Qt::DisplayRole).toString();
        value.chop(1);
        m_security.aa[item->row()].value = value.toDouble();
    }

    ui.gpAA->setTitle(QString("Asset Allocation (%L1%)").arg(aaListTotal(), 0, 'f', 2));
}

double frmTicker::aaListTotal()
{
    double total = 0;

    foreach(const globals::intdoublePair &pair, m_security.aa)
        total += pair.value;

    return total;
}

void frmTicker::addAA()
{
    if (ui.cmbAA->currentIndex() == -1)
        return;

    double total = aaListTotal();
    globals::intdoublePair newRow(ui.cmbAA->itemData(ui.cmbAA->currentIndex()).toInt(), total >= 100 ? 0 : 100 - total);
    m_security.aa.append(newRow);
    updateAAList(newRow);
}

void frmTicker::deleteAA()
{
    QList<int> indexes = functions::getSelectedRows(ui.aa->selectionModel()->selectedRows());
    if(indexes.count() == 0)
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        m_modelAA->removeRow(indexes.at(i));
        m_security.aa.removeAt(indexes.at(i));
    }
    aaListChange(0);
}

void frmTicker::updateAAList(const globals::intdoublePair &aa, const int &row)
{
     int i = row == -1 ? m_modelAA->rowCount() : row; // -1 is an insert

    if (!m_data->aa.contains(aa.key))
        return;

    QStandardItem *desc = new QStandardItem(m_data->aa.value(aa.key).description);
    QStandardItem *target = new QStandardItem(QLocale().toString(aa.value, 'f', 2).append("%"));
    m_modelAA->setItem(i, 0, desc);
    m_modelAA->setItem(i, 1, target);
}

void frmTicker::updateTradeList(const globals::dynamicTrade &trade, const int &row)
{
     int i = row == -1 ? m_modelTrade->rowCount() : row; // -1 is an insert

    QStandardItem *type = new QStandardItem(trade.tradeTypeToString());
    QStandardItem *value = new QStandardItem(trade.valueToString());
    QStandardItem *price = new QStandardItem(trade.price < 0 ? "Prev Close" : functions::doubleToCurrency(trade.price));
    QStandardItem *commission = new QStandardItem(trade.commission < 0 ? "" : functions::doubleToCurrency(trade.commission));
    QStandardItem *cashaccount = new QStandardItem(m_data->tickers.contains(trade.cashAccount) ? m_data->tickers.value(trade.cashAccount).symbol : "");
    QStandardItem *frequency = new QStandardItem(trade.frequencyToString());
    QStandardItem *date = new QStandardItem(trade.dateToString());
    QStandardItem *startdate = new QStandardItem(trade.startDate.isValid() ? trade.startDate.toString(Qt::SystemLocaleShortDate) : "");
    QStandardItem *enddate = new QStandardItem(trade.endDate.isValid() ? trade.endDate.toString(Qt::SystemLocaleShortDate) : "");
    m_modelTrade->setItem(i, 0, type);
    m_modelTrade->setItem(i, 1, value);
    m_modelTrade->setItem(i, 2, price);
    m_modelTrade->setItem(i, 3, commission);
    m_modelTrade->setItem(i, 4, cashaccount);
    m_modelTrade->setItem(i, 5, frequency);
    m_modelTrade->setItem(i, 6, date);
    m_modelTrade->setItem(i, 7, startdate);
    m_modelTrade->setItem(i, 8, enddate);
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
    connect(m_modelAA, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(aaListChange(QStandardItem*)));
    aaListChange(0);
}

void frmTicker::installTradeModel()
{
    m_modelTrade->setHeaderData(0, Qt::Horizontal, "Type");
    m_modelTrade->setHeaderData(1, Qt::Horizontal, "Value");
    m_modelTrade->setHeaderData(2, Qt::Horizontal, "Price");
    m_modelTrade->setHeaderData(3, Qt::Horizontal, "Comm.");
    m_modelTrade->setHeaderData(4, Qt::Horizontal, "$ Acct");
    m_modelTrade->setHeaderData(5, Qt::Horizontal, "Freq.");
    m_modelTrade->setHeaderData(6, Qt::Horizontal, "Date");
    m_modelTrade->setHeaderData(7, Qt::Horizontal, "Start");
    m_modelTrade->setHeaderData(8, Qt::Horizontal, "End");
    ui.trades->setModel(m_modelTrade);
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

    if (m_security == m_securityOriginal)
    {
        QDialog::reject();
        return;
    }

    m_sql->executeNonQuery(m_sql->updateSecurity(m_portfolioID, m_security));
    if (m_security.id == -1)
        m_security.id = m_sql->executeScalar(m_sql->getIdentity()).toInt();

    for(int i = 0; i < m_security.trades.count(); ++i)
    {
        m_sql->executeNonQuery(m_sql->updateSecurityTrade(m_security.id, m_security.trades.at(i)));
        if (m_security.trades.at(i).id == -1)
            m_security.trades[i].id = m_sql->executeScalar(m_sql->getIdentity()).toInt();
    }

    QVariantList tickerID, aaID, percent;
    foreach(const globals::intdoublePair &pair, m_security.aa)
    {
        tickerID.append(m_security.id);
        aaID.append(pair.key);
        percent.append(pair.value);
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

void frmTicker::editTrade()
{
    QModelIndexList il = ui.trades->selectionModel()->selectedRows();
    if(il.count() == 0)
        return;

    foreach(const QModelIndex &q, il)
    {
        int i = q.row();
        frmTrade f(this, &m_data->tickers, m_security.id, m_security.trades.value(i));
        if (f.exec())
        {
            updateTradeList(f.getReturnValues(), i);
            m_security.trades[i] = f.getReturnValues();
        }
    }
}

void frmTicker::addTrade()
{
    frmTrade f(this, &m_data->tickers, m_security.id);
    if (f.exec())
    {
        updateTradeList(f.getReturnValues());
        m_security.trades.append(f.getReturnValues());
    }
}

void frmTicker::deleteTrades()
{
    QList<int> indexes = functions::getSelectedRows(ui.trades->selectionModel()->selectedRows());
    if(indexes.count() == 0)
        return;

    for(int i = indexes.count() - 1; i >= 0; --i)
    {
        m_modelTrade->removeRow(indexes.at(i));
        m_security.trades.removeAt(indexes.at(i));
    }
}

void frmTicker::resetExpense()
{
    ui.sbExpense->setValue(-1);
}
