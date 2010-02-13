#ifndef FRMSECURITY_H
#define FRMSECURITY_H

#include <QtGui>
#include "frmSecurity_UI.h"
#include "securityAAModel.h"
#include "securityTradeModel.h"
#include "securityHistoryModel.h"
#include "portfolio.h"

class frmSecurity : public QDialog
{
    Q_OBJECT

public:
    const security& getReturnValuesSecurity() const { return m_security; }
    const int& getReturnValuesMinDate() const { return m_minDate; }

    frmSecurity(const int &portfolioID, const QMap<int, portfolio*> &data, const security& security, QWidget *parent = 0);
    ~frmSecurity() { delete m_modelTrade; delete m_modelAA; delete m_modelHistory; }

private:

    frmSecurity_UI ui;
    int m_portfolioID;
    const QMap<int, portfolio*> m_data;
    security m_security;
    security m_securityOriginal;
    securityTradeModel *m_modelTrade;
    securityAAModel *m_modelAA;
    securityHistoryModel *m_modelHistory;
    int m_minDate;

    void connectSlots();
    void loadSecurity();
    void saveSecurity();
    void loadDropDowns();
    void installAAModel();
    bool hasValidationErrors();

private slots:
    void accept();
    void resetExpense();
    void addAA();
    void updateAAPercentage();
    void customContextMenuRequested(const QPoint&);
    void historyIndexChange(int index);
    void historyToggled(bool checked);
    void historySortToggled();
    void copyPressed();
};

#endif // FRMSECURITY_H
