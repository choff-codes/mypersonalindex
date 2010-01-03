#ifndef FRMTICKER_H
#define FRMTICKER_H

#include <QtGui>
#include "frmTicker_UI.h"
#include "tickerAAModel.h"
#include "tickerTradeModel.h"
#include "security.h"
#include "portfolio.h"

class frmTicker : public QDialog
{
    Q_OBJECT

public:
    enum tickerHistoryChoice { history_All, history_Change, history_Dividends, history_Splits, history_Trades };

    const security& getReturnValuesSecurity() const { return m_security; }
    const int& getReturnValuesMinDate() const { return m_minDate; }
    //const int &getTickerID() const { return m_security.id; }
    const QMap<int, security::security>& getCashAccounts() const { return m_data.tickers; }

    frmTicker(const int &portfolioID, const portfolioData &data, const security& security, QWidget *parent = 0);
    ~frmTicker() { delete m_modelTrade; delete m_modelAA; }

private:

    frmTicker_UI ui;
    int m_portfolioID;
    const portfolioData &m_data;
    security m_security;
    security m_securityOriginal;
    tickerTradeModel *m_modelTrade;
    tickerAAModel *m_modelAA;
    int m_minDate;

    void connectSlots();
    void loadSecurity();
    void loadDropDowns();
    void installAAModel();

private slots:
    void accept();
    void resetExpense();
    void addAA();
    void updateAAPercentage();
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMTICKER_H
