#ifndef FRMTICKER_H
#define FRMTICKER_H

#include <QtGui>
#include "frmTicker_UI.h"
#include "globals.h"
#include "queries.h"
#include "tickerAAModel.h"
#include "tickerTradeModel.h"
#include "functions.h"

class frmTicker : public QDialog
{
    Q_OBJECT

public:

    const globals::security& getReturnValuesSecurity() const { return m_security; }
    const int& getReturnValuesMinDate() const { return m_minDate; }
    //const int &getTickerID() const { return m_security.id; }
    const QMap<int, globals::security>& getCashAccounts() const { return m_data.tickers; }

    frmTicker(const int &portfolioID, const globals::portfolioData &data, const globals::security& security, const queries &sql, QWidget *parent = 0);
    ~frmTicker() { delete m_modelTrade; delete m_modelAA; }

private:

    frmTicker_UI ui;
    int m_portfolioID;
    const globals::portfolioData &m_data;
    globals::security m_security;
    globals::security m_securityOriginal;
    const queries &m_sql;
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
    void saveItem(globals::dynamicTrade *trade);
    void deleteItem(const globals::dynamicTrade &trade);
    void customContextMenuRequested(const QPoint&);
};

#endif // FRMTICKER_H
