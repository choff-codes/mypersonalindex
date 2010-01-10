#ifndef FRMSECURITY_H
#define FRMSECURITY_H

#include <QtGui>
#include "frmSecurity_UI.h"
#include "securityAAModel.h"
#include "securityTradeModel.h"
#include "security.h"
#include "portfolio.h"

class frmSecurity : public QDialog
{
    Q_OBJECT

public:
    enum securityHistoryChoice { history_All, history_Change, history_Dividends, history_Splits, history_Trades };

    const security& getReturnValuesSecurity() const { return m_security; }
    const int& getReturnValuesMinDate() const { return m_minDate; }
    //const int &getSecurityID() const { return m_security.id; }
    const QMap<int, security::security>& getCashAccounts() const { return m_data.securities; }

    frmSecurity(const int &portfolioID, const portfolioData &data, const security& security, QWidget *parent = 0);
    ~frmSecurity() { delete m_modelTrade; delete m_modelAA; }

private:

    frmSecurity_UI ui;
    int m_portfolioID;
    const portfolioData &m_data;
    security m_security;
    security m_securityOriginal;
    securityTradeModel *m_modelTrade;
    securityAAModel *m_modelAA;
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

#endif // FRMSECURITY_H
