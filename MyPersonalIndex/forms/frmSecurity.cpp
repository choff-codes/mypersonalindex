#include "frmSecurity.h"
#include "mpiViewDelegates.h"

frmSecurity::frmSecurity(const int &portfolioID, const portfolioData &data, const security& security, QWidget *parent):
        QDialog(parent), m_portfolioID(portfolioID),  m_data(data), m_security(security), m_securityOriginal(security),
        m_modelHistory(0)
{
    ui.setupUI(this);
    this->setWindowTitle(QString("%1 Properties").arg(security.id == -1 ? "New Security" : m_security.symbol));

    if (m_security.id != -1)
        ui.btnAddAnother->setVisible(false);

    m_minDate = m_security.firstTradeDate();

    loadDropDowns();
    loadSecurity();
    connectSlots();
}

void frmSecurity::loadDropDowns()
{
    foreach(const assetAllocation &value, m_data.aa)
        ui.cmbAA->addItem(value.description, value.id);

    ui.cmbAcct->addItem("(None)", -1);
    foreach(const account &value, m_data.acct)
        ui.cmbAcct->addItem(value.description, value.id);
}

void frmSecurity::connectSlots()
{
    connect(ui.btnHistorical, SIGNAL(toggled(bool)), this, SLOT(historyToggled(bool)));
    connect(ui.sortHistorical, SIGNAL(toggled(bool)), this, SLOT(historySortToggled()));
    connect(ui.cmbHistorical, SIGNAL(currentIndexChanged(int)), this, SLOT(historyIndexChange(int)));
    connect(ui.btnOkCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnOkCancel, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui.btnAddAnother, SIGNAL(clicked()), this, SLOT(accept()));
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
}

void frmSecurity::loadSecurity()
{
    ui.txtSymbol->setText(m_security.symbol);
    ui.cmbAcct->setCurrentIndex(ui.cmbAcct->findData(m_security.account));
    ui.sbExpense->setValue(m_security.expense);
    ui.chkReinvest->setChecked(m_security.divReinvest);
    ui.chkHide->setChecked(m_security.hide);
    ui.chkCash->setChecked(m_security.cashAccount);
    ui.chkInclude->setChecked(m_security.includeInCalc);
    ui.btnHistorical->setDisabled(m_security.id == -1);

    m_modelAA = new securityAAModel(m_security.aa, m_data.aa, 2, ui.aa);
    installAAModel();

    m_modelTrade = new securityTradeModel(m_security.trades.values(), m_data.securities,  ui.trades, this);
    ui.trades->setModel(m_modelTrade);
    // HACK: could not get the model to resize correctly without this time
    QTimer::singleShot(0, m_modelTrade, SLOT(autoResize()));

}

void frmSecurity::updateAAPercentage()
{
    ui.gpAA->setTitle(QString("Asset Allocation (%L1%)").arg(m_modelAA->totalPercentage(), 0, 'f', 2));
}


void frmSecurity::addAA()
{
    if (ui.cmbAA->currentIndex() == -1)
        return;

    m_modelAA->addNew(ui.cmbAA->itemData(ui.cmbAA->currentIndex()).toInt());
}

void frmSecurity::installAAModel()
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
    updateAAPercentage();
}

void frmSecurity::accept()
{
    int result = QDialog::Accepted;
    bool newSecurity = m_security.id == -1;

    if (sender() == ui.btnAddAnother)
        ++result;

    m_security.symbol = ui.txtSymbol->text();
    m_security.account = ui.cmbAcct->itemData(ui.cmbAcct->currentIndex()).toInt();
    m_security.expense = ui.sbExpense->value();
    m_security.divReinvest = ui.chkReinvest->isChecked();
    m_security.cashAccount = ui.chkCash->isChecked();
    m_security.includeInCalc = ui.chkInclude->isChecked();
    m_security.hide = ui.chkHide->isChecked();
    if (!newSecurity)
        m_security.trades = m_modelTrade->saveList(m_securityOriginal.trades, m_security.id);
    m_security.aa = m_modelAA->getList();

    if (m_security == m_securityOriginal)
    {
        QDialog::reject();
        return;
    }

    m_security.save(m_portfolioID);

    if (newSecurity)
        m_security.trades = m_modelTrade->saveList(m_securityOriginal.trades, m_security.id);

    if (m_security.trades != m_securityOriginal.trades)
    {
        int newMinDate = m_security.firstTradeDate();
        if (newMinDate != -1 && (newMinDate < m_minDate || m_minDate == -1))
            m_minDate = newMinDate;
    }
    else
        m_minDate = -1;

    if(m_security.aa == m_securityOriginal.aa)
    {
        QDialog::done(result);
        return;
    }

    m_security.saveAATargets();
    QDialog::done(result);
}

void frmSecurity::resetExpense()
{
    ui.sbExpense->setValue(-1);
}

void frmSecurity::customContextMenuRequested(const QPoint&)
{
    ui.tradesPopup->popup(QCursor::pos());
}

void frmSecurity::historyIndexChange(int index)
{
    QAbstractItemModel *oldModel = ui.history->model();

    m_modelHistory = new securityHistoryModel((securityHistoryModel::historyChoice)index, m_data.executedTrades.value(m_security.id),
        prices::instance().history(m_security.symbol), ui.sortHistorical->isChecked(), ui.history);
    ui.history->setModel(m_modelHistory);
    ui.history->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    delete oldModel;
}

void frmSecurity::historyToggled(bool checked)
{
    ui.gpHistorical->setVisible(checked);
    if (!checked)
        return;

    ui.cmbHistorical->blockSignals(true);
    ui.cmbHistorical->setCurrentIndex(0);
    historyIndexChange(0);
    ui.cmbHistorical->blockSignals(false);
}
